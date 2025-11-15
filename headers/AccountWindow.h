//
// Created by adi on 11/15/25.
//

#ifndef ACCOUNTWINDOW_H
#define ACCOUNTWINDOW_H
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QVector>
#include <QLineEdit>
#include <QLabel>
#include "Account.h"
class AccountWindow:public QWidget {
    Q_OBJECT
    public:
    explicit AccountWindow(Account* account,QWidget* loginWindow,QWidget* parent=nullptr);
    ~AccountWindow() override;
    private:
    Account* account;
    QWidget* loginWindow;

};



#endif //ACCOUNTWINDOW_H
