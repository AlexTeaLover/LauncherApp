#include "dbhandler.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBHandler w;
    w.show();
    return a.exec();
}
