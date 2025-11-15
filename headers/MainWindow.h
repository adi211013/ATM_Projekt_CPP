#pragma once
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QVector>
#include <QLineEdit>
#include <QLabel>
#include "Account.h"
class QPushButton;
class QVBoxLayout;
class MainWindow : public QWidget {
    Q_OBJECT

    public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    bool cardOk = false;
    QVBoxLayout *mainLayout;
    QGridLayout *keypadLayout;
    QLabel *cardLabel,*pinLabel;
    QLineEdit *cardDisplay,*pinDisplay;
    QVector<QPushButton *> keypad;
    QPushButton *confirmButton;
    QPushButton *backButton;
    Account *account=nullptr;
    std::vector<Account> accounts;
private slots:
    void confirmClicked();
};