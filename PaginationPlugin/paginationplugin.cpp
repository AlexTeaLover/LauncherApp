#include "paginationplugin.h"

#include <QPushButton>
#include <QVBoxLayout>


LauncherPluginBase *createWidget(QWidget *parent)
{
    return new PaginationPlugin(parent);
}

PaginationPlugin::PaginationPlugin(QWidget *parent)
    : LauncherPluginBase(parent),
    pageController(nullptr),
    tableView(nullptr),
    prevButton(nullptr),
    nextButton(nullptr),
    pageInput(nullptr)
{
    PaginationPlugin::executeAll();
    PaginationPlugin::setupConnections();
}

PaginationPlugin::~PaginationPlugin()
{
    delete pageController;
    delete tableView;
    delete nextButton;
    delete prevButton;
    delete pageInput;
}

void PaginationPlugin::defineLayout()
{
    this->resize(1300, 800);

    // Создание кнопок
    nextButton = new QPushButton("Next", this);
    prevButton = new QPushButton("Previous", this);
    pageInput = new QLineEdit(this);  // Поле для ввода номера страницы
    pageInput->setFixedWidth(50);  // Устанавливаем ширину для ввода страницы
    pageInput->setText("1");  // Устанавливаем начальное значение (первая страница)
    totalPagesLabel = new QLabel("из ...", this);

    // Создание верхнего горизонтального лейаута
    topLayout = new QHBoxLayout();
    topLayout->addWidget(prevButton);
    topLayout->addWidget(pageInput);
    topLayout->addWidget(totalPagesLabel);
    topLayout->addWidget(nextButton);
    topLayout->addStretch(0);

    // Создание основного вертикального лейаута
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Добавление верхнего лейаута
    mainLayout->addLayout(topLayout);

    // Создание таблицы
    tableView = new QTableView(this);

    // Добавление таблицы в основной лейаут
    mainLayout->addWidget(tableView);

    setLayout(mainLayout);
}

void PaginationPlugin::setupConnections()
{
    pageController = new PageController("../Databases/MusicDataBase.db", tableView);

    connect(nextButton, &QPushButton::clicked, this, [this]()
            {
                if (pageController)
                {
                    pageController->showNextPage();
                    updateButtonStates();
                }
            });

    connect(prevButton, &QPushButton::clicked, this, [this]()
            {
                if (pageController)
                {
                    pageController->showPreviousPage();
                    updateButtonStates();
                }
            });


    connect(&pageController->futureWatcherPrev, &QFutureWatcher<TrackTableModel::PageDataPtr>::finished, this, &PaginationPlugin::updateButtonStates);
    connect(&pageController->futureWatcherNext, &QFutureWatcher<TrackTableModel::PageDataPtr>::finished, this, &PaginationPlugin::updateButtonStates);
    connect(&pageController->futureWatcherPagesCounting, &QFutureWatcher<void>::finished, this, [this]()
            {
                qDebug() << "Now";
                totalPagesLabel->setText(QString("из %1").arg(pageController->getPagesCount()));
            });

    // Подключение поля ввода для перехода к конкретной странице
    connect(pageInput, &QLineEdit::returnPressed, this, [this]()
            {
                int pageNumber = pageInput->text().toInt();

                // Проверяем, что введённое значение корректно
                if (pageNumber > 0)
                {
                    pageController->goToPage(pageNumber);
                    updateButtonStates();
                }
                else
                {
                    // Если ввод некорректен, сбрасываем на текущую страницу
                    pageInput->setText(QString::number(pageController->getCurrentPage()));
                }
            });

    updateButtonStates();
}

void PaginationPlugin::connectToDB()
{
//    if(QSqlDatabase::contains("MusicDataBase"))
//    {
//        dataBase = QSqlDatabase::database("MusicDataBase");
//    }
//    else
//    {
//        qDebug() << "The database is not yet open";

//        dataBase = QSqlDatabase::addDatabase("QSQLITE", "MusicDataBase");
//        dataBase.setDatabaseName("../Databases/MusicDataBase.db");

//        if(!dataBase.open()) qDebug() << "DB don't open";
//    }
}

void PaginationPlugin::updateButtonStates()
{
    prevButton->setEnabled(pageController->futureWatcherPrev.isFinished());
    nextButton->setEnabled(pageController->futureWatcherNext.isFinished());

    // Обновляем отображение текущей страницы
    pageInput->setText(QString::number(pageController->getCurrentPage()));
}

void PaginationPlugin::connectAction(QAction &action)
{
    Q_UNUSED(action);
}
