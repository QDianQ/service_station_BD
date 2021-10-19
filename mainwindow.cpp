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

    mainDB = QSqlDatabase::addDatabase("QSQLITE");

    mainDB.setDatabaseName("./mainDB.db");
//    hideDB.setDatabaseName("./hideDB.db");
//    lowwerDB.setDatabaseName("./lowwerDB.db");

    if(mainDB.open())
        qDebug("open main");
    else
        qDebug("no open");


    //mainDB for visual contents
    query = QSqlQuery(mainDB);
    query.exec("CREATE TABLE mainDB(prime_ID INTEGER PRIMARY KEY AUTOINCREMENT, Number varchar(255), Name varchar(255), Count varchar(255), Commentaries varchar(255));");

    //hide_db for creating fully contents db
    query.exec("CREATE TABLE hideDB(prime_ID INTEGER PRIMARY KEY AUTOINCREMENT, Number varchar(255), Name varchar(255), Count varchar(255), Commentaries varchar(255));");

    //lowwerDB for search into mainDB using ru language
    query.exec("CREATE TABLE lowwerDB(prime_ID INTEGER PRIMARY KEY AUTOINCREMENT, Number varchar(255), Name varchar(255), Count varchar(255), Commentaries varchar(255));");

    model = new QSqlTableModel(this, mainDB);
    model->setTable("mainDB");
    model->select();

    qDebug() << model->tableName();

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0,true);

    QCompleter *completer_number, *completer_name;

    completer_number = new QCompleter(model,this);
    completer_number->setCaseSensitivity(Qt::CaseInsensitive);
    completer_number->setCompletionColumn(1);
    ui->lineEdit->setCompleter(completer_number);

    completer_name = new QCompleter(model,this);
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

    query.prepare("INSERT INTO mainDB(Number, Name, Count, Commentaries)"
                   "VALUES(:number,:name, :count, :comm);");
    query.bindValue(":number",number);
    query.bindValue(":name",name);
    query.bindValue(":count",count);
    query.bindValue(":comm",comm);
    query.exec();
//    model->select();

    query.prepare("INSERT INTO hideDB(Number, Name, Count, Commentaries)"
                   "VALUES(:number,:name, :count, :comm);");
    query.bindValue(":number",number);
    query.bindValue(":name",name);
    query.bindValue(":count",count);
    query.bindValue(":comm",comm);
    query.exec();
//    model->select();

    query.prepare("INSERT INTO lowwerDB(Number, Name, Count, Commentaries)"
                   "VALUES(:number,:name, :count, :comm);");
    query.bindValue(":number",number.toLower());
    query.bindValue(":name",name.toLower());
    query.bindValue(":count",count.toLower());
    query.bindValue(":comm",comm.toLower());
    query.exec();

    model->select();

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


//    model->setFilter(QString("Name LIKE '%%1%'").arg(arg1));
//    int row = model->rowCount();
//    int row_num = -1;
//    qDebug() << model->filter();
//    if(row==0)
//    {
//        model->setFilter(QString("Number LIKE '%%1%'").arg(arg1));
//        row_num = model->rowCount();
//    }
//    if(row_num==0)
//        model->setFilter(QString("Commentaries LIKE '%%1%'").arg(arg1));

//    model->select();

//    qDebug("search_original: %s", arg1.toStdString().c_str());
//    query.prepare("SELECT Name FROM mainDB WHERE Name = :search_original;");
//    query.bindValue(":search_original",arg1);
//    query.exec();
//    qDebug() << query.isValid();
//    qDebug() << query.isSelect();
//    qDebug() << arg1.toLower();


    qDebug("search_original: %s", arg1.toStdString().c_str());

//    query.prepare("SELECT mainDB.Name"
//                  "FROM mainDB, lowwerDB"
//                  "WHERE "
//                  "mainDB.prime_ID = lowwerDB.prime_ID AND"
//                  "lowwerDB.Name LIKE %:search_result%;");

//    query.bindValue(":search_result",arg1.toLower());


//    query.exec();
//    model->setFilter(QString("SELECT mainDB.Name"
//                             "FROM mainDB, lowwerDB"
//                             "WHERE "
//                             "mainDB.prime_ID = lowwerDB.prime_ID AND"
//                             "lowwerDB.Name LIKE '%%1%';").arg(arg1));

    model->setTable("mainDB");
    qDebug() << model->tableName();
    model->setFilter(QString("mainDB.Number LIKE '%%1%'"
                             "OR mainDB.Name LIKE '%%2%'").arg(arg1).arg(arg1));

    model->select();
    qDebug() << model->filter();
    qDebug() << model->select();
}

//void MainWindow::on_lineEdit_6_textChanged(const QString &arg1)
//{
//    qDebug() << arg1.length();
//    qDebug() << testStr.at(1);

//    QString str = "";

//    for (int i = 0; i < arg1.length(); i++)
//    {
//        str += arg1.at(i);
//        qDebug() << str;
//    }
//}

//Нужно реализовать это в qt
//SELECT table_1.string from table_1 , table_2
//WHERE
//table_1.number = table_2.number AND
//table_2.string LIKE '%t%';

//возможно нужен QSqlRelationalTableModel
//https://itnan.ru/post.php?c=1&p=435134
