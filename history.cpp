#include "history.h"
#include <iostream>
#include <QWebEngineHistory>
#include <QWebEngineView>
#include <QtWidgets>
#include <QTextStream>
History::History()
{

}

/*
void History::saveToHistory(QUrl url, QString title){
   bool fileExists = QFile::exists("history.html");
   QFile historyFile("history.html");
   historyFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
   QTextStream stream(&historyFile);
   stream.seek(0);
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
   else if(!stream.seek(430)) std::cout << "Failed to go to pos \n";
       std::cout << "History cursor pos : " << stream.pos()<< std::endl;
       std::cout << "adding " <<title.toStdString()<<" to history \n";
       stream <<"<a href =" << url.toString() <<">" << title <<"</a><br><br>";
   historyFile.close();
}
*/

void History::saveToHistory(QUrl url, QString title){
   bool fileExists = QFile::exists("history.html");
   QFile historyFile("history.html");
   historyFile.open(QFile::ReadWrite);

   QString historyStr;

   if(!fileExists){
       std::cout << "history.html doesn't exist, creating file" << std::endl;
       historyStr =  "<!DOCTYPE html> \
                     \n <html> \
                     \n <head>\
                     \n <title>History</title>\
                     \n <meta charset =\"UTF-8\" />\
                     \n <style>a{color:black; text-decoration:none;}</style> \
                     \n <link href=\"https://fonts.googleapis.com/css?family=Lato\" rel=\"stylesheet\"> \
                     \n </head>\
                     \n  <body style=\"font-family: 'Lato', sans-serif;\"> \
                     \n <div style=\"color:white;height:70px; line-height:70px; background-color:#263238; box-shadow: 0 3px 15px 0 #000000;\"> \
                     \n <h2 style=\"margin-left:30px;\">Your History<h2> \
                     \n </div><br><br>";
       historyStr+="\n <a href="+url.toString()+">"+title+"</a><br><br>";
       std::cout << "adding " <<title.toStdString()<<" to history \n";


   }
   else {
       historyStr = historyFile.readAll();
       //std::cout << "History file : \n" << historyStr.toStdString() << std::endl;
       historyStr = historyStr.left(692) + " \n <a href="+url.toString()+">"+title+"</a><br><br>" + historyStr.mid(692,-1);


   }
   historyFile.close();
   historyFile.open(QFile::WriteOnly | QFile::Truncate);
   QTextStream stream(&historyFile);
   stream << historyStr;
   historyFile.close();

}
