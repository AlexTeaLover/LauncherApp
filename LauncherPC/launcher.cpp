#include "launcher.h"
#include "processing.h"

Launcher::Launcher(QWidget *parent) :
    QMainWindow(parent)
{
    assert(QSqlDatabase::contains("LauncherDataBase") && "Database not found");
    launcherDataBase = QSqlDatabase::database("LauncherDataBase");

    connect(tabWidget, &QTabWidget::currentChanged, this, &Launcher::tabChanged);
}

void Launcher::buildUi()
{
    this->setWindowIcon(QIcon(Processing::getIconPath()));
    this->setWindowTitle("Лаунчер");
    this->resize(900, 600);
    this->setMenuBar(buildMenuBar(Processing::launcherBelonging));
    this->setCentralWidget(launcherWidget);
    launcherWidget->setLayout(verticalLayout);
    verticalLayout->addWidget(tabWidget);
    verticalLayout->setContentsMargins(0,0,0,0);
}

Launcher::~Launcher()
{
    delete this;
}

QMenuBar *Launcher::buildMenuBar(const QString &belonging)
{
    QMenuBar *menuBar{new QMenuBar(launcherWidget)};

    QSqlRecord currentRecord;

    Processing::queryBuilder(&menuBarQuery, launcherDataBase, roles, belonging, Processing::isMenu, Processing::hasNoParent);

    for(int i = 0; i < menuBarQuery.rowCount(); ++i)
    {
        currentRecord = menuBarQuery.record(i);
        menuBar->addMenu(buildMenu(currentRecord.value("autMenus.MenuId").toString(), currentRecord.value("autMenus.Name").toString(), belonging));
    }

    Processing::queryBuilder(&menuBarQuery, launcherDataBase, roles, belonging, Processing::isNotMenu, Processing::hasNoParent);

    for(int i = 0; i < menuBarQuery.rowCount(); ++i)
    {
        currentRecord = menuBarQuery.record(i);
        menuBar->addAction(buildAction(currentRecord.value("autMenus.Name").toString(), currentRecord.value("autMenus.PluginName").toString(), belonging));
    }

    if(belonging == Processing::launcherBelonging)
    {
        QAction *exitAction = new QAction("Выход", this);
        connect(exitAction, &QAction::triggered, this, &QApplication::quit);
        menuBar->addAction(exitAction);
    }

    return menuBar;
}

QMenu *Launcher::buildMenu(QString &&parentMenuId, QString &&menuName, const QString &belonging)
{
    QMenu *menu{new QMenu(menuName, launcherWidget)};

    QSqlRecord currentRecord;

    Processing::queryBuilder(&menuQuery, launcherDataBase, roles, belonging, Processing::isMenu, parentMenuId);

    for(int i = 0; i < menuQuery.rowCount(); ++i)
    {
        currentRecord = menuQuery.record(i);
        menu->addMenu(buildMenu(currentRecord.value("autMenus.MenuId").toString(), currentRecord.value("autMenus.Name").toString(), belonging));
    }

    Processing::queryBuilder(&actionQuery, launcherDataBase, roles, belonging, Processing::isNotMenu, parentMenuId);

    for(int i = 0; i < actionQuery.rowCount(); ++i)
    {
        currentRecord = actionQuery.record(i);
        menu->addAction(buildAction(currentRecord.value("autMenus.Name").toString(), currentRecord.value("autMenus.PluginName").toString(), belonging));
    }

    return menu;
}

QAction *Launcher::buildAction(QString &&actionName, QString &&pluginName, const QString &belonging)
{
    QAction *action{new QAction(actionName, launcherWidget)};
    if(belonging == Processing::launcherBelonging)
    {
        if(!pluginName.isEmpty())
        {
        connect(action,
                &QAction::triggered,
                this,
                [=]()->void{getWidgetFromPlugin(pluginName);}
                );
        }
        else
        {
            action->setDisabled(true);
        }
    }
    else
    {
        plugin->connectAction(*action);
    }
    return action;
}

void Launcher::launcherOpen(const QVector<int> &userRolesId)
{
    roles = Processing::collectRoles(userRolesId);
    buildUi();
}

void Launcher::getWidgetFromPlugin(const QString &name)
{
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabWidget->tabText(i) == name) {
            QMessageBox::warning(this, "Плагин уже запущен",
                                QString("Плагин '%1' уже запущен.").arg(name));
            return;
        }
    }

    QLibrary library(name);

    assert(library.load() && "Library not found");

    qDebug() << "library loaded";

    typedef LauncherPluginBase *(*libraryWidget)(QWidget*);
    libraryWidget lw = (libraryWidget)(library.resolve("createWidget"));

    assert(lw && "Can't create widget");

    plugin = static_cast<LauncherPluginBase*>(lw(this));

    QMenuBar *menuBar{buildMenuBar(name)};
    pluginMenuBars->insert(plugin, menuBar);
    tabWidget->addTab(plugin, name);
}

void Launcher::tabChanged()
{
    verticalLayout->removeWidget(previousWidget);
    verticalLayout->insertWidget(0, pluginMenuBars->value(tabWidget->currentWidget()));
    previousWidget = pluginMenuBars->value(tabWidget->currentWidget());
}
