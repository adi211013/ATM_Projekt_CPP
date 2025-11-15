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
    QLabel *cardLabel,*pinLabel;
    QLineEdit *cardDisplay,*pinDisplay;
    std::vector<Account> accounts;
    int counter=0;
    int index=0;
    void clear();
private slots:
    void confirmClicked();
};