#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>
#include <QtSql>
#include <QTableView>
#include <QMessageBox>
#include <QDebug>
#include <QLineEdit>
#include <QLabel>

#include <launcher.h>

class Authorization : public QWidget
{
    Q_OBJECT

public:
    Authorization(QWidget *parent = nullptr);
    ~Authorization();

private:
    //Ui
    QVBoxLayout *mainLayout{new QVBoxLayout(this)};
    QFormLayout *dataLayout{new QFormLayout()};
    QHBoxLayout *buttonsLayout{new QHBoxLayout()};

    QLineEdit *loginLineEdit{new QLineEdit(this)};
    QLineEdit *passwordLineEdit{new QLineEdit(this)};

    QPushButton *exitButton{new QPushButton("Закрыть", this)};
    QPushButton *enterButton{new QPushButton("Войти", this)};

    void buildUi();

    //Autorization
    
    QSqlDatabase launcherDataBase;
    QSqlQueryModel modelOfUsers;

    Launcher *launcher;

    QVector<int> userRolesId;

    void getUserRoles(int userId);

signals:
    void openLauncher(QVector<int>&);


private slots:
    void onExitButtonClicked();
    void onEnterButtonClicked();
};
#endif // AUTHORIZATION_H
