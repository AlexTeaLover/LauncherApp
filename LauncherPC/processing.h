#ifndef PROCESSING_H
#define PROCESSING_H

#include <QMenu>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QString>

class Processing
{
private:
    inline static const QVector<QString> colors{"rgb(0, 150, 0)","rgb(180, 0, 150)","rgb(48, 213, 200)"};

    static QString correctingBelonging(const QString &belonging);

public:
    inline static const QString isMenu{"1"};
    inline static const QString isNotMenu{"NULL"};
    inline static const QString launcherBelonging{"NULL"};
    inline static const QString hasNoParent{"NULL"};

    static QString getIconPath(QString iconPath = "/icon.ico");
    static QString collectRoles(const QVector<int> &userRolesId);
    static void    queryBuilder(QSqlQueryModel *queryModel,
                                const QSqlDatabase &dataBase,
                                const QString &roles,
                                const QString &belonging,
                                const QString &isMenu,
                                const QString &parent);
    static void    setMenuStyle(QMenu *menu, const int depth);
    static bool    hasParent(QSqlRecord &record);
};

#endif // PROCESSING_H
