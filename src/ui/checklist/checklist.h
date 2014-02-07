#ifndef CHECKLIST_H
#define CHECKLIST_H

#include <QDialog>
#include <QListWidgetItem>
#include <QTimer>

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
    void onListItemClicked(QListWidgetItem* item);
    void localTimerEvent();
    
private:
    Ui::CheckList *ui;
    QString ItemToCheck;
    QString currentAudioString;
    QTimer *speaktimer;
    
};

#endif // CHECKLIST_H
