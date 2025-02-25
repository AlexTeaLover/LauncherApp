#include "testerplugin.h"

LauncherPluginBase *createWidget(QWidget* parent)
{
    return new TesterPlugin(parent);
}

TesterPlugin::TesterPlugin(QWidget *parent) : LauncherPluginBase(parent)
{
    TesterPlugin::executeAll();

    QSqlQuery query(dataBase);
    query.exec("SELECT RoleId, Roles FROM autRoles");

    // Заполнение QComboBox данными из базы данных
    while (query.next()) {
        QSqlRecord record = query.record();
        int roleId = record.value("RoleId").toInt();
        QString roleName = record.value("Roles").toString();
        rolesBox->addItem(roleName, QVariant(roleId));
    }
}

TesterPlugin::~TesterPlugin()
{
}

void TesterPlugin::defineLayout()
{
    this->resize(1000,600);
    this->setLayout(mainVerticalLayout);

    mainVerticalLayout->addStretch();
    mainVerticalLayout->addLayout(loginHorizontallLayout);
    mainVerticalLayout->addLayout(passwordHorizontalLayout);
    mainVerticalLayout->addLayout(rolesBoxLayout);
    mainVerticalLayout->addStretch();

    loginHorizontallLayout->addStretch();
    loginHorizontallLayout->addWidget(loginLabel);
    loginHorizontallLayout->addWidget(loginLineEdit);
    loginHorizontallLayout->addStretch();

    loginLabel->setText("Логин:");
    loginLabel->setMinimumWidth(100);

    loginLineEdit->setMaximumWidth(200);

    passwordHorizontalLayout->addStretch();
    passwordHorizontalLayout->addWidget(passwordLabel);
    passwordHorizontalLayout->addWidget(passwordLineEdit);
    passwordHorizontalLayout->addStretch();

    passwordLabel->setText("Пароль:");
    passwordLabel->setMinimumWidth(100);

    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setMaximumWidth(200);

    rolesBoxLayout->addStretch();
    rolesBoxLayout->addWidget(roleBoxLabel);
    rolesBoxLayout->addWidget(rolesBox);
    rolesBoxLayout->addStretch();

    roleBoxLabel->setText("Роль:");
    roleBoxLabel->setMinimumWidth(100);
    rolesBox->setMinimumWidth(200);
}

void TesterPlugin::connectToDB()
{
    if(QSqlDatabase::contains("LauncherDataBase"))
    {
        dataBase = QSqlDatabase::database("LauncherDataBase");
    }
    else
    {
        qDebug() << "DB not connect";

        dataBase = QSqlDatabase::addDatabase("QSQLITE", "LauncherDataBase");
        dataBase.setDatabaseName("LauncherDataBase.db");

        if(!dataBase.open()) qDebug() << "DB don't open";
    }
}

void TesterPlugin::connectAction(QAction &action)
{
    if(action.text() == "Добавить пользователя")
    {
        connect(&action, &QAction::triggered, this, [=]()->void
                    {
                        QString login = loginLineEdit->text();
                        QString password = passwordLineEdit->text();
                        int roleId = rolesBox->itemData(rolesBox->currentIndex()).toInt();

                        // Проверяем, не пустые ли поля
                        if (login.isEmpty() || password.isEmpty()) {
                            message.setText("Поля логина и пароля не могут быть пустыми.");
                            message.exec();
                            return;
                        }

                        // Проверка на существование логина
                        QSqlQuery queryCheck(dataBase);
                        queryCheck.prepare("SELECT COUNT(*) FROM autUsers WHERE Login = :login");
                        queryCheck.bindValue(":login", login);
                        if (!queryCheck.exec()) {
                            qDebug() << "Ошибка при проверке существования логина:" << queryCheck.lastError();
                            message.setText("Ошибка при проверке существования логина.");
                            message.exec();
                            return;
                        }
                        queryCheck.next();
                        int count = queryCheck.value(0).toInt();
                        if (count > 0) {
                            message.setText("Пользователь с таким логином уже существует.");
                            message.exec();
                            return;
                        }

                        // Вставка нового пользователя в таблицу autUsers
                        QSqlQuery queryUsers(dataBase);
                        queryUsers.prepare("INSERT INTO autUsers (Login, Password) VALUES (:login, :password)");
                        queryUsers.bindValue(":login", login);
                        queryUsers.bindValue(":password", password);

                        if (!queryUsers.exec()) {
                            qDebug() << "Ошибка при добавлении пользователя:" << queryUsers.lastError();
                            message.setText("Ошибка при добавлении пользователя. Проверьте логин и пароль.");
                            message.exec();
                            return;
                        }

                        // Получаем ID добавленного пользователя
                        int userId = queryUsers.lastInsertId().toInt();

                        // Вставка роли в таблицу autUserRoles
                        QSqlQuery queryRoles(dataBase);
                        queryRoles.prepare("INSERT INTO autUserRoles (UserId, RoleId) VALUES (:userId, :roleId)");
                        queryRoles.bindValue(":userId", userId);
                        queryRoles.bindValue(":roleId", roleId);

                        if (!queryRoles.exec()) {
                            qDebug() << "Ошибка при добавлении роли пользователя:" << queryRoles.lastError();
                            message.setText("Ошибка при добавлении роли пользователя.");
                            message.exec();
                            return;
                        }

                        message.setText("Пользователь успешно добавлен.");
                        message.exec();
                        // Очищаем поля ввода после успешного добавления
                        loginLineEdit->clear();
                        passwordLineEdit->clear();
                    }

                );
    }
}
