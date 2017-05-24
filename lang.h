#ifndef LANG_H
#define LANG_H

#include <QString>
#define CHINESE 0
#define ENGLISH 1


class LANG
{
public:
    LANG(int languageType);
    QString applicationTitle;//声强检测仪  Acoustometer
    QString deviceSetting;//仪器设置 Device Settings
    QString portSetting;//串口设置  Serial Settings

    QString deviceList;//设备列表   Device List
    QString device; //测量仪 Device
    QString realTimeData;//实时数据  realtime data
    QString sampling;//实时数据 ==> 正在采集  realtime data ==> sampling
    QString historyData;//历史数据   history data

    QString warring;//警告 Warring
    QString warringInfo;//请先停止正在采集的实时数据! Please stop sampling the realtime data first!
    QString connectError;//连接错误 Connect Error
    QString connectErrorInfo;//与下位机连接异常! Can not connect to the device!

    QString buttonPlot;//二维曲线 2D-Plot
    QString buttonStart;//开始采集 Start
    QString buttonStop;//停止采集 Stop
    QString buttonExport;//导出 Export

    QString tableTitleDateTime;// 日期/时间  DateTime
    QString tableTitleStrength; // 声强    Acoustical Strength
    QString tableTitleFrequency;// 频率    Frequency

    QString exporting;

    QString selection;//当前选中:测量仪 1 selection: device 1
    QString formContent(int id,int type);//表格内容:测量仪 1 实时数据/历史数据  form content: device 1 realtime data
    QString connectedDevices(int count);//已连接设备数:1  the number of connected devices:2



private:
    int languageType;
    void init();
};

#endif // LANG_H
