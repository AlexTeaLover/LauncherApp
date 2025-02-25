#ifndef LAUNCHERPLUGINBASE_H
#define LAUNCHERPLUGINBASE_H

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QList>

class LauncherPluginBase : public QWidget
{
public:
    virtual void connectAction(QAction &action) = 0;

protected:
    explicit LauncherPluginBase(QWidget *parent = nullptr);
    ~LauncherPluginBase();

    QSqlDatabase dataBase;

    virtual void executeAll();
    virtual void connectToDB();
    virtual void defineLayout() = 0;
};

#endif // LAUNCHERPLUGINBASE_H
