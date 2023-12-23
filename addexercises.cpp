#include "addexercises.h"
#include "ui_addexercises.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>

AddExercises::AddExercises(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddExercises)
{
    ui->setupUi(this);

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
}

AddExercises::~AddExercises()
{
    delete ui;
}

void AddExercises::on_saveButton2_clicked()
{
    QString exercises = ui->exercises->text();
    QString approaches = ui->approaches->text();
    QString repetitions = ui->repetitions->text();
    int account_id = key;
    bool status = true;

    QSqlQuery query;
    query.prepare("INSERT INTO trainings (account_id, exercises, approaches, repetitions, training_date, status) VALUES (:account_id, :exercises, :approaches, :repetitions, :training_date, :status)");
    query.bindValue(":account_id", account_id);
    query.bindValue(":exercises", exercises);
    query.bindValue(":approaches", approaches.toInt());
    query.bindValue(":repetitions", repetitions.toInt());
    query.bindValue(":training_date", date);
    query.bindValue(":status", status);

    qDebug() << "date" << date.toString("dd.MM.yyyy");

    if(query.exec()) {
        qDebug() << "Запись успешно добавлена в базу данных!";
    } else {
        qDebug() << "Ошибка при добавлении записи в базу данных:" << query.lastError().text();
    }

}

