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

    main_db = QSqlDatabase::addDatabase("QSQLITE");

    main_db.setDatabaseName("./main_db.db");
    hideDB.setDatabaseName("./hideDB.db");
    lowwerDB.setDatabaseName("./lowwerDB.db");

    if(main_db.open())
    {
        qDebug("open main");
    }
    else
    {
        qDebug("no open");
    }
    if(hideDB.open())
    {
        qDebug("open main");
    }
    else
    {
        qDebug("no open");
    }
    if(lowwerDB.open())
    {
        qDebug("open main");
    }
    else
    {
        qDebug("no open");
    }


    //main_db for visual contents
    query = QSqlQuery(main_db);
    query.exec("CREATE TABLE main_db(№ INTEGER PRIMARY KEY AUTOINCREMENT, Number varchar(255), Name varchar(255), Count varchar(255), Commentaries varchar(255));");

    model = new QSqlTableModel(this, main_db);
    model->setTable("main_db");
    model->select();

    //hide_db for creating fully contents db
    query_hideDB = QSqlQuery(hideDB);
    query_hideDB.exec("CREATE TABLE hideDB(№ INTEGER PRIMARY KEY AUTOINCREMENT, Number varchar(255), Name varchar(255), Count varchar(255), Commentaries varchar(255));");

    model_hideDB = new QSqlTableModel(this, hideDB);
    model_hideDB->setTable("hideDB");
    model_hideDB->select();

    //lowwerDB for search into main_db using ru language
    query_lowwerDB = QSqlQuery(lowwerDB);
    query_lowwerDB.exec("CREATE TABLE lowwerDB(№ INTEGER PRIMARY KEY AUTOINCREMENT, Number varchar(255), Name varchar(255), Count varchar(255), Commentaries varchar(255));");

    model_lowwerDB = new QSqlTableModel(this, lowwerDB);
    model_lowwerDB->setTable("lowwerDB");
    model_lowwerDB->select();

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

    query.prepare("INSERT INTO main_db(Number, Name, Count, Commentaries)"
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

    query_lowwerDB.prepare("INSERT INTO lowwerDB(Number, Name, Count, Commentaries)"
                   "VALUES(:number,:name, :count, :comm);");
    query_lowwerDB.bindValue(":number",number.toLower());
    query_lowwerDB.bindValue(":name",name.toLower());
    query_lowwerDB.bindValue(":count",count.toLower());
    query_lowwerDB.bindValue(":comm",comm.toLower());
    query_lowwerDB.exec();
    model_lowwerDB->select();

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

    qDebug("search_original: %s", arg1.toStdString().c_str());
    query.prepare("SELECT Name FROM main_db WHERE Name = :search_original;");
    query.bindValue(":search_original",arg1);
    query.exec();
    qDebug() << query.isValid();
    qDebug() << query.isSelect();
    qDebug() << arg1.toLower();
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

//нужно реализовать это в qt
//SELECT table_1.string from table_1 , table_2
//WHERE
//table_1.number = table_2.number AND
//table_2.string LIKE '%t%';
