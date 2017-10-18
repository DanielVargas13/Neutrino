
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

    lastSite=QUrl("about.blank");
    predictedPage = new QWebEngineView();
    predictedPage->load(QUrl("blank.org"));
    m_predictor = new Predictor();
    m_history = new History();
    tabs = new QTabWidget;
    tabs->setMovable(true);
    tabs->setTabsClosable(true);
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    QToolButton *newTabButton = new QToolButton();
    newTabButton->setText("+");
    tabs->setCornerWidget(newTabButton,Qt::TopRightCorner);
    connect(newTabButton, SIGNAL(clicked(bool)),this, SLOT(newTab()));
    tabs->addTab(createTab(tr("http://www.unsplash.com")),tr("First tab"));
    std::cout << "creating BrowserWindow"<< '\n';
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(handleTabChanged(int)));
    setCentralWidget(tabs);

    setMinimumSize(750,500);
    setWindowTitle("Neutrino");


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
{
    return tabs->currentWidget()->findChild<QWebEngineView *>();
}

QWidget *BrowserWindow::createTab(QString url)
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
    if(lastSite.toString().toStdString() != "about.blank" && lastSite.toString().length() > 0 && lastSite.toString() != m_webView->url().toString()) m_predictor->addSite(lastSite.toString().toStdString(),m_webView->url().toString().toStdString());
    lastSite = m_webView->url();
    connect(m_webView, SIGNAL(titleChanged(QString)),this,SLOT(handleTitleChanged(QString)));
    connect(m_webView, SIGNAL(urlChanged(QUrl)),this, SLOT(handleUrlChanged(QUrl)));
    connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(handleLoadFinished()));
    return m_tab;
    //connect(m_webView, SIGNAL(loadProgress(int)),this,SLOT(handleWebviewLoadProgress(int)));




}

void BrowserWindow::nextPage()
{
    currentTab()->forward();
    m_adressBar->setUrl(currentTab()->url());

}

void BrowserWindow::previousPage()
{
    currentTab()->back();
    m_adressBar->setUrl(currentTab()->url());

}

void BrowserWindow::loadUrl()
{   std::cout << "enter pressed; loading url :" << m_adressBar->text().toStdString() << std::endl;
    //QWidget *currentWidget = tabs->currentWidget();
    //std::cout << "current widget : "<< currentWidget <<'\n';
    QString url = m_adressBar->text();
    if (url.left(7) != "http://" && url.left(8) != "https://") {
        url = "http://" + url;
    }
    loadPage(url);
    m_adressBar->setUrl(currentTab()->url());
    std::cout << "url loaded \n";
}

void BrowserWindow::loadPage(const QString url)
{   if(predictedPage->url() == url && predictedPage->url().toString() != lastSite.toString())loadPredictedPage(tabs->currentIndex());
    currentTab()->load(QUrl(url));
    std::cout << "Loaded : "<< url.toStdString()<< std::endl;
    std::string predictedSite = m_predictor->predictNextSite(currentTab()->url().toString().toStdString());
    if (predictedSite != "about.blank") predictedPage->load(QUrl(QString::fromStdString(predictedSite)));
    m_adressBar->setText(url);
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
{   QWidget *m_newTab = createTab("http://www.google.com");
    tabs->addTab(m_newTab,"New Tab");
    tabs->setCurrentWidget(m_newTab);
}

void BrowserWindow::closeTab(int index){
    tabs->removeTab(index);

}


void BrowserWindow::handleUrlChanged(QUrl url)
{   if(predictedPage->url() == url && predictedPage->url() != lastSite) loadPredictedPage(tabs->currentIndex());

    else{
        if(lastSite.toString().length() > 0 && url.toString().length()> 0 && lastSite.toString() != url.toString()) m_predictor->addSite(lastSite.toString().toStdString(),url.toString().toStdString());
        lastSite = url;
        std::string predictedSite = m_predictor->predictNextSite(currentTab()->url().toString().toStdString());
        std::cout <<predictedSite << std::endl;
        if (predictedSite != "about.blank") predictedPage->load(QUrl(QString::fromStdString(predictedSite)));
    }
    m_adressBar->setUrl(url);



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
    m_predictor->saveToFile();
}
void BrowserWindow::adressBarClicked(){
    m_adressBar->setAlignment(Qt::AlignLeft);
}

void BrowserWindow::handleTabChanged(int index){

    if(index >= 0) m_adressBar->setUrl(tabs->widget(index)->findChild<QWebEngineView *>()->url());
}

void BrowserWindow::loadPredictedPage(int index){
    std::cout << "Loading predicted page " << predictedPage->url().toString().toStdString()<< '\n';
    QWidget *m_tab = new QWidget;
    QWebEngineView *m_webView = new QWebEngineView;
    std::swap(predictedPage,m_webView);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_webView);
    m_tab->setLayout(layout);
    m_tab->addAction(closeTabAction);
    if(lastSite.toString().toStdString() != "about.blank" && lastSite.toString().length() > 0 && lastSite.toString() != m_webView->url().toString()) m_predictor->addSite(lastSite.toString().toStdString(),m_webView->url().toString().toStdString());
    lastSite = m_webView->url();
    connect(m_webView, SIGNAL(titleChanged(QString)),this,SLOT(handleTitleChanged(QString)));
    connect(m_webView, SIGNAL(urlChanged(QUrl)),this, SLOT(handleUrlChanged(QUrl)));
    connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(handleLoadFinished()));
    tabs->removeTab(index);
    int insertedIndex = tabs->insertTab(index,m_tab,m_webView->title());
    tabs->setCurrentIndex(index);
    std::cout << "Tab inserted at index "<<insertedIndex<<std::endl;
}
