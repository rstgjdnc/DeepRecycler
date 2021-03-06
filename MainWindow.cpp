#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QTimer"
#include "QList"

#include "qdebug.h"
#include "qcamerainfo.h"

using namespace cv;
using namespace std;

QList<vector<Point>> m_contourTempList;
QList<vector<Point>> m_contourList;
vector<vector<Point>>hull(200);
vector<Point> contour_1;
vector<Point> contour_2;
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
        //      if (!availableCameras.value(i).description().compare("Logitech Webcam C930e"))
        if (!availableCameras.value(i).description().compare("USB 視訊裝置"))
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
    //    Mat matDst = Mat::zeros(source.rows, source.cols, CV_8UC3);
    Mat matGaussianBlur;
    GaussianBlur(source, matGaussianBlur, Size(3,3), 10, 0);
    Mat matCanny;
    Canny(matGaussianBlur, matCanny, 50, 150);
    Mat matDilate;
    dilate(matCanny,matDilate,getStructuringElement(MORPH_RECT, Size(15,15)));

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(matDilate, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    // 符合條件的 Contours
    for(int i = 0 ; i < contours.size() ; i++)
    {
        if (totalFrame%16 == 0)
        {
            RotatedRect m_minContours = minAreaRect(contours[i]);
            if (m_minContours.size.width * m_minContours.size.height > 20000)
            {
                m_contourTempList.push_back(contours[i]);
            }
        }
    }
    if (m_contourTempList.count() > 0)
    {
        m_contourList = m_contourTempList;
        m_contourTempList.clear();
    }
    for (int j = 0 ; j < m_contourList.size() ; j++ )
    {
        //        double ration =0;
        //        RotatedRect  min = minAreaRect(m_contourList.value(j));
        //        qDebug() << min.size.width << min.size.height;
        //        if (min.size.width >= min.size.height)
        //        {
        //           ration =  min.size.width / min.size.height;
        //        }
        Rect rectContour = boundingRect(m_contourList.value(j));
        rectangle(source, rectContour.tl(), rectContour.br(), Scalar(125,125,125), 2, 8, 0);
        int text_x = rectContour.tl().x + 10 ;
        int text_y = rectContour.tl().y + 30;
        putText(source, "Mike", Point(text_x,text_y), FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 1, LINE_AA);
        //        drawContours(matDst, contours, j, Scalar(125,125,125), CV_FILLED, 8, hierarchy);
        //        double comres = 1;
        //        comres = matchShapes(contours[j], contours[j], CV_CONTOURS_MATCH_I1, 0.0);
        //        qDebug() << "Mike " << comres;
        //        bool isConvex = isContourConvex(contours[j]);
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

void MainWindow::on_pushButton_clicked()
{
    contour_1 = m_contourList.value(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    contour_2 = m_contourList.value(0);
}

void MainWindow::on_pushButton_3_clicked()
{
    double comres = 1;
    comres = matchShapes(contour_1, contour_2, CV_CONTOURS_MATCH_I1, 0.0);
    qDebug() << "Mike " << comres;
}
