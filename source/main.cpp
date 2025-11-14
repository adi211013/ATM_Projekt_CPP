#include <QApplication>
#include <QPushButton>
#include "../headers/MainWindow.h"
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();
}