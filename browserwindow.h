#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QtWidgets>
#include "history.h"


class BrowserWindow : public QMainWindow
{
    Q_OBJECT

public:
    BrowserWindow();
    ~BrowserWindow();
    void loadPage(const QString url);
    QWebEngineView *currentTab();
    QWidget *tabCreate(const QString url);

private slots :
    //void handleWebviewLoadProgress(int);
    void handleTitleChanged(QString);
    void handleUrlChanged(QUrl);
    void handleLoadFinished();
    void nextPage();
    void previousPage();
    void loadUrl();
    void refresh();
    void stopLoad();
    void newTab();
    void closeTab();
    void showHistory();

private :
    void createIcons();
    void createActions();
    void createToolBar();
    void createFavHistoryMenu();
    //void createStatusBar;


    QTabWidget *tabs;
    QLineEdit *m_adressBar;
    QIcon previousIcon;
    QIcon nextIcon;
    QIcon refreshIcon;
    QIcon fhMenuIcon;

    QAction *closeAction;
    QAction *closeTabAction;
    QAction *newTabAction;
    QAction *stopAction;
    QAction *reloadAction;
    QAction *previousAction;
    QAction *nextAction;
    QAction *loadAction;
    QAction *refreshAction;

    History *m_history;

    //QProgressBar *m_loadProgress;


};

#endif // BROWSERWINDOW_H
