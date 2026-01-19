#pragma once
#include <QVBoxLayout>
#include <QVector>
#include <QLineEdit>
#include <QLabel>
#include "BankSystem.h"
#include <QComboBox>

#include "AccountWindow.h"
class QPushButton;
class QVBoxLayout;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    QComboBox *cardSelector;
    QVBoxLayout *mainLayout;
    QWidget *cardSelectionContainer;
    QLabel *cardLabel;
    BankSystem *bankSystem;
    std::vector<int> cards;
    QPushButton *putCard;
    KeypadWidget *keypad;
    void pinEnter();
    void clear();

private slots:
    void confirmClicked();
    void cardSelected();
};
