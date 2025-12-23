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

    infoLabel = new QLabel("Wybierz kwote do wypłaty:", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-size: 22px;");

    amountDisplay = new QLineEdit(this);
    amountDisplay->setReadOnly(true);
    amountDisplay->setAlignment(Qt::AlignCenter);
    amountDisplay->setStyleSheet("font-size: 30px; padding: 10px;");
    amountDisplay->setPlaceholderText("0 PLN");
    //szybka wyplata
    //50zl
    button50 = new QPushButton(this);
    button50->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button50->setStyleSheet("font-size: 20px; font-weight: bold;");
    button50->setText("50zl");
    //100zl
    button100 = new QPushButton(this);
    button100->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button100->setStyleSheet("font-size: 20px; font-weight: bold;");
    button100->setText("100zl");
    //200zl
    button200 = new QPushButton(this);
    button200->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button200->setStyleSheet("font-size: 20px; font-weight: bold;");
    button200->setText("200zl");
    //500zl
    button500 = new QPushButton(this);
    button500->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button500->setStyleSheet("font-size: 20px; font-weight: bold;");
    button500->setText("500zl");
    //inna kwota
    otherButton = new QPushButton(this);
    otherButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    otherButton->setStyleSheet("font-size: 20px; font-weight: bold;");
    otherButton->setText("Inna kwota");
    fastPaymentContainer = new QWidget(this);
    fastPaymentLayout = new QGridLayout(fastPaymentContainer);
    fastPaymentLayout->addWidget(button50, 0, 0);
    fastPaymentLayout->addWidget(button100, 0, 1);
    fastPaymentLayout->addWidget(button200, 1, 0);
    fastPaymentLayout->addWidget(button500, 1, 1);
    fastPaymentLayout->addWidget(otherButton, 2, 0, 1, 2);
    //connect
    connect(button50, &QPushButton::clicked, this, [this]() {
        amountDisplay->setText("50");
        onWithdrawClicked();
    });
    connect(button100, &QPushButton::clicked, this, [this]() {
        amountDisplay->setText("100");
        onWithdrawClicked();
    });
    connect(button200, &QPushButton::clicked, this, [this]() {
        amountDisplay->setText("200");
        onWithdrawClicked();
    });
    connect(button500, &QPushButton::clicked, this, [this]() {
        amountDisplay->setText("500");
        onWithdrawClicked();
    });
    connect(otherButton, &QPushButton::clicked, this, [this]() {
        fastPaymentContainer->hide();
        infoLabel->setText("Wpisz kwote do wypłaty:");
        keypad = new KeypadWidget(this);
        mainLayout->addWidget(keypad);
        amountDisplay->show();
        mainLayout->setStretch(0, 0);
        mainLayout->setStretch(1, 0);
        mainLayout->setStretch(2, 1);
        mainLayout->setStretch(3, 2);

        connect(keypad, &KeypadWidget::digitClicked, this, &AccountWindow::onDigitClicked);
        connect(keypad, &KeypadWidget::backClicked, this, &AccountWindow::onBackspaceClicked);
        connect(keypad, &KeypadWidget::confirmClicked, this, &AccountWindow::onWithdrawClicked);
    });

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(balanceLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(amountDisplay);
    amountDisplay->hide();
    mainLayout->addWidget(fastPaymentContainer);
    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 0);
    mainLayout->setStretch(2, 1);
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
                if (count != 0)
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
            QMessageBox::critical(this, "Błąd sesji", "Wystapil blad autoryzacji, zaloguj sie ponownie");
            this->close();
            loginWindow->show();
            break;
        default:
            QMessageBox::critical(this, "Wystapil problem", "");
    }
}
