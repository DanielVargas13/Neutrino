#ifndef FHMENU_H
#define FHMENU_H
#include <QtWidgets>
#include <QWebEngineView>
#include <QWebEngineHistory>
#include <iostream>
#include <QTextStream>

class History
{
public:
    History();
    void saveToHistory(QUrl,QString);


};

#endif // FHMENU_H
