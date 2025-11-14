#include "../headers/MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    label = new QLabel("Podaj numer karty",this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size:24px;");
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignCenter);
    display->setStyleSheet("font-size:24px;");
    keypad.reserve(10);
    for (int i = 0; i < 10; ++i) {
        QString buttonText = QString::number(i);
        QPushButton* button = new QPushButton(buttonText, this);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setStyleSheet("font-size: 20px; font-weight: bold;");
        keypad.append(button);

        connect(button, &QPushButton::clicked, this, [this, button]() {
            display->setText(display->text() + button->text());
        });
    }
    backButton = new QPushButton("Del", this);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    backButton->setStyleSheet("font-size: 16px; background-color: #FFAAAA;");

    confirmButton = new QPushButton("OK", this);
    confirmButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    confirmButton->setStyleSheet("font-size: 16px; background-color: #AAFFAA;");
    keypadLayout= new QGridLayout();
    //123
    keypadLayout->addWidget(keypad[1],0,0);
    keypadLayout->addWidget(keypad[2],0,1);
    keypadLayout->addWidget(keypad[3],0,2);
    //456
    keypadLayout->addWidget(keypad[4],1,0);
    keypadLayout->addWidget(keypad[5],1,1);
    keypadLayout->addWidget(keypad[6],1,2);
    //789
    keypadLayout->addWidget(keypad[7],2,0);
    keypadLayout->addWidget(keypad[8],2,1);
    keypadLayout->addWidget(keypad[9],2,2);
    //ok0back
    keypadLayout->addWidget(confirmButton,3,0);
    keypadLayout->addWidget(keypad[0],3,1);
    keypadLayout->addWidget(backButton,3,2);
    mainLayout= new QVBoxLayout();
    mainLayout->addWidget(label);
    mainLayout->addWidget(display);
    mainLayout->addLayout(keypadLayout);
    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 0);
    mainLayout->setStretch(2, 1);
    connect(backButton, &QPushButton::clicked, this, [this]() {
        display->backspace();
    });
    connect(confirmButton, &QPushButton::clicked, this, &MainWindow::confirmClicked);
    setLayout(mainLayout);
    resize(1200, 800);
    setWindowTitle("Bankomat");
}
MainWindow::~MainWindow() {
}
void MainWindow::confirmClicked() {
    QString cardNumber = display->text();
    qDebug() << "Zatwierdzono: " << cardNumber;
    display->clear();
}