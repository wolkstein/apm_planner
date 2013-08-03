#include <QMessageBox>
#include <QDebug>

#include "CameraGimbalConfig.h"

CameraGimbalConfig::CameraGimbalConfig(QWidget *parent) : AP2ConfigWidget(parent)
{
    ui.setupUi(this);

    addOutputRcChannels(ui.tiltChannelComboBox);
    addOutputRcChannels(ui.rollChannelComboBox);
    addOutputRcChannels(ui.panChannelComboBox);
    addOutputRcChannels(ui.shutterChannelComboBox);

    addInputRcChannels(ui.tiltInputChannelComboBox);
    addInputRcChannels(ui.rollInputChannelComboBox);
    addInputRcChannels(ui.panInputChannelComboBox);

    initConnections();
}

void CameraGimbalConfig::initConnections()
{
    connect(ui.tiltServoMinSpinBox,SIGNAL(editingFinished()),this,SLOT(updateTilt()));
    connect(ui.tiltServoMaxSpinBox,SIGNAL(editingFinished()),this,SLOT(updateTilt()));
    connect(ui.tiltAngleMinSpinBox,SIGNAL(editingFinished()),this,SLOT(updateTilt()));
    connect(ui.tiltAngleMaxSpinBox,SIGNAL(editingFinished()),this,SLOT(updateTilt()));
    connect(ui.tiltChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateTilt(int)));
    connect(ui.tiltInputChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateTilt(int)));
    connect(ui.tiltReverseCheckBox,SIGNAL(clicked(bool)),this,SLOT(updateTilt()));
    connect(ui.tiltStabilizeCheckBox,SIGNAL(clicked(bool)),this,SLOT(updateTilt()));

    connect(ui.rollServoMinSpinBox,SIGNAL(editingFinished()),this,SLOT(updateRoll()));
    connect(ui.rollServoMaxSpinBox,SIGNAL(editingFinished()),this,SLOT(updateRoll()));
    connect(ui.rollAngleMinSpinBox,SIGNAL(editingFinished()),this,SLOT(updateRoll()));
    connect(ui.rollAngleMaxSpinBox,SIGNAL(editingFinished()),this,SLOT(updateRoll()));
    connect(ui.rollChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateRoll(int)));
    connect(ui.rollInputChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateRoll(int)));
    connect(ui.rollReverseCheckBox,SIGNAL(clicked(bool)),this,SLOT(updateRoll()));
    connect(ui.rollStabilizeCheckBox,SIGNAL(clicked(bool)),this,SLOT(updateRoll()));

    connect(ui.panServoMinSpinBox,SIGNAL(editingFinished()),this,SLOT(updatePan()));
    connect(ui.panServoMaxSpinBox,SIGNAL(editingFinished()),this,SLOT(updatePan()));
    connect(ui.panAngleMinSpinBox,SIGNAL(editingFinished()),this,SLOT(updatePan()));
    connect(ui.panAngleMaxSpinBox,SIGNAL(editingFinished()),this,SLOT(updatePan()));
    connect(ui.panChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updatePan(int)));
    connect(ui.panInputChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updatePan(int)));
    connect(ui.panReverseCheckBox,SIGNAL(clicked(bool)),this,SLOT(updatePan()));
    connect(ui.panStabilizeCheckBox,SIGNAL(clicked(bool)),this,SLOT(updatePan()));


    connect(ui.shutterServoMinSpinBox,SIGNAL(editingFinished()),this,SLOT(updateShutter()));
    connect(ui.shutterServoMaxSpinBox,SIGNAL(editingFinished()),this,SLOT(updateShutter()));
    connect(ui.shutterPushedSpinBox,SIGNAL(editingFinished()),this,SLOT(updateShutter()));
    connect(ui.shutterNotPushedSpinBox,SIGNAL(editingFinished()),this,SLOT(updateShutter()));
    connect(ui.shutterDurationSpinBox,SIGNAL(editingFinished()),this,SLOT(updateShutter()));
    connect(ui.shutterChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateShutter(int)));

    connect(ui.retractXSpinBox,SIGNAL(editingFinished()),this,SLOT(updateRetractAngles()));
    connect(ui.retractYSpinBox,SIGNAL(editingFinished()),this,SLOT(updateRetractAngles()));
    connect(ui.retractZSpinBox,SIGNAL(editingFinished()),this,SLOT(updateRetractAngles()));

    connect(ui.controlXSpinBox,SIGNAL(editingFinished()),this,SLOT(updateControlAngles()));
    connect(ui.controlYSpinBox,SIGNAL(editingFinished()),this,SLOT(updateControlAngles()));
    connect(ui.controlZSpinBox,SIGNAL(editingFinished()),this,SLOT(updateControlAngles()));

    connect(ui.neutralXSpinBox,SIGNAL(editingFinished()),this,SLOT(updateNeutralAngles()));
    connect(ui.neutralYSpinBox,SIGNAL(editingFinished()),this,SLOT(updateNeutralAngles()));
    connect(ui.neutralZSpinBox,SIGNAL(editingFinished()),this,SLOT(updateNeutralAngles()));
    AP2ConfigWidget::initConnections();
}


void CameraGimbalConfig::addOutputRcChannels(QComboBox* comboBox)
{
    comboBox->addItem(tr("Disable"), 0);
    comboBox->addItem("RC5", 5);
    comboBox->addItem("RC6", 6);
    comboBox->addItem("RC7", 7);
    comboBox->addItem("RC8", 8);
    comboBox->addItem("RC9 (APlane)", 9);
    comboBox->addItem("RC10", 10);
    comboBox->addItem("RC11", 11);

}

void CameraGimbalConfig::addInputRcChannels(QComboBox* comboBox)
{
    comboBox->addItem(tr("None", 0));
    comboBox->addItem("RC5", 5);
    comboBox->addItem("RC6", 6);
    comboBox->addItem("RC7", 7);
    comboBox->addItem("RC8", 8);
}


void CameraGimbalConfig::activeUASSet(UASInterface *uas)
{
    AP2ConfigWidget::activeUASSet(uas);
    if (!uas) return;
    // Get all the following Parameters

    for (int channelCount=5; channelCount <= 11; ++channelCount) {
        QString function = "RC?_FUNCTION";
        function.replace(2,1,QString::number(channelCount));
        QString min = "RC?_MIN";
        min.replace(2,1,QString::number(channelCount));
        QString max = "RC?_MAX";
        max.replace(2,1,QString::number(channelCount));
        QString trim = "RC?_TRIM";
        trim.replace(2,1,QString::number(channelCount));
        QString rev = "RC?_REV";
        rev.replace(2,1,QString::number(channelCount));
        QString dz =  "RC?_DZ";
        dz.replace(2,1,QString::number(channelCount));

        m_cameraParams << function << min << max << trim << rev << dz;
    }

    m_cameraParams << "MNT_RETRACT_X"
            << "MNT_RETRACT_Y"
            << "MNT_RETRACT_Z"
            << "MNT_NEUTRAL_X"

            << "MNT_NEUTRAL_Y"
            << "MNT_NEUTRAL_Z"

            << "MNT_CONTROL_X"
            << "MNT_CONTROL_Y"
            << "MNT_CONTROL_Z"

            << "MNT_STAB_TILT"
            << "MNT_STAB_PAN"

            << "MNT_RC_IN_ROLL"
            << "MNT_ANGMIN_ROL"
            << "MNT_ANGMAX_ROL"

            << "MNT_RC_IN_TILT"
            << "MNT_ANGMIN_TIL"
            << "MNT_ANGMAX_TIL"

            << "MNT_RC_IN_PAN"
            << "MNT_ANGMIN_PAN"
            << "MNT_ANGMAX_PAN"

            << "MNT_JSTICK_SPD"

            << "MNT2_STAB_TILT"
            << "MNT2_STAB_PAN"

            << "MNT2_RC_IN_ROLL"
            << "MNT2_ANGMIN_ROL"
            << "MNT2_ANGMAX_ROL"

            << "MNT2_RC_IN_TILT"
            << "MNT2_ANGMIN_TIL"
            << "MNT2_ANGMAX_TIL"

            << "MNT2_RC_IN_PAN"
            << "MNT2_ANGMIN_PAN"
            << "MNT2_ANGMAX_PAN"

            << "MNT2_JSTICK_SPD";

    qDebug() << "cameraParams" << m_cameraParams;

    QGCUASParamManager *pm = m_uas->getParamManager();
    foreach(QString parameter, m_cameraParams) {
        pm->requestParameterUpdate(1, parameter);
    };
}

void CameraGimbalConfig::updateRetractAngles()
{
    if (showNullMAVErrorMessageBox())
        return;

    m_uas->getParamManager()->setParameter(1,"MNT_RETRACT_X",ui.retractXSpinBox->value());
    m_uas->getParamManager()->setParameter(1,"MNT_RETRACT_Y",ui.retractYSpinBox->value());
    m_uas->getParamManager()->setParameter(1,"MNT_RETRACT_Z",ui.retractZSpinBox->value());
}

void CameraGimbalConfig::updateNeutralAngles()
{
    if (showNullMAVErrorMessageBox())
        return;

    m_uas->getParamManager()->setParameter(1,"MNT_NEUTRAL_X",ui.neutralXSpinBox->value());
    m_uas->getParamManager()->setParameter(1,"MNT_NEUTRAL_Y",ui.neutralYSpinBox->value());
    m_uas->getParamManager()->setParameter(1,"MNT_NEUTRAL_Z",ui.neutralZSpinBox->value());
}

void CameraGimbalConfig::updateControlAngles()
{
    if (showNullMAVErrorMessageBox())
        return;

    m_uas->getParamManager()->setParameter(1,"MNT_CONTROL_X",ui.controlXSpinBox->value());
    m_uas->getParamManager()->setParameter(1,"MNT_CONTROL_Y",ui.controlYSpinBox->value());
    m_uas->getParamManager()->setParameter(1,"MNT_CONTROL_Z",ui.controlZSpinBox->value());
}

void CameraGimbalConfig::setRCParameters(MAV_COMPONENT component, QString channel,
                                         RC_FUNCTION function, int min, int max, bool reverse)
{
    qDebug() << "Setting RC Parameters Channel:" << channel << function
             << " min:" << min << " max:" << max << " reverse:" << reverse << "comp:" << component;
    QGCUASParamManager *pm = m_uas->getParamManager();
    pm->setParameter(1, "RC" + channel + "_FUNCTION", function);
    pm->setParameter(1, "RC" + channel + "_MIN",min);
    pm->setParameter(1, "RC" + channel + "_MAX",max);
    pm->setParameter(1, "RC" + channel + "_REV",reverse);
}

void CameraGimbalConfig::setMountParameters(MAV_COMPONENT component, QString type, int minAngle, int maxAngle, int inChannel)
{
    qDebug() << "Setting Mount Parameters input Channel:" << inChannel
             << " minAngle:" << minAngle << " maxAngle:" << maxAngle << "comp:" << component;
    QGCUASParamManager *pm = m_uas->getParamManager();
    pm->setParameter(1, "MNT_RC_IN_" + type , inChannel);
    type.resize(3); // makes the string either ROL or TIL
    pm->setParameter(1, "MNT_ANGMIN_" + type, minAngle * 100); // centiDegrees
    pm->setParameter(1, "MNT_ANGMAX_" + type, maxAngle * 100); // centiDegrees
}

void CameraGimbalConfig::updateTilt(int index)
{
//    if (ui.tiltChannelComboBox->currentIndex() == index)
//        return;
    updateTilt();
}

void CameraGimbalConfig::updateTilt()
{
    if (showNullMAVErrorMessageBox())
        return;

    if (!m_tiltPrefix.isEmpty())
    {
        //We need to set this to 0 for disabled.
        m_uas->getParamManager()->setParameter(1,m_tiltPrefix + "FUNCTION",0);
    }
    if (ui.tiltChannelComboBox->currentIndex() == 0)
    {
        //Disabled
        return;
    }

    QString channel = ui.tiltChannelComboBox->itemData(ui.tiltChannelComboBox->currentIndex()).toString();

    // component is currently ignored by APM, so MAV_COMP_ID_CAMERA seems ideal.
    setRCParameters(MAV_COMP_ID_CAMERA, channel, mount_tilt,
                    ui.tiltServoMinSpinBox->value(),
                    ui.tiltServoMaxSpinBox->value(),
                    ui.tiltReverseCheckBox->isChecked() );

    int inChannel = ui.tiltInputChannelComboBox->itemData(ui.tiltInputChannelComboBox->currentIndex()).toInt();

    setMountParameters(MAV_COMP_ID_CAMERA, "TILT",
                       ui.tiltAngleMinSpinBox->value(),
                       ui.tiltAngleMaxSpinBox->value(),
                       inChannel);
}

void CameraGimbalConfig::updateRoll(int index)
{
    if (ui.rollChannelComboBox->currentIndex() == index)
        return;
    updateRoll();
}

void CameraGimbalConfig::updateRoll()
{
    if (showNullMAVErrorMessageBox())
        return;

    if (!m_rollPrefix.isEmpty())
    {
        //We need to set this to 0 for disabled.
        m_uas->getParamManager()->setParameter(1,m_rollPrefix + "FUNCTION",0);
    }
    if (ui.rollChannelComboBox->currentIndex() == 0)
    {
        //Disabled
        return;
    }

    QString channel = ui.rollChannelComboBox->itemData(ui.rollChannelComboBox->currentIndex()).toString();
    setRCParameters(MAV_COMP_ID_CAMERA, channel, mount_roll,
                    ui.rollServoMinSpinBox->value(),
                    ui.rollServoMaxSpinBox->value(),
                    ui.rollReverseCheckBox->isChecked());

    int inChannel = ui.rollInputChannelComboBox->itemData(ui.rollInputChannelComboBox->currentIndex()).toInt();

    setMountParameters(MAV_COMP_ID_CAMERA, "ROLL",
                       ui.rollAngleMinSpinBox->value(),
                       ui.rollAngleMaxSpinBox->value(),
                       inChannel);


}

void CameraGimbalConfig::updatePan(int index)
{
    if (ui.panChannelComboBox->currentIndex() == index)
        return;
    updatePan();
}

void CameraGimbalConfig::updatePan()
{
    if (showNullMAVErrorMessageBox())
        return;

    if (!m_panPrefix.isEmpty())
    {
        //We need to set this to 0 for disabled.
        m_uas->getParamManager()->setParameter(1,m_panPrefix + "FUNCTION",0);
    }
    if (ui.panChannelComboBox->currentIndex() == 0)
    {
        //Disabled
        return;
    }

    QString channel = ui.panChannelComboBox->itemData(ui.panChannelComboBox->currentIndex()).toString();
    setRCParameters(MAV_COMP_ID_CAMERA, channel, mount_pan,
                    ui.panServoMinSpinBox->value(),
                    ui.panServoMaxSpinBox->value(),
                    ui.panReverseCheckBox->isChecked());

    int inChannel = ui.panInputChannelComboBox->itemData(ui.panInputChannelComboBox->currentIndex()).toInt();

    setMountParameters(MAV_COMP_ID_CAMERA, "PAN",
                       ui.panAngleMinSpinBox->value(),
                       ui.panAngleMaxSpinBox->value(),
                       inChannel);
}

void CameraGimbalConfig::updateShutter(int newIndex)
{
    if (ui.shutterChannelComboBox->currentIndex() == newIndex)
        return; // Guard to stop recursion
    updateShutter();
}

void CameraGimbalConfig::updateShutter()
{
    if (showNullMAVErrorMessageBox())
        return;

    if (ui.shutterChannelComboBox->currentIndex() == 0) //Disabled
    {
        m_uas->getParamManager()->setParameter(1,"CAM_TRIGG_TYPE",0);
    }
    else if (ui.shutterChannelComboBox->currentIndex() == 1) //Relay
    {
        m_uas->getParamManager()->setParameter(1,"CAM_TRIGG_TYPE",1);
    }
    else if (ui.shutterChannelComboBox->currentIndex() == 2) //Transistor
    {
        m_uas->getParamManager()->setParameter(1,"CAM_TRIGG_TYPE",4);
    }
    else
    {
        m_uas->getParamManager()->setParameter(1,ui.shutterChannelComboBox->currentText() + "_FUNCTION",10);
        m_uas->getParamManager()->setParameter(1,"CAM_TRIGG_TYPE",0);
    }
    m_uas->getParamManager()->setParameter(1,ui.shutterChannelComboBox->currentText() + "_MIN",ui.shutterServoMinSpinBox->value());
    m_uas->getParamManager()->setParameter(1,ui.shutterChannelComboBox->currentText() + "_MAX",ui.shutterServoMaxSpinBox->value());
    m_uas->getParamManager()->setParameter(1,"CAM_SERVO_ON",ui.shutterPushedSpinBox->value());
    m_uas->getParamManager()->setParameter(1,"CAM_SERVO_OFF",ui.shutterNotPushedSpinBox->value());
    m_uas->getParamManager()->setParameter(1,"CAM_DURATION",ui.shutterDurationSpinBox->value());


}


CameraGimbalConfig::~CameraGimbalConfig()
{

}

void CameraGimbalConfig::refreshTilt()
{

}

void CameraGimbalConfig::refreshPan()
{

}

void CameraGimbalConfig::refreshRoll()
{

}

void CameraGimbalConfig::refreshMountParameters(QString mount, QString parameterName, QVariant &value)
{
    qDebug() << "refresh parameters " << mount;

    if (parameterName == "MNT_ANGMIN_TIL") //TILT
    {
        ui.tiltAngleMinSpinBox->setValue(value.toInt() / 100.0);
    }
    else if (parameterName == "MNT_ANGMAX_TIL")
    {
        ui.tiltAngleMaxSpinBox->setValue(value.toInt() / 100.0);
    }
    else if (parameterName == "MNT_RC_IN_TILT")
    {
        disconnect(ui.tiltInputChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateTilt()));
        if (value.toInt() == 0)
        {
            ui.tiltInputChannelComboBox->setCurrentIndex(0);
        }
        else
        {
            ui.tiltInputChannelComboBox->setCurrentIndex(value.toInt()-4);
        }
        connect(ui.tiltInputChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateTilt()));
    }
    else if (parameterName == "MNT_ANGMIN_ROL") //ROLL
    {
        ui.rollAngleMinSpinBox->setValue(value.toInt() / 100.0);
    }
    else if (parameterName == "MNT_ANGMAX_ROL")
    {
        ui.rollAngleMaxSpinBox->setValue(value.toInt() / 100.0);
    }
    else if (parameterName == "MNT_RC_IN_ROLL")
    {
        disconnect(ui.rollInputChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateRoll()));
        if (value.toInt() == 0)
        {
            ui.rollInputChannelComboBox->setCurrentIndex(0);
        }
        else
        {
            ui.rollInputChannelComboBox->setCurrentIndex(value.toInt()-4);
        }
        connect(ui.rollInputChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateRoll()));
    }
    else if (parameterName == "MNT_ANGMIN_PAN") //PAN
    {
        ui.panAngleMinSpinBox->setValue(value.toInt() / 100.0);
    }
    else if (parameterName == "MNT_ANGMAX_PAN")
    {
        ui.panAngleMaxSpinBox->setValue(value.toInt() / 100.0);
    }
    else if (parameterName == "MNT_RC_IN_PAN")
    {
        disconnect(ui.panInputChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updatePan()));
        if (value.toInt() == 0)
        {
            ui.panInputChannelComboBox->setCurrentIndex(0);
        }
        else
        {
            ui.panInputChannelComboBox->setCurrentIndex(value.toInt()-4);
        }
        connect(ui.panInputChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updatePan()));
    }

}

void CameraGimbalConfig::refreshCameraParameters(QString parameterName, QVariant value)
{
    qDebug() << "refresh Camera parameters ";

    if (parameterName == "CAM_DURATION")
    {
        ui.shutterDurationSpinBox->setValue(value.toInt());
    }
    else if (parameterName == "CAM_SERVO_OFF")
    {
        ui.shutterNotPushedSpinBox->setValue(value.toInt());
    }
    else if (parameterName == "CAM_SERVO_ON")
    {
        ui.shutterPushedSpinBox->setValue(value.toInt());
    }
    else if (parameterName == "CAM_TRIGG_TYPE")
    {
        disconnect(ui.shutterChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateShutter()));
        if (value.toInt() == 0) //Disabled
        {
            ui.shutterChannelComboBox->setCurrentIndex(0);
            ///TODO: Request all _FUNCTIONs here to find out if shutter is actually disabled.
        }
        else if (value.toInt() == 1) // Relay
        {
            ui.shutterChannelComboBox->setCurrentIndex(1);
        }
        else if (value.toInt() == 4) //Transistor
        {
            ui.shutterChannelComboBox->setCurrentIndex(2);
        }
        connect(ui.shutterChannelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateShutter()));
    }
    if (parameterName.startsWith(m_shutterPrefix) && !m_shutterPrefix.isEmpty())
    {
        if (parameterName.endsWith("MIN"))
        {
            ui.shutterServoMinSpinBox->setValue(value.toInt());
        }
        else if (parameterName.endsWith("MAX"))
        {
            ui.shutterServoMaxSpinBox->setValue(value.toInt());
        }
    }
}

void CameraGimbalConfig::parameterChanged(int uas, int component, QString parameterName, QVariant value)
{
    // Silently ignore all params we don't care about
    if (m_cameraParams.contains(parameterName))
        return;

    qDebug() << "Camera Gimbal Param Changed:" << m_uas->getUASName()
             << "param:" << parameterName << value << "compiD:" << component;

    if (parameterName.startsWith("MNT")){
        refreshMountParameters(parameterName.section('_',0), parameterName, value);

    } else if (parameterName.startsWith("CAM")){
        refreshCameraParameters(parameterName, value);

    } else if (parameterName.startsWith(m_tiltPrefix) && !m_tiltPrefix.isEmpty()) {
        if (parameterName.endsWith("MIN"))
        {
            ui.tiltServoMinSpinBox->setValue(value.toInt());
        }
        else if (parameterName.endsWith("MAX"))
        {
            ui.tiltServoMaxSpinBox->setValue(value.toInt());
        }
        else if (parameterName.endsWith("REV"))
        {
            if (value.toInt() == 0)
            {
                ui.tiltReverseCheckBox->setChecked(false);
            }
            else
            {
                ui.tiltReverseCheckBox->setChecked(true);
            }
        }
    } else if (parameterName.startsWith(m_rollPrefix) && !m_rollPrefix.isEmpty()) {
        if (parameterName.endsWith("MIN"))
        {
            ui.rollServoMinSpinBox->setValue(value.toInt());
        }
        else if (parameterName.endsWith("MAX"))
        {
            ui.rollServoMaxSpinBox->setValue(value.toInt());
        }
        else if (parameterName.endsWith("REV"))
        {
            if (value.toInt() == 0)
            {
                ui.rollReverseCheckBox->setChecked(false);
            }
            else
            {
                ui.rollReverseCheckBox->setChecked(true);
            }
        }
    } else if (parameterName.startsWith(m_panPrefix) && !m_panPrefix.isEmpty()) {
        if (parameterName.endsWith("MIN"))
        {
            ui.panServoMinSpinBox->setValue(value.toInt());
        }
        else if (parameterName.endsWith("MAX"))
        {
            ui.panServoMaxSpinBox->setValue(value.toInt());
        }
        else if (parameterName.endsWith("REV"))
        {
            if (value.toInt() == 0)
            {
                ui.panReverseCheckBox->setChecked(false);
            }
            else
            {
                ui.panReverseCheckBox->setChecked(true);
            }
        }
    } else if (parameterName == "RC5_FUNCTION"){
        refreshRcFunctionComboxBox("RC5_FUNCTION", value);

    } else if (parameterName == "RC6_FUNCTION"){
        refreshRcFunctionComboxBox("RC6_FUNCTION", value);

    } else if (parameterName == "RC7_FUNCTION"){
        refreshRcFunctionComboxBox("RC7_FUNCTION", value);

    } else if (parameterName == "RC8_FUNCTION"){
        refreshRcFunctionComboxBox("RC8_FUNCTION", value);

    } else if (parameterName == "RC8_FUNCTION"){
        refreshRcFunctionComboxBox("RC8_FUNCTION", value);

    } else if (parameterName == "RC10_FUNCTION"){
        refreshRcFunctionComboxBox("RC10_FUNCTION", value);

    } else if (parameterName == "RC11_FUNCTION") {
        refreshRcFunctionComboxBox("RC11_FUNCTION", value);

    }
}

void CameraGimbalConfig::refreshRcFunctionComboxBox(QString rcChannelName, QVariant &value)
{
    if (value.toInt() == camera_trigger)
    {
        //RC11 is shutter.
        ui.shutterChannelComboBox->setCurrentIndex(ui.shutterChannelComboBox->findData(value));
        m_shutterPrefix = rcChannelName + "_";
    }
    else if (value.toInt() == mount_roll)
    {
        //RC11 is roll
        ui.rollChannelComboBox->setCurrentIndex(ui.rollChannelComboBox->findData(value));
        m_rollPrefix = rcChannelName + "_";
    }
    else if (value.toInt() == mount_tilt)
    {
        //RC11 is tilt
        ui.tiltChannelComboBox->setCurrentIndex(ui.tiltChannelComboBox->findData(value));
        m_tiltPrefix = rcChannelName + "_";
    }
    else if (value.toInt() == mount_pan)
    {
        //RC11 is pan
        ui.panChannelComboBox->setCurrentIndex(ui.panChannelComboBox->findData(value));
        m_panPrefix = rcChannelName + "_";
    }
    m_uas->getParamManager()->requestParameterUpdate(1, rcChannelName + "_MIN");
    m_uas->getParamManager()->requestParameterUpdate(1, rcChannelName + "_MAX");
    m_uas->getParamManager()->requestParameterUpdate(1, rcChannelName + "_REV");
}


