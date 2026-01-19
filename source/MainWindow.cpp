#include "../headers/MainWindow.h"
#include <iostream>
#include <QMessageBox>
#include <QTimer>

#include "../headers/AccountWindow.h"
#include "../headers/KeypadWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    keypad=nullptr;
    bankSystem = new BankSystem();
    mainLayout = new QVBoxLayout();
    //
    cardSelectionContainer = new QWidget(this);
    QVBoxLayout *helperLayout=new QVBoxLayout(cardSelectionContainer);

    cardLabel = new QLabel("Wybierz karte z listy");
    cardLabel->setAlignment(Qt::AlignCenter);
    cardSelector=new QComboBox(this);
    cardSelector->addItem("Wybierz karte");
    cardLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    cardSelector->setStyleSheet("font-size: 18px; padding: 5px;");
    cards=bankSystem->getCardsNumber();
    for (auto card:cards) {
        cardSelector->addItem(QString::number(card));
    }
    putCard=new QPushButton("Wloz karte");
    helperLayout->addWidget(cardLabel);
    helperLayout->addWidget(cardSelector);
    helperLayout->addWidget(putCard);
    connect(putCard, &QPushButton::clicked, this, &MainWindow::cardSelected);
    mainLayout->addWidget(cardSelectionContainer);
    setLayout(mainLayout);
    resize(1200, 800);
    setWindowTitle("Bankomat");
}


MainWindow::~MainWindow() {
    delete bankSystem;
}
void MainWindow::cardSelected() {
    mainLayout->removeWidget(cardSelector);
    int cardNumber = cardSelector->currentText().toInt();
    LoginResult rl=bankSystem->enterCard(cardNumber);
    switch (rl) {
        case LoginResult::Success:
            cardSelectionContainer->hide();
            pinEnter();
            break;
        case LoginResult::InvalidCardNumber:
            cardLabel->setText("Wystapil blad, bledny numer karty");
            QTimer::singleShot(1000, this, [this]() {
                cardLabel->setText("Wybierz karte z listy");
                });
            break;
        case LoginResult::Blocked:
            cardLabel->setText("Konto zablokowane, skontaktuj sie z bankiem");
            QTimer::singleShot(1000, this, [this]() {
                cardLabel->setText("Wybierz karte z listy");
                });
            break;
        default:
            break;
    }
}
void MainWindow::pinEnter() {
    if (keypad==nullptr) {
        keypad = new KeypadWidget(this);
        mainLayout->addWidget(keypad);
        connect(keypad, &KeypadWidget::confirmClicked, this, &MainWindow::confirmClicked);
    }
    keypad->setScreenText("Podaj PIN");
    keypad->setEncrypted(true);
    keypad->show();
    keypad->clearInputText();
    keypad->clearInputText();
}

void MainWindow::confirmClicked() {
    int pin = keypad->getInputText().toInt();
    LoginResult result = bankSystem->enterPin(pin);
    switch (result) {
        case LoginResult::Success: {
            keypad->setErrorLabelText("Zostales zalogowany");
            QTimer::singleShot(1000,this,[this]() {
                AccountWindow *accountWindow = new AccountWindow(bankSystem, this);
                accountWindow->show();
                this->hide();
                clear();
            });
            break;
        }
        case LoginResult::InvalidPin:
            keypad->clearInputText();
            keypad->setErrorLabelText("Podano bledny pin, po 3 nieudanych probach konto zostanie zablokowane");
            break;
        case LoginResult::Blocked:
                        QTimer::singleShot(1000,this,[this]() {
                clear();
            });
            keypad->setErrorLabelText("Podales bledny pin 3 razy, konto zostanie zablokowane");
            break;
        case LoginResult::AccountError:
            keypad->setErrorLabelText("Wpisz ponownie numer karty");
            keypad->clearInputText();
        default:
            break;
    }
}


void MainWindow::clear() {
    keypad->clearInputText();
    cardSelector->setCurrentIndex(0);
    cardSelectionContainer->show();
    if (keypad!=nullptr) {
        keypad->clearInputText();
        keypad->hide();
    }
}
