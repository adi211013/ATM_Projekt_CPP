//
// Created by adi on 11/15/25.
//

#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QVector>


class KeypadWidget : public QWidget {
    Q_OBJECT

public:
    explicit KeypadWidget(QWidget *parent = nullptr);

    ~KeypadWidget() override;

signals:
    void digitClicked(const QString &digit);

    void confirmClicked();

    void backClicked();

private:
    QGridLayout *keypadLayout;
    QVector<QPushButton *> keypad;
    QPushButton *confirmButton;
    QPushButton *backButton;
};
