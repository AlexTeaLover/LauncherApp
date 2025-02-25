#include "processing.h"

#include <QApplication>
#include <QFileInfo>

QString Processing::correctingBelonging(const QString &belonging)
{
    if(belonging == launcherBelonging) return belonging;
    else return "'" + belonging + "'";
}

QString Processing::getIconPath(QString iconPath)
{
    QString exePath = QApplication::applicationFilePath();
    QString exeDir = QFileInfo(exePath).absolutePath();
    return exeDir + iconPath;

}

QString Processing::collectRoles(const QVector<int> &userRolesId)
{
    QString roles;

    for(int i = 0; i < userRolesId.size(); ++i)
    {
        if(userRolesId.size() != i + 1)     roles += QString::number(userRolesId.at(i)) + ",";
        else                                roles += QString::number(userRolesId.at(i));
    }

    return roles;
}

void Processing::queryBuilder(QSqlQueryModel *queryModel,
                              const QSqlDatabase &dataBase,
                              const QString &roles,
                              const QString &belonging,
                              const QString &isMenu,
                              const QString &parent)
{
    queryModel->setQuery(QString("SELECT DISTINCT autMenus.*"
                                " FROM autRoles, autMenuRoles, autMenus"
                                " WHERE autRoles.RoleId = autMenuRoles.RoleId"
                                " AND autMenus.MenuId = autMenuRoles.MenuId"
                                " AND autRoles.RoleId IN (%1)"
                                " AND autMenus.Belonging IS %2"
                                " AND autMenus.IsMenu IS %3"
                                " AND autMenus.Parent IS %4"
                                " ORDER BY autMenus.Theme,"
                                " autMenus.Parent").arg(
                                 roles,
                                 Processing::correctingBelonging(belonging),
                                 isMenu,
                                 parent),
                         dataBase);
}

void Processing::setMenuStyle(QMenu *menu, const int depth)
{
    menu->setStyleSheet("QMenu::item{"
                        "background-color: " + colors[depth] + ";"
                        "color: rgb(255, 255, 255);"
                        "}"
                        "QMenu::item:selected{"
                        "background-color: rgb(0, 85, 127);"
                        "color: rgb(255, 255, 255);"
                        "}"
                        "QMenu::item:disabled{"
                        "background-color: rgb(255, 255, 255);"
                        "color: rgb(70, 70, 70);"
                        "}");
}

bool Processing::hasParent(QSqlRecord &record)
{
    return record.value("autMenus.Parent").toBool();
}
