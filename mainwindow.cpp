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

//    adjustSize();

    connectToDB();

    query = QSqlQuery(db);

    model = new QSqlTableModel(this,db);
    model->setTable("maindb");
    sortTable();
    model->select();

    ui->tableView->setModel(model);

//    ui->tableView->horizontalHeader()

    ui->tableView->horizontalHeader()->setHighlightSections(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);


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

    //    --- insert a row to the maindb ---
    query.prepare("INSERT INTO maindb(Номер, Название, Количество, Комментарий)"
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

//    --- insert a row to the hidedb ---
    query.prepare("INSERT INTO hidedb(Номер,Название)"
                  "VALUES(NULL, NULL)");
    if (!query.exec())
    {
        qDebug() << "Error: " << query.lastError().text();
    }
    else
    {
        updateCompleter();
        qDebug() << "insert into null hidedb is succesful";
    }
    if (ui->lineEdit->text() != ""){
        query.prepare("INSERT INTO numberdb(Номер)"
                      "VALUES(:number)");
        query.bindValue(":number",number);

        if (!query.exec())
        {
            qDebug() << "Error: " << query.lastError().text();
        }
        else
        {
            updateCompleter();
            qDebug() << "insert into numberdb is succesful";
        }
    }
    if (ui->lineEdit_2->text() != ""){
        query.prepare("INSERT INTO namedb(Название)"
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
    query.prepare("UPDATE hidedb SET Номер = numberdb.Номер FROM numberdb WHERE uniq_id = numberdb.uniq_number;");

    if (!query.exec())
    {
        qDebug() << "Error: " << query.lastError().text();
    }
    else
    {
        updateCompleter();
        qDebug() << "update 1 is succesful";
    }
    query.prepare("UPDATE hidedb SET Название = namedb.Название FROM namedb WHERE uniq_id = namedb.uniq_name;");

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
    if(model->tableName()=="maindb")
    {
        if(!model->removeRow(model_index.row()))
            qDebug() << "Delete error: "
                     << model->lastError().text();
        else
            qDebug() << "Delete from maindb succesful";
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

        if (query.exec(QString("DELETE FROM namedb WHERE Название = '%1'").arg(deleteCell)))
                qDebug() << "delete from name";

        if(query.exec(QString("DELETE FROM numberdb WHERE Номер = '%1'").arg(deleteCell)))
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

void MainWindow::deleteNullRows()
{
    query.exec("SELECT * FROM namedb");
    int countOfName = query.size();
    qDebug() << "namedb "
             << countOfName;

    query.exec("SELECT * FROM numberdb");
    int countOfNumber = query.size();
    qDebug() << "numberdb "
             << countOfNumber;

    query.exec("SELECT * FROM hidedb");
    int countOfHide = query.size();
    qDebug() << "hidedb "
             << countOfHide;

    if( (countOfName < countOfHide) or (countOfNumber < countOfHide))
    {

        int currentOfMax = __max(countOfName,countOfNumber) + 1;

        if(!query.exec(QString("DELETE FROM hidedb WHERE Номер IS NULL AND Название IS NULL;"
                   "SELECT pg_get_serial_sequence('hidedb', 'uniq_id');"
                   "ALTER SEQUENCE public.hidedb_uniq_id_seq RESTART WITH %1;").arg(currentOfMax)))

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

    model->setTable("hidedb");
    model->select();
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
    model->setTable("maindb");
    sortTable();
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
          query.exec("delete from maindb where Номер is not null;");
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
          query.exec("delete from hidedb where Номер is null or Название is null;"
                     "delete from hidedb where Номер is not null or Название is not null;"
                     "SELECT pg_get_serial_sequence('hidedb', 'uniq_id');"
                     "ALTER SEQUENCE public.hidedb_uniq_id_seq RESTART WITH 1;"

                     "delete from numberdb where Номер is not null;"
                     "SELECT pg_get_serial_sequence('numberdb', 'uniq_number');"
                     "ALTER SEQUENCE public.numberdb_uniq_number_seq RESTART WITH 1;"

                     "delete from namedb where Название is not null;"
                     "SELECT pg_get_serial_sequence('namedb', 'uniq_name');"
                     "ALTER SEQUENCE public.namedb_uniq_name_seq RESTART WITH 1;");
          model->select();
          break;
      default:
          // should never be reached
          break;
    }

    updateCompleter();
}

