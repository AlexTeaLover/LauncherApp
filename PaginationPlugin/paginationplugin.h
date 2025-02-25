#ifndef PAGINATIONPLUGIN_H
#define PAGINATIONPLUGIN_H

#include <paginationplugin_global.h>
#include <launcherpluginbase.h>
#include <PageController.h>
#include <TrackTableModel.h>
#include <QPushButton>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFrame>
#include <QLabel>

class PaginationPlugin : public LauncherPluginBase
{
public:
    explicit PaginationPlugin(QWidget *parent = nullptr);
    ~PaginationPlugin();

    void connectAction(QAction &action) override;

private:
    void defineLayout() override;
    void setupConnections();
    void connectToDB() override;

    void updateButtonStates();

    PageController *pageController;
    QTableView *tableView;

    QPushButton *prevButton;
    QPushButton *nextButton;
    QLineEdit *pageInput;
    QLabel *totalPagesLabel;
    QHBoxLayout *topLayout;
};

extern "C" PAGINATIONPLUGIN_EXPORT LauncherPluginBase *createWidget(QWidget *parent);

#endif // PAGINATIONPLUGIN_H
