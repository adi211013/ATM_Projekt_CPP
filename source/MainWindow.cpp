#include "../headers/MainWindow.h"
#include <iostream>
#include <QMessageBox>

#include "../headers/AccountWindow.h"
#include "../headers/KeypadWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    bankSystem = new BankSystem();
    cardOk = false;
    //tworzenie labeli
    cardLabel = new QLabel("Podaj numer karty", this);
    cardLabel->setAlignment(Qt::AlignCenter);
    cardLabel->setStyleSheet("font-size:24px;");
    cardDisplay = new QLineEdit(this);
    cardDisplay->setReadOnly(true);
    cardDisplay->setAlignment(Qt::AlignCenter);
    cardDisplay->setStyleSheet("font-size:24px;");
    pinLabel = new QLabel("Podaj numer PIN", this);
    pinLabel->setAlignment(Qt::AlignCenter);
    pinLabel->setStyleSheet("font-size:24px;");
    pinDisplay = new QLineEdit(this);
    pinDisplay->setReadOnly(true);
    pinDisplay->setAlignment(Qt::AlignCenter);
    pinDisplay->setStyleSheet("font-size:24px;");
    //linkowanie layoutow
    KeypadWidget *keypad = new KeypadWidget(this);
    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(cardLabel);
    mainLayout->addWidget(cardDisplay);
    mainLayout->addWidget(pinLabel);
    mainLayout->addWidget(pinDisplay);
    mainLayout->addWidget(keypad);
    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 1);
    mainLayout->setStretch(2, 0);
    mainLayout->setStretch(3, 1);
    mainLayout->setStretch(4, 2);
    connect(keypad, &KeypadWidget::backClicked, this, [this]() {
        if (!cardOk)
            cardDisplay->backspace();
        else
            pinDisplay->backspace();
    });
    connect(keypad, &KeypadWidget::confirmClicked, this, &MainWindow::confirmClicked);
    connect(keypad, &KeypadWidget::digitClicked, this, [this](const QString &number) {
        if (!cardOk)
            cardDisplay->setText(cardDisplay->text() + number);
        else
            pinDisplay->setText(pinDisplay->text() + number);
    });
    setLayout(mainLayout);
    resize(1200, 800);
    setWindowTitle("Bankomat");
}

MainWindow::~MainWindow() {
}

void MainWindow::confirmClicked() {
    if (!cardOk) {
        int cardNumber = cardDisplay->text().toInt();
        LoginResult result = bankSystem->enterCard(cardNumber);
        switch (result) {
            case LoginResult::Success:
                cardOk = true;
                break;
            case LoginResult::InvalidCardNumber:
                QMessageBox::critical(this, "Bledny numer karty", "Podales bledny numer karty");
                cardDisplay->clear();
                break;
            case LoginResult::Blocked:
                cardDisplay->clear();
                QMessageBox::critical(this, "Konto zablokowane", "Konto zablokowane, skontaktuj sie z bankiem");
                break;
            default:
                cardDisplay->clear();
                QMessageBox::critical(this, "Wystapil problem", "");
        }
    }
    else {
        int pin = pinDisplay->text().toInt();
        LoginResult result = bankSystem->enterPin(pin);
        switch (result) {
            case LoginResult::Success: {
                QMessageBox::information(this, "Zalogowano", "Zostales Zalogowany");
                AccountWindow *accountWindow = new AccountWindow(bankSystem, this);
                accountWindow->show();
                this->hide();
                clear();
                break;
            }
            case LoginResult::InvalidPin:
                pinDisplay->clear();
                QMessageBox::critical(this, "Bledny pin","Podano bledny pin, po 3 nieudanych probach konto zostanie zablokowane");
                break;
            case LoginResult::Blocked:
                QMessageBox::critical(this, "Konto zablokowane", "Podales bledny pin 3 razy, konto zostanie zablokowane");
                clear();
                break;
            case LoginResult::AccountError:
                QMessageBox::critical(this, "Wystapil problem", "Wpisz ponownie numer karty");
                cardOk = false;
                pinDisplay->clear();
                cardDisplay->clear();
            default:
                QMessageBox::critical(this, "Wystapil problem", "");
        }
    }
}

void MainWindow::clear() {
    cardOk = false;
    cardDisplay->clear();
    pinDisplay->clear();
}
