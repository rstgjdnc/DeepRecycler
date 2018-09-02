#ifndef ROBOTARMCONTROLLER_H
#define ROBOTARMCONTROLLER_H

#include <QWidget>

namespace Ui {
class RobotArmController;
}

class RobotArmController : public QWidget
{
    Q_OBJECT

public:
    explicit RobotArmController(QWidget *parent = 0);
    ~RobotArmController();

private slots:
    void on_btnServoX_clicked();
    void on_btnServoY_clicked();
    void on_btnServoZ_clicked();
    void on_btnServoADef_clicked();
    void on_btnServoX_2_clicked();
    void on_btnServoY_2_clicked();
    void on_btnServoZ_2_clicked();
    void on_btnPumpOff_clicked();
    void on_btnPumpOn_clicked();

private:
    Ui::RobotArmController *ui;
    QByteArray moveCoordinate(int x, int y, int z, int f);
    QByteArray enablePump(int enable);
    int m_x = 100;
    int m_y = 100;
    int m_z = 100;
    int m_f = 10000;
    int m_pump = 0;
    void initUi();
    void initData();
};

#endif // ROBOTARMCONTROLLER_H
