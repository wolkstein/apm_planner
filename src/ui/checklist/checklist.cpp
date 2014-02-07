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
    ItemToCheck = "----";
    currentAudioString = "Pre Flight Check List.";
    QFile file(QGC::appDataDirectory() + "/PreFlightCheckList.txt");
    if (!file.exists()){
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << "============== Pre flight Checklist =================\n";
        out << "an \"#\" on first position of row will comment this row.\n";
        out << "each line can contains an checklist option\n";
        out << "this file is created in your home dir/apmplanner2/PreFlightCheckList.txt\n";
        out << "simple edit it with an normal texteditor like notepad\n";
        out << "for example:\n";
        out << "all motor mounts\n";
        out << "voltage of main battery\n";
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
        //process_line(line);
        qDebug()<<line;
        QListWidgetItem *newItem = new QListWidgetItem;
        
        newItem->setFont (QFont ("Sans Serif", 9));
        newItem->setText(line);
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
    if(QString(item->text()) != QString(ItemToCheck))
        return;
    speaktimer->stop();
    GAudioOutput::instance()->say(ItemToCheck + ": is checked!"); 
    item->setCheckState(Qt::Checked);
    findNextUncheckedItem();
    item->setFont (QFont ("Sans Serif", 9));
}

void CheckList::findNextUncheckedItem()
{
    for(int i = 0; i < ui->CheckListWidget->count(); i++ ){
        QListWidgetItem *item = ui->CheckListWidget->item(i);
        if(item->checkState() == Qt::Unchecked){
            item->setFont (QFont ("Sans Serif", 20));
            ItemToCheck = item->text();
            currentAudioString = QString("Position: ")+ QString("%1").arg(i+1) + QString(": ") + QString("Please check") + ItemToCheck;
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