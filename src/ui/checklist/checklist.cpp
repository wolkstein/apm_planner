/*=====================================================================

PIXHAWK Micro Air Vehicle Flying Robotics Toolkit

(c) 2009, 2010 PIXHAWK PROJECT  <http://pixhawk.ethz.ch>

This file is part of the PIXHAWK project

    PIXHAWK is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PIXHAWK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PIXHAWK. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Definition of Pre Flight CheckList
 *
 *   @author Michael Wolkstein
 *
 */

#include "checklist.h"
#include "ui_checklist.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QGC.h>
#include <GAudioOutput.h>

CheckList::CheckList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckList)
{
    ui->setupUi(this);
    currentAudioString = "Pre Flight Check List.";
    SkippedItemColor = QColor(Qt::blue);
    InfoItemColor = QColor(Qt::green);
    QFile file(QGC::appDataDirectory() + "/PreFlightCheckList.txt");
    if (!file.exists()){
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << "============== Pre flight Checklist =================\n";
        out << "an \"#\" on first position of row will comment this row.\n";
        out << "each line can contains an checklist option\n";
        out << "this file is created in your home dir/apmplanner2/PreFlightCheckList.txt\n";
        out << "simple edit it with an normal texteditor like notepad\n";
        out << "an \"[\"  on first position of row will leave this item uncheckable \n";
        out << "for example:\n";
        out << "[mechanical UAS checklist]\n";
        out << "all motor mounts\n";
        out << "voltage of main battery\n";
        out << "[power UAS]\n";
        out << "turn on UAS transmitter\n";
        out << "connect main battery\n";
        out << "and so on";
        
        // optional, as QFile destructor will already do it:
        
        file.close(); 
    }
    
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(line.left(1)  == "#"){
            qDebug()<<line<< "enthält einen comentator";
            continue;
        }
        QListWidgetItem *newItem = new QListWidgetItem;
        
        newItem->setFont (QFont ("Sans Serif", 9));
        newItem->setText(line);
        
        if(line.left(1)  == "["){
            qDebug()<<line<< "ignorieren";
            newItem->setBackgroundColor(InfoItemColor);
        }
        ui->CheckListWidget->insertItem(ui->CheckListWidget->count(), newItem);
        file.close();
    }
    
    connect(ui->CheckListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListItemClicked(QListWidgetItem*)));
    GAudioOutput::instance()->say(currentAudioString);
    speaktimer = new QTimer(this);
    connect(speaktimer, SIGNAL(timeout()), this, SLOT(localTimerEvent()));
    
    findNextUncheckedItem();
    
}

CheckList::~CheckList()
{
    speaktimer->stop();
    delete ui;
}

void CheckList::onListItemClicked(QListWidgetItem* item)
{
    if(QString(item->text()) != QString(CurrentItem->text()))
        return;
    speaktimer->stop();
    GAudioOutput::instance()->say("verified!"); 
    item->setCheckState(Qt::Checked);
    item->setFont (QFont ("Sans Serif", 9));
    findNextUncheckedItem();
}

void CheckList::findNextUncheckedItem()
{
    int correctposition = 0;
    for(int i = 0; i < ui->CheckListWidget->count(); i++ ){
        CurrentItem = ui->CheckListWidget->item(i);
        if(CurrentItem->checkState() == Qt::Unchecked){
            if (CurrentItem->backgroundColor() == SkippedItemColor)
                continue;
            if(QString(CurrentItem->text()).left(1)  == "["){
                correctposition++;
                continue;
            }
            CurrentItem->setFont (QFont ("Sans Serif", 20));
            currentAudioString = QString("Position: ")+ QString("%1").arg(i+1-correctposition) + QString(": ") + CurrentItem->text();
            GAudioOutput::instance()->say( currentAudioString );
            speaktimer->start(20000);
            return;
        }
    }
    
}

void CheckList::localTimerEvent()
{
    GAudioOutput::instance()->say( currentAudioString );
}

void CheckList::on_ClosePushButton_clicked()
{
    this->close();
}

void CheckList::on_NextItemPushButton_clicked()
{
    speaktimer->stop();
    GAudioOutput::instance()->say("verified!"); 
    CurrentItem->setCheckState(Qt::Checked);
    CurrentItem->setFont (QFont ("Sans Serif", 9));
    findNextUncheckedItem();
    
}

void CheckList::on_SkipItemPushButton_clicked()
{
    if(CurrentItem->checkState() == Qt::Unchecked){
        CurrentItem->setFont (QFont ("Sans Serif", 9));
        CurrentItem->setBackgroundColor(SkippedItemColor);
        currentAudioString = QString("unverified!");
        GAudioOutput::instance()->say( currentAudioString );
        speaktimer->start(20000);
        findNextUncheckedItem();
    }   
}
