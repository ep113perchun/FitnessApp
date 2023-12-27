#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "premier.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

    settings = new QSettings(this);
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::saveSettings()
{
    settings->beginGroup("forms");
    settings->beginGroup(objectName());
    settings->setValue("geometry", geometry());
    settings->endGroup();
    settings->endGroup();
}

void MainWindow::loadSettings()
{
    settings->beginGroup("forms");
    settings->beginGroup(objectName());
    setGeometry(settings->value("geometry", QRect(200, 200, 300, 300)).toRect ());
    settings->endGroup ();
    settings->endGroup ();
}


void MainWindow::on_logIn_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();
    if (ui->login->text() == "" || ui->password->text() == "")
    {
        QMessageBox::warning(this, "Error", "Заполните все поля!");
        return;
    }else{
        if (db.open()) {
            QSqlQuery query;
            query.prepare("SELECT id FROM account WHERE login = :login AND pass = :pass");
            query.bindValue(":login", login);
            query.bindValue(":pass", password);

            if (query.exec() && query.next()) {
                Premier premier;
                premier.key = query.value(0).toInt();
                premier.setModal(true);
                premier.exec();
            } else {
                ui->statusbar->showMessage("Неправильный логин или пароль");
            }

        } else {
            ui->statusbar->showMessage("Ошибка при открытии базы данных: " + db.lastError().text());
        }
    }
}


void MainWindow::on_logUp_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();
    if (ui->login->text() == "" || ui->password->text() == "")
    {
        QMessageBox::warning(this, "Error", "Заполните все поля!");
        return;
    }else{
        if (db.open()) {
            QSqlQuery query;
            query.prepare("INSERT INTO account (login, pass) VALUES (:login, :pass)");
            query.bindValue(":login", login);
            query.bindValue(":pass", password);

            if (query.exec()) {
                ui->statusbar->showMessage("Вы успешно авторизовались");
            } else {
                ui->statusbar->showMessage("Ошибка при добавлении записи: " + query.lastError().text());
            }
        } else {
            ui->statusbar->showMessage("Ошибка при открытии базы данных: " + db.lastError().text());
        }
    }
}


void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, "Выберите изображение",
    QDir::currentPath(), "*.png *.jpg *.gif *.jpeg");
    QImage image1(filename);
    ui->label_3->setPixmap(QPixmap::fromImage(image1));
}

