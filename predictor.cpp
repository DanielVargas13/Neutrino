
#include "predictor.h"
#include <map>
#include <QTextStream>
#include <iostream>
#include <algorithm>

Predictor::Predictor()
{
    bool fileExists = QFile::exists("Matrix.txt");
    std::cout << "File exists ? "<< fileExists<< std::endl;
    QFile matrixFile("Matrix.txt");
    std::cout << "Permissions ? " << matrixFile.permissions()<<'\n';
    bool fileOpen = matrixFile.open(QIODevice::ReadOnly| QIODevice::Text);
    std::cout << "File open : "<<fileOpen<<std::endl;
    QTextStream stream(&matrixFile);
    if(!fileExists){
        std::cout << "Matrix file doesn't exist, creating file" << std::endl;
        siteMatrix[0][0] = 0;
        index = 0;
        siteMap["https://unsplash.com/"] = 0;
        numberMap[0] = "https://unsplash.com/";
    }
    else{
        bool readingMatrix = false;
        int i = 0;
        while(!matrixFile.atEnd()){
            QString line = matrixFile.readLine();
            std::cout <<"Reading from matrixFile : "<<line.toStdString() << std::endl;
            if(line == "[Matrix]") readingMatrix = true;
            if(!readingMatrix){
                int n = line.indexOf(":");
                if(line.split(":").length() > 0){
                    std::string url = line.mid(n).toStdString();
                    siteMap[url] = line.left(n).toInt();
                    numberMap[n] = url;
                }
            }
            else if(line != "[Matrix]" && readingMatrix){
                std::cout << "Matrix read from file : \n";
                for(int j = 0; j<line.split(" ").length(); j++){
                    siteMatrix[i][j] = line.split(" ")[j].toInt();
                    std::cout << siteMatrix[i][j];
                }
                std::cout << std::endl;
                i++;
            }
        }
    index = i;

    }
}

void Predictor::addSite(std::string previousSite, std::string site){
    std::map<std::string, int>::iterator it = siteMap.find(site);
    if(it == siteMap.end()){
        index++;
        siteMap[site] = index;
        std::cout << "adding site "<<site<<" with index "<<index <<" and previous site : "<<previousSite<<std::endl;
        numberMap[index] = site;
        siteMatrix.resize(index+1);
        for(int i = 0; i < siteMatrix.size();i++){
            siteMatrix[i].resize(index+1);
        }
        siteMatrix[siteMap[previousSite]][siteMap[site]] = 1;
        std::cout << "adding site in the matrix at "<<siteMap[previousSite]<<" "<<siteMap[site] <<std::endl;
    }
    else siteMatrix[siteMap[previousSite]][siteMap[site]]++;
    std::cout << "Matrix size : "<<siteMatrix.size()<<" "<<siteMatrix[0].size()<< std::endl;
    for(int j = 0; j <siteMatrix.size();j++){
        for(int k = 0; k < siteMatrix[0].size();k++){
            std::cout<<siteMatrix[j][k]<<" ";
        }
        std::cout<<std::endl;
    }

}

std::string Predictor::predictNextSite(std::string site){
    int x = siteMap[site];
    int k = 0;
    int s = 0;
    for(int i = 0; i<index; i++){
        if(siteMatrix[x][i] > k) {
            k = siteMatrix[x][i];
            s = i;
        }
    }
    if(k > 0){
        std::cout << "Next site will be : "<<numberMap[s]<< std::endl;
        return numberMap[s];
    }
    else{
        std::cout << "Next site cannot be predicted !" << std::endl;
        return "about.blank";
    }

}

void Predictor::saveToFile(){
    std::cout <<"Saving siteMatrix to file" << std::endl;
    QFile matrixFile("Matrix.txt");
    bool fileOpen = matrixFile.open(QIODevice::WriteOnly| QIODevice::Text);
    std::cout << "File open when saving : "<<fileOpen<<std::endl;
    QTextStream stream(&matrixFile);
    for(int i = 0; i <= index; i++){
        stream <<i<<":"<<QString::fromStdString(numberMap[i])<< "\n";
    }
    stream << "[Matrix] \n";
    for(int i = 0; i <= index; i++){
        for(int j = 0; j <= index; j++){
            stream << siteMatrix[i][j]<<" ";
        }
        stream <<"\n";
    }
    matrixFile.close();
}
