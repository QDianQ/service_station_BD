#ifndef TESTMAINWINDOW_H
#define TESTMAINWINDOW_H

#include <QMainWindow>

#include <QSqlDataBase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
#include <QCompleter>
#include <QStringList>

namespace Ui {
class TestMainWindow;
}

class TestMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestMainWindow(QWidget *parent = nullptr);
    ~TestMainWindow();
    void connectToDB();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::TestMainWindow *ui;
    QSqlDatabase mainDB, hideDB, lowwerDB, db;
    QSqlQuery query;
    QSqlTableModel *model;
    QCompleter *completer;

    int row;
    QString name, number, comm, search_original, count, testStr;
    QStringList str_list;
};

#endif // TESTMAINWINDOW_H
