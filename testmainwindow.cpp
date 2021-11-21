#include "testmainwindow.h"
#include "ui_testmainwindow.h"

#include <QMessageBox> //для вывода сообщения пользователю
#include <QSqlDatabase>
#include <QSqlError>

TestMainWindow::TestMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestMainWindow)
{
    ui->setupUi(this);

//    connectToDB();
    QSqlDatabase sqliteDB = QSqlDatabase::addDatabase("QSQLITE");
    sqliteDB.setDatabaseName("./sqliteDB.db");
    if (!sqliteDB.open())
    QMessageBox::critical(NULL,QObject::tr("Ошибка"),sqliteDB.lastError().text());
    else
        qDebug() << "Connect succesful";

    query = QSqlQuery(sqliteDB);

    model = new QSqlTableModel(this, sqliteDB);
    model->setTable("mainDB");
    model->select();

    ui->tableView->setModel(model);


    qDebug() << "name " << sqliteDB.databaseName();
    qDebug() << "active table" << model->tableName();
    qDebug() << "active db" << model->database();

}
TestMainWindow::~TestMainWindow()
{
    delete ui;
}
void TestMainWindow::connectToDB()
{
    /*QPSQL - название драйвера для подключения к той или иной СУБД, в данном случае - к PostgreSQL; для подключения к базе MS SQL Server вам необходимо сначала настроить источник данных ODBC*/

    db = QSqlDatabase::addDatabase("QPSQL");  /*после наименование драйвера БД также можно указать произвольное наименование подключения к серверу БД: addDatabase("QPSQL", "myConnection1")
    имя или адрес сервера, где находится СУБД*/
    db.setHostName("127.0.0.1");
    //имя базы данных
    db.setDatabaseName("testMainDB");
    //имя пользователя
    db.setUserName("postgres");
    //его пароль
    db.setPassword("1234");
    db.setPort(5432);
    /*открываем подключение; если неудачно - выводим сообщение об ошибке*/
    if (!db.open())
    QMessageBox::critical(NULL,QObject::tr("Ошибка"),db.lastError().text());
    else
        qDebug() << "Connect succesful";





}
void TestMainWindow::on_pushButton_clicked()    //insert button
{
    qDebug() << "clicked the insert button";
    query.prepare("INSERT INTO mainDB(Number, Name, Count, Commentaries)"
                  "VALUES('num_qt', 'name_qt', 'count_qt', 'comm_qt');");

    if (!query.exec())
        qDebug() << query.lastError().text();
    else
        qDebug() << "query's succesful";
}
void TestMainWindow::on_pushButton_2_clicked() //create table
{
    qDebug() << "clicked the select() button";
    model->select();
}

