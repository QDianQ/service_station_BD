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


private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_clicked(const QModelIndex &index);

     void on_lineEdit_5_textEdited(const QString &arg1);

     void on_radioButton_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db, hideDB, lowwerDB;
    QSqlQuery query;
    QSqlTableModel *model, *hide_model;
    QCompleter *completer;


    QModelIndex model_index;
    QString name, number, comm, search_original, count, testStr;
    QStringList str_list;
};
#endif // MAINWINDOW_H
