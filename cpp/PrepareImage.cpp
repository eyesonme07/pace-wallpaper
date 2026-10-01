#include"../header/PrepareImage.h"
#include <qpainter.h>

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
    std::vector<Driver> drivers = pareser.getDriversList();

    if(drivers.empty()){
        qWarning()<<"driver empty";
        return;
    }

    QImage image = matToQImage(wallpaper).convertToFormat(QImage::Format_ARGB32_Premultiplied);

    
    for(int i=0 , y = 90;i<drivers.size();++i,y+=80){
        QPainter p(&image);
        QColor teamColor(QString::fromStdString(drivers[i].team.color));

        p.setPen(QPen(teamColor,3));
        p.setRenderHint(QPainter::Antialiasing);
        p.setFont(QFont("Comic Sans MS",35));
        p.drawText(QPoint(30,y),QString::fromStdString(drivers[i].tla + " - " + drivers[i].point+" - "+drivers[i].team.name));
    }

    {
        QPainter p(&image);

        QColor timeColor("White");

        p.setPen(QPen(timeColor,3));
        p.setRenderHint(QPainter::Antialiasing);
        p.setFont(QFont("Comic Sans MS",17));

        p.drawText(QPoint(40,image.height()-150),"Данные обновлены в ("+QDate::currentDate().toString()+" "+QTime::currentTime().toString()+")");
    }

    cv::Mat result = qImageToMat(image);
    cv::imwrite(path.toStdString(),result);
}

























