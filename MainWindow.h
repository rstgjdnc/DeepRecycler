#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QCamera>
// OpenCV
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();   

private slots:
    void initCamera();
    void displayImage(int id,QImage image);
    void videoFrameCapture();
    cv::Mat qImage2cvMat(QImage image);
    QImage cvMat2QImage(const cv::Mat& mat);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QCameraImageCapture *m_cameraImgCapture;
};

#endif // MAINWINDOW_H
