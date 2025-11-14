#pragma once
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QVector>
#include <QLineEdit>
#include <QLabel>
class QPushButton;
class QVBoxLayout;
class MainWindow : public QWidget {
    Q_OBJECT

    public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QVBoxLayout *mainLayout;
    QGridLayout *keypadLayout;
    QLabel *label;
    QLineEdit *display;
    QVector<QPushButton *> keypad;
    QPushButton *confirmButton;
    QPushButton *backButton;
private slots:
    void confirmClicked();
};