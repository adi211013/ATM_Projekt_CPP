//
// Created by adi on 11/15/25.
//

#include "../headers/KeypadWidget.h"

KeypadWidget::KeypadWidget(QWidget *parent): QWidget(parent) {
    keypad.reserve(10);
    for (int i = 0; i < 10; ++i) {
        QString buttonText = QString::number(i);
        QPushButton *button = new QPushButton(buttonText, this);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setStyleSheet("font-size: 20px; font-weight: bold;");
        keypad.append(button);
        connect(button, &QPushButton::clicked, this, [this, buttonText]() {
            emit digitClicked(buttonText);
        });
    }
    backButton = new QPushButton("Del", this);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    backButton->setStyleSheet("font-size: 16px; background-color: #FFAAAA;");

    confirmButton = new QPushButton("OK", this);
    confirmButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    confirmButton->setStyleSheet("font-size: 16px; background-color: #AAFFAA;");
    keypadLayout = new QGridLayout(this);
    //123
    keypadLayout->addWidget(keypad[1], 0, 0);
    keypadLayout->addWidget(keypad[2], 0, 1);
    keypadLayout->addWidget(keypad[3], 0, 2);
    //456
    keypadLayout->addWidget(keypad[4], 1, 0);
    keypadLayout->addWidget(keypad[5], 1, 1);
    keypadLayout->addWidget(keypad[6], 1, 2);
    //789
    keypadLayout->addWidget(keypad[7], 2, 0);
    keypadLayout->addWidget(keypad[8], 2, 1);
    keypadLayout->addWidget(keypad[9], 2, 2);
    //ok0back
    keypadLayout->addWidget(confirmButton, 3, 0);
    keypadLayout->addWidget(keypad[0], 3, 1);
    keypadLayout->addWidget(backButton, 3, 2);
    connect(backButton, &QPushButton::clicked, this, &KeypadWidget::backClicked);
    connect(confirmButton, &QPushButton::clicked, this, &KeypadWidget::confirmClicked);
    for (int i = 0; i < 4; ++i) keypadLayout->setRowStretch(i, 1);
    for (int i = 0; i < 3; ++i) keypadLayout->setColumnStretch(i, 1);
}

KeypadWidget::~KeypadWidget() {
}
