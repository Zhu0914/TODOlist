#ifndef KTODO_H
#define KTODO_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QStringListModel>
#include <QStandardItemModel>
QT_BEGIN_NAMESPACE
namespace Ui { class Ktodo; }
QT_END_NAMESPACE

class Ktodo : public QMainWindow
{
    Q_OBJECT

public:
    Ktodo(QWidget *parent = nullptr);
    void label_todoCount_update();
    ~Ktodo();
signals:
    void dataChanged(QModelIndex,QModelIndex);
private slots:
    void on_pushButton_addGroup_clicked();
    void ChangeData(QModelIndex);
    void on_pushButton_deleteGroup_clicked();
    void on_pushButton_firstWidget_clicked();
    void on_pushButton_secondWidget_clicked();
    void on_tableViewCustomContextMenuRequested(const QPoint &pos);
    void on_action_deleteGroup_triggered();
    void on_tableView_clicked(const QModelIndex &index);
    void on_pushButton_addAct_clicked();
    void on_pushButton_done_clicked(bool checked);
    void on_tableView_actTodo_clicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void on_tableView_actTodoCustomContextMenuRequested(const QPoint &pos);
    void on_tableView_actDoneCustomContextMenuRequested(const QPoint &pos);
    void completeActTodo();
    void deleteActTodo();
    void deleteActDone();
    void on_action_orderDue_triggered();
    void on_action_orderCreate_triggered();
    void on_action_orderPriority_triggered();
    void on_action_refresh_triggered();
    void on_treeView_clicked(const QModelIndex &index);
    void on_tableView_actDone_clicked(const QModelIndex &index);
private:
    Ui::Ktodo *ui;
    QSqlDatabase m_sqldb;
    QSqlTableModel *m_sqlGroupList;
    QSqlTableModel *m_sqlActList;
    QSqlTableModel *m_sqlActListTodo;
    QSqlTableModel *m_sqlActListDone;
    QStandardItemModel *m_model;
//    QSqlQueryModel *m_sqlmodel;
};
#endif // KTODO_H
