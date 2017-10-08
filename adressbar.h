#ifndef ADRESSBAR_H
#define ADRESSBAR_H

#include <QLineEdit>
#include <QWidget>
#include <Qt>
class AdressBar : public QLineEdit
{
    Q_OBJECT
public:
    AdressBar(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~AdressBar();
    void setUrl(QUrl);
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
};


#endif // ADRESSBAR_H
