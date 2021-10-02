#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSqlDataBase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
#include <QCompleter>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_clicked(const QModelIndex &index);

     void on_lineEdit_5_textEdited(const QString &arg1);


private:
    Ui::MainWindow *ui;
    QSqlDatabase main_db, hideDB, lowwerDB;
    QSqlQuery query, query_hideDB, query_lowwerDB;
    QSqlTableModel *model, *model_hideDB, *model_lowwerDB;
    QCompleter *completer;

    int row;
    QString name, number, comm, search_original, count, testStr;
    QStringList str_list;
};
#endif // MAINWINDOW_H
