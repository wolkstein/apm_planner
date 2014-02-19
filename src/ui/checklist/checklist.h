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

#ifndef CHECKLIST_H
#define CHECKLIST_H

#include <QDialog>
#include <QListWidgetItem>
#include <QTimer>
#include <QColor>
#include <QFile>

namespace Ui {
class CheckList;
}

class CheckList : public QDialog
{
    Q_OBJECT
    
public:
    explicit CheckList(QWidget *parent = 0);
    ~CheckList();
    
    void findNextUncheckedItem();
    
private slots:
    //void onListItemClicked(QListWidgetItem* item);
    void localTimerEvent();   
    void on_ClosePushButton_clicked();
    
    void on_VerifiedItemPushButton_clicked();
    
    void on_SkipItemPushButton_clicked();
    
private:
    Ui::CheckList *ui;
    QString currentAudioString;
    QTimer *speaktimer;
    QListWidgetItem *CurrentItem;
    QColor SkippedItemColor;
    QColor InfoItemColor;
    QString myDateAsString;
    QFile *dayFile;
    unsigned long myTimeInMillis;
};

#endif // CHECKLIST_H
