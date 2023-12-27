#include "premier.h"
#include "ui_premier.h"
#include "addexercises.h"
#include "addfood.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QDesktopServices>
#include <QUrl>


Premier::Premier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Premier)
{
    ui->setupUi(this);

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(Timer_TimeOut_Event_Slot()));
    Timer->setInterval(1000);
    Timer->start();

    elapsedSeconds = 0;
    elapsedMinuts = 0;
    connect(&timer, &QTimer::timeout, this, &Premier::updateElapsedTime);
    timer.start(1000);

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
        ui->listWidget->clear();

        QSqlQuery query;
        query.prepare(currentQuery);
        query.bindValue(":date", date);
        query.bindValue(":str", str);

        if (!query.exec()) {
            qDebug() << "Failed to execute query";
            return;
        }

        while (query.next()) {
            QString exercises = query.value(0).toString();
            int approaches = query.value(1).toInt();
            int repetitions = query.value(2).toInt();
            bool status = query.value(3).toBool();

            QString itemStr = QString("%1 | %2 x %3").arg(exercises).arg(approaches).arg(repetitions);

            QListWidgetItem* listItem = new QListWidgetItem(itemStr);
            listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
            listItem->setCheckState(status ? Qt::Checked : Qt::Unchecked);

            ui->listWidget->addItem(listItem);
        }
}

void Premier::updateTaskList_2(const QString &currentQuery, QString str)
{
        ui->listWidget_2->clear();

        QSqlQuery query;
        query.prepare(currentQuery);
        query.bindValue(":date", date);
        query.bindValue(":str", str);

        if (!query.exec()) {
            qDebug() << "Failed to execute query";
            return;
        }

        while (query.next()) {
            QString dish = query.value(0).toString();
            int calories = query.value(1).toInt();
            bool status = query.value(2).toBool();

            QString itemStr = QString("%1 | %2 ccal").arg(dish).arg(calories);

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
    updateTaskList("SELECT exercises, approaches, repetitions, status FROM trainings WHERE training_date = :date", NULL);
    markDate(date);
}


void Premier::on_addFood_clicked()
{
    AddFood addFood;
    addFood.key = this->key;
    addFood.date = this->date;
    addFood.setModal(true);
    addFood.exec();
    updateTaskList_2("SELECT dish, calories, status FROM eat WHERE eat_date = :date", NULL);
    markDate(date);
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

void Premier::markDate(QDate &date) {
    QTextCharFormat format;
    format.setFontWeight(QFont::ExtraBold);
    format.setFontUnderline(true);

    ui->calendarWidget->setDateTextFormat(date, format);
}

void Premier::unMarkDate(QDate &date) {
    QTextCharFormat format;
    format.setFontWeight(QFont::Normal);
    format.setFontUnderline(false);

    ui->calendarWidget->setDateTextFormat(date, format);
}


void Premier::on_pushButton_2_clicked()
{
    QSqlQuery query;
    query.prepare("DELETE FROM trainings WHERE status = TRUE AND training_date = :date");
    query.bindValue(":date", date);
    if(query.exec()) {
        updateTaskList("SELECT exercises, approaches, repetitions, status FROM trainings WHERE training_date = :date", NULL);
    } else {
        qDebug() << "Ошибка удаления" << query.lastError().text();
    }
}


void Premier::on_pushButton_5_clicked()
{
    QSqlQuery query;
    query.prepare("DELETE FROM eat WHERE status = TRUE AND eat_date = :date");
    query.bindValue(":date", date);
    if(query.exec()) {
        updateTaskList_2("SELECT dish, calories, status FROM eat WHERE eat_date = :date", NULL);
    } else {
        qDebug() << "Ошибка удаления 2" << query.lastError().text();
    }
    unMarkDate(date);
}


void Premier::on_pushButton_clicked()
{
    QUrl link("https://github.com/ep113perchun/FitnessApp");
    QDesktopServices::openUrl(link);

}

void Premier::updateElapsedTime()
{
    elapsedSeconds++;
    if (elapsedSeconds == 60) {
        elapsedSeconds = 0;
        elapsedMinuts++;
    }
    ui->label->setText("Минут: " + QString::number(elapsedMinuts) + " Секунд: " + QString::number(elapsedSeconds));
}

