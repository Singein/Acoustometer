#include "lang.h"

LANG::LANG(int languageType)
{
    this->languageType = languageType;
    init();
}

void LANG::init()
{
    switch (languageType) {
    case CHINESE:
        applicationTitle = "声强检测仪";
        deviceSetting = "仪器设置";
        portSetting = "串口设置";
        deviceList = "设备列表";
        device = "测量仪"; //测量仪 Device
        realTimeData = "实时数据";//实时数据  realtime data
        sampling = "实时数据 ==> 正在采集";//实时数据 ==> 正在采集  realtime data ==> sampling
        historyData = "历史数据";//历史数据   history data
        warring = "警告";//警告 Warring
        warringInfo = "请先停止正在采集的实时数据!";//请先停止正在采集的实时数据! Please stop sampling the realtime data first!
        connectError = "连接错误";//连接错误 Connect Error
        connectErrorInfo = "与下位机连接异常!";//与下位机连接异常! Can not connect to the device!
        buttonPlot = "二维曲线";//二维曲线 2D-Plot
        buttonStart = "开始采集";//开始采集 Start
        buttonStop = "停止采集";//停止采集 Stop
        buttonExport = "导出";//导出 Export
        buttonConnect = "连接串口";
        buttonDisConnect = "断开连接";
        tableTitleDateTime = "日期/时间";// 日期/时间  DateTime
        tableTitleStrength = "声强(W/cm²)"; // 声强    Acoustical Strength
        tableTitleFrequency = "频率(KHz)";// 频率    Frequency
        exporting = "正在导出数据...";
        selection = "当前选中: ";
        break;
    case ENGLISH:
        applicationTitle = "Acoustometer";
        deviceSetting = "Device Settings";
        portSetting = "Serial Settings";
        deviceList = "Device List";
        device = "Device"; //测量仪 Device
        realTimeData = "realtime data";//实时数据  realtime data
        sampling = "realtime data ==> sampling";//实时数据 ==> 正在采集  realtime data ==> sampling
        historyData = "history data";//历史数据   history data
        warring = "Warring";//警告 Warring
        warringInfo = "Please stop sampling the realtime data first!";//请先停止正在采集的实时数据! Please stop sampling the realtime data first!
        connectError = "Connect Error";//连接错误 Connect Error
        connectErrorInfo = "Can not connect to the device!";//与下位机连接异常! Can not connect to the device!
        buttonPlot = "2D-Plot";//二维曲线 2D-Plot
        buttonStart = "Start";//开始采集 Start
        buttonStop = "Stop";//停止采集 Stop
        buttonExport = "Export";//导出 Export
        buttonConnect = "Connect";
        buttonDisConnect = "Disconnect";
        tableTitleDateTime = "DateTime";// 日期/时间  DateTime
        tableTitleStrength = "Acoustical Strength(W/cm²)"; // 声强    Acoustical Strength
        tableTitleFrequency = "Frequency(KHz)";// 频率    Frequency
        exporting = "exporting...";
        selection = "selection: ";
        break;
    default:
        break;
    }
}

//QString LANG::selection(int id)
//{
//    QString s = "";
//    switch (languageType) {
//    case CHINESE:
//        s += "当前选中:测量仪 " + QString::number(id);
//        break;
//    case ENGLISH:
//        s += "selection: device " + QString::number(id);
//        break;
//    default:
//        break;
//    }
//    return s;
//}

QString LANG::formContent(int id, int type)
{
    QString s = "";

    switch (languageType) {
    case CHINESE:
        s += "表格内容:测量仪 " + QString::number(id);
        switch (type) {
        case 0:
            s += " 实时数据";
            break;
        case 1:
            s += " 历史数据";
            break;
        default:
            break;
        }
        break;
    case ENGLISH:
        s += "form content: device " + QString::number(id);
        switch (type) {
        case 0:
            s += " realtime data";
            break;
        case 1:
            s += " history data";
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return s;
}

QString LANG::connectedDevices(int count)
{
    QString s = "";
    switch (languageType) {
    case CHINESE:
        s += "已连接设备数:" + QString::number(count);
        break;
    case ENGLISH:
        s += "the number of connected devices:" + QString::number(count);
        break;
    default:
        break;
    }
    return s;
}
