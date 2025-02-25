#ifndef TESTERPLUGIN_H
#define TESTERPLUGIN_H

#include "TesterPlugin_global.h"
#include "launcherpluginbase.h"

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>

class TesterPlugin : public LauncherPluginBase
{
public:
    explicit TesterPlugin(QWidget *parent = nullptr);
    ~TesterPlugin();

    void connectAction(QAction &action) override;

private:

    QVBoxLayout *mainVerticalLayout{new QVBoxLayout(this)};
    QHBoxLayout *loginHorizontallLayout{new QHBoxLayout};
    QHBoxLayout *passwordHorizontalLayout{new QHBoxLayout};
    QHBoxLayout *rolesBoxLayout{new QHBoxLayout};

    QLabel *loginLabel{new QLabel(this)};
    QLineEdit *loginLineEdit{new QLineEdit(this)};

    QLabel *passwordLabel{new QLabel(this)};
    QLineEdit *passwordLineEdit{new QLineEdit(this)};

    QLabel *roleBoxLabel{new QLabel(this)};
    QComboBox* rolesBox = new QComboBox;

    QMessageBox message;

    void defineLayout() override;
    void connectToDB() override;
};

extern "C" TESTERPLUGIN_EXPORT LauncherPluginBase *createWidget(QWidget *parent);

#endif // TESTERPLUGIN_H
