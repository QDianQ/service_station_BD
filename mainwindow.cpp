#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->verticalLayout_4);

//    adjustSize();

    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("./database.db");

    hideDB.setDatabaseName("./hideDB.db");

    if(database.open())
    {
        qDebug("open");
    }
    else
    {
        qDebug("no open");
    }

    query = QSqlQuery(database);
    query.exec("CREATE TABLE DataBase(№ INTEGER PRIMARY KEY AUTOINCREMENT, Number varchar(255), Name varchar(255), Count varchar(255), Commentaries varchar(255));");

    model = new QSqlTableModel(this, database);
    model->setTable("DataBase");
    model->select();

    query_hideDB = QSqlQuery(hideDB);
    query_hideDB.exec("CREATE TABLE hideDB(№ INTEGER PRIMARY KEY AUTOINCREMENT, Number varchar(255), Name varchar(255), Count varchar(255), Commentaries varchar(255));");

    model_hideDB = new QSqlTableModel(this, hideDB);
    model_hideDB->setTable("hideDB");
    model_hideDB->select();

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0,true);

    QCompleter *completer_number, *completer_name;

    completer_number = new QCompleter(model_hideDB,this);
    completer_number->setCaseSensitivity(Qt::CaseInsensitive);
    completer_number->setCompletionColumn(1);
    ui->lineEdit->setCompleter(completer_number);

    completer_name = new QCompleter(model_hideDB,this);
    completer_name->setCaseSensitivity(Qt::CaseInsensitive);
    completer_name->setCompletionColumn(2);
    ui->lineEdit_2->setCompleter(completer_name);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked() //add row
{
    number = ui->lineEdit->text();
    name = ui->lineEdit_2->text();
    count = ui->lineEdit_3->text();
    comm = ui->lineEdit_4->text();

    qDebug("%s", name.toStdString().c_str());
    qDebug("%s", number.toStdString().c_str());
    qDebug("%s", comm.toStdString().c_str());
    qDebug() << "row count", model->rowCount();

    query.prepare("INSERT INTO DataBase(Number, Name, Count, Commentaries)"
                   "VALUES(:number,:name, :count, :comm);");
    query.bindValue(":number",number);
    query.bindValue(":name",name);
    query.bindValue(":count",count);
    query.bindValue(":comm",comm);
    query.exec();
    model->select();

    query_hideDB.prepare("INSERT INTO hideDB(Number, Name, Count, Commentaries)"
                   "VALUES(:number,:name, :count, :comm);");
    query_hideDB.bindValue(":number",number);
    query_hideDB.bindValue(":name",name);
    query_hideDB.bindValue(":count",count);
    query_hideDB.bindValue(":comm",comm);
    query_hideDB.exec();
    model_hideDB->select();

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}

void MainWindow::on_pushButton_2_clicked() //delete row
{
    qDebug("before delete %d", model->rowCount());
    model->removeRow(row);
    qDebug("after delete %d", model->rowCount());
    model->select();
}
void MainWindow::on_tableView_clicked(const QModelIndex &index) //choose row from table
{
    row = index.row();
    qDebug("index %d", row);
}
void MainWindow::on_lineEdit_5_textEdited(const QString &arg1)  //search text in rows
{
    model->setFilter(QString("Name LIKE '%%1%'").arg(arg1));
    int row = model->rowCount();
    int row_num = -1;
    qDebug() << model->filter();
    if(row==0)
    {
        model->setFilter(QString("Number LIKE '%%1%'").arg(arg1));
        row_num = model->rowCount();
    }
    if(row_num==0)
        model->setFilter(QString("Commentaries LIKE '%%1%'").arg(arg1));

    model->select();

    qDebug("search: %s", arg1.toStdString().c_str());
    query.prepare("SELECT Name FROM DataBase WHERE Name = :search;");
    query.bindValue(":search",arg1);
    query.exec();
    qDebug() << query.isValid();
    qDebug() << query.isSelect();
}

void MainWindow::on_lineEdit_6_textChanged(const QString &arg1)
{
//    qDebug() << arg1.length();
//    qDebug() << testStr.at(1);

//    QString str = "";

//    for (int i = 0; i < arg1.length(); i++)
//    {
//        str += arg1.at(i);
//        qDebug() << str;
//    }
}
