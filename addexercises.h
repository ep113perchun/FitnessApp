#ifndef ADDEXERCISES_H
#define ADDEXERCISES_H

#include <QDialog>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
class AddExercises;
}

class AddExercises : public QDialog
{
    Q_OBJECT

public:
    explicit AddExercises(QWidget *parent = nullptr);
    ~AddExercises();
    int key;
    QDate date;

private slots:
    void on_saveButton2_clicked();

private:
    Ui::AddExercises *ui;
    QSqlDatabase db;
};

#endif // ADDEXERCISES_H
