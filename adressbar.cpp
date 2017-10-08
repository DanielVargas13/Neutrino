#include "adressbar.h"
#include <QUrl>
AdressBar::AdressBar(QWidget* parent, Qt::WindowFlags f): QLineEdit(parent)
{

}

AdressBar::~AdressBar(){}

void AdressBar::mousePressEvent(QMouseEvent* event){
    emit clicked();
}

void AdressBar::setUrl(QUrl url){
    QString url2 = url.toString();
    QString shortUrl = "";
    if(url2.left(11) == "http://www.") shortUrl = url2.right(url2.length()-10);
    if(url2.left(12) == "https://www.") shortUrl = url2.right(url2.length()-12);
    if(url2.left(4) == "www.") shortUrl = url2.right(url2.length()-3);
    if(shortUrl.indexOf("/") != -1)shortUrl = shortUrl.left(shortUrl.indexOf("/"));
    setText(shortUrl);
    setAlignment(Qt::AlignCenter);
}
