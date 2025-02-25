#include "dbhandler.h"

LauncherPluginBase *createWidget(QWidget *parent)
{
    return new DBHandler(parent);
}

DBHandler::DBHandler(QWidget *parent) : LauncherPluginBase(parent)
{
    DBHandler::executeAll();
}

DBHandler::~DBHandler()
{
    delete tableModel;
    delete tableView;
    delete formLayout;
}

void DBHandler::defineLayout()
{
    tableModel = new QSqlTableModel(this, dataBase);

    tableModel->setTable("autMenus");
    tableModel->select();
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->setHeaderData(0, Qt::Horizontal, "Идентификатор");
    tableModel->setHeaderData(1, Qt::Horizontal, "Название");
    tableModel->setHeaderData(2, Qt::Horizontal, "Родитель");
    tableModel->setHeaderData(3, Qt::Horizontal, "Является меню");
    tableModel->setHeaderData(4, Qt::Horizontal, "Название плагина");
    tableModel->setHeaderData(5, Qt::Horizontal, "Тема");
    tableModel->setHeaderData(6, Qt::Horizontal, "Отношение");

    tableView->setModel(tableModel);
    tableView->resizeColumnsToContents();

    formLayout->addRow(tableView);
}

void DBHandler::connectAction(QAction &action)
{
    if(action.text() == "Отменить")
    {
        connect(&action, &QAction::triggered, this, [=]()->void{tableModel->revertAll();});
    }

    if(action.text() == "Удалить строку")
    {
        connect(&action, &QAction::triggered, this, [=]()->void
                {
                    QModelIndexList selection = tableView->selectionModel()->selectedRows();
                    for(int i = 0; i < selection.count(); ++i)
                    {
                        tableModel->removeRow(selection.at(i).row());
                    }
                });
    }

    if(action.text() == "Добавить строку")
    {
        connect(&action, &QAction::triggered, this, [=]()->void{tableModel->insertRow(tableModel->rowCount());});
    }

    if(action.text() == "Подтвердить")
    {
        connect(&action, &QAction::triggered, this, [=]()->void{tableModel->submitAll();});
    }
}
