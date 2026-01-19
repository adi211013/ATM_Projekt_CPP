//
// Created by adi on 11/15/25.
//

#include "../headers/AccountWindow.h"

#include <QMessageBox>
#include <QTimer>

AccountWindow::AccountWindow(BankSystem *bankSystem, QWidget *loginWindow, QWidget *parent)
    : QWidget(parent), bankSystem(bankSystem), loginWindow(loginWindow) {
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Bankomat");
    resize(1200, 800);
    mainLayout = new QVBoxLayout(this);
    keypad = new KeypadWidget(this);
    keypad->setScreenText(
        "Dostępne środki: " + QString::number(bankSystem->getBalance()) + " PLN\n Wpisz kwote wyplaty");
    keypad->setSideButtonText("50PLN", 0);
    keypad->setSideButtonText("100PLN", 1);
    keypad->setSideButtonText("150PLN", 2);
    keypad->setSideButtonText("200PLN", 3);
    keypad->setSideButtonText("250PLN", 4);
    keypad->setSideButtonText("300PLN", 5);
    keypad->setSideButtonText("500PLN", 6);
    keypad->setSideButtonText("Wyjscie", 7);
    mainLayout->addWidget(keypad);
    connect(keypad, &KeypadWidget::confirmClicked, this, &AccountWindow::onWithdrawClicked);
    connect(keypad, &KeypadWidget::sideButtonClicked, this, &AccountWindow::onSideButtonClicked);
}
AccountWindow::~AccountWindow() {
    loginWindow->show();
}
void AccountWindow::onSideButtonClicked(int index) {
    switch (index) {
        case 0:
            keypad->setInputText("50");
            onWithdrawClicked();
            break;
        case 1:
            keypad->setInputText("100");
            onWithdrawClicked();
            break;
        case 2:
            keypad->setInputText("150");
            onWithdrawClicked();
            break;
        case 3:
            keypad->setInputText("200");
            onWithdrawClicked();
            break;
        case 4:
            keypad->setInputText("250");
            onWithdrawClicked();
            break;
        case 5:
            keypad->setInputText("300");
            onWithdrawClicked();
            break;
        case 6:
            keypad->setInputText("500");
            onWithdrawClicked();
            break;
        case 7:
            this->close();
            break;
    }
}

void AccountWindow::onWithdrawClicked() {
    int amount = keypad->getInputText().toInt();
    std::map<int, int> notesToGive;
    WithdrawResult result = bankSystem->withdraw(amount, notesToGive);
    switch (result) {
        case WithdrawResult::Success: {
            QString info = "Wypłacono: " + QString::number(amount) + " PLN\n\n";
            info += "Nominały:\n";
            for (auto const &[nominal, count]: notesToGive) {
                if (count != 0)
                    info += QString::number(nominal) + " PLN x " + QString::number(count) + "\n";
            }
            keypad->setErrorLabelText(info);
            keypad->setScreenText("");
            QTimer::singleShot(1000,this,[this]() {
            this->close();
            });
            break;
        }
        case WithdrawResult::ATMError:
            keypad->setErrorLabelText(
                "Przepraszamy, bankomat nie posiada odpowiednich banknotów, aby wydać tę kwotę.\n");
            break;
        case WithdrawResult::InsufficientFunds:
            keypad->setErrorLabelText("Nie ma wystarczajacych srodkow na koncie.\n");
            keypad->clearInputText();
            break;
        case WithdrawResult::DailyLimitExceeded:
            keypad->setErrorLabelText("Przekroczono dzienny limit wypłat.\n");
            keypad->clearInputText();
            break;
        case WithdrawResult::MonthlyLimitExceeded:
            keypad->setErrorLabelText("Przekroczono miesieczny limit wypłat.\n");
            keypad->clearInputText();
            break;
        case WithdrawResult::CardLimitExceeded:
            keypad->setErrorLabelText("Przekroczono limit jednorazowej wypłaty karty.\n");
            keypad->clearInputText();
            break;
        case WithdrawResult::AmountNotDivisible:
            keypad->setErrorLabelText("Nie można wypłacić kwoty nie dzielącej się przez 10.\n");
            keypad->clearInputText();
            break;
        case WithdrawResult::AmountTooLow:
            keypad->setErrorLabelText("FAIL: Nie mozna wyplacic kwoty mniejszej od 50zl.\n");
            keypad->clearInputText();
            break;
        case WithdrawResult::AuthError:
            keypad->setErrorLabelText("Wystapil blad autoryzacji, zaloguj sie ponownie");
            this->close();
            keypad->clearInputText();
            break;
        default:
            QMessageBox::critical(this, "Wystapil problem", "");
            break;
    }
}
