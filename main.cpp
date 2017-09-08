#include "browserwindow.h"
#include <QApplication>
#include <QWebEngineView>
#include <QFile>
#include <iostream>

int main(int argc, char *argv[])
{
    QFile file(":/styles/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setStyleSheet(styleSheet);
    BrowserWindow w;
    w.show();

    return a.exec();
}
