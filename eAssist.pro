#-------------------------------------------------
#
# Project created by QtCreator 2013-07-11T11:51:14
#
#-------------------------------------------------

QT       += network core gui quick qml

CONFIG   += c++11 static

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = eAssist
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    firmwaredumpviewer.cpp \
    hexedit/xbytearray.cpp \
    hexedit/qhexedit_p.cpp \
    hexedit/qhexedit.cpp \
    hexedit/commands.cpp \
    DockLogger.cpp \
    SignalsTree.cpp \
    QCP/qcustomplot.cpp \
    TelData.cpp \
    threadtelreaddata.cpp \
    ED/EDCommand.cpp \
    ED/EDCommandDispatcher.cpp \
    ED/EDUdpInterface.cpp \
    ED/EDCommandGroup.cpp \
    ED/Prg/EDFirmware.cpp \
    ED/Prg/EDPrgProgress.cpp \
    ED/Prg/EDPrgCom_FinishProg.cpp \
    ED/Prg/EDPrgCom_PrepForProg.cpp \
    ED/Prg/EDPrgCom_Read.cpp \
    ED/Prg/EDPrgCom_ReadDescr.cpp \
    ED/Prg/EDPrgCom_ReadID.cpp \
    ED/Prg/EDPrgCom_ReqDevNum.cpp \
    ED/Prg/EDPrgCom_Restart.cpp \
    ED/Prg/EDPrgCom_Write.cpp \
    ED/Prg/EDPrgGrp.cpp \
    ED/Sig/EDSigCom_Ctrl.cpp \
    ED/Sig/EDSigCom_ReadValue.cpp \
    ED/Sig/EDSigCom_ReqDescr.cpp \
    ED/Sig/EDSigCom_ReqNum.cpp \
    ED/Sig/EDSigCom_SetSignature.cpp \
    ED/Sig/EDSignal.cpp \
    ED/Sig/EDSigGrp.cpp \
    ED/sys/EDSysCom_CheckChannel.cpp \
    ED/sys/EDSysCom_ReqDevInfo.cpp \
    ED/sys/EDSysCom_Reset.cpp \
    ED/sys/EDSysCom_SaveSettings.cpp \
    ED/sys/EDSysCom_SelfTest.cpp \
    ED/sys/EDSysCom_SetScanMode.cpp \
    ED/sys/EDSysCom_TestChanel.cpp \
    ED/sys/EDSysGrp.cpp \
    ED/Tel/EDTel_SignalData.cpp \
    ED/Tel/EDTel_SignalDataBuffer.cpp \
    ED/Tel/EDTelCom_AddSignal.cpp \
    ED/Tel/EDTelCom_ReadData.cpp \
    ED/Tel/EDTelCom_ReadDescr.cpp \
    ED/Tel/EDTelCom_SetMode.cpp \
    ED/Tel/EDTelGrp.cpp \
    ED/EDILogger.cpp \
    ED/EDLogger.cpp \
    SettingsDlg.cpp \
    Settings.cpp \
    SignalsDockWidget.cpp \
    SignalsWidget.cpp \
    ED/ED.cpp \
    Firmware.cpp \
    ED/Sig/EDSignalsList.cpp \
    TelemetryWidget.cpp \
    TelemetryPlot.cpp \
    Telemetry.cpp \
    ED/EDComPortInterface.cpp \
    QextSerialPort/qextserialport.cpp \
    QextSerialPort/win_qextserialport.cpp \
    QextSerialPort/qextserialbase.cpp \
    SForm/SFormWidget.cpp \
    SForm/SFormItemValue.cpp \
    SForm/SFormItemTextField.cpp \
    SForm/SFormItemText.cpp \
    SForm/SFormItemBool.cpp \
    SForm/SFormItem.cpp \
    SForm/SFormDockWidget.cpp

HEADERS  += mainwindow.h \
    firmwaredumpviewer.h \
    hexedit/xbytearray.h \
    hexedit/qhexedit_p.h \
    hexedit/qhexedit.h \
    hexedit/commands.h \
    DockLogger.h \
    SignalsTree.h \
    QCP/qcustomplot.h \
    TelData.h \
    threadtelreaddata.h \
    ED/EDCommand.h \
    ED/EDCommandDispatcher.h \
    ED/EDUdpInterface.h \
    ED/EDCommandGroup.h \
    ED/Prg/EDFirmware.h \
    ED/Prg/EDPrgProgress.h \
    ED/Prg/EDPrgCom_FinishProg.h \
    ED/Prg/EDPrgCom_PrepForProg.h \
    ED/Prg/EDPrgCom_Read.h \
    ED/Prg/EDPrgCom_ReadDescr.h \
    ED/Prg/EDPrgCom_ReadID.h \
    ED/Prg/EDPrgCom_ReqDevNum.h \
    ED/Prg/EDPrgCom_Restart.h \
    ED/Prg/EDPrgCom_Write.h \
    ED/Prg/EDPrgGrp.h \
    ED/Sig/EDSigCom_Ctrl.h \
    ED/Sig/EDSigCom_ReadValue.h \
    ED/Sig/EDSigCom_ReqDescr.h \
    ED/Sig/EDSigCom_ReqNum.h \
    ED/Sig/EDSigCom_SetSignature.h \
    ED/Sig/EDSignal.h \
    ED/Sig/EDSigGrp.h \
    ED/sys/EDSysCom_CheckChannel.h \
    ED/sys/EDSysCom_ReqDevInfo.h \
    ED/sys/EDSysCom_Reset.h \
    ED/sys/EDSysCom_SaveSettings.h \
    ED/sys/EDSysCom_SelfTest.h \
    ED/sys/EDSysCom_SetScanMode.h \
    ED/sys/EDSysCom_TestChanel.h \
    ED/sys/EDSysGrp.h \
    ED/Tel/EDTel_SignalData.h \
    ED/Tel/EDTel_SignalDataBuffer.h \
    ED/Tel/EDTelCom_AddSignal.h \
    ED/Tel/EDTelCom_ReadData.h \
    ED/Tel/EDTelCom_ReadDescr.h \
    ED/Tel/EDTelCom_SetMode.h \
    ED/Tel/EDTelGrp.h \
    ED/EDILogger.h \
    ED/EDLogger.h \
    SettingsDlg.h \
    Settings.h \
    SignalsDockWidget.h \
    SignalsWidget.h \
    ED/ED.h \
    Firmware.h \
    ED/Sig/EDSignalsList.h \
    TelemetryWidget.h \
    TelemetryPlot.h \
    Telemetry.h \
    ED/EDComPortInterface.h \
    QextSerialPort/qextserialport.h \
    QextSerialPort/win_qextserialport.h \
    QextSerialPort/qextserialbase.h \
    SForm/SFormWidget.h \
    SForm/SFormItemValue.h \
    SForm/SFormItemTextField.h \
    SForm/SFormItemText.h \
    SForm/SFormItemBool.h \
    SForm/SFormItem.h \
    SForm/SFormDockWidget.h

FORMS    += mainwindow.ui \
    firmwaredumpviewer.ui \
    settings.ui

RESOURCES += \
    images.qrc

OTHER_FILES += \
    qml/test.qml \
    qml/stim210.qml \
    ramka_pult.qml \
    QML/ramka_pult.qml \
    QML/thales_pult.qml

DISTFILES += \
    QML/7155_pult.qml \
    QML/7599_pult.qml \
    QML/7599_pult_rus.qml
