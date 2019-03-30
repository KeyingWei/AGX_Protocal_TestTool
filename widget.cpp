#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QList>
#include <QQueue>
#include <stdlib.h>
#include <qstring.h>
#include <qstylefactory.h>

#pragma pack(1)
//CRC16 initial: 0x2B3B poly: 0x2333////////////////////////////////////////////////////
static const unsigned short CRC16_Table[256] =
{
    0x0000, 0x0E53, 0x1CA6, 0x12F5, 0x394C, 0x371F, 0x25EA, 0x2BB9,
    0x34FF, 0x3AAC, 0x2859, 0x260A, 0x0DB3, 0x03E0, 0x1115, 0x1F46,
    0x2F99, 0x21CA, 0x333F, 0x3D6C, 0x16D5, 0x1886, 0x0A73, 0x0420,
    0x1B66, 0x1535, 0x07C0, 0x0993, 0x222A, 0x2C79, 0x3E8C, 0x30DF,
    0x1955, 0x1706, 0x05F3, 0x0BA0, 0x2019, 0x2E4A, 0x3CBF, 0x32EC,
    0x2DAA, 0x23F9, 0x310C, 0x3F5F, 0x14E6, 0x1AB5, 0x0840, 0x0613,
    0x36CC, 0x389F, 0x2A6A, 0x2439, 0x0F80, 0x01D3, 0x1326, 0x1D75,
    0x0233, 0x0C60, 0x1E95, 0x10C6, 0x3B7F, 0x352C, 0x27D9, 0x298A,
    0x32AA, 0x3CF9, 0x2E0C, 0x205F, 0x0BE6, 0x05B5, 0x1740, 0x1913,
    0x0655, 0x0806, 0x1AF3, 0x14A0, 0x3F19, 0x314A, 0x23BF, 0x2DEC,
    0x1D33, 0x1360, 0x0195, 0x0FC6, 0x247F, 0x2A2C, 0x38D9, 0x368A,
    0x29CC, 0x279F, 0x356A, 0x3B39, 0x1080, 0x1ED3, 0x0C26, 0x0275,
    0x2BFF, 0x25AC, 0x3759, 0x390A, 0x12B3, 0x1CE0, 0x0E15, 0x0046,
    0x1F00, 0x1153, 0x03A6, 0x0DF5, 0x264C, 0x281F, 0x3AEA, 0x34B9,
    0x0466, 0x0A35, 0x18C0, 0x1693, 0x3D2A, 0x3379, 0x218C, 0x2FDF,
    0x3099, 0x3ECA, 0x2C3F, 0x226C, 0x09D5, 0x0786, 0x1573, 0x1B20,
    0x2333, 0x2D60, 0x3F95, 0x31C6, 0x1A7F, 0x142C, 0x06D9, 0x088A,
    0x17CC, 0x199F, 0x0B6A, 0x0539, 0x2E80, 0x20D3, 0x3226, 0x3C75,
    0x0CAA, 0x02F9, 0x100C, 0x1E5F, 0x35E6, 0x3BB5, 0x2940, 0x2713,
    0x3855, 0x3606, 0x24F3, 0x2AA0, 0x0119, 0x0F4A, 0x1DBF, 0x13EC,
    0x3A66, 0x3435, 0x26C0, 0x2893, 0x032A, 0x0D79, 0x1F8C, 0x11DF,
    0x0E99, 0x00CA, 0x123F, 0x1C6C, 0x37D5, 0x3986, 0x2B73, 0x2520,
    0x15FF, 0x1BAC, 0x0959, 0x070A, 0x2CB3, 0x22E0, 0x3015, 0x3E46,
    0x2100, 0x2F53, 0x3DA6, 0x33F5, 0x184C, 0x161F, 0x04EA, 0x0AB9,
    0x1199, 0x1FCA, 0x0D3F, 0x036C, 0x28D5, 0x2686, 0x3473, 0x3A20,
    0x2566, 0x2B35, 0x39C0, 0x3793, 0x1C2A, 0x1279, 0x008C, 0x0EDF,
    0x3E00, 0x3053, 0x22A6, 0x2CF5, 0x074C, 0x091F, 0x1BEA, 0x15B9,
    0x0AFF, 0x04AC, 0x1659, 0x180A, 0x33B3, 0x3DE0, 0x2F15, 0x2146,
    0x08CC, 0x069F, 0x146A, 0x1A39, 0x3180, 0x3FD3, 0x2D26, 0x2375,
    0x3C33, 0x3260, 0x2095, 0x2EC6, 0x057F, 0x0B2C, 0x19D9, 0x178A,
    0x2755, 0x2906, 0x3BF3, 0x35A0, 0x1E19, 0x104A, 0x02BF, 0x0CEC,
    0x13AA, 0x1DF9, 0x0F0C, 0x015F, 0x2AE6, 0x24B5, 0x3640, 0x3813
};


unsigned short crc16_calc(unsigned char *buf, int length)
{
    unsigned short Result = 0x2B3B;

    for (int i = 0; i < length; i++)
    {
        Result = (Result >> 8) ^ CRC16_Table[(Result ^ buf[i]) & 0xff];
    }
    return Result;
}


QString open_button_style = "QPushButton{background-color:green;\
                                   color: white;   border-radius: 10px;  border: 2px groove gray;\
                                   border-style: outset;}";
QString close_button_style = "QPushButton{background-color:red;\
                             color: white;   border-radius: 10px;  border: 2px groove gray;\
                             border-style: outset;}";

                                 //  "QPushButton:hover{background-color:white; color: black;}"\
                                     "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                     border-style: inset; }";

 QString set_button_style = "QPushButton{background-color:green;\
                              color: white;   border-radius: 10px;  border: 2px groove gray;\
                              border-style: outset;}"\
                              "QPushButton:hover{background-color:white; color: black;}"\
                              "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                              border-style: inset; }";



 QString mode_select = "QPushButton{background-color:blue;\
                             color: white;\
                               }";
 QString mode_deselect = "QPushButton{background-color:gray;\
                             color: black;\
                               }";


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

     setWindowTitle(tr("AGX协议测试上位机"));
    ui->baudBox->setCurrentIndex(0);
    ui->DataBitsBox->setCurrentIndex(0);
    ui->ParityBox->setCurrentIndex(0);
    ui->StopBitsBox->setCurrentIndex(0);

    timer =new QTimer(this);//创建一个定时器
    connect(timer,&QTimer::timeout,this,&Widget::ScanfSerialPort);//信号连接
    timer->start(100);//开启计时

    ScanfSerialPort();

    //ui->feedback_display->setReadOnly(true);
    setFixedSize(this->width(),this->height());

    ui->reset_all->setStyleSheet(set_button_style);

   rest_all();
   diable_button();

   ui->turning_feedback_mode->setReadOnly(true);
   ui->truning_feedback_crtl_value->setReadOnly(true);
   ui->accel_feedback_mode->setReadOnly(true);
   ui->accel_feedback_ctrl_value->setReadOnly(true);
   ui->brake_feedback_mode->setReadOnly(true);
   ui->brake_feedback_crtl_value->setReadOnly(true);
   ui->gear_feedback_mode->setReadOnly(true);
   ui->gear_feedback_crtl_value->setReadOnly(true);
   ui->parking_feedback_mode->setReadOnly(true);
   ui->parking_feedbakc_crtl_value->setReadOnly(true);
   ui->light_feedback_mode->setReadOnly(true);
   ui->light_feedback_crtl_value->setReadOnly(true);
   ui->system_data_speed->setReadOnly(true);
   ui->system_data_vol->setReadOnly(true);

    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(15,15,15));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
    palette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(palette);
    qDebug()<<tr("界面设置成功");
}

Widget::~Widget()
{
    delete ui;
}


Widget::ScanfSerialPort()
{
    static int serila_num_curr = 0,serial_num_last = 0;
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
         ui->PortNumber->clear();
         serila_num_curr = 0;
         ui->StartOrStopButton->setEnabled(false);
    }
    else
    {
       QStringList list;
       ui->StartOrStopButton->setEnabled(true);
       serial_num_last = serila_num_curr;
        foreach(QSerialPortInfo info,infos)
        {
             list << info.portName();
        }
        serila_num_curr = list.length();
      //  qDebug()<<serila_num_curr<< "and" <<serial_num_last;

         if(serila_num_curr != serial_num_last)
         {
             ui->PortNumber->clear();
             qSort(list.begin(),list.end());
             ui->PortNumber->addItems(list);
         }

    }

    return 0;
}

typedef struct headPack{
    uint16_t sof;
    uint16_t length;
    uint8_t  cmd_type;
    int16_t data ;
    uint16_t crc16;
}control_Cmd_t;

typedef union {
  control_Cmd_t cmds;
   char buf[8];
}Cmd_u;


void Widget::sendData(uint8_t cmd_type, int16_t data)
{
    Cmd_u tx_data;

    tx_data.cmds.sof      = 0x5AA5;
    tx_data.cmds.length   = sizeof(tx_data);
    tx_data.cmds.cmd_type = cmd_type;
    tx_data.cmds.data = data;
    tx_data.cmds.crc16 = crc16_calc((unsigned char *)tx_data.buf,sizeof(struct headPack) - 2);

    serial->write(tx_data.buf, sizeof( tx_data.cmds));
}

void Widget::rest_all()
{
    ui->angle_input->setText(tr("请输入角度值，范围正负2666"));
    ui->speed_input->setText(tr("请输入速度值，范围0-250"));
    ui->break_input->setText(tr("请输入制动值，范围0-100"));
    ui->accel_input->setText(tr("请输入油门值，范围0-100"));

    ui->StartOrStopButton->setStyleSheet(set_button_style);

    ui->set_open_left_light->setText(tr("打开左向灯"));
    ui->set_open_left_light->setStyleSheet(open_button_style);

    ui->set_open_right_light->setText(tr("打开右向灯"));
    ui->set_open_right_light->setStyleSheet(open_button_style);

    ui->set_open_low_beam_light->setText(tr("打开近光灯"));
    ui->set_open_low_beam_light->setStyleSheet(open_button_style);

    ui->set_open_hight_beam_light->setText(tr("打开远光灯"));
    ui->set_open_hight_beam_light->setStyleSheet(open_button_style);

    ui->set_open_warning_light->setText(tr("打开警示灯"));
    ui->set_open_warning_light->setStyleSheet(open_button_style);

    ui->set_open_front_fog_light->setText(tr("打开前雾灯"));
    ui->set_open_front_fog_light->setStyleSheet(open_button_style);

    ui->set_open_rear_fog_light->setText(tr("打开后雾灯"));
    ui->set_open_rear_fog_light->setStyleSheet(open_button_style);

    ui->set_close_all_lights->setText(tr("关闭所有灯光"));
    ui->set_close_all_lights->setStyleSheet(close_button_style);

    ui->set_open_parking->setText(tr("打开驻车"));
    ui->set_open_parking->setStyleSheet(open_button_style);

   ui->set_gear_forward->setStyleSheet(close_button_style);
   ui->set_gear_low->setStyleSheet(close_button_style);
   ui->set_gear_neutral->setStyleSheet(close_button_style);
   ui->set_grear_parking->setStyleSheet(close_button_style);
   ui->set_grear_reverse->setStyleSheet(close_button_style);
   ui->gear_clear->setStyleSheet(close_button_style);

   ui->pushButton_7->setStyleSheet(set_button_style);
   ui->set_accel->setStyleSheet(set_button_style);

   ui->set_speed->setStyleSheet(set_button_style);
   ui->set_turnning_angle->setStyleSheet(set_button_style);


   ui->turning_ctl_clear->setStyleSheet(mode_deselect);
   ui->turning_ctl_manu->setStyleSheet(mode_deselect);
   ui->turning_ctl_mode_btn->setStyleSheet(mode_deselect);
   ui->turning_trl_auto->setStyleSheet(mode_deselect);
   ui->turin_ctl_eps->setStyleSheet(mode_select);


   ui->accel_auto_mode->setStyleSheet(mode_deselect);
   ui->accel_clear->setStyleSheet(mode_deselect);
   ui->accel_ecu_mode->setStyleSheet(mode_select);
   ui->accel_manu_mode->setStyleSheet(mode_deselect);
   ui->accel_mode_crl->setStyleSheet(mode_deselect);


   ui->gear_auto_mode->setStyleSheet(mode_deselect);
   ui->gear_clear_mode->setStyleSheet(mode_deselect);
   ui->gear_crl_mode->setStyleSheet(mode_select);
   ui->gear_manu_mode->setStyleSheet(mode_deselect);
   ui->gear_standby_mode->setStyleSheet(mode_deselect);


   ui->parking_auto_mode->setStyleSheet(mode_deselect);
   ui->parking_clear->setStyleSheet(mode_deselect);
   ui->parking_crl_mode->setStyleSheet(mode_select);
   ui->parking_manu_mode->setStyleSheet(mode_deselect);
   ui->parking_standby_mode->setStyleSheet(mode_deselect);

   ui->light_auto_mode->setStyleSheet(mode_deselect);
   ui->light_standby_mode->setStyleSheet(mode_deselect);
   ui->light_clear->setStyleSheet(mode_deselect);
   ui->light_ctl_mode->setStyleSheet(mode_select);
   ui->light_manu_mode->setStyleSheet(mode_deselect);

   ui->brake_auto_mode->setStyleSheet(mode_deselect);
   ui->brake_clear_mode->setStyleSheet(mode_deselect);
   ui->brake_ebs_mode->setStyleSheet(mode_select);
   ui->brake_manu_mode->setStyleSheet(mode_deselect);
   ui->brake_standby_mode->setStyleSheet(mode_deselect);

   ui->pushButton->setStyleSheet(close_button_style);
   ui->pushButton->setText(tr("停止心跳帧"));
}

void Widget::diable_button()
{
    ui->set_open_left_light->setEnabled(false);
    ui->set_open_right_light->setEnabled(false);

    ui->set_open_low_beam_light->setEnabled(false);
    ui->set_open_hight_beam_light->setEnabled(false);
    ui->set_open_warning_light->setEnabled(false);
    ui->set_open_front_fog_light->setEnabled(false);
    ui->set_open_rear_fog_light->setEnabled(false);
    ui->set_close_all_lights->setEnabled(false);
    ui->set_close_all_lights->setEnabled(false);
    ui->set_open_parking->setEnabled(false);

   ui->set_gear_forward->setEnabled(false);
   ui->set_gear_low->setEnabled(false);
   ui->set_gear_neutral->setEnabled(false);
   ui->set_grear_parking->setEnabled(false);
   ui->set_grear_reverse->setEnabled(false);
   ui->gear_clear->setEnabled(false);

   ui->pushButton_7->setEnabled(false);
   ui->set_accel->setEnabled(false);
   ui->set_speed->setEnabled(false);
   ui->set_turnning_angle->setEnabled(false);

   ui->turning_ctl_clear->setEnabled(false);
   ui->turning_ctl_manu->setEnabled(false);
   ui->turning_ctl_mode_btn->setEnabled(false);
   ui->turning_trl_auto->setEnabled(false);
   ui->turin_ctl_eps->setEnabled(false);


   ui->accel_auto_mode->setEnabled(false);
   ui->accel_clear->setEnabled(false);
   ui->accel_ecu_mode->setEnabled(false);
   ui->accel_manu_mode->setEnabled(false);
   ui->accel_mode_crl->setEnabled(false);


   ui->gear_auto_mode->setEnabled(false);
   ui->gear_clear_mode->setEnabled(false);
   ui->gear_crl_mode->setEnabled(false);
   ui->gear_manu_mode->setEnabled(false);
   ui->gear_standby_mode->setEnabled(false);


   ui->parking_auto_mode->setEnabled(false);
   ui->parking_clear->setEnabled(false);
   ui->parking_crl_mode->setEnabled(false);
   ui->parking_manu_mode->setEnabled(false);
   ui->parking_standby_mode->setEnabled(false);

   ui->light_auto_mode->setEnabled(false);
   ui->light_standby_mode->setEnabled(false);
   ui->light_clear->setEnabled(false);
   ui->light_ctl_mode->setEnabled(false);
   ui->light_manu_mode->setEnabled(false);

   ui->brake_auto_mode->setEnabled(false);
   ui->brake_clear_mode->setEnabled(false);
   ui->brake_ebs_mode->setEnabled(false);
   ui->brake_manu_mode->setEnabled(false);
   ui->brake_standby_mode->setEnabled(false);
   ui->pushButton->setEnabled(false);
}

void Widget::enable_button()
{
    ui->set_open_left_light->setEnabled(true);
    ui->set_open_right_light->setEnabled(true);

    ui->set_open_low_beam_light->setEnabled(true);
    ui->set_open_hight_beam_light->setEnabled(true);
    ui->set_open_warning_light->setEnabled(true);
    ui->set_open_front_fog_light->setEnabled(true);
    ui->set_open_rear_fog_light->setEnabled(true);
    ui->set_close_all_lights->setEnabled(true);
    ui->set_close_all_lights->setEnabled(true);
    ui->set_open_parking->setEnabled(true);

   ui->set_gear_forward->setEnabled(true);
   ui->set_gear_low->setEnabled(true);
   ui->set_gear_neutral->setEnabled(true);
   ui->set_grear_parking->setEnabled(true);
   ui->set_grear_reverse->setEnabled(true);
   ui->gear_clear->setEnabled(true);

   ui->pushButton_7->setEnabled(true);
   ui->set_accel->setEnabled(true);
   ui->set_speed->setEnabled(true);
   ui->set_turnning_angle->setEnabled(true);

   ui->turning_ctl_clear->setEnabled(true);
   ui->turning_ctl_manu->setEnabled(true);
   ui->turning_ctl_mode_btn->setEnabled(true);
   ui->turning_trl_auto->setEnabled(true);
   ui->turin_ctl_eps->setEnabled(true);


   ui->accel_auto_mode->setEnabled(true);
   ui->accel_clear->setEnabled(true);
   ui->accel_ecu_mode->setEnabled(true);
   ui->accel_manu_mode->setEnabled(true);
   ui->accel_mode_crl->setEnabled(true);


   ui->gear_auto_mode->setEnabled(true);
   ui->gear_clear_mode->setEnabled(true);
   ui->gear_crl_mode->setEnabled(true);
   ui->gear_manu_mode->setEnabled(true);
   ui->gear_standby_mode->setEnabled(true);


   ui->parking_auto_mode->setEnabled(true);
   ui->parking_clear->setEnabled(true);
   ui->parking_crl_mode->setEnabled(true);
   ui->parking_manu_mode->setEnabled(true);
   ui->parking_standby_mode->setEnabled(true);

   ui->light_auto_mode->setEnabled(true);
   ui->light_standby_mode->setEnabled(true);
   ui->light_clear->setEnabled(true);
   ui->light_ctl_mode->setEnabled(true);
   ui->light_manu_mode->setEnabled(true);

   ui->brake_auto_mode->setEnabled(true);
   ui->brake_clear_mode->setEnabled(true);
   ui->brake_ebs_mode->setEnabled(true);
   ui->brake_manu_mode->setEnabled(true);
   ui->brake_standby_mode->setEnabled(true);
   ui->pushButton->setEnabled(true);
}

void Widget::on_StartOrStopButton_clicked()
{
    if(ui->StartOrStopButton->text() == tr("打开串口"))
       {
          enable_button();
           serial = new QSerialPort;
           serial->setPortName(ui->PortNumber->currentText());
           serial->open(QIODevice::ReadWrite);

           /*设置波特率*/
           serial->setBaudRate(ui->baudBox->currentText().toInt());
           qDebug()<<serial->baudRate();
            /*设置数据位*/
           switch(ui->DataBitsBox->currentText().toInt())
           {
                case 8:serial->setDataBits(QSerialPort::Data8);break;
                case 7:serial->setDataBits(QSerialPort::Data7);break;
                case 6:serial->setDataBits(QSerialPort::Data6);break;
                case 5:serial->setDataBits(QSerialPort::Data5);break;
           default:break;
           }
            qDebug()<<serial->dataBits();
           /*设置校验位*/
           switch(ui->ParityBox->currentIndex())
           {
                case 0:serial->setParity(QSerialPort::NoParity);break;
                case 2:serial->setParity(QSerialPort::EvenParity);break;
                case 1:serial->setParity(QSerialPort::OddParity);break;
           default:break;
           }
           qDebug()<<serial->parity();

           /*设置停止位*/
           switch(ui->StopBitsBox->currentIndex())
           {
                case 0:serial->setStopBits(QSerialPort::OneStop);break;
                case 1:serial->setStopBits(QSerialPort::OneAndHalfStop);break;
                case 2:serial->setStopBits(QSerialPort::TwoStop);break;
              default:break;
           }
              qDebug()<<serial->stopBits();
             serial->setFlowControl(QSerialPort::NoFlowControl);//无控制流

             ui->baudBox->setEnabled(false);
             ui->StopBitsBox->setEnabled(false);
             ui->ParityBox->setEnabled(false);
             ui->DataBitsBox->setEnabled(false);
             ui->PortNumber->setEnabled(false);
             ui->StartOrStopButton->setText(tr("关闭串口"));
        QObject::connect(serial,&QSerialPort::readyRead,this,&Widget::Read_data);
    }
    else
    {
       diable_button();
        serial->clear();
        serial->close();
        serial->deleteLater();

        ui->baudBox->setEnabled(true);
        ui->StopBitsBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->DataBitsBox->setEnabled(true);
        ui->PortNumber->setEnabled(true);
        ui->StartOrStopButton->setText(tr("打开串口"));

        ui->turning_feedback_mode->setText(tr("0"));
        ui->turning_feedback_mode->setText(tr("0"));
        ui->truning_feedback_crtl_value->setText(tr("0"));
        ui->accel_feedback_mode->setText(tr("0"));
        ui->accel_feedback_ctrl_value->setText(tr("0"));
        ui->brake_feedback_mode->setText(tr("0"));
        ui->brake_feedback_crtl_value->setText(tr("0"));
        ui->gear_feedback_mode->setText(tr("0"));
        ui->gear_feedback_crtl_value->setText(tr("0"));
        ui->parking_feedback_mode->setText(tr("0"));
        ui->parking_feedbakc_crtl_value->setText(tr("0"));
        ui->light_feedback_mode->setText(tr("0"));
        ui->light_feedback_crtl_value->setText(tr("0"));
        ui->system_data_speed->setText(tr("0"));
        ui->system_data_vol->setText(tr("0"));
    }
}

char convertCharToHex(char ch)
{
    /*
    0x30等于十进制的48，48也是0的ASCII值，，
    1-9的ASCII值是49-57，，所以某一个值－0x30，，
    就是将字符0-9转换为0-9

    */
    if((ch >= '0') && (ch <= '9'))
         return ch-0x30;
     else if((ch >= 'A') && (ch <= 'F'))
         return ch-'A'+10;
     else if((ch >= 'a') && (ch <= 'f'))
         return ch-'a'+10;
     else return (-1);
}

Widget::Read_data()
{
    QByteArray info=serial->readAll();//在缓冲区中读串口数据
    QByteArray hexData ;//= info.toHex();
    hexData.append(info.toHex());
    QByteArray temp ;

    qDebug()<<"收到数据";
    //qDebug()<<hexData;
    uchar buffer[100] = { 0 };
    uint16_t length;
    char i = 0;
     qDebug()<<"数据长度为："<<hexData.size();
    if(hexData.size() >= 9 * 2)
    {
        if((hexData.at(0) == 'A' && hexData.at(1) == '5'\
           && hexData.at(2) == '5' && hexData.at(3) == 'A') || (hexData.at(0) == 'a' && hexData.at(1) == '5'\
           && hexData.at(2) == '5' && hexData.at(3) == 'a') )
        {
            qDebug()<<"帧头校验成功";
            length = ((convertCharToHex(hexData.at(4)) << 4 )+ convertCharToHex(hexData.at(5))) |\
                    (((convertCharToHex(hexData.at(6)) << 4 )+ (convertCharToHex(hexData.at(7))))<<8);
             qDebug()<<"帧长度为："<<length;

            temp = hexData.mid(0,length * 2) ;
            hexData.remove(0,length * 2);

            qDebug()<<"rx data"<<temp;

            for(char i=0;i< length ; i++)
            {
               buffer[i] = (convertCharToHex(temp.at( 2 * i )) << 4 )+ convertCharToHex(temp.at(2*i + 1));
            }
            i = 0;
            qDebug()<<"buf[0]"<<buffer[0];
            uint16_t crc_result;
            crc_result = crc16_calc(buffer,length);
            qDebug()<<"帧校验结果为："<<crc_result;
            if( crc_result== 0)
            {
                qDebug()<<"crc校验成功";
                switch(buffer[i+4])
                {
                    case 0xa1:
                      ui->turning_feedback_mode->setText(QString::number(buffer[5]));
                      ui->truning_feedback_crtl_value->setText(QString::number((short)(buffer[6] | (buffer[7] <<8))));
                      break;
                    case 0xb1:
                      ui->accel_feedback_mode->setText(QString::number(buffer[5]));
                      ui->accel_feedback_ctrl_value->setText(QString::number((buffer[6] | (buffer[7] << 8))));
                      break;
                    case 0xc1:
                      ui->brake_feedback_mode->setText(QString::number(buffer[5]));
                      ui->brake_feedback_crtl_value->setText(QString::number((buffer[6] | (buffer[7] << 8))));
                      break;
                    case 0xd1:
                      ui->gear_feedback_mode->setText(QString::number(buffer[5]));
                      ui->gear_feedback_crtl_value->setText(QString::number((buffer[6] | (buffer[7] <<8))));
                      break;
                    case 0xe1:
                     qDebug()<<"接收到驻车反馈";
                      ui->parking_feedback_mode->setText(QString::number(buffer[5]));
                      ui->parking_feedbakc_crtl_value->setText(QString::number(buffer[6] | (buffer[7] <<8)));
                      break;
                    case 0xf1:
                      ui->light_feedback_mode->setText(QString::number(buffer[5]));
                      ui->light_feedback_crtl_value->setText(QString::number((buffer[6] | (buffer[7] <<8))));
                      break;
                    case 0xf2:
                      ui->system_data_vol->setText(QString::number(buffer[5]|(buffer[6] << 8)));
                      ui->system_data_speed->setText(QString::number((buffer[7] | (buffer[8] << 8))));
                      break;
                    default:
                      break;
                  }
            }
            else
            {
               qDebug() << "crc校验失败";
            }
        }
        else
        {

            hexData.remove(0,1);
            for(;;)
            {
                 if(hexData.size() >= 10 *2)
                 {
                     if((hexData.at(0) == 'A' && hexData.at(1) == '5'\
                        && hexData.at(2) == '5' && hexData.at(3) == 'A') || (hexData.at(0) == 'a' && hexData.at(1) == '5'\
                        && hexData.at(2) == '5' && hexData.at(3) == 'a') )
                     {
                        qDebug()<<"成功找到帧头";
                        break;
                     }
                     else
                     {
                         qDebug()<<"正在找帧头";
                          hexData.remove(0,1);
                     }

                 }
                 else
                 {
                     break;
                 }

            }
        }

    }
  /*  if(!hexData.isEmpty())
    {

         for (int i = 0; i < hexData.size()/2; i++)
         {
             if(i >= 100 )
                 break;

             buffer[i] = (convertCharToHex(hexData.at( 2 * i )) << 4 )+ convertCharToHex(hexData.at(2*i + 1));
         }
    }
    for(char i = 0;i<50;i++)
        if(buffer[i] == 0xa5 && buffer[i + 1] == 0x5a)
            {
                     uint16_t frame_length ;
                     memcpy(&frame_length,&buffer[i+2],2);

                     qDebug()<<"frame_length ="<<frame_length;
                    if(crc16_calc(&buffer[i],frame_length) == 0)
                      {
                        qDebug()<<"crc校验成功";
                        switch(buffer[i+4])
                        {
                            case 0xa1:
                              qDebug()<<hexData;
                              ui->turning_feedback_mode->setText(QString::number(buffer[5]));
                              ui->truning_feedback_crtl_value->setText(QString::number((short)(buffer[6] | (buffer[7] <<8))));
                              break;
                            case 0xb1:
                              ui->accel_feedback_mode->setText(QString::number(buffer[5]));
                              ui->accel_feedback_ctrl_value->setText(QString::number((buffer[6] | (buffer[7] << 8))));
                              break;
                            case 0xc1:
                              ui->brake_feedback_mode->setText(QString::number(buffer[5]));
                              ui->brake_feedback_crtl_value->setText(QString::number((buffer[6] | (buffer[7] << 8))));
                              break;
                            case 0xd1:
                              ui->gear_feedback_mode->setText(QString::number(buffer[5]));
                              ui->gear_feedback_crtl_value->setText(QString::number((buffer[6] | (buffer[7] <<8))));
                              break;
                            case 0xe1:
                              ui->parking_feedback_mode->setText(QString::number(buffer[5]));
                              ui->parking_feedbakc_crtl_value->setText(QString::number(buffer[6] | (buffer[7] <<8)));
                              break;
                            case 0xf1:
                              ui->light_feedback_mode->setText(QString::number(buffer[5]));
                              ui->light_feedback_crtl_value->setText(QString::number((buffer[6] | (buffer[7] <<8))));
                              break;
                            case 0xf2:
                              ui->system_data_vol->setText(QString::number(buffer[5]|(buffer[6] << 8)));
                              ui->system_data_speed->setText(QString::number((buffer[7] | (buffer[8] << 8))));
                              break;
                            default:
                              break;
                        }
                        return 0;
                    }
                    else
                    {
                        qDebug()<<"crc校验失败";
                       return -1;
                    }

           }*/
    return 0;
}





void Widget::on_turning_ctl_mode_btn_clicked()
{
    ui->turning_ctl_clear->setStyleSheet(mode_deselect);
    ui->turning_ctl_manu->setStyleSheet(mode_deselect);
    ui->turning_ctl_mode_btn->setStyleSheet(mode_select);
    ui->turning_trl_auto->setStyleSheet(mode_deselect);
    ui->turin_ctl_eps->setStyleSheet(mode_deselect);
  sendData(0x11,0);
}

void Widget::on_turning_trl_auto_clicked()
{
    ui->turning_ctl_clear->setStyleSheet(mode_deselect);
    ui->turning_ctl_manu->setStyleSheet(mode_deselect);
    ui->turning_ctl_mode_btn->setStyleSheet(mode_deselect);
    ui->turning_trl_auto->setStyleSheet(mode_select);
    ui->turin_ctl_eps->setStyleSheet(mode_deselect);
   sendData(0x11,1);
}

void Widget::on_turin_ctl_eps_clicked()
{
    ui->turning_ctl_clear->setStyleSheet(mode_deselect);
    ui->turning_ctl_manu->setStyleSheet(mode_deselect);
    ui->turning_ctl_mode_btn->setStyleSheet(mode_deselect);
    ui->turning_trl_auto->setStyleSheet(mode_deselect);
    ui->turin_ctl_eps->setStyleSheet(mode_select);
   sendData(0x11,2);
}

void Widget::on_turning_ctl_manu_clicked()
{
    ui->turning_ctl_clear->setStyleSheet(mode_deselect);
    ui->turning_ctl_manu->setStyleSheet(mode_select);
    ui->turning_ctl_mode_btn->setStyleSheet(mode_deselect);
    ui->turning_trl_auto->setStyleSheet(mode_deselect);
    ui->turin_ctl_eps->setStyleSheet(mode_deselect);
   sendData(0x11,3);
}

void Widget::on_turning_ctl_clear_clicked()
{
    ui->turning_ctl_clear->setStyleSheet(mode_select);
    ui->turning_ctl_manu->setStyleSheet(mode_deselect);
    ui->turning_ctl_mode_btn->setStyleSheet(mode_deselect);
    ui->turning_trl_auto->setStyleSheet(mode_deselect);
    ui->turin_ctl_eps->setStyleSheet(mode_deselect);
    sendData(0x11,4);
}

void Widget::on_set_turnning_angle_clicked()
{
    int16_t data;
    data = ui->angle_input->text().toInt();
    qDebug()<<data;
    sendData(0x12,data);
}



void Widget::on_set_speed_clicked()
{
    int16_t data;
    data = ui->speed_input->text().toInt();
    sendData(0x13,data);
}

void Widget::on_accel_mode_crl_clicked()
{
    ui->accel_auto_mode->setStyleSheet(mode_deselect);
    ui->accel_clear->setStyleSheet(mode_deselect);
    ui->accel_ecu_mode->setStyleSheet(mode_deselect);
    ui->accel_manu_mode->setStyleSheet(mode_deselect);
    ui->accel_mode_crl->setStyleSheet(mode_select);
   sendData(0x21,0);
}

void Widget::on_accel_auto_mode_clicked()
{
    ui->accel_auto_mode->setStyleSheet(mode_select);
    ui->accel_clear->setStyleSheet(mode_deselect);
    ui->accel_ecu_mode->setStyleSheet(mode_deselect);
    ui->accel_manu_mode->setStyleSheet(mode_deselect);
    ui->accel_mode_crl->setStyleSheet(mode_deselect);
   sendData(0x21,1);
}

void Widget::on_accel_ecu_mode_clicked()
{
    ui->accel_auto_mode->setStyleSheet(mode_deselect);
    ui->accel_clear->setStyleSheet(mode_deselect);
    ui->accel_ecu_mode->setStyleSheet(mode_select);
    ui->accel_manu_mode->setStyleSheet(mode_deselect);
    ui->accel_mode_crl->setStyleSheet(mode_deselect);

   sendData(0x21,2);
}

void Widget::on_accel_manu_mode_clicked()
{
    ui->accel_auto_mode->setStyleSheet(mode_deselect);
    ui->accel_clear->setStyleSheet(mode_deselect);
    ui->accel_ecu_mode->setStyleSheet(mode_deselect);
    ui->accel_manu_mode->setStyleSheet(mode_select);
    ui->accel_mode_crl->setStyleSheet(mode_deselect);

  sendData(0x21,3);
}

void Widget::on_accel_clear_clicked()
{
    ui->accel_auto_mode->setStyleSheet(mode_deselect);
    ui->accel_clear->setStyleSheet(mode_select);
    ui->accel_ecu_mode->setStyleSheet(mode_deselect);
    ui->accel_manu_mode->setStyleSheet(mode_deselect);
    ui->accel_mode_crl->setStyleSheet(mode_deselect);

  sendData(0x21,4);
}

void Widget::on_set_accel_clicked()
{
    int16_t data;
    data = ui->accel_input->text().toInt();
    sendData(0x22,data);
}

void Widget::on_brake_standby_mode_clicked()
{
    ui->brake_auto_mode->setStyleSheet(mode_deselect);
    ui->brake_clear_mode->setStyleSheet(mode_deselect);
    ui->brake_ebs_mode->setStyleSheet(mode_deselect);
    ui->brake_manu_mode->setStyleSheet(mode_deselect);
    ui->brake_standby_mode->setStyleSheet(mode_select);
   sendData(0x31,0);
}

void Widget::on_brake_auto_mode_clicked()
{
    ui->brake_auto_mode->setStyleSheet(mode_select);
    ui->brake_clear_mode->setStyleSheet(mode_deselect);
    ui->brake_ebs_mode->setStyleSheet(mode_deselect);
    ui->brake_manu_mode->setStyleSheet(mode_deselect);
    ui->brake_standby_mode->setStyleSheet(mode_deselect);

    sendData(0x31,1);
}

void Widget::on_brake_ebs_mode_clicked()
{
    ui->brake_auto_mode->setStyleSheet(mode_deselect);
    ui->brake_clear_mode->setStyleSheet(mode_deselect);
    ui->brake_ebs_mode->setStyleSheet(mode_select);
    ui->brake_manu_mode->setStyleSheet(mode_deselect);
    ui->brake_standby_mode->setStyleSheet(mode_deselect);
     sendData(0x31,2);
}

void Widget::on_brake_manu_mode_clicked()
{
    ui->brake_auto_mode->setStyleSheet(mode_deselect);
    ui->brake_clear_mode->setStyleSheet(mode_deselect);
    ui->brake_ebs_mode->setStyleSheet(mode_deselect);
    ui->brake_manu_mode->setStyleSheet(mode_select);
    ui->brake_standby_mode->setStyleSheet(mode_deselect);
     sendData(0x31,3);
}

void Widget::on_brake_clear_mode_clicked()
{
    ui->brake_auto_mode->setStyleSheet(mode_deselect);
    ui->brake_clear_mode->setStyleSheet(mode_select);
    ui->brake_ebs_mode->setStyleSheet(mode_deselect);
    ui->brake_manu_mode->setStyleSheet(mode_deselect);
    ui->brake_standby_mode->setStyleSheet(mode_deselect);
   sendData(0x31,4);
}


void Widget::on_set_brake_clicked()
{
    int16_t data;
    data = ui->break_input->text().toInt();
    sendData(0x32,data);
}

void Widget::on_gear_standby_mode_clicked()
{
    ui->gear_auto_mode->setStyleSheet(mode_deselect);
    ui->gear_clear_mode->setStyleSheet(mode_deselect);
    ui->gear_crl_mode->setStyleSheet(mode_deselect);
    ui->gear_manu_mode->setStyleSheet(mode_deselect);
    ui->gear_standby_mode->setStyleSheet(mode_select);
  sendData(0x41,0);
}

void Widget::on_gear_auto_mode_clicked()
{
    ui->gear_auto_mode->setStyleSheet(mode_select);
    ui->gear_clear_mode->setStyleSheet(mode_deselect);
    ui->gear_crl_mode->setStyleSheet(mode_deselect);
    ui->gear_manu_mode->setStyleSheet(mode_deselect);
    ui->gear_standby_mode->setStyleSheet(mode_deselect);
  sendData(0x41,1);
}

void Widget::on_gear_crl_mode_clicked()
{
    ui->gear_auto_mode->setStyleSheet(mode_deselect);
    ui->gear_clear_mode->setStyleSheet(mode_deselect);
    ui->gear_crl_mode->setStyleSheet(mode_select);
    ui->gear_manu_mode->setStyleSheet(mode_deselect);
    ui->gear_standby_mode->setStyleSheet(mode_deselect);
  sendData(0x41,2);
}

void Widget::on_gear_manu_mode_clicked()
{
    ui->gear_auto_mode->setStyleSheet(mode_deselect);
    ui->gear_clear_mode->setStyleSheet(mode_deselect);
    ui->gear_crl_mode->setStyleSheet(mode_deselect);
    ui->gear_manu_mode->setStyleSheet(mode_select);
    ui->gear_standby_mode->setStyleSheet(mode_deselect);
  sendData(0x41,3);
}

void Widget::on_gear_clear_mode_clicked()
{
    ui->gear_auto_mode->setStyleSheet(mode_deselect);
    ui->gear_clear_mode->setStyleSheet(mode_select);
    ui->gear_crl_mode->setStyleSheet(mode_deselect);
    ui->gear_manu_mode->setStyleSheet(mode_deselect);
    ui->gear_standby_mode->setStyleSheet(mode_deselect);
   sendData(0x41,4);
}

void Widget::on_set_grear_parking_clicked()
{
    ui->set_gear_forward->setStyleSheet(close_button_style);
    ui->set_gear_low->setStyleSheet(close_button_style);
    ui->set_gear_neutral->setStyleSheet(close_button_style);
    ui->set_grear_parking->setStyleSheet(open_button_style);
    ui->set_grear_reverse->setStyleSheet(close_button_style);
   sendData(0x42,1);
}

void Widget::on_set_grear_reverse_clicked()
{
    ui->set_gear_forward->setStyleSheet(close_button_style);
    ui->set_gear_low->setStyleSheet(close_button_style);
    ui->set_gear_neutral->setStyleSheet(close_button_style);
    ui->set_grear_parking->setStyleSheet(close_button_style);
    ui->set_grear_reverse->setStyleSheet(open_button_style);
    sendData(0x42,2);
}

void Widget::on_set_gear_neutral_clicked()
{
    ui->set_gear_forward->setStyleSheet(close_button_style);
    ui->set_gear_low->setStyleSheet(close_button_style);
    ui->set_gear_neutral->setStyleSheet(open_button_style);
    ui->set_grear_parking->setStyleSheet(close_button_style);
    ui->set_grear_reverse->setStyleSheet(close_button_style);

   sendData(0x42,3);
}

void Widget::on_set_gear_forward_clicked()
{
    ui->set_gear_forward->setStyleSheet(open_button_style);
    ui->set_gear_low->setStyleSheet(close_button_style);
    ui->set_gear_neutral->setStyleSheet(close_button_style);
    ui->set_grear_parking->setStyleSheet(close_button_style);
    ui->set_grear_reverse->setStyleSheet(close_button_style);
   sendData(0x42,4);
}

void Widget::on_set_gear_low_clicked()
{
    ui->set_gear_forward->setStyleSheet(close_button_style);
    ui->set_gear_low->setStyleSheet(open_button_style);
    ui->set_gear_neutral->setStyleSheet(close_button_style);
    ui->set_grear_parking->setStyleSheet(close_button_style);
    ui->set_grear_reverse->setStyleSheet(close_button_style);
   sendData(0x42,5);
}

void Widget::on_gear_clear_clicked()
{
    ui->set_gear_forward->setStyleSheet(close_button_style);
    ui->set_gear_low->setStyleSheet(close_button_style);
    ui->set_gear_neutral->setStyleSheet(close_button_style);
    ui->set_grear_parking->setStyleSheet(close_button_style);
    ui->set_grear_reverse->setStyleSheet(close_button_style);
    ui->gear_clear->setStyleSheet(close_button_style);
    sendData(0x42,0);
}

void Widget::on_parking_standby_mode_clicked()
{
    ui->parking_auto_mode->setStyleSheet(mode_deselect);
    ui->parking_clear->setStyleSheet(mode_deselect);
    ui->parking_crl_mode->setStyleSheet(mode_deselect);
    ui->parking_manu_mode->setStyleSheet(mode_deselect);
    ui->parking_standby_mode->setStyleSheet(mode_select);
  sendData(0x51,0);
}

void Widget::on_parking_auto_mode_clicked()
{
    ui->parking_auto_mode->setStyleSheet(mode_select);
    ui->parking_clear->setStyleSheet(mode_deselect);
    ui->parking_crl_mode->setStyleSheet(mode_deselect);
    ui->parking_manu_mode->setStyleSheet(mode_deselect);
    ui->parking_standby_mode->setStyleSheet(mode_deselect);

   sendData(0x51,1);
}

void Widget::on_parking_crl_mode_clicked()
{
    ui->parking_auto_mode->setStyleSheet(mode_deselect);
    ui->parking_clear->setStyleSheet(mode_deselect);
    ui->parking_crl_mode->setStyleSheet(mode_select);
    ui->parking_manu_mode->setStyleSheet(mode_deselect);
    ui->parking_standby_mode->setStyleSheet(mode_deselect);
   sendData(0x51,2);
}

void Widget::on_parking_manu_mode_clicked()
{
    ui->parking_auto_mode->setStyleSheet(mode_deselect);
    ui->parking_clear->setStyleSheet(mode_deselect);
    ui->parking_crl_mode->setStyleSheet(mode_deselect);
    ui->parking_manu_mode->setStyleSheet(mode_select);
    ui->parking_standby_mode->setStyleSheet(mode_deselect);
   sendData(0x51,3);
}

void Widget::on_parking_clear_clicked()
{
    ui->parking_auto_mode->setStyleSheet(mode_deselect);
    ui->parking_clear->setStyleSheet(mode_select);
    ui->parking_crl_mode->setStyleSheet(mode_deselect);
    ui->parking_manu_mode->setStyleSheet(mode_deselect);
    ui->parking_standby_mode->setStyleSheet(mode_deselect);
  sendData(0x51,4);
}



void Widget::on_set_open_parking_clicked()
{
   if(ui->set_open_parking->text() == tr("打开驻车"))
   {
        sendData(0x52,1);
        ui->set_open_parking->setText(tr("关闭驻车"));
        ui->set_open_parking->setStyleSheet(close_button_style);
   }
   else
   {
       sendData(0x52,0);
        ui->set_open_parking->setText(tr("打开驻车"));
       ui->set_open_parking->setStyleSheet(open_button_style);
   }




}

void Widget::on_set_close_parking_clicked()
{
   //sendData(0x52,0);
}



void Widget::on_light_standby_mode_clicked()
{
    ui->light_standby_mode->setStyleSheet(mode_select);
    ui->light_clear->setStyleSheet(mode_deselect);
    ui->light_ctl_mode->setStyleSheet(mode_deselect);
    ui->light_manu_mode->setStyleSheet(mode_deselect);
    ui->light_auto_mode->setStyleSheet(mode_deselect);
   sendData(0x61,0);
}

void Widget::on_light_auto_mode_clicked()
{
    ui->light_auto_mode->setStyleSheet(mode_select);
    ui->light_standby_mode->setStyleSheet(mode_deselect);
    ui->light_clear->setStyleSheet(mode_deselect);
    ui->light_ctl_mode->setStyleSheet(mode_deselect);
    ui->light_manu_mode->setStyleSheet(mode_deselect);
  sendData(0x61,1);
}

void Widget::on_light_ctl_mode_clicked()
{
    ui->light_auto_mode->setStyleSheet(mode_deselect);
    ui->light_standby_mode->setStyleSheet(mode_deselect);
    ui->light_clear->setStyleSheet(mode_deselect);
    ui->light_ctl_mode->setStyleSheet(mode_select);
    ui->light_manu_mode->setStyleSheet(mode_deselect);
  sendData(0x61,2);
}

void Widget::on_light_manu_mode_clicked()
{
    ui->light_auto_mode->setStyleSheet(mode_deselect);
    ui->light_standby_mode->setStyleSheet(mode_deselect);
    ui->light_clear->setStyleSheet(mode_deselect);
    ui->light_ctl_mode->setStyleSheet(mode_deselect);
    ui->light_manu_mode->setStyleSheet(mode_select);
    sendData(0x61,3);
}

void Widget::on_light_clear_clicked()
{
    ui->light_auto_mode->setStyleSheet(mode_deselect);
    ui->light_standby_mode->setStyleSheet(mode_deselect);
    ui->light_clear->setStyleSheet(mode_select);
    ui->light_ctl_mode->setStyleSheet(mode_deselect);
    ui->light_manu_mode->setStyleSheet(mode_deselect);
    sendData(0x61,4);
}


uint16_t light_crl_bits = 0;
void Widget::on_set_open_left_light_clicked()
{
    if(ui->set_open_left_light->text() == tr("打开左向灯"))
     {

           light_crl_bits |= (1<<0);
           ui->set_open_left_light->setText(tr("关闭左向灯"));
           ui->set_open_left_light->setStyleSheet(close_button_style);
     }
    else
    {
          light_crl_bits &= ~(1<<0);
          ui->set_open_left_light->setText(tr("打开左向灯"));
          ui->set_open_left_light->setStyleSheet(open_button_style);

    }
   qDebug() << "Light_crt_bits="<<light_crl_bits;
    sendData(0x62,light_crl_bits);
}


void Widget::on_set_open_right_light_clicked()
{

    if(ui->set_open_right_light->text() == tr("打开右向灯"))
     {
         light_crl_bits |= (1<<1);
          ui->set_open_right_light->setText(tr("关闭右向灯"));
          ui->set_open_right_light->setStyleSheet(close_button_style);
     }
    else
    {
        light_crl_bits &= ~(1<<1);
        ui->set_open_right_light->setText(tr("打开右向灯"));
        ui->set_open_right_light->setStyleSheet(open_button_style);
    }
    qDebug() << "Light_crt_bits="<<light_crl_bits;
   sendData(0x62,light_crl_bits);
}

void Widget::on_set_open_low_beam_light_clicked()
{
    if(ui->set_open_low_beam_light->text() == tr("打开近光灯"))
     {
            light_crl_bits |= (1<<2);
           ui->set_open_low_beam_light->setText(tr("关闭近光灯"));
           ui->set_open_low_beam_light->setStyleSheet(close_button_style);
     }
    else
    {
        light_crl_bits &= ~(1<<2);
        ui->set_open_low_beam_light->setText(tr("打开近光灯"));
        ui->set_open_low_beam_light->setStyleSheet(open_button_style);
    }
     qDebug() << "Light_crt_bits="<<light_crl_bits;
    sendData(0x62,light_crl_bits);
}
void Widget::on_set_open_hight_beam_light_clicked()
{
    if(ui->set_open_hight_beam_light->text() == tr("打开远光灯"))
     {
           light_crl_bits |= (1<<3);
           ui->set_open_hight_beam_light->setText(tr("关闭远光灯"));
           ui->set_open_hight_beam_light->setStyleSheet(close_button_style);
     }
    else
    {
       light_crl_bits &= ~(1<<3);
        ui->set_open_hight_beam_light->setText(tr("打开远光灯"));
        ui->set_open_hight_beam_light->setStyleSheet(open_button_style);
    }
     qDebug() << "Light_crt_bits="<<light_crl_bits;
    sendData(0x62,light_crl_bits);
}

void Widget::on_set_open_warning_light_clicked()
{
    if(ui->set_open_warning_light->text() == tr("打开警示灯"))
     {
         light_crl_bits |= (1<<4);
          ui->set_open_warning_light->setText(tr("关闭警示灯"));
          ui->set_open_warning_light->setStyleSheet(close_button_style);
     }
    else
    {
        light_crl_bits &= ~(1<<4);
        ui->set_open_warning_light->setText(tr("打开警示灯"));
        ui->set_open_warning_light->setStyleSheet(open_button_style);
    }
     qDebug() << "Light_crt_bits="<<light_crl_bits;
    sendData(0x62,light_crl_bits);
}


void Widget::on_set_open_front_fog_light_clicked()
{
    if(ui->set_open_front_fog_light->text() == tr("打开前雾灯"))
     {
           light_crl_bits |= (1<<5);
           ui->set_open_front_fog_light->setText(tr("关闭前雾灯"));
           ui->set_open_front_fog_light->setStyleSheet(close_button_style);
     }
    else
    {
        light_crl_bits &= ~(1<<5);
        ui->set_open_front_fog_light->setText(tr("打开前雾灯"));
        ui->set_open_front_fog_light->setStyleSheet(open_button_style);
    }
     qDebug() << "Light_crt_bits="<<light_crl_bits;
    sendData(0x62,light_crl_bits);
}

void Widget::on_set_open_rear_fog_light_clicked()
{
    if(ui->set_open_rear_fog_light->text() == tr("打开后雾灯"))
     {
           light_crl_bits |= (1<<6);
           ui->set_open_rear_fog_light->setText(tr("关闭后雾灯"));
           ui->set_open_rear_fog_light->setStyleSheet(close_button_style);
     }
    else
    {
        light_crl_bits &= ~(1<<6);
        ui->set_open_rear_fog_light->setText(tr("打开后雾灯"));
        ui->set_open_rear_fog_light->setStyleSheet(open_button_style);
    }
     qDebug() << "Light_crt_bits="<<light_crl_bits;
    sendData(0x62,light_crl_bits);
}

void Widget::on_set_close_all_lights_clicked()
{
    ui->set_open_left_light->setText(tr("打开左向灯"));
    ui->set_open_left_light->setStyleSheet(open_button_style);

    ui->set_open_right_light->setText(tr("打开右向灯"));
    ui->set_open_right_light->setStyleSheet(open_button_style);

    ui->set_open_low_beam_light->setText(tr("打开近光灯"));
    ui->set_open_low_beam_light->setStyleSheet(open_button_style);

    ui->set_open_hight_beam_light->setText(tr("打开远光灯"));
    ui->set_open_hight_beam_light->setStyleSheet(open_button_style);

    ui->set_open_warning_light->setText(tr("打开警示灯"));
    ui->set_open_warning_light->setStyleSheet(open_button_style);

    ui->set_open_front_fog_light->setText(tr("打开前雾灯"));
    ui->set_open_front_fog_light->setStyleSheet(open_button_style);

    ui->set_open_rear_fog_light->setText(tr("打开后雾灯"));
    ui->set_open_rear_fog_light->setStyleSheet(open_button_style);

    light_crl_bits  = 0;

     qDebug() << "Light_crt_bits="<<light_crl_bits;

    sendData(0x62,light_crl_bits);
}



void Widget::on_pushButton_7_clicked()
{
    int16_t data;
    data = ui->break_input->text().toInt();
    sendData(0x32,data);
}

void Widget::on_pushButton_clicked()
{
    if(ui->pushButton->text() == tr("停止心跳帧"))
        {
           ui->pushButton->setStyleSheet(open_button_style);
           ui->pushButton->setText(tr("发送心跳帧"));
           sendData(0x71,0);
       }
    else
    {
        ui->pushButton->setStyleSheet(close_button_style);
        ui->pushButton->setText(tr("停止心跳帧"));
        sendData(0x71,1);
    }

}

void Widget::on_reset_all_clicked()
{
    rest_all();
}
