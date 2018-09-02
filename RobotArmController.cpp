#include "RobotArmController.h"
#include "ui_RobotArmController.h"
#include "QtSerialPort/qserialport.h"
#include "QtSerialPort/qserialportinfo.h"
#include "qdebug.h"

QSerialPort serial;

RobotArmController::RobotArmController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotArmController)
{
    ui->setupUi(this);
    initUi();
    initData();
}

RobotArmController::~RobotArmController()
{
    delete ui;
}

void RobotArmController::initUi()
{

}

void RobotArmController::initData()
{
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        serial.setPortName(serialPortInfo.portName());
    }
    serial.open(QIODevice::ReadWrite);
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
}

void RobotArmController::on_btnServoADef_clicked()
{
    m_x = 100;
    m_y = 90;
    m_z = 80;
    if (serial.isOpen() && serial.isWritable())
    {
        qDebug() << "Serial is open";
        QByteArray coordinate = moveCoordinate(m_x, m_y, m_z, m_f);
        serial.write(coordinate);
        serial.flush();
        QByteArray input = serial.readAll();
        if (input.size() > 0)
        {
            qDebug() << QString(input);
        }
    }
    else
    {
        qDebug() << "Serial is not open";
    }
}

void RobotArmController::on_btnServoX_clicked()
{
    m_x += 10;
    if (serial.isOpen() && serial.isWritable())
    {
        qDebug() << "Serial is open";
        QByteArray coordinate = moveCoordinate(m_x, m_y, m_z, m_f);
        serial.write(coordinate);
        serial.flush();
        QByteArray input = serial.readAll();
        if (input.size() > 0)
        {
            qDebug() << QString(input);
        }
    }
    else
    {
        qDebug() << "Serial is not open";
    }
}

void RobotArmController::on_btnServoY_clicked()
{
    m_y += 10;
    if (serial.isOpen() && serial.isWritable())
    {
        qDebug() << "Serial is open";
        QByteArray coordinate = moveCoordinate(m_x, m_y, m_z, m_f);
        serial.write(coordinate);
        serial.flush();
        QByteArray input = serial.readAll();
        if (input.size() > 0)
        {
            qDebug() << QString(input);
        }
    }
    else
    {
        qDebug() << "Serial is not open";
    }
}

void RobotArmController::on_btnServoZ_clicked()
{
    m_z += 10;
    if (serial.isOpen() && serial.isWritable())
    {
        qDebug() << "Serial is open";
        QByteArray coordinate = moveCoordinate(m_x, m_y, m_z, m_f);
        serial.write(coordinate);
        serial.flush();
        QByteArray input = serial.readAll();
        if (input.size() > 0)
        {
            qDebug() << QString(input);
        }
    }
    else
    {
        qDebug() << "Serial is not open";
    }
}

void RobotArmController::on_btnServoX_2_clicked()
{

    m_x -= 10;
    if (serial.isOpen() && serial.isWritable())
    {
        qDebug() << "Serial is open";
        QByteArray coordinate = moveCoordinate(m_x, m_y, m_z, m_f);
        serial.write(coordinate);
        serial.flush();
        QByteArray input = serial.readAll();
        if (input.size() > 0)
        {
            qDebug() << QString(input);
        }
    }
    else
    {
        qDebug() << "Serial is not open";
    }
}

void RobotArmController::on_btnServoY_2_clicked()
{
    m_y -= 10;
    if (serial.isOpen() && serial.isWritable())
    {
        qDebug() << "Serial is open";
        QByteArray coordinate = moveCoordinate(m_x, m_y, m_z, m_f);
        serial.write(coordinate);
        serial.flush();
        QByteArray input = serial.readAll();
        if (input.size() > 0)
        {
            qDebug() << QString(input);
        }
    }
    else
    {
        qDebug() << "Serial is not open";
    }
}

void RobotArmController::on_btnServoZ_2_clicked()
{
    m_z -= 10;
    if (serial.isOpen() && serial.isWritable())
    {
        qDebug() << "Serial is open";
        QByteArray coordinate = moveCoordinate(m_x, m_y, m_z, m_f);
        serial.write(coordinate);
        serial.flush();
        QByteArray input = serial.readAll();
        if (input.size() > 0)
        {
            qDebug() << QString(input);
        }
    }
    else
    {
        qDebug() << "Serial is not open";
    }
}

void RobotArmController::on_btnPumpOn_clicked()
{
    m_pump = 1;
    if (serial.isOpen() && serial.isWritable())
    {
        qDebug() << "Serial is open";
        QByteArray pump = enablePump(m_pump);
        serial.write(pump);
        serial.flush();
        QByteArray input = serial.readAll();
        if (input.size() > 0)
        {
            qDebug() << QString(input);
        }
    }
    else
    {
        qDebug() << "Serial is not open";
    }
}

void RobotArmController::on_btnPumpOff_clicked()
{
    m_pump = 0;
    if (serial.isOpen() && serial.isWritable())
    {
        qDebug() << "Serial is open";
        QByteArray pump = enablePump(m_pump);
        serial.write(pump);
        serial.flush();
        QByteArray input = serial.readAll();
        if (input.size() > 0)
        {
            qDebug() << QString(input);
        }
    }
    else
    {
        qDebug() << "Serial is not open";
    }
}

QByteArray RobotArmController::moveCoordinate(int x, int y, int z, int f)
{
     QString value = "G2201 S" + QString::number(x) +
                     " R" + QString::number(y) +
                     " H" + QString::number(z) +
                     " F" + QString::number(f) + "\n";
     qDebug() << value;
     return value.toLatin1();
}

QByteArray RobotArmController::enablePump(int enable)
{
     QString value = "M2231 V" + QString::number(enable) + "\n";
     qDebug() << value;
     return value.toLatin1();
}

