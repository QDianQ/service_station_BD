#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSqlDataBase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
#include <QCompleter>
#include <QStringList>
#include <QMessageBox>
#include <QSqlError>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QVariant>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectToDB();
    void setCompleterLE();
    void updateCompleter();
    void deleteNullRows();
    void sortTable();
    void insertRow();

private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_lineEdit_5_textEdited(const QString &arg1);

    void on_action_triggered();

    void on_pushButton_3_clicked();

    void on_action_2_triggered();

    void on_comboBox_activated(int index);

    void on_comboBox_2_activated(int index);



    void on_lineEdit_returnPressed();

    void on_lineEdit_2_returnPressed();

    void on_lineEdit_3_returnPressed();

    void on_lineEdit_4_returnPressed();

    void on_action_5_triggered();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db, hideDB, lowwerDB;
    QSqlQuery query;
    QSqlTableModel *model, *hide_model;
    QCompleter *completer;


    QModelIndex model_index;
    int indexColumn, indexSort;
    QString name, number, comm, count, deleteCell;
    QStringList str_list;
};
#endif // MAINWINDOW_H
