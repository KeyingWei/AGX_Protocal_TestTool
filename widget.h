#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QWidget>
#include <QDebug>
#include <qstylefactory.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_StartOrStopButton_clicked();

    void on_turning_ctl_mode_btn_clicked();

    void on_turning_trl_auto_clicked();

    void on_turin_ctl_eps_clicked();

    void on_turning_ctl_manu_clicked();

    void on_turning_ctl_clear_clicked();

    void on_set_turnning_angle_clicked();

    void on_set_speed_clicked();

    void on_accel_mode_crl_clicked();

    void on_accel_auto_mode_clicked();

    void on_accel_ecu_mode_clicked();

    void on_accel_manu_mode_clicked();

    void on_accel_clear_clicked();

    void on_set_accel_clicked();

    void on_brake_standby_mode_clicked();

    void on_brake_auto_mode_clicked();

    void on_brake_ebs_mode_clicked();

    void on_brake_manu_mode_clicked();

    void on_brake_clear_mode_clicked();

    void on_pushButton_7_clicked();

    void on_gear_standby_mode_clicked();

    void on_gear_auto_mode_clicked();

    void on_gear_crl_mode_clicked();

    void on_gear_manu_mode_clicked();

    void on_gear_clear_mode_clicked();

    void on_set_grear_parking_clicked();

    void on_set_grear_reverse_clicked();

    void on_set_gear_neutral_clicked();

    void on_set_gear_forward_clicked();

    void on_set_gear_low_clicked();

    void on_gear_clear_clicked();

    void on_parking_standby_mode_clicked();

    void on_parking_auto_mode_clicked();

    void on_parking_crl_mode_clicked();

    void on_parking_manu_mode_clicked();

    void on_parking_clear_clicked();

    void on_set_open_parking_clicked();

    void on_set_close_parking_clicked();

    void on_light_standby_mode_clicked();

    void on_light_auto_mode_clicked();

    void on_light_ctl_mode_clicked();

    void on_light_manu_mode_clicked();

    void on_light_clear_clicked();

    void on_set_open_left_light_clicked();

    void on_set_open_low_beam_light_clicked();

    void on_set_open_front_fog_light_clicked();

    void on_set_open_warning_light_clicked();

    void on_set_open_right_light_clicked();

    void on_set_open_hight_beam_light_clicked();

    void on_set_open_rear_fog_light_clicked();

    void on_set_close_all_lights_clicked();

    void on_set_brake_clicked();

    void on_pushButton_clicked();

    void on_reset_all_clicked();

private:
    Ui::Widget *ui;
    QSerialPort *serial;
    Read_data();
    ScanfSerialPort();
    void sendData(uint8_t cmd_type,int16_t data);
    QTimer *timer;
    void rest_all();
    void diable_button();
    void enable_button();
};

#endif // WIDGET_H
