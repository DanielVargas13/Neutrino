#include "history.h"
#include <iostream>
#include <QWebEngineHistory>
#include <QWebEngineView>
#include <QtWidgets>
#include <QTextStream>
History::History()
{

}


void History::saveToHistory(QUrl url, QString title){
   bool fileExists = QFile::exists("history.html");
   QFile historyFile("history.html");
   historyFile.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Append);
   QTextStream stream(&historyFile);
   if(!fileExists){
       std::cout << "history.html doesn't exist, creating file" << std::endl;
       stream << "<!DOCTYPE html> \
                    <html> \
                        <head>\
                        <title>History</title>\
                        <meta charset =\"UTF-8\" />\
                        <style>a{color:black; text-decoration:none;}</style> \
                        <link href=\"https://fonts.googleapis.com/css?family=Lato\" rel=\"stylesheet\"> \
                        </head>\
                        <body style=\"font-family: 'Lato', sans-serif;\"> \
                        <div style=\"color:white;height:70px; line-height:70px; background-color:#263238; box-shadow: 0 3px 15px 0 #000000;\"> \
                        <h2 style=\"margin-left:30px;\">Your History<h2> \
                        </div><br>";

   }
       std::cout << "adding " <<title.toStdString()<<" to history \n";
       stream <<"<a href =" << url.toString() <<">" << title <<"</a><br><br>";

   historyFile.close();
}
