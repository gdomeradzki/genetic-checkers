#include <QApplication>

#include "FrontendController.hpp"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    FrontendController frontend;

    return a.exec();
}
