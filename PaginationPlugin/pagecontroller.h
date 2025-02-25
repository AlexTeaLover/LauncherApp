#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <TrackTableModel.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>
#include <QSqlDatabase>
#include <QTableView>
#include <QThread>
#include <QVariant>

class PageController : public QObject
{
    Q_OBJECT

public:
    PageController(const QString& connectionString, QTableView *tableView, QObject *parent = nullptr);
    ~PageController();

    void goToPage(int pageNumber);
    void showNextPage();
    void showPreviousPage();

    int getCurrentPage() const;

    QFutureWatcher<TrackTableModel::PageDataPtr> futureWatcherNext;
    QFutureWatcher<TrackTableModel::PageDataPtr> futureWatcherSelected;
    QFutureWatcher<TrackTableModel::PageDataPtr> futureWatcherPrev;
    QFutureWatcher<void> futureWatcherPagesCounting;

    int getPagesCount() const;

private:
    constexpr static int recordsPerPage = 50;

    int pagesCount;
    int currentPage;
    QString connectionString;
    QEventLoop loop;
    TrackTableModel *tableModel;

    void displayPage(TrackTableModel::PageDataPtr page);
    void loadAdjacentPages();
    TrackTableModel::PageDataPtr loadPage(int pageNumber);

    int pagesCounting() const;
};

#endif // PAGECONTROLLER_H
