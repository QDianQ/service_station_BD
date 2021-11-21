#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->verticalLayout_4);

//    adjustSize();

    connectToDB();

    query = QSqlQuery(db);

    model = new QSqlTableModel(this,db);
    model->setTable("maindb");
    model->select();

    ui->tableView->setModel(model);

    qDebug() << "table's names"
             << db.tables();
    qDebug() << "active table"
             << model->tableName();
    qDebug() << "active db"
             << model->database();


//    ui->tableView->setColumnHidden(0,true);

    setCompleterLE();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToDB()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("testMainDB");
    db.setUserName("postgres");
    db.setPassword("1234");

    if (!db.open())
    QMessageBox::critical(NULL,QObject::tr("Ошибка"),db.lastError().text());
    else
        qDebug() << "Connect succesful";
}

void MainWindow::setCompleterLE()
{
    QCompleter *completer_number, *completer_name;

    hide_model = new QSqlTableModel(this,db);
    hide_model->setTable("hidedb");
    hide_model->select();

    completer_number = new QCompleter(hide_model,this);
    completer_number->setCaseSensitivity(Qt::CaseInsensitive);
    completer_number->setCompletionColumn(1);
    ui->lineEdit->setCompleter(completer_number);

    completer_name = new QCompleter(hide_model,this);
    completer_name->setCaseSensitivity(Qt::CaseInsensitive);
    completer_name->setCompletionColumn(2);
    ui->lineEdit_2->setCompleter(completer_name);
}

void MainWindow::updateCompleter()
{
    hide_model->select();
}

void MainWindow::on_pushButton_clicked() //insert row
{

    number = ui->lineEdit->text();
    name = ui->lineEdit_2->text();
    count = ui->lineEdit_3->text();
    comm = ui->lineEdit_4->text();

    qDebug() << "Name: "
             << name.toStdString().c_str();
    qDebug() << "Number: "
             << number.toStdString().c_str();
    qDebug() << "Count: "
             << count.toStdString().c_str();
    qDebug() << "Comm: "
             << comm.toStdString().c_str();

    //    --- insert a row to the maindb ---
    query.prepare("INSERT INTO maindb(Номер, Название, Количество, Комментарий)"
                   "VALUES(:number,:name, :count, :comm);");
    query.bindValue(":number",number);
    query.bindValue(":name",name);
    query.bindValue(":count",count);
    query.bindValue(":comm",comm);

    if (!query.exec())
        qDebug() << "Error: "
                 << query.lastError().text();
    else
    {
        model->select();
        qDebug() << "query's succesful";
    }
//    --- the end of insert ---

//    --- insert a row to the hidedb ---
    query.prepare("INSERT INTO hidedb(Номер)"
                   "VALUES(:number);");
    query.bindValue(":number",number);

    if (!query.exec())
    {
        qDebug() << "Error: " << query.lastError().text();
    }
    else
    {
        updateCompleter();
        qDebug() << "query's succesful";
    }
    query.prepare("INSERT INTO hidedb(Название)"
                   "VALUES(:name);");
    query.bindValue(":name",name);

    if (!query.exec())
    {
        qDebug() << "Error: " << query.lastError().text();
    }
    else
    {
        updateCompleter();
        qDebug() << "query's succesful";
    }

//    --- the end of insert ---
}

void MainWindow::on_pushButton_2_clicked() //delete row
{
//    if(!model->removeRow(model_index.row()))
//        qDebug() << "Delete error: "
//                 << model->lastError().text();
//    else
//        qDebug() << "Delete succesful";

    if(!hide_model->clearItemData(model_index))
        qDebug() << "Delete item error: "
                 << hide_model->lastError().text();
    else
        qDebug() << "Delete item succesful";

    model->select();
    updateCompleter();
}

void MainWindow::on_tableView_clicked(const QModelIndex &index) //choose row from table
{
    model_index = index;

    qDebug() << "item data"
             << hide_model->data(model_index);


    qDebug() << "index row: "
             << model_index.row();

    qDebug() << "index column: "
             << model_index.column();
}

void MainWindow::on_lineEdit_5_textEdited(const QString &arg1)  //search text in rows
{

//    --- searching for number ---
    model->setFilter(QString("Номер ILIKE '%%1%'").arg(arg1));

    qDebug() << "searching for name...  "
             << arg1;
    qDebug() << "count of rows "
             << model->rowCount();

//    --- searching for name ---
    if(model->rowCount()==0)
    {
    model->setFilter(QString("Название ILIKE '%%1%'").arg(arg1));

    qDebug() << "searching for number...  "
             << arg1;
    qDebug() << "count of rows "
             << model->rowCount();
    }

//    --- searching for commentary ---
    if(model->rowCount()==0)
    {
    model->setFilter(QString("Комментарий ILIKE '%%1%'").arg(arg1));

    qDebug() << "searching for commentaries...  "
             << arg1;
    qDebug() << "count of rows "
             << model->rowCount();
    }
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    qDebug() << "Editer is "
             << checked;

    if (checked)
        ui->tableView->setModel(hide_model);
    else
        ui->tableView->setModel(model);

//    if (checked)
//        model->setTable("hidedb");
//    else
//        model->setTable("maindb");
//    model->select();

}
