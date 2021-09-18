#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->verticalLayout_4);

//    adjustSize();

    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("./testDB.db");

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

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0,true);
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

    model->setFilter(QString("upper(Name, ru_ru) LIKE '%%1%'").arg(arg1));
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


//    model->setFilter(QString("Number LIKE '%%1%'").arg(arg1));
//    model->setFilter(QString("Commentaries LIKE '%%1%'").arg(arg1));
    model->select();

    qDebug("search: %s", arg1.toStdString().c_str());
    query.prepare("SELECT Name FROM DataBase WHERE Name = :search;");
    query.bindValue(":search",arg1);
    query.exec();
    qDebug() << query.isValid();
    qDebug() << query.isSelect();
}
