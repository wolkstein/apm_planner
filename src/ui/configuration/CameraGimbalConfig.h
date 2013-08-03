
#ifndef CAMERAGIMBALCONFIG_H
#define CAMERAGIMBALCONFIG_H

#include <QWidget>
#include "AP2ConfigWidget.h"
#include "ui_CameraGimbalConfig.h"

enum RC_FUNCTION {
    Disabled = 0,
    Manual = 1,
    Flap = 2,
    Flap_auto = 3,
    Aileron = 4,
    flaperon = 5,
    mount_pan = 6,
    mount_tilt = 7,
    mount_roll = 8,
    mount_open = 9,
    camera_trigger = 10,
    release = 11,
    mount2_pan = 12,
    mount2_tilt = 13,
    mount2_roll = 14,
    mount2_open = 15,
    DifferentialSpoiler1 = 16,
    DifferentialSpoiler2 = 17,
    AileronWithInput = 18,
    Elevator = 19,
    ElevatorWithInput = 20,
    Rudder = 21 };

class CameraGimbalConfig : public AP2ConfigWidget
{
    Q_OBJECT
    
public:
    explicit CameraGimbalConfig(QWidget *parent = 0);
    ~CameraGimbalConfig();

public slots:
    void activeUASSet(UASInterface *uas);

private slots:
    void parameterChanged(int uas, int component, QString parameterName, QVariant value);

    void updateTilt();
    void updateTilt(int index);
    void updateRoll();
    void updateRoll(int index);
    void updatePan();
    void updatePan(int index);

    void refreshTilt();
    void refreshRoll();
    void refreshPan();

    void updateShutter();
    void updateShutter(int index);

    void updateRetractAngles();
    void updateNeutralAngles();
    void updateControlAngles();



private:
    void initConnections();
    void setRCParameters(MAV_COMPONENT component, QString channel,
                         RC_FUNCTION function, int min, int max, bool reverse);
    void setMountParameters(MAV_COMPONENT component, QString type, int minAngle, int maxAngle, int inChannel);
    void addOutputRcChannels(QComboBox *comboBox);
    void addInputRcChannels(QComboBox *comboBox);

    void refreshMountParameters(QString mount, QString parameter, QVariant &value);
    void refreshCameraParameters(QString parameter, QVariant value);
    void refreshRcFunctionComboxBox(QString rcChannelName, QVariant &value);

private:
    Ui::CameraGimbalConfig ui;
    QString m_shutterPrefix;
    QString m_rollPrefix;
    QString m_tiltPrefix;
    QString m_panPrefix;

    QList<QString> m_cameraParams;
};

#endif // CAMERAGIMBALCONFIG_H
