//
// Created by adi on 11/15/25.
//

#include "../headers/AccountWindow.h"
AccountWindow::AccountWindow(Account* account, QWidget* loginWindow, QWidget* parent)
    : QWidget(parent), account(account), loginWindow(loginWindow)
{



    setWindowTitle("Bankomat");
    resize(1200, 800);
}
AccountWindow::~AccountWindow() {}