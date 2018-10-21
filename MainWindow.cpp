#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QTimer"
#include "QList"

#include "qdebug.h"
#include "qcamerainfo.h"

using namespace cv;
using namespace std;

int largest_area = 0;
int largest_contour_index = 0;
QList<Rect> bounding_rect_temp_list;
QList<Rect> bounding_rect_list;
Rect bounding_rect;
int totalFrame = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initCamera();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initCamera()
{
    QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    QByteArray logitechC930;
    for (int i = 0 ; i < availableCameras.count() ; i++)
    {
        if (!availableCameras.value(i).description().compare("Logitech Webcam C930e"))
        {
            logitechC930 = availableCameras.value(i).deviceName().toUtf8();
        }
    }
    QCamera *camera = new QCamera(logitechC930);
    m_cameraImgCapture = new QCameraImageCapture(camera);
    QCameraViewfinder *cameraViewfinder = new QCameraViewfinder();
    QTimer *pollTimer = new QTimer(this);

    connect(pollTimer,SIGNAL(timeout()), this,SLOT(videoFrameCapture()));

    connect(m_cameraImgCapture ,SIGNAL(imageCaptured(int, QImage))
            ,this,SLOT(displayImage(int, QImage)));

    //設定視訊參數
    camera->setCaptureMode(QCamera::CaptureViewfinder);
    camera->setViewfinder(cameraViewfinder);

    camera->start();//開始擷取畫面
    pollTimer->start(30); //每33毫秒觸發displayImage()
}

void MainWindow::displayImage(int /* id */, QImage image)
{
    totalFrame++;
    Mat source = qImage2cvMat(image);
    Mat matDst = Mat::zeros(source.rows, source.cols, CV_8UC3);
    Mat matGaussianBlur;
    GaussianBlur(source, matGaussianBlur, Size(3,3), 10, 0);
    Mat matCanny;
    Canny(matGaussianBlur, matCanny, 50, 150);
    Mat matDilate;
    dilate(matCanny,matDilate,getStructuringElement(MORPH_RECT, Size(30,30)));

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(matDilate, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    for(int i = 0 ; i < contours.size() ; i++)
    {
        Scalar color(255,0,0,255);
        drawContours(matDst, contours, i, color, CV_FILLED, 8, hierarchy);
        if (boundingRect(contours[i]).height * boundingRect(contours[i]).width > 30000 && totalFrame%33 == 0)
        {
            bounding_rect = boundingRect(contours[i]);
            bounding_rect_temp_list.push_back(bounding_rect);
        }
    }
    if (bounding_rect_temp_list.count() > 0)
    {
        bounding_rect_list = bounding_rect_temp_list;
        bounding_rect_temp_list.clear();
    }
    for (int j = 0 ; j < bounding_rect_list.size() ; j++ )
    {
        rectangle(source, bounding_rect_list.value(j).tl(), bounding_rect_list.value(j).br(), Scalar(125,125,125), 2, 8, 0);
    }

    QImage imageDst = cvMat2QImage(source);
    ui->label->setPixmap(QPixmap::fromImage(imageDst));
}

void MainWindow::videoFrameCapture()
{
    m_cameraImgCapture->capture();
}

QImage MainWindow::cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }

    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

cv::Mat MainWindow::qImage2cvMat(QImage image)
{
    cv::Mat mat;
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}
