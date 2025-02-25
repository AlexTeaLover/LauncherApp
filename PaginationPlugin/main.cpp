#include "paginationplugin.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PaginationPlugin w;
    w.show();
    return a.exec();
}
