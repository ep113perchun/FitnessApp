#include "premier.h"
#include "ui_premier.h"

Premier::Premier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Premier)
{
    ui->setupUi(this);

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(Timer_TimeOut_Event_Slot()));
    Timer->setInterval(1000);
    Timer->start();

    updateTaskList();
}

Premier::~Premier()
{
    delete ui;
}

void Premier::updateTaskList()
{
    QStringList strList;
    strList << "monitor" << "mouse" << "keyboard" << "hard disk drive"
            << "graphic card" << "sound card" << "memory" << "motherboard";

    ui->listWidget->addItems(strList);


    QListWidgetItem* item = 0;
    for(int i = 0; i < ui->listWidget->count(); ++i){
        item = ui->listWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void Premier::Timer_TimeOut_Event_Slot()
{
    ui->timeEdit->setTimeRange(QTime::currentTime(), QTime(23, 59, 59));
}


void Premier::on_calendarWidget_clicked(const QDate &date)
{
    qDebug() << "date ))))" << date.toString();
}



