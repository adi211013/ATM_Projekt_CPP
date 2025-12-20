//
// Created by adi on 11/15/25.
//

#include "../headers/AccountWindow.h"

#include <QMessageBox>

AccountWindow::AccountWindow(BankSystem *bankSystem, QWidget *loginWindow, QWidget *parent)
    : QWidget(parent), bankSystem(bankSystem), loginWindow(loginWindow) {
    setWindowTitle("Bankomat");
    resize(1200, 800);

    balanceLabel = new QLabel(this);
    balanceLabel->setAlignment(Qt::AlignCenter);
    balanceLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #4CAF50;");
    balanceLabel->setText("Dostępne środki: " + QString::number(bankSystem->getBalance()) + " PLN");

    infoLabel = new QLabel("Wpisz kwote do wypłaty:", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-size: 22px;");

    amountDisplay = new QLineEdit(this);
    amountDisplay->setReadOnly(true);
    amountDisplay->setAlignment(Qt::AlignCenter);
    amountDisplay->setStyleSheet("font-size: 30px; padding: 10px;");
    amountDisplay->setPlaceholderText("0 PLN");

    keypad = new KeypadWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(balanceLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(amountDisplay);
    mainLayout->addWidget(keypad);

    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 0);
    mainLayout->setStretch(2, 1);
    mainLayout->setStretch(3, 2);

    connect(keypad, &KeypadWidget::digitClicked, this, &AccountWindow::onDigitClicked);
    connect(keypad, &KeypadWidget::backClicked, this, &AccountWindow::onBackspaceClicked);
    connect(keypad, &KeypadWidget::confirmClicked, this, &AccountWindow::onWithdrawClicked);
}

AccountWindow::~AccountWindow() {
}

void AccountWindow::onDigitClicked(const QString &digit) {
    amountDisplay->setText(amountDisplay->text() + digit);
}

void AccountWindow::onBackspaceClicked() {
    amountDisplay->backspace();
}

void AccountWindow::onWithdrawClicked() {
    int amount = amountDisplay->text().toInt();
    std::map<int, int> notesToGive;
    WithdrawResult result = bankSystem->withdraw(amount, notesToGive);
    switch (result) {
        case WithdrawResult::Success: {
            QString info = "Wypłacono: " + QString::number(amount) + " PLN\n\n";
            info += "Nominały:\n";
            for (auto const &[nominal, count]: notesToGive) {
                if (count !=0)
                    info += QString::number(nominal) + " PLN x " + QString::number(count) + "\n";
            }
            QMessageBox::information(this, "Gotówka", info);
            amountDisplay->clear();
            loginWindow->show();
            this->close();
            break;
        }
        case WithdrawResult::ATMError:
            QMessageBox::critical(this, "Błąd bankomatu",
                              "Przepraszamy, bankomat nie posiada odpowiednich banknotów, aby wydać tę kwotę.\n");
            break;
        case WithdrawResult::InvalidAmount:
            QMessageBox::critical(this, "Błąd wypłaty",
                      "Nie można wypłacić takiej kwoty.\n");
            amountDisplay->clear();
            break;
        case WithdrawResult::AuthError:
            QMessageBox::critical(this, "Błąd sesji","Wystapil blad autoryzacji, zaloguj sie ponownie");
            this->close();
            loginWindow->show();
            break;
        default:
                QMessageBox::critical(this, "Wystapil problem", "");
    }
}
