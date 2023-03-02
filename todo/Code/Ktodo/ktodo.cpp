#include "ktodo.h"
#include "./ui_ktodo.h"

Ktodo::Ktodo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Ktodo)
{
    ui->setupUi(this);
//    setAttribute(Qt::WA_DeleteOnClose);
    //1.添加使用的数据库驱动
    m_sqldb = QSqlDatabase::addDatabase("QSQLITE");
    //2.设置需要打开的数据库文件
    m_sqldb.setDatabaseName("ktodolist.db");
    //3.打开数据库文件
    if(m_sqldb.open())
    {
        qDebug() << "ktodolist.db";
    }
    //Grouplist
    //4.创建一个表，并插入
    QSqlQuery query(m_sqldb);
    query.exec("create table kGroupList(id text primary key)");
    query.exec("insert into kGroupList values('work')");
    query.exec("insert into kGroupList values('life')");
    //5.绑定数据库，并显示在tableview上面
    m_sqlGroupList = new QSqlTableModel(ui->tableView,m_sqldb);
    //选择数据库中的数据表
    m_sqlGroupList->setTable("kGroupList");
    /*设置修改tableview的时候，数据更改缓存在模型中*/
    m_sqlGroupList->setEditStrategy(QSqlTableModel::OnManualSubmit);
    /*选取表中所有的行*/
    m_sqlGroupList->select();
    ui->tableView->setModel(m_sqlGroupList);//TODO
    //使其不可编辑。
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->hide();

    //actlist
    QString sql = "CREATE TABLE kActList(activity TEXT PRIMARY KEY,id TEXT NULL,status INTEGER NOT NULL,priority INTEGER  NULL,duetime TEXT  NULL,remtime TEXT  NULL,createtime TEXT NOT NULL,description TEXT NULL,location TEXT NULL,note TEXT NULL)";
    if(!query.exec(sql))
    {
       qDebug()<<"!create";
    }

    m_sqlActList = new QSqlTableModel(this,m_sqldb);
    m_sqlActList->setTable("kActList");
    m_sqlActListTodo = new QSqlTableModel(this,m_sqldb);
    m_sqlActListTodo->setTable("kActList");
    m_sqlActListTodo->setFilter(QString("status = 0"));
    m_sqlActListTodo->select();
    ui->tableView_actTodo->setModel(m_sqlActListTodo);
    ui->tableView_actTodo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_actTodo->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView_actTodo->horizontalHeader()->hide();
    ui->tableView_actTodo->verticalHeader()->hide();
    ui->tableView_actTodo->setColumnHidden(1,true);
    ui->tableView_actTodo->setColumnHidden(2,true);
    ui->tableView_actTodo->setColumnHidden(3,true);
    ui->tableView_actTodo->setColumnHidden(4,true);
    ui->tableView_actTodo->setColumnHidden(5,true);
    ui->tableView_actTodo->setColumnHidden(6,true);
    ui->tableView_actTodo->setColumnHidden(7,true);
    ui->tableView_actTodo->setColumnHidden(8,true);
    ui->tableView_actTodo->setColumnHidden(9,true);
    ui->tableView_actTodo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_sqlActListDone = new QSqlTableModel(this,m_sqldb);
    m_sqlActListDone->setTable("kActList");
    m_sqlActListDone->setFilter(QString("status = 1"));
    m_sqlActListDone->select();
    ui->tableView_actDone->setModel(m_sqlActListDone);
    ui->tableView_actDone->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_actDone->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView_actDone->horizontalHeader()->hide();
    ui->tableView_actDone->verticalHeader()->hide();
    ui->tableView_actDone->setColumnHidden(1,true);
    ui->tableView_actDone->setColumnHidden(2,true);
    ui->tableView_actDone->setColumnHidden(3,true);
    ui->tableView_actDone->setColumnHidden(4,true);
    ui->tableView_actDone->setColumnHidden(5,true);
    ui->tableView_actDone->setColumnHidden(6,true);
    ui->tableView_actDone->setColumnHidden(7,true);
    ui->tableView_actDone->setColumnHidden(8,true);
    ui->tableView_actDone->setColumnHidden(9,true);
    ui->tableView_actDone->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_actDone->setEditTriggers(QAbstractItemView::NoEditTriggers);
    label_todoCount_update();
    ui->tableView_actDone->hide();
    ui->pushButton_done->setStyleSheet("text-align:left");
    ui->stackedWidget_2->setCurrentWidget(ui->page_5);
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    on_action_refresh_triggered();

    connect(m_sqlGroupList,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(ChangeData(QModelIndex)));

    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &Ktodo::on_tableViewCustomContextMenuRequested);
    connect(ui->tableView_actTodo,&QTableView::customContextMenuRequested,this,&Ktodo::on_tableView_actTodoCustomContextMenuRequested);
    connect(ui->tableView_actDone,&QTableView::customContextMenuRequested,this,&Ktodo::on_tableView_actDoneCustomContextMenuRequested);
}

void Ktodo::label_todoCount_update()
{
    QSqlQuery query;
    query.exec("select count(*) from kActList where status = 0");
    query.next();
    qDebug()<<query.value(0).toInt();
    ui->label_todoCount->setText(QString("(%1)").arg(query.value(0).toInt()));

}

Ktodo::~Ktodo()
{
    delete ui;
    m_sqldb.close();
    delete m_sqlActList;
    delete m_model;//TODO
    delete m_sqlGroupList;
    delete m_sqlActListDone;
    delete m_sqlActListTodo;
}


void Ktodo::on_pushButton_addGroup_clicked()
{
    QString newGroup = ui->lineEdit_addGroup->text();
    int rowCnt = m_sqlGroupList->rowCount();
    qDebug()<<newGroup;
    m_sqlGroupList->insertRow(rowCnt);
    qDebug()<<rowCnt;
    m_sqlGroupList->setData(m_sqlGroupList->index(rowCnt,0),newGroup);
    if(!m_sqlGroupList->submitAll()){
            qDebug()<<"更新数据库失败";
     }
    ui->lineEdit_addGroup->clear();
}

void Ktodo::ChangeData(QModelIndex x)
{
    qDebug()<<x.row();
    if(!m_sqlGroupList->submitAll()){
            qDebug()<<"更新数据库失败";
     }
    bool y=m_sqlGroupList->setData(m_sqlGroupList->index(x.row(),x.column()),x.data()); //x.data()返回QVariantl类型数据 QVariant内置支持所有QMetaType::Type里声明的类型如:int，QString，QFont，QColor等，甚至QList，QMap<QString， QVariant>等组成的任意复杂类型。

    if(y){
    m_sqlGroupList->submitAll();
    }
    else {
        m_sqlGroupList->blockSignals(true);//阻塞datachange信号发送，没这句about会弹两次
        m_sqlGroupList->revertAll();
        qDebug()<<"修改失败","请检查修改数据！";
    }
}


void Ktodo::on_pushButton_deleteGroup_clicked()
{
    int curRow = ui->tableView->currentIndex().row();

    m_sqlGroupList->removeRow(curRow);

    m_sqlGroupList->submitAll();
}

void Ktodo::on_pushButton_firstWidget_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    ui->stackedWidget_2->setCurrentWidget(ui->page_5);
}

void Ktodo::on_pushButton_secondWidget_clicked()
{
    ui->stackedWidget_2->setCurrentWidget(ui->page_5);
    ui->stackedWidget->setCurrentWidget(ui->page);
    m_sqlActListTodo->setFilter("status = 0");
    m_sqlActListTodo->select();
    ui->tableView_actTodo->setModel(m_sqlActListTodo);
    m_sqlActListDone->setFilter("status = 1");
    m_sqlActListDone->select();
    ui->tableView_actDone->setModel(m_sqlActListDone);
}

void Ktodo::on_tableViewCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu;
    menu.addAction(u8"删除",this,SLOT(on_pushButton_deleteGroup_clicked()));
    menu.exec(QCursor::pos());
}
void Ktodo::on_action_deleteGroup_triggered()
{
    int curRow = ui->tableView->currentIndex().row();

    m_sqlGroupList->removeRow(curRow);

    m_sqlGroupList->submitAll();
    m_sqlGroupList->select();
    ui->tableView->setModel(m_sqlGroupList);
}



void Ktodo::on_tableView_clicked(const QModelIndex &index)
{
    on_pushButton_secondWidget_clicked();
    ui->stackedWidget_2->setCurrentWidget(ui->page_5);
    qDebug()<<index.data().toString();
    qDebug()<<"on_tableView_clicked";
    if(index.data().toString()==""){
        on_pushButton_secondWidget_clicked();
        return;
    }
    m_sqlActListTodo->setFilter(QString("id = '%1' and status = 0").arg(index.data().toString()));
    m_sqlActListTodo->select();
    ui->tableView_actTodo->setModel(m_sqlActListTodo);
    ui->tableView_actTodo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_sqlActListDone->setFilter(QString("id = '%1' and status = 1").arg(index.data().toString()));
    m_sqlActListDone->select();
    ui->tableView_actDone->setModel(m_sqlActListDone);
    ui->tableView_actDone->setEditTriggers(QAbstractItemView::NoEditTriggers);
}



void Ktodo::on_pushButton_addAct_clicked()
{
    QString activity=ui->lineEdit_addAct->text();
    QString group(ui->tableView->currentIndex().data().toString());
    int status = 0;
    int priority = 0;
    QDateTime createtime = QDateTime::currentDateTime();
    QSqlQuery query(m_sqldb);
    query.prepare("INSERT INTO kActList(activity,id,status,priority,createtime) VALUES(:activity,:id,:status,:priority,:createtime)");
    query.bindValue(0,activity);
    query.bindValue(1,group);
    query.bindValue(2,status);
    query.bindValue(3,priority);
//    query.bindValue(4,duetime.toString("yyyy-MM-dd"));
    query.bindValue(4,createtime.toString("yyyy-MM-dd hh:mm:ss"));
    query.exec();
    ui->lineEdit_addAct->clear();
    on_tableView_clicked(ui->tableView->currentIndex());
    label_todoCount_update();
}

void Ktodo::on_pushButton_done_clicked(bool checked)
{
    qDebug()<<checked;
    if(checked){
        ui->tableView_actDone->show();
        ui->pushButton_done->setText(u8"隐藏已完成：");
    }
    else{
        ui->tableView_actDone->hide();
        ui->pushButton_done->setText(u8"显示已完成：");
    }
}

void Ktodo::on_tableView_actTodo_clicked(const QModelIndex &index)
{
    ui->pushButton->show();
    ui->stackedWidget_2->setCurrentWidget(ui->page_6);
    qDebug()<<index.data().toString();
    qDebug()<<"on_tableView_actTodo_clicked";
    QSqlQuery query;
    query.exec(QString("select * from kActList where activity = '%1'").arg(index.data().toString()));
    query.next();
    ui->lineEdit_activity->setText(index.data().toString());
    ui->comboBox_priority->setEditText(query.value(3).toString());
    qDebug()<<query.value(3).toString();
    ui->dateEdit_duetime->setDate(QDate::fromString(query.value(4).toString(),"yyyy-MM-dd"));
    qDebug()<<QDate::fromString(query.value(4).toString(),"yyyy-MM-dd");
    ui->dateTimeEdit_remtime->setDateTime(QDateTime::fromString(query.value(5).toString(),"yyyy-MM-dd hh:mm:ss"));
    ui->lineEdit_description->setText(query.value(7).toString());
    ui->lineEdit_location->setText(query.value(8).toString());
    ui->plainTextEdit_note->setPlainText(query.value(9).toString());
}

void Ktodo::on_pushButton_clicked()
{
    m_sqlActListTodo->setData(m_sqlActListTodo->index(ui->tableView_actTodo->currentIndex().row(),0),ui->lineEdit_activity->text());
    if(!m_sqlActListTodo->submitAll()){
        qDebug()<<"Error";
    }
    qDebug()<<QString("update kActList set priority = %1,duetime = '%2',remtime = '%3',description = '%4',location = '%5',note = '%6' "
                      "where acticity = '%7'")
              .arg(ui->comboBox_priority->currentText().toInt())
              .arg(ui->dateEdit_duetime->date().toString("yyyy-MM-dd"))
              .arg(ui->dateTimeEdit_remtime->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
              .arg(ui->lineEdit_description->text())
              .arg(ui->lineEdit_location->text())
              .arg(ui->plainTextEdit_note->toPlainText())
              .arg(ui->lineEdit_activity->text());
    QSqlQuery query;
    query.exec(QString("update kActList set priority = %1,duetime = '%2',remtime = '%3',description = '%4',location = '%5',note = '%6' "
                       "where activity = '%7'")
               .arg(ui->comboBox_priority->currentText().toInt())
               .arg(ui->dateEdit_duetime->date().toString("yyyy-MM-dd"))
               .arg(ui->dateTimeEdit_remtime->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
               .arg(ui->lineEdit_description->text())
               .arg(ui->lineEdit_location->text())
               .arg(ui->plainTextEdit_note->toPlainText())
               .arg(ui->lineEdit_activity->text()));
}

void Ktodo::on_tableView_actTodoCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu;
    menu.addAction(u8"完成",this,SLOT(completeActTodo()));
    menu.addAction(u8"删除",this,SLOT(deleteActTodo()));
    menu.exec(QCursor::pos());
}

void Ktodo::on_tableView_actDoneCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu;
    menu.addAction(u8"删除",this,SLOT(deleteActDone()));
    menu.exec(QCursor::pos());
}

void Ktodo::completeActTodo()
{
    QSqlQuery query;
    query.exec(QString("update kActList set status = 1 where activity = '%1'").arg(ui->tableView_actTodo->currentIndex().data().toString()));
    m_sqlActListTodo->select();
    ui->tableView_actTodo->setModel(m_sqlActListTodo);
    m_sqlActListDone->select();
    ui->tableView_actDone->setModel(m_sqlActListDone);
    label_todoCount_update();
}

void Ktodo::deleteActTodo()
{
    int curRow = ui->tableView_actTodo->currentIndex().row();
    m_sqlActListTodo->removeRow(curRow);
    m_sqlActListTodo->submitAll();
    m_sqlActListTodo->select();
    ui->tableView_actTodo->setModel(m_sqlActListTodo);
    label_todoCount_update();
}

void Ktodo::deleteActDone()
{
    int curRow = ui->tableView_actDone->currentIndex().row();
    m_sqlActListDone->removeRow(curRow);
    m_sqlActListDone->submitAll();
    m_sqlActListDone->select();
    ui->tableView_actDone->setModel(m_sqlActListDone);
}

void Ktodo::on_action_orderDue_triggered()
{
    ui->tableView_actTodo->sortByColumn(4,Qt::AscendingOrder);
    ui->tableView_actTodo->setSortingEnabled(true);
    ui->tableView_actDone->sortByColumn(4,Qt::AscendingOrder);
    ui->tableView_actDone->setSortingEnabled(true);
}

void Ktodo::on_action_orderCreate_triggered()
{
    ui->tableView_actTodo->sortByColumn(6,Qt::AscendingOrder);
    ui->tableView_actTodo->setSortingEnabled(true);
    ui->tableView_actDone->sortByColumn(6,Qt::AscendingOrder);
    ui->tableView_actDone->setSortingEnabled(true);
}

void Ktodo::on_action_orderPriority_triggered()
{
    ui->tableView_actTodo->sortByColumn(2,Qt::AscendingOrder);
    ui->tableView_actTodo->setSortingEnabled(true);
    ui->tableView_actDone->sortByColumn(2,Qt::AscendingOrder);
    ui->tableView_actDone->setSortingEnabled(true);
}

void Ktodo::on_action_refresh_triggered()
{
    ui->stackedWidget_2->setCurrentWidget(ui->page_5);
    m_model = new QStandardItemModel(ui->treeView);
    QStandardItem *temp,*tempchild;
    QSqlQuery query;
    query.exec("select count(*) from kActList where duetime = ''and status = 0");
    query.next();
    qDebug()<<query.value(0).toInt();
    if(query.value(0).toInt()){
        temp = new QStandardItem(QString(u8"未分配"));
        temp->setData(-1);
        query.exec("select activity from kActList where duetime = ''and status = 0");
        while(query.next()){
            tempchild =  new QStandardItem(query.value(0).toString());
            temp->appendRow(tempchild);
        }
        m_model->appendRow(temp);
    }
    query.exec(QString("select count(*) from kActList where duetime between '%1' and '%2' and status = 0")
               .arg(QDate::currentDate().toString("yyyy-MM-dd"))
               .arg(QDate::currentDate().addDays(7).toString("yyyy-MM-dd")));
    query.next();
    qDebug()<<query.value(0).toInt();
    if(query.value(0).toInt()){
        temp = new QStandardItem(QString(u8"未来七天"));
        temp->setData(-1);
        query.exec(QString("select activity from kActList where duetime between '%1' and '%2' and status = 0")
                   .arg(QDate::currentDate().toString("yyyy-MM-dd"))
                   .arg(QDate::currentDate().addDays(7).toString("yyyy-MM-dd")));
        while(query.next()){
            tempchild =  new QStandardItem(query.value(0).toString());
            temp->appendRow(tempchild);

        }
        m_model->appendRow(temp);
    }
    query.exec(QString("select count(*) from kActList where duetime > '%1' and status = 0")
               .arg(QDate::currentDate().addDays(7).toString("yyyy-MM-dd")));
    query.next();
    qDebug()<<query.value(0).toInt();
    if(query.value(0).toInt()){
        temp = new QStandardItem(QString(u8"以后"));
        temp->setData(-1);
        query.exec(QString("select activity from kActList where duetime > '%1'and status = 0")
                   .arg(QDate::currentDate().addDays(7).toString("yyyy-MM-dd")));
        while(query.next()){
            tempchild =  new QStandardItem(query.value(0).toString());
            temp->appendRow(tempchild);
        }
        m_model->appendRow(temp);
    }

    ui->treeView->setModel(m_model);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->expandAll();
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void Ktodo::on_treeView_clicked(const QModelIndex &index)
{
    ui->pushButton->hide();
    ui->stackedWidget_2->setCurrentWidget(ui->page_6);
    qDebug()<<index.data().toString();
    qDebug()<<"on_tableView_actTodo_clicked";
    QSqlQuery query;
    query.exec(QString("select * from kActList where activity = '%1'").arg(index.data().toString()));
    query.next();
    ui->lineEdit_activity->setText(index.data().toString());
    ui->comboBox_priority->setEditText(query.value(3).toString());
    qDebug()<<query.value(3).toString();
    ui->dateEdit_duetime->setDate(QDate::fromString(query.value(4).toString(),"yyyy-MM-dd"));
    qDebug()<<QDate::fromString(query.value(4).toString(),"yyyy-MM-dd");
    ui->dateTimeEdit_remtime->setDateTime(QDateTime::fromString(query.value(5).toString(),"yyyy-MM-dd hh:mm:ss"));
    ui->lineEdit_description->setText(query.value(7).toString());
    ui->lineEdit_location->setText(query.value(8).toString());
    ui->plainTextEdit_note->setPlainText(query.value(9).toString());
}

void Ktodo::on_tableView_actDone_clicked(const QModelIndex &index)
{
    ui->pushButton->show();
    ui->stackedWidget_2->setCurrentWidget(ui->page_6);
    qDebug()<<index.data().toString();
    qDebug()<<"on_tableView_actTodo_clicked";
    QSqlQuery query;
    query.exec(QString("select * from kActList where activity = '%1'").arg(index.data().toString()));
    query.next();
    ui->lineEdit_activity->setText(index.data().toString());
    ui->comboBox_priority->setEditText(query.value(3).toString());
    qDebug()<<query.value(3).toString();
    ui->dateEdit_duetime->setDate(QDate::fromString(query.value(4).toString(),"yyyy-MM-dd"));
    qDebug()<<QDate::fromString(query.value(4).toString(),"yyyy-MM-dd");
    ui->dateTimeEdit_remtime->setDateTime(QDateTime::fromString(query.value(5).toString(),"yyyy-MM-dd hh:mm:ss"));
    ui->lineEdit_description->setText(query.value(7).toString());
    ui->lineEdit_location->setText(query.value(8).toString());
    ui->plainTextEdit_note->setPlainText(query.value(9).toString());
}
