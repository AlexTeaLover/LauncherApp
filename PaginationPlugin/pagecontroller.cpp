#include "pagecontroller.h"

PageController::PageController(const QString& connectionString, QTableView *tableView, QObject *parent)
    : QObject(parent), currentPage(10), connectionString(connectionString), tableModel(new TrackTableModel(this))
{
    connect(&futureWatcherSelected, &QFutureWatcher<TrackTableModel::PageDataPtr>::finished, this, [this]()
    {
        displayPage(futureWatcherSelected.future().takeResult());
    });

    connect(&futureWatcherPagesCounting, &QFutureWatcher<void>::finished, &loop, &QEventLoop::quit);

    futureWatcherPagesCounting.setFuture(QtConcurrent::run([this] {pagesCount = pagesCounting();}));

    goToPage(currentPage);

    tableView->setModel(tableModel);
}

PageController::~PageController()
{
}

void PageController::goToPage(int pageNumber)
{
    //здесь нужно знать max число страниц
    if(futureWatcherPagesCounting.isRunning())
    {
        loop.exec();
    }

    if (pageNumber < 1 && pageNumber > pagesCount)
        return;

    currentPage = pageNumber;

    loadAdjacentPages();

    futureWatcherSelected.setFuture(QtConcurrent::run([this] { return loadPage(currentPage); }));
}

void PageController::showNextPage()
{
    //здесь нужно знать max число страниц
    ++currentPage;
    displayPage(futureWatcherNext.future().takeResult());
    loadAdjacentPages();
}

void PageController::showPreviousPage()
{
    if (currentPage > 0)
    {
        --currentPage;
        displayPage(futureWatcherPrev.future().takeResult());
        loadAdjacentPages();
    }
}

void PageController::displayPage(TrackTableModel::PageDataPtr page)
{
    if (page)
        tableModel->setTracks(std::move(page));
}

void PageController::loadAdjacentPages()
{
    futureWatcherNext.setFuture(QtConcurrent::run([this] { return loadPage(currentPage + 1); }));

    futureWatcherPrev.setFuture(QtConcurrent::run([this] { return loadPage(currentPage - 1); }));
}

int PageController::getPagesCount() const
{
    return pagesCount;
}

TrackTableModel::PageDataPtr PageController::loadPage(int pageNumber)
{
    auto pageData = std::make_unique<QList<QList<QVariant>>>();

    quintptr threadId = reinterpret_cast<quintptr>(QThread::currentThreadId());
    QString connectionName = QString("ThreadConnection_%1").arg(threadId);

    {
        QThread::sleep(QRandomGenerator::global()->bounded(2, 3));

        QSqlDatabase dbThread = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        dbThread.setDatabaseName(connectionString);

        if (!dbThread.open())
        {
            qWarning() << "Failed to open database in thread:" << dbThread.lastError().text();
            return nullptr;
        }

        QSqlQuery query(dbThread);
        query.prepare("SELECT TrackId, Name, AlbumId, MediaTypeId, GenreId, Composer, Milliseconds, Bytes, UnitPrice "
                      "FROM Track "
                      "LIMIT 50 OFFSET :offset");
        query.bindValue(":offset", pageNumber * recordsPerPage);

        if (!query.exec())
        {
            qWarning() << "Failed to execute query:" << query.lastError().text();
            return nullptr;
        }

        while (query.next())
        {
            QList<QVariant> rowData;
            rowData << query.value(0) << query.value(1) << query.value(2) << query.value(3)
                    << query.value(4) << query.value(5) << query.value(6) << query.value(7)
                    << query.value(8);
            pageData->append(rowData);
        }

        dbThread.close();
    }

    QSqlDatabase::removeDatabase(connectionName);

    return pageData;
}

int PageController::pagesCounting() const
{
    //здесь определяется max число страниц
    quintptr threadId = reinterpret_cast<quintptr>(QThread::currentThreadId());
    QString connectionName = QString("ThreadConnection_%1").arg(threadId);

    int totalRecords = 0;

    {
        QThread::sleep(QRandomGenerator::global()->bounded(3, 4));

        QSqlDatabase dbThread = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        dbThread.setDatabaseName(connectionString);

        if (!dbThread.open())
        {
            qWarning() << "Failed to open database in thread:" << dbThread.lastError().text();
            return -1;
        }

        QSqlQuery query(dbThread);
        if (!query.exec("SELECT COUNT(*) FROM Track"))
        {
            qWarning() << "Failed to execute query:" << query.lastError().text();
            return -1;
        }

        if (query.next())
        {
            totalRecords = query.value(0).toInt();
        }

        dbThread.close();
    }

    QSqlDatabase::removeDatabase(connectionName);

    return (totalRecords + recordsPerPage - 1) / recordsPerPage;
}

int PageController::getCurrentPage() const
{
    return currentPage;
}
