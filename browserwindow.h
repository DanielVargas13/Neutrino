#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QtWidgets>
#include "adressbar.h"
#include "history.h"
#include "predictor.h"


class BrowserWindow : public QMainWindow
{
    Q_OBJECT

public:
    BrowserWindow();
    ~BrowserWindow();
    void loadPage(const QString url);
    QWebEngineView *currentTab();
    QWidget *createTab(const QString url);

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
    void adressBarClicked();

private :
    void createIcons();
    void createActions();
    void createToolBar();
    void createFavHistoryMenu();
    //void createStatusBar;


    QTabWidget *tabs;
    AdressBar *m_adressBar;
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

    QWebEngineView *predictedPage;
    QUrl lastSite;
    History *m_history;
    Predictor *m_predictor;

    //QProgressBar *m_loadProgress;


};

#endif // BROWSERWINDOW_H
