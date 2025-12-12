#include "../headers/MainWindow.h"
#include <iostream>
#include <QMessageBox>

#include "../headers/AccountWindow.h"
#include "../headers/KeypadWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent){
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
    KeypadWidget* keypad=new KeypadWidget(this);
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
    connect(keypad, &KeypadWidget::digitClicked, this, [this](const QString& number) {
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
        bool foundCard = false;
        accounts=Account::pullAccounts();
        QString s_card=cardDisplay->text();
        int i_card=s_card.toInt();
        for (int i=0; i<accounts.size(); ++i) {
            if (accounts[i].getCardNumber()==i_card) {
                foundCard = true;
                index=i;
                break;
            }
        }
        if (!foundCard) {
            QMessageBox::critical(this,"Bledny numer karty","Podales bledny numer karty");
            cardDisplay->clear();
            return;
        }
        if (accounts[index].getBlocked()) {
            cardDisplay->clear();
            QMessageBox::critical(this,"Konto zablokowane","Konto zablokowane, skontaktuj sie z bankiem");
        }
        else {
            cardOk=true;
        }
    }
    else {
        QString s_pin=pinDisplay->text();
        int i_pin=s_pin.toInt();
        if (accounts[index].getPin()==i_pin) {
            QMessageBox::information(this,"Zalogowano","Zostales Zalogowany");
            accounts[index].checkAndResetLimits();
            AccountWindow* accountWindow=new AccountWindow(&accounts[index],this);
            accountWindow->show();
            this->hide();
            clear();

        }
        else if (counter==2) {
            accounts[index].block();
            QMessageBox::critical(this,"Konto zablokowane","Podales bledny pin 3 razy, konto zostanie zablokowane");
            Account::pushAccounts(accounts);
            cardOk=false;
            pinDisplay->clear();
            cardDisplay->clear();
        }
        else {
            counter++;
            pinDisplay->clear();
            QMessageBox::critical(this,"Bledny pin","Podano bledny pin, po 3 nieudanych probach konto zostanie zablokowane");

        }
    }
}
void MainWindow::clear() {
    cardOk=false;
    counter=0;
    index=0;
    cardDisplay->clear();
    pinDisplay->clear();
}
