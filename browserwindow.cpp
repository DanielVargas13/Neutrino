
#include "browserwindow.h"
#include "history.h"
#include <QWidget>
#include <iostream>
#include <QWebEngineProfile>
#include <QWebEngineHistory>

BrowserWindow::BrowserWindow()
{
    QIcon previousIcon;
    QIcon nextIcon;
    createIcons();
    createActions();
    createToolBar();

    m_history = new History();
    tabs = new QTabWidget;
    tabs->setMovable(true);
    tabs->setTabsClosable(true);
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab()));
    QToolButton *newTabButton = new QToolButton();
    newTabButton->setText("+");
    tabs->setCornerWidget(newTabButton,Qt::TopRightCorner);
    connect(newTabButton, SIGNAL(clicked(bool)),this, SLOT(newTab()));
    tabs->addTab(tabCreate(tr("http://www.unsplash.com")),tr("New tab"));
    std::cout << "creating BrowserWindow"<< '\n';
    //connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
    setCentralWidget(tabs);

    setMinimumSize(750,500);
    setWindowTitle("Eclair project");


}

BrowserWindow::~BrowserWindow()
{
}
void BrowserWindow::createIcons() {
    QPixmap previousPm("icons/previous2.svg");
    QPixmap previousHoveredPm("icons/previous2hovered.svg");
    QPixmap nextPm("icons/next2.svg");
    QPixmap nextHoveredPm("icons/next2hovered.svg");
    QPixmap refreshPm("icons/refresh2.svg");
    QPixmap refreshHoveredPm("icons/refresh2hovered.svg");
    QPixmap fhMenuPm("icons/menu.svg");
    QPixmap fhMenuHoveredPm("icons/menuhovered.svg");

    previousIcon.addPixmap(previousPm, QIcon::Normal, QIcon::Off);
    previousIcon.addPixmap(previousHoveredPm, QIcon::Active, QIcon::Off);
    nextIcon.addPixmap(nextPm, QIcon::Normal, QIcon::Off);
    nextIcon.addPixmap(nextHoveredPm, QIcon::Active, QIcon::Off);
    refreshIcon.addPixmap(refreshPm, QIcon::Normal, QIcon::Off);
    refreshIcon.addPixmap(refreshHoveredPm, QIcon::Active, QIcon::Off);
    fhMenuIcon.addPixmap(fhMenuPm, QIcon::Normal, QIcon::Off);
    fhMenuIcon.addPixmap(fhMenuHoveredPm, QIcon::Active, QIcon::Off);
}

void BrowserWindow::createActions() {

    closeAction = new QAction(tr("&Quit"),this);
    closeTabAction = new QAction(QIcon("icons/refresh2.svg"),tr("&Close Tab"),this);
    newTabAction = new QAction(tr("&New Tab"),this);
    stopAction = new QAction(tr("&Stop load"),this);
    previousAction = new QAction(previousIcon,"&Previous page",this);
    nextAction = new QAction(nextIcon,"&Next page",this);
    loadAction = new QAction(this);
    refreshAction = new QAction(refreshIcon,tr("&Refresh"),this);

    connect(closeAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    connect (previousAction, SIGNAL(triggered(bool)), this, SLOT(previousPage()));
    connect (nextAction, SIGNAL(triggered(bool)), this, SLOT(nextPage()));
    connect (refreshAction, SIGNAL(triggered(bool)), this, SLOT(refresh()));
    connect (loadAction, SIGNAL(triggered(bool)), this, SLOT(loadUrl()));

}

void BrowserWindow::createToolBar()
{
    m_adressBar = new AdressBar;
    m_adressBar->setStyleSheet("QLineEdit {  border: 1px solid gray; border-radius: 5px;}");
    m_adressBar->setAttribute(Qt::WA_MacShowFocusRect,0);

    QGraphicsDropShadowEffect* dropShadowEffect = new QGraphicsDropShadowEffect(this);
    dropShadowEffect->setOffset(0);
    dropShadowEffect->setBlurRadius(15);
    m_adressBar->setGraphicsEffect(dropShadowEffect);

    connect(m_adressBar, SIGNAL(clicked()), this, SLOT(adressBarClicked()));
    connect(m_adressBar, SIGNAL(returnPressed()), this, SLOT(loadUrl()));
    QToolBar *m_toolBar = addToolBar("ToolBar");
    m_toolBar->setMovable(false);
    QToolButton *fhButton = new QToolButton();
    fhButton->setIcon(fhMenuIcon);
    connect(fhButton, SIGNAL(pressed()), this, SLOT(showHistory()));


    m_toolBar->addAction(previousAction);
    m_toolBar->addAction(nextAction);
    m_toolBar->addAction(refreshAction);
    m_toolBar->addWidget(m_adressBar);
    m_toolBar->addWidget(fhButton);


}


QWebEngineView *BrowserWindow::currentTab()
{   QWebEngineView *currenttab = tabs->currentWidget()->findChild<QWebEngineView *>();
    std::cout << "current tab : "<< currenttab <<'\n';
    return tabs->currentWidget()->findChild<QWebEngineView *>();
}

QWidget *BrowserWindow::tabCreate(QString url)
{
    std::cout << "Creating tab with url : " << url.toStdString() << '\n';
    QWidget *m_tab = new QWidget;
    QWebEngineView *m_webView = new QWebEngineView;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_webView);
    m_tab->setLayout(layout);

    m_tab->addAction(closeTabAction);
    if (url.isEmpty())
    {
        m_webView->load(QUrl(tr("http://www.google.com")));
    }
    else {
        m_webView->load(QUrl(url));
    }
    connect(m_webView, SIGNAL(titleChanged(QString)),this,SLOT(handleTitleChanged(QString)));
    connect(m_webView, SIGNAL(urlChanged(QUrl)),this, SLOT(handleUrlChanged(QUrl)));
    connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(handleLoadFinished()));
    return m_tab;
    //connect(m_webView, SIGNAL(loadProgress(int)),this,SLOT(handleWebviewLoadProgress(int)));




}

void BrowserWindow::nextPage()
{
    currentTab()->forward();
    m_adressBar->setText(currentTab()->url().toString());

}

void BrowserWindow::previousPage()
{
    currentTab()->back();
    m_adressBar->setText(currentTab()->url().toString());

}

void BrowserWindow::loadUrl()
{   std::cout << "return pressed; loading url \n";
    QWidget *currentWidget = tabs->currentWidget();
    std::cout << "current widget : "<< currentWidget <<'\n';
    QString url = m_adressBar->text();
    if (url.left(7) != "http://" && url.left(8) != "https://") {
        url = "http://" + url;
    }
    currentTab()->load(QUrl(url));
    m_adressBar->setText(currentTab()->url().toString());
    std::cout << "url loaded \n";
}

void BrowserWindow::loadPage(const QString url)
{
    currentTab()->load(QUrl(url));
    m_adressBar->setText(currentTab()->url().toString());
}

void BrowserWindow::refresh()
{
    currentTab()->reload();
}

void BrowserWindow::stopLoad()
{
    currentTab()->stop();
}

void BrowserWindow::newTab()
{   QWidget *m_newTab = tabCreate("http://www.google.com");
    tabs->addTab(m_newTab,"New Tab");
    tabs->setCurrentWidget(m_newTab);
}

void BrowserWindow::closeTab(){
    tabs->removeTab(tabs->currentIndex());

}


void BrowserWindow::handleUrlChanged(QUrl url)
{   QString url2 = url.toString();
    QString shortUrl = "";
    if(url2.left(11) == "http://www.") shortUrl = url2.right(url2.length()-10);
    if(url2.left(12) == "https://www.") shortUrl = url2.right(url2.length()-12);
    if(url2.left(4) == "www.") shortUrl = url2.right(url2.length()-3);
    if(shortUrl.indexOf("/") != -1)shortUrl = shortUrl.left(shortUrl.indexOf("/"));
    m_adressBar->setText(shortUrl);
    m_adressBar->setAlignment(Qt::AlignCenter);

}

void BrowserWindow::handleTitleChanged(QString title)
{
    tabs->setTabText(tabs->currentIndex(),title);
}



void BrowserWindow::showHistory(){
    QFile historyFile("history.html");
    historyFile.open(QFile::ReadOnly);
    QString historyHtml = historyFile.readAll();
    historyHtml += "</body></HTML>";
    currentTab()->setHtml(historyHtml,QUrl());
    tabs->setTabText(tabs->currentIndex(),"History");
}

void BrowserWindow::handleLoadFinished(){
    m_history->saveToHistory(currentTab()->url(),currentTab()->title());
}
void BrowserWindow::adressBarClicked(){
    m_adressBar->setAlignment(Qt::AlignLeft);
}

