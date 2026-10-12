#include"../header/PrepareImage.h"
#include <qpainter.h>

struct TextLine {
    QString text;
    QFont   font;
    QColor  color;
};

cv::Mat PrepareImage::qImageToMat(const QImage &src)
{
    switch (src.format()) {
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    case QImage::Format_RGB32:
    {

        cv::Mat view(src.height(), src.width(), CV_8UC4,
                     const_cast<uchar*>(src.constBits()),
                     src.bytesPerLine());
        return view.clone();
    }

    case QImage::Format_RGB888:
    {
        cv::Mat view(src.height(), src.width(), CV_8UC3,
                     const_cast<uchar*>(src.constBits()),
                     src.bytesPerLine());
        cv::Mat bgr;
        cv::cvtColor(view, bgr, cv::COLOR_RGB2BGR);
        return bgr;
    }

    case QImage::Format_Grayscale8:
    {
        cv::Mat view(src.height(), src.width(), CV_8UC1,
                     const_cast<uchar*>(src.constBits()),
                     src.bytesPerLine());
        return view.clone();
    }

    default:
        return qImageToMat(src.convertToFormat(QImage::Format_ARGB32));
    }
}

QImage PrepareImage::matToQImage(const cv::Mat &mat)
{
    switch (mat.type()) {
    case CV_8UC4:
    {
        QImage img(mat.data, mat.cols, mat.rows,
                   static_cast<int>(mat.step), QImage::Format_ARGB32);
        return img.copy();
    }
    case CV_8UC3:
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        QImage img(rgb.data, rgb.cols, rgb.rows,
                   static_cast<int>(rgb.step), QImage::Format_RGB888);
        return img.copy();
    }
    case CV_8UC1:
    {
        QImage img(mat.data, mat.cols, mat.rows,
                   static_cast<int>(mat.step), QImage::Format_Grayscale8);
        return img.copy();
    }
    default:
        return QImage();
    }
}

void PrepareImage::prepareImage(cv::Mat &wallpaper, const QString &path)
{
    cv::Mat withTime = addUpdateTime(wallpaper);

    std::optional<cv::Mat> withDriver = addDriversToImage(withTime);//с гонщиками
    if(withDriver == std::nullopt){
        return;
    }
    std::optional<cv::Mat> withDriverAndNextEvent = addNextGrandPrix(withDriver.value());//сл гонка
    if(withDriverAndNextEvent == std::nullopt){
        return;
    }

    cv::imwrite(path.toStdString(),withDriverAndNextEvent.value());
}

std::optional<cv::Mat> PrepareImage::addDriversToImage(cv::Mat &wallpaper)
{
    std::vector<Driver> drivers = pareser.getDriversList();

    QImage image = matToQImage(wallpaper).convertToFormat(QImage::Format_ARGB32_Premultiplied);

    const int imageWidth  = image.width();
    const int imageHeight = image.height();
    const double scale = static_cast<double>(imageHeight) / 1080.0;

    const int driverCount = std::min<int>(10, static_cast<int>(drivers.size()));

    const int startY      = static_cast<int>(settings().getTopPadding() * scale);
    const int leftPadding = static_cast<int>(settings().getLeftPadding() * scale);
    const int bottomSpace = static_cast<int>(90 * scale);
    const double stepFactor = 0.67;

    const int available  = imageHeight - startY - bottomSpace;
    const int itemHeight = static_cast<int>(available / (1.0 + (driverCount - 1) * stepFactor));
    const int yStep      = static_cast<int>(itemHeight * stepFactor);

    QFont nameFont("Cascadia Code");
    nameFont.setPixelSize(std::max(10, static_cast<int>(itemHeight * 0.20)));
    nameFont.setBold(true);

    QFont teamFont("Cascadia Code");
    teamFont.setPixelSize(std::max(8, static_cast<int>(itemHeight * 0.11)));

    QFont pointFont("Cascadia Code");
    pointFont.setPixelSize(std::max(10, static_cast<int>(itemHeight * 0.22)));
    pointFont.setBold(true);

    QFontMetrics nameFm(nameFont);
    QFontMetrics teamFm(teamFont);
    QFontMetrics pointFm(pointFont);

    const int pad = static_cast<int>(14 * scale);
    int maxNameW = 0, maxTeamW = 0, maxPointW = 0;
    for (int i = 0; i < driverCount; ++i) {
        maxNameW  = std::max(maxNameW,  nameFm.horizontalAdvance(
                                          QString::fromStdString(drivers[i].lastName).toUpper() + "  00"));
        maxTeamW  = std::max(maxTeamW,  teamFm.horizontalAdvance(QString::fromStdString(drivers[i].team.name)));
        maxPointW = std::max(maxPointW, pointFm.horizontalAdvance(QString::fromStdString(drivers[i].point)));
    }
    const int textBlockW = std::max(maxNameW, maxTeamW) + pad + maxPointW;

    const int plateH = static_cast<int>(itemHeight * 0.50);
    const int plateX = leftPadding + itemHeight / 2;
    const int plateW = (itemHeight / 2) + pad * 2 + textBlockW;
    const int stripeW = std::max(3, static_cast<int>(6 * scale));

    QPainter p(&image);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    auto teamColorOf = [](const Driver &d) {
        QString c = QString::fromStdString(d.team.color);
        if (!c.isEmpty() && !c.startsWith('#')) c.prepend('#');
        QColor col(c);
        return col.isValid() ? col : QColor(Qt::white);
    };

    for (int i = 0; i < driverCount; ++i) {
        const Driver &d = drivers[i];
        const QColor teamColor = teamColorOf(d);

        const int rowY   = startY + i * yStep;
        const int plateY = rowY + (yStep - plateH) / 2 + static_cast<int>(itemHeight * 0.12);

        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0, 0, 0, 150));
        p.drawRoundedRect(QRect(plateX, plateY, plateW, plateH), 10 * scale, 10 * scale);

        p.setBrush(teamColor);
        p.drawRoundedRect(QRect(plateX + plateW - stripeW, plateY, stripeW, plateH), 3, 3);

        const int textX = leftPadding + itemHeight + pad;
        const int innerRight = plateX + plateW - stripeW - pad;

        p.setFont(nameFont);
        p.setPen(Qt::white);
        const QString nameLine = QString("%1  %2")
                                     .arg(i + 1, 2)
                                     .arg(QString::fromStdString(d.lastName).toUpper());
        p.drawText(QRect(textX, plateY + pad / 2, innerRight - textX, nameFm.height()),
                   Qt::AlignLeft | Qt::AlignVCenter, nameLine);

        p.setFont(teamFont);
        p.setPen(teamColor);
        p.drawText(QRect(textX, plateY + pad / 2 + nameFm.height(), innerRight - textX, teamFm.height()),
                   Qt::AlignLeft | Qt::AlignVCenter, QString::fromStdString(d.team.name));

        p.setFont(pointFont);
        p.setPen(Qt::white);
        p.drawText(QRect(textX, plateY, innerRight - textX, plateH),
                   Qt::AlignRight | Qt::AlignVCenter, QString::fromStdString(d.point));
    }
    for (int i = 0; i < driverCount; ++i) {
        const Driver &d = drivers[i];
        const int rowY = startY + i * yStep;

        QString photoPath = QString::fromStdString(
            settings().getDocumentDir() + "/pace-wallpaper/" + d.tla + ".png");

        QImage photo(photoPath);
        if (photo.isNull()) {
            p.setFont(nameFont);
            p.setPen(teamColorOf(d));
            p.drawText(QRect(leftPadding, rowY, itemHeight, itemHeight),
                       Qt::AlignCenter, QString::fromStdString(d.tla));
            continue;
        }
        QImage scaled = photo.scaled(itemHeight, itemHeight,
                                     Qt::KeepAspectRatio, Qt::SmoothTransformation);
        p.drawImage(leftPadding + (itemHeight - scaled.width()) / 2, rowY, scaled);
    }
    return qImageToMat(image);
}


std::optional<cv::Mat> PrepareImage::addNextGrandPrix(cv::Mat &wallpaper)
{
    cv::Mat withRace = wallpaper.clone();

    EventParser parser;
    Event nextEvent = parser.getNextEvent();
    Event::Schedule nextSchedule = parser.getNextSchedule(nextEvent);

    QImage image = matToQImage(withRace).convertToFormat(QImage::Format_ARGB32_Premultiplied);

    int imageHeight = image.height();
    int imageWidth  = image.width();

    double scale = static_cast<double>(imageHeight) / 1080.0;

    int itemHeight   = imageHeight / 5;
    int itemWidth    = imageWidth / 4;
    int startY       = static_cast<int>(settings().getTopPadding()  * scale);
    int rightPadding = static_cast<int>(settings().getRightPadding() * scale);
    int penWidth     = std::max(1, static_cast<int>(3 * scale));
    int fontSize     = std::max(10, static_cast<int>(28 * scale));


    {
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);

        const QColor accent(225, 6, 0);
        const QColor grayText(170, 170, 170);
        const QColor white(Qt::white);


        auto makeFont = [&](double scale, bool bold, double spacing = 0.0) {
            QFont f("Cascadia Code");
            f.setPixelSize(int(fontSize * scale));
            f.setBold(bold);
            f.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
            return f;
        };

        auto q = [](const std::string &s) { return QString::fromStdString(s); };

        QString place = q(nextEvent.location.city);
        if (!nextEvent.location.counryName.empty())
            place += ", " + q(nextEvent.location.counryName);

        QString session = q(nextSchedule.name);
        if (!nextSchedule.type.empty())
            session += " (" + q(nextSchedule.type) + ")";

        QString date = nextSchedule.timeStart.toLocalTime()
                           .toString("dd.MM.yyyy  HH:mm");

        std::vector<TextLine> lines = {
        { "СЛЕДУЮЩАЯ ГОНКА",       makeFont(0.65, true, 2.0), accent    },
        { q(nextEvent.name),        makeFont(1.30, true),      white     },
        { place,                    makeFont(0.80, false),     grayText  },
        { QString(),                makeFont(0.40, false),     white     },
        { session,                  makeFont(0.90, true),      white     },
        { date,                     makeFont(0.90, false),     accent    },
        };


        const int padding = fontSize;
        const int lineGap = fontSize / 5;
        int maxW = 0, totalH = 0;
        for (const auto &l : lines) {
            QFontMetrics fm(l.font);
            maxW = std::max(maxW, fm.horizontalAdvance(l.text));
            totalH += fm.height() + lineGap;
        }

        const int panelW = maxW + padding * 2;
        const int panelH = totalH + padding * 2 - lineGap;
        const int panelX = image.width() - panelW - rightPadding;
        const int panelY = startY;

        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 150));
        painter.drawRoundedRect(QRect(panelX, panelY, panelW, panelH), 10, 10);

        painter.setBrush(accent);
        painter.drawRoundedRect(QRect(panelX + panelW - 6, panelY, 6, panelH), 3, 3);

        int y = panelY + padding;
        const int textRight = panelX + panelW - padding - 6;
        for (const auto &l : lines) {
            QFontMetrics fm(l.font);
            painter.setFont(l.font);
            painter.setPen(l.color);
            painter.drawText(QRect(panelX + padding, y, textRight - (panelX + padding), fm.height()),
                             Qt::AlignRight | Qt::AlignVCenter,
                             l.text);
            y += fm.height() + lineGap;
        }
    }

    return qImageToMat(image);
}
//добавление времени обновления
cv::Mat PrepareImage::addUpdateTime(cv::Mat &wallpaper)
{
    QImage image = matToQImage(wallpaper).convertToFormat(QImage::Format_ARGB32_Premultiplied);

    int imageHeight = image.height();
    int imageWidth  = image.width();

    double scale = static_cast<double>(imageHeight) / 1080.0;


    QFont timeFont("Comic Sans MS");
    timeFont.setPixelSize(std::max(8, static_cast<int>(14 * scale)));
    QPainter p(&image);

    p.setFont(timeFont);
    p.setPen(Qt::white);

    const QString updated = "Данные обновлены в ("
                            + QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss") + ")";
    p.drawText(QPoint(static_cast<int>(40 * scale), imageHeight - static_cast<int>(50 * scale)), updated);

    p.end();

    return qImageToMat(image);
}