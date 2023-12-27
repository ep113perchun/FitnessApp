#include "addfood.h"
#include "ui_addfood.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>


AddFood::AddFood(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFood)
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

AddFood::~AddFood()
{
    delete ui;
}

void AddFood::on_saveButton_clicked()
{
    QString dish = ui->dish->text();
    QString calories = ui->calories->text();
    int account_id = key;
    bool status = false;

    QSqlQuery query;
    query.prepare("INSERT INTO eat (account_id, dish, calories, eat_date, status) VALUES (:account_id, :dish, :calories, :eat_date, :status)");
    query.bindValue(":account_id", account_id);
    query.bindValue(":dish", dish);
    query.bindValue(":calories", calories.toInt());
    query.bindValue(":eat_date", date);
    query.bindValue(":status", status);

    qDebug() << "date" << date.toString("dd.MM.yyyy");

    if(query.exec()) {
        qDebug() << "Запись успешно добавлена в базу данных!";
    } else {
        qDebug() << "Ошибка при добавлении записи в базу данных:" << query.lastError().text();
    }

    AddFood::close();
}

