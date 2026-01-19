#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>

class KeypadWidget : public QWidget {
    Q_OBJECT

public:
    explicit KeypadWidget(QWidget *parent = nullptr);
    ~KeypadWidget() override;
    void setSideButtonText(const QString &text, int index);
    void setEncrypted(bool encrypted);
    void clearSideButtons();
    QString getInputText() const;
    void setInputText(const QString &text);
    void clearInputText();
    void setScreenText(const QString &text);
    void setCustomScreen(QWidget *widget);
    void showDefaultScreen();
    void setErrorLabelText(const QString &text);
    signals:
        void digitClicked(const QString &digit);
    void confirmClicked();
    void backClicked();
    void sideButtonClicked(int index);

private:
    QVBoxLayout *mainLayout;
    QStackedWidget *monitorStack;
    QWidget *defaultScreenWidget;
    QLabel *errorLabel;
    QLabel *infoLabel;
    QLineEdit *inputDisplay;

    QVector<QPushButton*> sideButtons;
    QGridLayout *keypadLayout;
    QVector<QPushButton *> keypad;
    QPushButton *confirmButton;
    QPushButton *backButton;
};