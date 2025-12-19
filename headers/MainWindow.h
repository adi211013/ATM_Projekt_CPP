#pragma once
#include <QVBoxLayout>
#include <QVector>
#include <QLineEdit>
#include <QLabel>
#include "BankSystem.h"
class QPushButton;
class QVBoxLayout;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    QVBoxLayout *mainLayout;
    QLabel *cardLabel, *pinLabel;
    QLineEdit *cardDisplay, *pinDisplay;
    BankSystem *bankSystem;
    bool cardOk;

    void clear();

private slots:
    void confirmClicked();
};
