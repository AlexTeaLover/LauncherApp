#ifndef PAGECOUNTER_H
#define PAGECOUNTER_H

#include <QObject>
#include <QSqlQuery>
#include <QDebug>
#include <QThread>
#include <QEventLoop>

class PageCounter
{
    Q_OBJECT
public:
    PageCounter(const QSqlDatabase &db)  : m_db(db) {}



signals:
    void countingFinished(int count); // Сигнал для завершения подсчета
    void finished(); // Сигнал для завершения работы объекта

private slots:
    void startCounting() {
        QSqlQuery query(m_db);
        if (query.exec("SELECT COUNT(*) FROM pages")) {
            if (query.next()) {
                m_pageCount = query.value(0).toInt();
                emit countingFinished(m_pageCount);
            }
        }
        emit finished();
    }

private:
    QSqlDatabase m_db;
    int m_pageCount = 0;
};

#endif // PAGECOUNTER_H
