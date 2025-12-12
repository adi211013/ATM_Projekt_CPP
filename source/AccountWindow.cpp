//
// Created by adi on 11/15/25.
//

#include "../headers/AccountWindow.h"

#include <QMessageBox>

AccountWindow::AccountWindow(Account* account, QWidget* loginWindow, QWidget* parent)
    : QWidget(parent), account(account), loginWindow(loginWindow)
{
    atm=ATM::pullATM();

    setWindowTitle("Bankomat");
    resize(1200, 800);

    balanceLabel = new QLabel(this);
    balanceLabel->setAlignment(Qt::AlignCenter);
    balanceLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #4CAF50;");
    balanceLabel->setText("Dostępne środki: " + QString::number(account->getBalance()) + " PLN");

    infoLabel = new QLabel("Wpisz kwote do wypłaty:", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-size: 22px;");

    amountDisplay = new QLineEdit(this);
    amountDisplay->setReadOnly(true);
    amountDisplay->setAlignment(Qt::AlignCenter);
    amountDisplay->setStyleSheet("font-size: 30px; padding: 10px;");
    amountDisplay->setPlaceholderText("0 PLN");

    keypad = new KeypadWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(balanceLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(amountDisplay);
    mainLayout->addWidget(keypad);

    mainLayout->setStretch(0,0);
    mainLayout->setStretch(1,0);
    mainLayout->setStretch(2,1);
    mainLayout->setStretch(3,2);

    connect(keypad, &KeypadWidget::digitClicked, this, &AccountWindow::onDigitClicked);
    connect(keypad, &KeypadWidget::backClicked, this, &AccountWindow::onBackspaceClicked);
    connect(keypad, &KeypadWidget::confirmClicked, this, &AccountWindow::onWithdrawClicked);

}
AccountWindow::~AccountWindow() {}

void AccountWindow::onDigitClicked(const QString& digit) {
        amountDisplay->setText(amountDisplay->text() + digit);
}

void AccountWindow::onBackspaceClicked() {
    amountDisplay->backspace();
}

void AccountWindow::onWithdrawClicked() {
    QString sAmount = amountDisplay->text();
    if (sAmount.isEmpty()) return;
    int amount = sAmount.toInt();
    if (!account->canWithdraw(amount)) {
        QMessageBox::critical(this, "Błąd wypłaty",
            "Nie można wypłacić takiej kwoty.\n");
        amountDisplay->clear();
        return;
    }
    std::map<int, int> notesToGive;
    if (!atm.canPayOut(amount, notesToGive)) {
         QMessageBox::critical(this, "Błąd bankomatu",
            "Przepraszamy, bankomat nie posiada odpowiednich banknotów, aby wydać tę kwotę.\n");
         return;
    }
    account->recordWithdrawal(amount);
    atm.commitPayOut(notesToGive);
    std::vector<Account> allAccounts = Account::pullAccounts();
    for(auto &acc : allAccounts) {
        if(acc.getCardNumber() == account->getCardNumber()) {
            acc = *account;
            break;
        }
    }
    Account::pushAccounts(allAccounts);
    QString info = "Wypłacono: " + QString::number(amount) + " PLN\n\n";
    info += "Nominały:\n";
    for (auto const& [nominal, count] : notesToGive) {
        info += QString::number(nominal) + " PLN x " + QString::number(count) + "\n";
    }
    QMessageBox::information(this, "Gotówka", info);
    amountDisplay->clear();
    loginWindow->show();
    this->close();
}