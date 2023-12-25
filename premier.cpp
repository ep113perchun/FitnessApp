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
            bool status = query.value(2).toBool();

            // Создаем строку с данными и добавляем ее в список
            QString itemStr = QString("%1 | %2").arg(dish).arg(calories);

            // Создаем элемент списка с проверкой в зависимости от статуса и добавляем его в виджет
            QListWidgetItem* listItem = new QListWidgetItem(itemStr);
            listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
//            qDebug() << status;
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
    QString exercises = item->text();
    QStringList parts = exercises.split(" | ");
    QString firstWord = parts.at(0);

    bool status = false;

    QSqlQuery query;
    query.prepare("SELECT status FROM trainings WHERE training_date = :date AND exercises = :exercises");
    query.bindValue(":date", date);
    query.bindValue(":exercises", firstWord);

    if (!query.exec()) {
        qDebug() << "Failed to execute query";
        return;
    }

    while (query.next()) {
        status = query.value(0).toBool();
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (db.isValid()) {
        QSqlQuery query(db);
        query.prepare("UPDATE trainings SET status = :status WHERE exercises = :exercises AND training_date = :date");
        query.bindValue(":exercises", firstWord);
        query.bindValue(":status", !status);
        query.bindValue(":date", date);

        if (query.exec()) {
            qDebug() << "Статус тренировки " << !status << " успешно обновлен.";
        } else {
            qDebug() << "Ошибка обновления статуса тренировки" << query.lastError().text();
        }
    } else {
        qDebug() << "Невозможно получить доступ к базе данных.";
    }

}

void Premier::on_listWidget_2_itemChanged(QListWidgetItem *item)
{
    QString dish = item->text();
    QStringList parts = dish.split(" | ");
    QString firstWord = parts.at(0);

    bool status = false;

    QSqlQuery query;
    query.prepare("SELECT status FROM eat WHERE eat_date = :date AND dish = :dish");
    query.bindValue(":date", date);
    query.bindValue(":dish", firstWord);

    if (!query.exec()) {
        qDebug() << "Failed to execute query";
        return;
    }

    while (query.next()) {
        status = query.value(0).toBool();
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (db.isValid()) {
        QSqlQuery query(db);
        // Формируем и выполняем SQL-запрос
        query.prepare("UPDATE eat SET status = :status WHERE dish = :dish AND eat_date = :date");
        query.bindValue(":dish", firstWord);
        query.bindValue(":status", !status);
        query.bindValue(":date", date);
        if (query.exec()) {
        // Запрос выполнен успешно
            qDebug() << "Статус еды " << firstWord << " " << status << " успешно обновлен.";
        } else {
        // Обработка ошибки запроса
            qDebug() << "Ошибка обновления статуса еды" << query.lastError().text();
        }
    } else {
        qDebug() << "Невозможно получить доступ к базе данных.";
    }
}

void Premier::on_seatchB_clicked()
{
    QString str = ui->lineEdit->text();
    updateTaskList("SELECT exercises, approaches, repetitions, status FROM trainings WHERE training_date = :date AND exercises LIKE :str", str);
    updateTaskList_2("SELECT dish, calories, status FROM eat WHERE eat_date = :date AND dish LIKE :str", str);
}




void Premier::on_deleteButton_clicked()
{
    QSqlQuery query;
    query.prepare("DELETE FROM trainings WHERE status = TRUE AND training_date = :date");
    query.bindValue(":date", date);
    if(query.exec()) {
        updateTaskList("SELECT exercises, approaches, repetitions, status FROM trainings WHERE training_date = :date", NULL);
    } else {
        qDebug() << "Ошибка удаления" << query.lastError().text();
    }

    QSqlQuery query2;
        query.prepare("DELETE FROM eat WHERE status = TRUE AND eat_date = :date");
    query2.bindValue(":date", date);
    if(query2.exec()) {
        updateTaskList_2("SELECT dish, calories, status FROM eat WHERE eat_date = :date", NULL);
    } else {
        qDebug() << "Ошибка удаления 2" << query2.lastError().text();
    }
}

