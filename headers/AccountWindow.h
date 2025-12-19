//
// Created by adi on 11/15/25.
//

#pragma once

#include <QLineEdit>
#include <QLabel>
#include "BankSystem.h"
#include "KeypadWidget.h"

class AccountWindow : public QWidget {
    Q_OBJECT

public:
    explicit AccountWindow(BankSystem *bankSystem, QWidget *loginWindow, QWidget *parent = nullptr);

    ~AccountWindow() override;

private:
    BankSystem *bankSystem;
    QWidget *loginWindow;
    QLabel *balanceLabel;
    QLabel *infoLabel;
    QLineEdit *amountDisplay;
    KeypadWidget *keypad;

private slots:
    void onDigitClicked(const QString &digit);

    void onBackspaceClicked();

    void onWithdrawClicked();
};
