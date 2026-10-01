#ifndef PREPAREIMAGE_H
#define PREPAREIMAGE_H

#include<QImage>
#include<opencv2/opencv.hpp>
#include"F1_Request.h"
#include<QDate>
#include<QPainter>

class PrepareImage{
public:
    cv::Mat qImageToMat(const QImage &src);

    QImage matToQImage(const cv::Mat &mat);

    void prepareImage(cv::Mat &wallpaper,const QString &path);
private:
    DriverParser pareser;
};

#endif // PREPAREIMAGE_H
