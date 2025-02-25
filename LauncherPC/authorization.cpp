#include "authorization.h"
#include "processing.h"

Authorization::Authorization(QWidget *parent)
    : QWidget(parent)
{
    buildUi();

    connect(exitButton, &QPushButton::clicked, this, &Authorization::onExitButtonClicked);
    connect(enterButton, &QPushButton::clicked, this, &Authorization::onEnterButtonClicked);

    launcherDataBase = QSqlDatabase::addDatabase("QSQLITE", "LauncherDataBase");
    launcherDataBase.setDatabaseName("../Databases/LauncherDataBase.db");

    assert(launcherDataBase.open() && "Database don't open!");

    modelOfUsers.setQuery("SELECT * FROM autUsers", launcherDataBase);

    if(modelOfUsers.lastError().isValid()) qDebug() << modelOfUsers.lastError();
}

Authorization::~Authorization()
{
}

void Authorization::buildUi()
{
    this->setWindowIcon(QIcon(Processing::getIconPath()));
    this->setWindowTitle("Авторизация");
    this->resize(400, 200);
    this->setLayout(mainLayout);

    mainLayout->addSpacing(50);

    mainLayout->addLayout(dataLayout);
    dataLayout->addRow("Логин:", loginLineEdit);
    dataLayout->addRow("Пароль:", passwordLineEdit);

    passwordLineEdit->setEchoMode(QLineEdit::Password);

    mainLayout->addLayout(buttonsLayout);

    buttonsLayout->addWidget(exitButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(enterButton);
}

void Authorization::onExitButtonClicked()
{
    close();
}

void Authorization::onEnterButtonClicked()
{
    for (int i = 0; i < modelOfUsers.rowCount(); ++i)
    {
        if(modelOfUsers.record(i).value("Login").toString() == loginLineEdit->text())
        {
            if(modelOfUsers.record(i).value("Password").toString() == passwordLineEdit->text())
            {
                getUserRoles(modelOfUsers.record(i).value("UserId").toInt());
                launcher = new Launcher;
                connect(this, &Authorization::openLauncher, launcher, &Launcher::launcherOpen);
                emit openLauncher(userRolesId);
                launcher->show();
                this->close();
                return;
            }
        }
    }
    QMessageBox::warning(this, "Ошибка", "Авторизация неудачна");
}

void Authorization::getUserRoles(int userId)
{
    modelOfUsers.setQuery("SELECT autRoles.RoleId"
                          " FROM autUsers, autUserRoles, autRoles"
                          " WHERE autUsers.UserId = autUserRoles.UserId"
                          " AND autRoles.RoleId = autUserRoles.RoleId"
                          " AND autUsers.UserId = " + QString::number(userId), launcherDataBase);

    for(int i = 0; i < modelOfUsers.rowCount(); i++)
    {
        userRolesId.push_back(modelOfUsers.record(i).value("autRoles.RoleId").toInt());
    }
}
