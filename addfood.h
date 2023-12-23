#ifndef ADDFOOD_H
#define ADDFOOD_H

#include <QDialog>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
class AddFood;
}

class AddFood : public QDialog
{
    Q_OBJECT

public:
    explicit AddFood(QWidget *parent = nullptr);
    ~AddFood();
    int key;
    QDate date;

private slots:
    void on_saveButton_clicked();

private:
    Ui::AddFood *ui;
    QSqlDatabase db;
};

#endif // ADDFOOD_H
