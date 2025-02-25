#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "qmenubar.h"
#include <QMainWindow>
#include <QtSql>
#include <QTableView>
#include <QDebug>
#include <QMenu>
#include <QPushButton>
#include <QFormLayout>
#include <QApplication>
#include <QMessageBox>

#include "launcherpluginbase.h"

class Launcher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Launcher(QWidget *parent = nullptr);
    ~Launcher();

private:
    //Ui

    bool hasPluginMenuBar = false;

    QWidget *launcherWidget{new QWidget(this)};
    QVBoxLayout *verticalLayout{new QVBoxLayout(launcherWidget)};
    QTabWidget *tabWidget{new QTabWidget(launcherWidget)};

    void buildUi();

    //menuBar building

    int previousIndex = 0;

    QSqlQueryModel menuBarQuery;
    QSqlQueryModel menuQuery;
    QSqlQueryModel actionQuery;

    QMenuBar *buildMenuBar(const QString &belonging);
    QMenu *buildMenu(QString &&parentMenuId, QString &&menuName, const QString &belonging);
    QAction *buildAction(QString &&actionName, QString &&pluginName, const QString &belonging);

    //plugin

    LauncherPluginBase *plugin;
    QWidget *previousWidget{nullptr};
    QMap<QWidget *, QMenuBar *> *pluginMenuBars{new QMap<QWidget *, QMenuBar *>};

    void tabChanged();
    void getWidgetFromPlugin(const QString &name);

    //dataBase

    QSqlDatabase   launcherDataBase;
    QString        roles;

public slots:
    void launcherOpen(const QVector<int> &userRolesId);
};

#endif // LAUNCHER_H
