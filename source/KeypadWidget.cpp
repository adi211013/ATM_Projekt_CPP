#include "../headers/KeypadWidget.h"
#include <QVBoxLayout>
#include <QTimer>
KeypadWidget::KeypadWidget(QWidget *parent): QWidget(parent) {
    mainLayout = new QVBoxLayout(this);
    QWidget *topSection = new QWidget(this);
    QGridLayout *screenGrid = new QGridLayout(topSection);
    monitorStack = new QStackedWidget(this);
    defaultScreenWidget = new QWidget(this);
    defaultScreenWidget->setStyleSheet("background-color: #333; border: 3px solid #555");
    QVBoxLayout *monitorLayout = new QVBoxLayout(defaultScreenWidget);
    errorLabel = new QLabel("error", defaultScreenWidget);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setStyleSheet("color: #00FF00; font-size: 18px; font-weight: bold; border: none;");
    infoLabel = new QLabel("BANKOMAT", defaultScreenWidget);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("color: #00FF00; font-size: 18px; font-weight: bold; border: none;");
    monitorLayout->addWidget(errorLabel);
    errorLabel->hide();
    inputDisplay = new QLineEdit(defaultScreenWidget);
    inputDisplay->setReadOnly(true);
    inputDisplay->setAlignment(Qt::AlignCenter);
    inputDisplay->setStyleSheet(
        "background-color: #222; color: white; font-size: 24px; border: 1px solid #00FF00; padding: 5px;");
    monitorLayout->addWidget(infoLabel);
    monitorLayout->addWidget(inputDisplay);
    monitorStack->addWidget(defaultScreenWidget);
    screenGrid->addWidget(monitorStack, 0, 1, 4, 1);
    sideButtons.reserve(8);
    for (int i = 0; i < 8; i++) {
        QPushButton *btn = new QPushButton("", this);
        btn->setFixedSize(120, 50);
        btn->setStyleSheet("background-color: #CCC; border: 2px solid #888; border-radius: 4px; font-weight: bold;");
        sideButtons.push_back(btn);
        connect(btn, &QPushButton::clicked, [this, i]() {
            emit sideButtonClicked(i);
        });
    }
    for (int i = 0; i < 4; i++) screenGrid->addWidget(sideButtons[i], i, 0);
    for (int i = 0; i < 4; i++) screenGrid->addWidget(sideButtons[i + 4], i, 2);
    mainLayout->addWidget(topSection, 2);
    QWidget *bottomSection = new QWidget(this);
    keypadLayout = new QGridLayout(bottomSection);
    keypad.reserve(10);
    for (int i = 0; i < 10; ++i) {
        QString buttonText = QString::number(i);
        QPushButton *button = new QPushButton(buttonText, this);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setStyleSheet("font-size: 20px; font-weight: bold;");
        keypad.append(button);
        connect(button, &QPushButton::clicked, this, [this, buttonText]() {
            inputDisplay->setText(inputDisplay->text() + buttonText); // <-- TO JEST WAŻNE
            emit digitClicked(buttonText);
        });
    }
    backButton = new QPushButton("Del", this);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    backButton->setStyleSheet("font-size: 16px; background-color: #FFAAAA;");
    connect(backButton, &QPushButton::clicked, this, [this]() {
        inputDisplay->backspace();
        emit backClicked();
    });

    confirmButton = new QPushButton("OK", this);
    confirmButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    confirmButton->setStyleSheet("font-size: 16px; background-color: #AAFFAA;");
    connect(confirmButton, &QPushButton::clicked, this, &KeypadWidget::confirmClicked);
    keypadLayout->addWidget(keypad[1], 0, 0);
    keypadLayout->addWidget(keypad[2], 0, 1);
    keypadLayout->addWidget(keypad[3], 0, 2);
    keypadLayout->addWidget(keypad[4], 1, 0);
    keypadLayout->addWidget(keypad[5], 1, 1);
    keypadLayout->addWidget(keypad[6], 1, 2);
    keypadLayout->addWidget(keypad[7], 2, 0);
    keypadLayout->addWidget(keypad[8], 2, 1);
    keypadLayout->addWidget(keypad[9], 2, 2);
    keypadLayout->addWidget(confirmButton, 3, 0);
    keypadLayout->addWidget(keypad[0], 3, 1);
    keypadLayout->addWidget(backButton, 3, 2);
    for (int i = 0; i < 4; ++i) keypadLayout->setRowStretch(i, 1);
    for (int i = 0; i < 3; ++i) keypadLayout->setColumnStretch(i, 1);
    mainLayout->addWidget(bottomSection, 1);
}

KeypadWidget::~KeypadWidget() {
}
void KeypadWidget::setErrorLabelText(const QString &text) {
    errorLabel->setText(text);
    errorLabel->show();
    this->setEnabled(false);
    QTimer::singleShot(3000, this, [this]() {
        this->setEnabled(true);
        errorLabel->hide();
    });
}

void KeypadWidget::setSideButtonText(const QString &text, int index) {
    if (index >= 0 && index < sideButtons.size()) {
        sideButtons[index]->setText(text);
        sideButtons[index]->setVisible(!text.isEmpty());
    }
}

void KeypadWidget::setEncrypted(bool encrypted) {
    if (encrypted) inputDisplay->setEchoMode(QLineEdit::Password);
    else inputDisplay->setEchoMode(QLineEdit::Normal);
}

void KeypadWidget::clearSideButtons() {
    for (auto b: sideButtons) {
        b->setText("");
    }
}

QString KeypadWidget::getInputText() const {
    return inputDisplay->text();
}

void KeypadWidget::clearInputText() {
    inputDisplay->clear();
}

void KeypadWidget::setScreenText(const QString &text) {
    infoLabel->setText(text);
}
void KeypadWidget::setInputText(const QString &text) {
    inputDisplay->setText(text);
}
void KeypadWidget::setCustomScreen(QWidget *widget) {
    int index = monitorStack->addWidget(widget);
    monitorStack->setCurrentIndex(index);
}

void KeypadWidget::showDefaultScreen() {
    monitorStack->setCurrentIndex(0);
}
