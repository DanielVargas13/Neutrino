#include "adressbar.h"

AdressBar::AdressBar(QWidget* parent, Qt::WindowFlags f): QLineEdit(parent)
{

}

AdressBar::~AdressBar(){}

void AdressBar::mousePressEvent(QMouseEvent* event){
    emit clicked();
}
