#include "launcherpluginbase.h"

LauncherPluginBase::LauncherPluginBase(QWidget *parent) : QWidget(parent)
{
}

LauncherPluginBase::~LauncherPluginBase()
{
}

void LauncherPluginBase::executeAll()
{
    connectToDB();
    defineLayout();
}

void LauncherPluginBase::connectToDB()
{
    if(QSqlDatabase::contains("LauncherDataBase"))
    {
        dataBase = QSqlDatabase::database("LauncherDataBase");
    }
    else
    {
        qDebug() << "DB not connect";

        dataBase = QSqlDatabase::addDatabase("QSQLITE", "LauncherDataBase");
        dataBase.setDatabaseName("../Databases/LauncherDataBase.db");

        if(!dataBase.open()) qDebug() << "DB don't open";
    }
}
