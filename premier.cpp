#include "premier.h"
#include "ui_premier.h"
#include "addexercises.h"
#include "addfood.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>

Premier::Premier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Premier)
{
    ui->setupUi(this);

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(Timer_TimeOut_Event_Slot()));
    Timer->setInterval(1000);
    Timer->start();

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("ExerciseAndDiet");
    db.setHostName("127.0.0.1");
    db.setPort(5433);
    db.setUserName("postgres");
    db.setPassword("vjtj,kfrj");
    if (!db.open()) {
        qDebug() << db.lastError().text();
    } else {
        qDebug() << "connected...";
    }

    updateTaskList("SELECT exercises, approaches, repetitions, status FROM trainings WHERE training_date = :date", NULL);
    updateTaskList_2("SELECT dish, calories, status FROM eat WHERE eat_date = :date", NULL);
}

Premier::~Premier()
{
    delete ui;
}

void Premier::updateTaskList(const QString &currentQuery, QString str)
{
        ui->listWidget->clear(); // Очищаем виджет перед обновлением

        // Выполняем запрос к базе данных, чтобы получить записи для заданной даты
        QSqlQuery query;
        query.prepare(currentQuery);
        query.bindValue(":date", date);
        query.bindValue(":str", str);

        if (!query.exec()) {
            // Обработка ошибки запроса к базе данных
            qDebug() << "Failed to execute query";
            return;
        }

        // Обрабатываем результаты запроса
        while (query.next()) {
            QString exercises = query.value(0).toString();
            int approaches = query.value(1).toInt();
            int repetitions = query.value(2).toInt();
            bool status = query.value(3).toBool();

            // Создаем строку с данными и добавляем ее в список
            QString itemStr = QString("%1 | %2 | %3").arg(exercises).arg(approaches).arg(repetitions);

            // Создаем элемент списка с проверкой в зависимости от статуса и добавляем его в виджет
            QListWidgetItem* listItem = new QListWidgetItem(itemStr);
            listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
            listItem->setCheckState(status ? Qt::Checked : Qt::Unchecked);

            ui->listWidget->addItem(listItem);
        }
}

void Premier::updateTaskList_2(const QString &currentQuery, QString str)
{
        ui->listWidget_2->clear(); // Очищаем виджет перед обновлением

        // Выполняем запрос к базе данных, чтобы получить записи для заданной даты
        QSqlQuery query;
        query.prepare(currentQuery);
        query.bindValue(":date", date);
        query.bindValue(":str", str);

        if (!query.exec()) {
            // Обработка ошибки запроса к базе данных
            qDebug() << "Failed to execute query";
            return;
        }

        // Обрабатываем результаты запроса
        while (query.next()) {
            QString dish = query.value(0).toString();
            int calories = query.value(1).toInt();
            bool status = query.value(3).toBool();

            // Создаем строку с данными и добавляем ее в список
            QString itemStr = QString("%1 | %2").arg(dish).arg(calories);

            // Создаем элемент списка с проверкой в зависимости от статуса и добавляем его в виджет
            QListWidgetItem* listItem = new QListWidgetItem(itemStr);
            listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
            listItem->setCheckState(status ? Qt::Checked : Qt::Unchecked);

            ui->listWidget_2->addItem(listItem);
        }
}

void Premier::Timer_TimeOut_Event_Slot()
{
    ui->timeEdit->setTimeRange(QTime::currentTime(), QTime(23, 59, 59));
}


void Premier::on_calendarWidget_clicked(const QDate &date)
{
    this->date = date;
    updateTaskList("SELECT exercises, approaches, repetitions, status FROM trainings WHERE training_date = :date", NULL);
    updateTaskList_2("SELECT dish, calories, status FROM eat WHERE eat_date = :date", NULL);
    qDebug() << "date" << date.toString("dd.MM.yyyy");
}




void Premier::on_addExercise_clicked()
{
    AddExercises addExercises;
    addExercises.key = this->key;
    addExercises.date = this->date;
    addExercises.setModal(true);
    addExercises.exec();
}


void Premier::on_addFood_clicked()
{
    AddFood addFood;
    addFood.key = this->key;
    addFood.date = this->date;
    addFood.setModal(true);
    addFood.exec();
}


void Premier::on_listWidget_itemChanged(QListWidgetItem *item)
{

}


void Premier::on_seatchB_clicked()
{
    QString str = ui->lineEdit->text();
    updateTaskList("SELECT exercises, approaches, repetitions, status FROM trainings WHERE training_date = :date AND exercises LIKE :str", str);
    updateTaskList_2("SELECT dish, calories, status FROM eat WHERE eat_date = :date AND dish LIKE :str", str);
}


void Premier::on_pushButton_2_clicked()
{
    QSqlQuery query;
    query.prepare("");
}

