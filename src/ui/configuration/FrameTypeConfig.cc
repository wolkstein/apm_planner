/*===================================================================
APM_PLANNER Open Source Ground Control Station

(c) 2013 APM_PLANNER PROJECT <http://www.diydrones.com>

This file is part of the APM_PLANNER project

    APM_PLANNER is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    APM_PLANNER is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with APM_PLANNER. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Airframe type configuration widget source.
 *
 *   @author Michael Carpenter <malcom2073@gmail.com>
 *
 */

#include "QsLog.h"
#include "FrameTypeConfig.h"

static const int FRAME_TYPE_PLUS = 0;
static const int FRAME_TYPE_X = 1;
static const int FRAME_TYPE_V = 2;
static const int FRAME_TYPE_H = 3;
static const int FRAME_TYPE_NEWY6 = 10;

FrameTypeConfig::FrameTypeConfig(QWidget *parent) : AP2ConfigWidget(parent)
{
    ui.setupUi(this);

    //Disable until we get a FRAME parameter.
    ui.xRadioButton->setEnabled(false);
    ui.vRadioButton->setEnabled(false);
    ui.plusRadioButton->setEnabled(false);
    ui.hRadioButton->setEnabled(false);

    connect(ui.plusRadioButton,SIGNAL(clicked()),this,SLOT(plusFrameSelected()));
    connect(ui.xRadioButton,SIGNAL(clicked()),this,SLOT(xFrameSelected()));
    connect(ui.vRadioButton,SIGNAL(clicked()),this,SLOT(vFrameSelected()));
    connect(ui.hRadioButton,SIGNAL(clicked()),this,SLOT(hFrameSelected()));
    connect(ui.newY6radioButton,SIGNAL(clicked()),this,SLOT(newY6FrameSelected()));
    initConnections();
}

FrameTypeConfig::~FrameTypeConfig()
{
}
void FrameTypeConfig::parameterChanged(int uas, int component, QString parameterName, QVariant value)
{
    Q_UNUSED(uas);
    Q_UNUSED(component);

    if (parameterName == "FRAME")
    {
        ui.xRadioButton->setEnabled(true);
        ui.vRadioButton->setEnabled(true);
        ui.plusRadioButton->setEnabled(true);
        ui.hRadioButton->setEnabled(true);
        ui.newY6radioButton->setEnabled(true);

        switch(value.toInt()){
        case FRAME_TYPE_PLUS:
            ui.plusRadioButton->setChecked(true);
        break;
        case FRAME_TYPE_X:
            ui.xRadioButton->setChecked(true);
        break;
        case FRAME_TYPE_V:
            ui.vRadioButton->setChecked(true);
        break;
        case FRAME_TYPE_H:
            ui.hRadioButton->setChecked(true);
            break;
        break;
        case FRAME_TYPE_NEWY6:
            ui.hRadioButton->setChecked(true);
        break;
        default:
            QLOG_ERROR() << "Unknown Frame Type" << value.toInt();
        }
    }
}

void FrameTypeConfig::xFrameSelected()
{
    if (!m_uas)
    {
        showNullMAVErrorMessageBox();
        return;
    }
    m_uas->getParamManager()->setParameter(1,"FRAME",QVariant(FRAME_TYPE_X));
}

void FrameTypeConfig::hFrameSelected()
{
    if (!m_uas)
    {
        showNullMAVErrorMessageBox();
        return;
    }
    m_uas->getParamManager()->setParameter(1,"FRAME",QVariant(FRAME_TYPE_H));
}

void FrameTypeConfig::plusFrameSelected()
{
    if (!m_uas)
    {
        showNullMAVErrorMessageBox();
        return;
    }
    m_uas->getParamManager()->setParameter(1,"FRAME",QVariant(FRAME_TYPE_PLUS));
}

void FrameTypeConfig::vFrameSelected()
{
    if (!m_uas)
    {
        showNullMAVErrorMessageBox();
        return;
    }
    m_uas->getParamManager()->setParameter(1,"FRAME",QVariant(FRAME_TYPE_V));
}

void FrameTypeConfig::newY6FrameSelected()
{
    if (!m_uas)
    {
        showNullMAVErrorMessageBox();
        return;
    }
    m_uas->getParamManager()->setParameter(1,"FRAME",QVariant(FRAME_TYPE_NEWY6));
}
