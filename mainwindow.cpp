#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_3->setVisible(false);
    indexColumn = 0;
    indexSort = 0;

    connectToDB();
    query = QSqlQuery(db);
    createTable();
    model = new QSqlTableModel(this,db);
    model->setTable("maintable");
    sortTable();
    model->select();

    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #FFFFFF }");
    ui->tableView->setModel(model);


    setSizeColumns();

    qDebug() << "table's names"
             << db.tables();
    qDebug() << "active table"
             << model->tableName();
    qDebug() << "active db"
             << model->database();

    setCompleterLE();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSizeColumns()
{
    ui->tableView->horizontalHeader()->setHighlightSections(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);

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
    hide_model->setTable("hidetable");
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

void MainWindow::createTable()
{
    query.exec("create table mainTable(\"Номер детали\" varchar(255),"
                                       "Название varchar(255),"
                                       "Количество integer,"
                                       "Комментарий varchar(255));"

                "create table hideTable(uniq_id SERIAL,"
                                       "\"Номер детали\" varchar(255),"
                                       "Название varchar(255));"

                "create table nameTable(uniq_name SERIAL,"
                                       "Название varchar(255) not null unique);"

                "create table numberTable(uniq_number SERIAL,"
                                       "\"Номер детали\" varchar(255) not null unique);");
}

void MainWindow::insertRow()
{
    number = ui->lineEdit->text();
    name = ui->lineEdit_2->text();
    count = ui->lineEdit_3->text();
    comm = ui->lineEdit_4->text();

    if(count=="")
        count="1";

    qDebug() << "Number: "
             << number.toStdString().c_str();
    qDebug() << "Name: "
             << name.toStdString().c_str();
    qDebug() << "Count: "
             << count.toStdString().c_str();
    qDebug() << "Comm: "
             << comm.toStdString().c_str();

    //    --- insert a row to the maintable ---
    query.prepare("INSERT INTO maintable(\"Номер детали\", Название, Количество, Комментарий)"
                   "VALUES(:number,:name, :count, :comm);");
    query.bindValue(":number",number);
    query.bindValue(":name",name);
    query.bindValue(":count",count.toInt());
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

//    --- insert a row to the hidetable ---
    query.prepare("INSERT INTO hidetable(\"Номер детали\",Название)"
                  "VALUES(NULL, NULL)");
    if (!query.exec())
    {
        qDebug() << "Error: " << query.lastError().text();
    }
    else
    {
        updateCompleter();
        qDebug() << "insert into null hidetable is succesful";
    }
    if (ui->lineEdit->text() != ""){
        query.prepare("INSERT INTO numbertable(\"Номер детали\")"
                      "VALUES(:number)");
        query.bindValue(":number",number);

        if (!query.exec())
        {
            qDebug() << "Error: " << query.lastError().text();
        }
        else
        {
            updateCompleter();
            qDebug() << "insert into numbertable is succesful";
        }
    }
    if (ui->lineEdit_2->text() != ""){
        query.prepare("INSERT INTO nametable(Название)"
                       "VALUES(:name)");
        query.bindValue(":name",name);

        if (!query.exec())
        {
            qDebug() << "Error: " << query.lastError().text();
        }
        else
        {
            updateCompleter();
            qDebug() << "insert into namerdb is succesful";
        }
    }
    query.prepare("UPDATE hidetable SET \"Номер детали\" = numbertable.\"Номер детали\" FROM numbertable WHERE uniq_id = numbertable.uniq_number;");

    if (!query.exec())
    {
        qDebug() << "Error: " << query.lastError().text();
    }
    else
    {
        updateCompleter();
        qDebug() << "update 1 is succesful";
    }
    query.prepare("UPDATE hidetable SET Название = nametable.Название FROM nametable WHERE uniq_id = nametable.uniq_name;");

    if (!query.exec())
    {
        qDebug() << "Error: " << query.lastError().text();
    }
    else
    {
        updateCompleter();
        qDebug() << "update 2 is succesful";
    }
//    --- the end of insert ---
    sortTable();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();

}

void MainWindow::on_pushButton_clicked() //insert row
{
    insertRow();
}

void MainWindow::on_pushButton_2_clicked() //delete row
{
    if(model->tableName()=="maintable")
    {
        if(!model->removeRow(model_index.row()))
            qDebug() << "Delete error: "
                     << model->lastError().text();
        else
            qDebug() << "Delete from maintable succesful";
    }
    else
    {
        if(!model->clearItemData(model_index))
            qDebug() << "Delete item error: "
                     << model->lastError().text();
        else
        {
            model->submitAll();
            qDebug() << "isEmpty "
                     << model->itemData(model_index).empty();
            qDebug() << "Delete item succesful";
        }

        if (query.exec(QString("DELETE FROM nametable WHERE Название = '%1'").arg(deleteCell)))
                qDebug() << "delete from name";

        if(query.exec(QString("DELETE FROM numbertable WHERE \"Номер детали\" = '%1'").arg(deleteCell)))
                qDebug() << "delete from number";

    }

    sortTable();
    updateCompleter();

}

void MainWindow::on_tableView_clicked(const QModelIndex &index) //choose row from table
{
    model_index = index;
    deleteCell = model->data(model_index).toString();
    QVariant getData = model->data(model_index);



    qDebug() << "deleteCell "
             << deleteCell;

    qDebug() << "item data"
             << model->data(model_index);

    qDebug() << "index row: "
             << model_index.row();

    qDebug() << "index column: "
             << model_index.column();
}

void MainWindow::on_lineEdit_5_textEdited(const QString &arg1)  //search text in rows
{

//    --- searching for number ---
    model->setFilter(QString("\"Номер детали\" ILIKE '%%1%'").arg(arg1));

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

void MainWindow::deleteNullRows()
{
    query.exec("SELECT * FROM nametable");
    int countOfName = query.size();
    qDebug() << "nametable "
             << countOfName;

    query.exec("SELECT * FROM numbertable");
    int countOfNumber = query.size();
    qDebug() << "numbertable "
             << countOfNumber;

    query.exec("SELECT * FROM hidetable");
    int countOfHide = query.size();
    qDebug() << "hidetable "
             << countOfHide;

    if( (countOfName < countOfHide) or (countOfNumber < countOfHide))
    {

        int currentOfMax = __max(countOfName,countOfNumber) + 1;

        if(!query.exec(QString("DELETE FROM hidetable WHERE \"Номер детали\" IS NULL AND Название IS NULL;"
                   "SELECT pg_get_serial_sequence('hidetable', 'uniq_id');"
                   "ALTER SEQUENCE public.hidetable_uniq_id_seq RESTART WITH %1;").arg(currentOfMax)))

            qDebug() << "Error ALTER: " << query.lastError().text();
        else
        {
        updateCompleter();
        qDebug() << "delete complete";
        }

    }
}

void MainWindow::on_action_triggered()  //MenuBar.Actions "Редактировать записи",
{
    ui->pushButton_3->setVisible(true);
    ui->pushButton->setVisible(false);

    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->lineEdit_4->setVisible(false);

    deleteNullRows();

    model->setTable("hidetable");
    model->select();

    ui->tableView->horizontalHeader()->setHighlightSections(true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);



    ui->tableView->setColumnHidden(0,true);

    qDebug() << model->tableName();
}

void MainWindow::on_pushButton_3_clicked()  //Button "Сохранить"
{
    ui->pushButton_3->setVisible(false);
    ui->pushButton->setVisible(true);

    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->label_3->setVisible(true);
    ui->label_4->setVisible(true);
    ui->lineEdit->setVisible(true);
    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_3->setVisible(true);
    ui->lineEdit_4->setVisible(true);
    model->setTable("maintable");
    sortTable();
    setSizeColumns();
    qDebug() << model->tableName();
}

void MainWindow::on_action_2_triggered()    //MenuBar.Actions "Очистить таблицу"
{
    QMessageBox msgBox;
    QPushButton *cancelBtn, *deleteBtn;
    cancelBtn = new QPushButton();
    deleteBtn = new QPushButton();

    msgBox.setText("Вы точно хотите очистить ВСЕ данные из таблицы?");
    msgBox.setInformativeText("Действие невозможно отменить.");

    msgBox.addButton(cancelBtn, QMessageBox::NoRole);
    msgBox.addButton(deleteBtn, QMessageBox::YesRole);

    msgBox.setButtonText(0,"Отменить");
    msgBox.setButtonText(1,"Удалить");

    int ret = msgBox.exec();

    qDebug() << "ret " << ret;
    switch (ret) {
      case 0:
          // Save was clicked
          break;
      case 1:
        qDebug() << "del";
          query.exec("delete from maintable where \"Номер детали\" is not null;");
          model->select();
          break;
      default:
          // should never be reached
          break;
    }


}

void MainWindow::sortTable()    //sort table
{
    if(indexSort==0)
    {
        model->sort(indexColumn,Qt::AscendingOrder);
        qDebug() <<"asc";
    }
    if(indexSort==1)
    {
        model->sort(indexColumn,Qt::DescendingOrder);
        qDebug() <<"dsc";
    }
    model->select();

}

void MainWindow::on_comboBox_activated(int index)   // ComboBox choose columns for sort
{
    indexColumn = index;
    sortTable();
}

void MainWindow::on_comboBox_2_activated(int index) // ComboBox choose types for sort
{
    indexSort = index;
    sortTable();
}

void MainWindow::on_lineEdit_returnPressed()    //press enter into line_edit
{
    insertRow();
}

void MainWindow::on_lineEdit_2_returnPressed()  //press enter into line_edit
{
    insertRow();
}

void MainWindow::on_lineEdit_3_returnPressed()  //press enter into line_edit
{
    insertRow();
}

void MainWindow::on_lineEdit_4_returnPressed()  //press enter into line_edit
{
    insertRow();
}


void MainWindow::on_action_5_triggered()    //MenuBar.Actions "Очистить все записи автозаполнения"
{
    QMessageBox msgBox;
    QPushButton *cancelBtn, *deleteBtn;
    cancelBtn = new QPushButton();
    deleteBtn = new QPushButton();

    msgBox.setText("Вы точно хотите очистить ВСЕ записи автозаполнения?");
    msgBox.setInformativeText("Действие невозможно отменить.");

    msgBox.addButton(cancelBtn, QMessageBox::NoRole);
    msgBox.addButton(deleteBtn, QMessageBox::YesRole);

    msgBox.setButtonText(0,"Отменить");
    msgBox.setButtonText(1,"Удалить");

    int ret = msgBox.exec();

    qDebug() << "ret " << ret;
    switch (ret) {
      case 0:
          // Save was clicked
          break;
      case 1:
        qDebug() << "del";
          query.exec("delete from hidetable where \"Номер детали\" is null or Название is null;"
                     "delete from hidetable where \"Номер детали\" is not null or Название is not null;"
                     "SELECT pg_get_serial_sequence('hidetable', 'uniq_id');"
                     "ALTER SEQUENCE public.hidetable_uniq_id_seq RESTART WITH 1;"

                     "delete from numbertable where \"Номер детали\" is not null;"
                     "SELECT pg_get_serial_sequence('numbertable', 'uniq_number');"
                     "ALTER SEQUENCE public.numbertable_uniq_number_seq RESTART WITH 1;"

                     "delete from nametable where Название is not null;"
                     "SELECT pg_get_serial_sequence('nametable', 'uniq_name');"
                     "ALTER SEQUENCE public.nametable_uniq_name_seq RESTART WITH 1;");
          model->select();
          break;
      default:
          // should never be reached
          break;
    }

    updateCompleter();
}

