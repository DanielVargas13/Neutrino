#ifndef PREDICTOR_H
#define PREDICTOR_H
#include <map>
#include <QFile>
#include <algorithm>
#include <QTextStream>


class Predictor
{
public:
    Predictor();
    void addSite(std::string previousSite, std::string currentSite);
    std::string predictNextSite(std::string site);
    ~Predictor();
    void saveToFile();
private:
    std::map<std::string,int> siteMap;
    std::map<int, std::string> numberMap;
    int index;
    std::vector<std::vector<int>> siteMatrix;
    QFile matrixFile;
    QTextStream stream;



};

#endif // PREDICTOR_H
