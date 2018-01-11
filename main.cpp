#include <QCoreApplication>
#include "appcore.h"
#include <QDateTime>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    AppCore * appCore = new AppCore(0);
    appCore->appStart();

    return a.exec();
}
