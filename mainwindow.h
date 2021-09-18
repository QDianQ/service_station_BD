#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSqlDataBase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>

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
    QSqlDatabase database;
    QSqlQuery query;
    QSqlTableModel *model;

    int row;
    QString name, number, comm, search, count;
};
#endif // MAINWINDOW_H
