#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <DBHandler_global.h>
#include <launcherpluginbase.h>

#include <QFormLayout>
#include <QMenuBar>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class DBHandler : public LauncherPluginBase
{
public:
    explicit DBHandler(QWidget *parent = nullptr);
    ~DBHandler();

    void connectAction(QAction &action) override;

private:
    QFormLayout    *formLayout{new QFormLayout(this)};
    QTableView     *tableView{new QTableView(this)};
    QSqlTableModel *tableModel;

    void defineLayout() override;
};

extern "C" DBHANDLER_EXPORT LauncherPluginBase *createWidget(QWidget *parent);

#endif // DBHANDLER_H
