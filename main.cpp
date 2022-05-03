#include <QCoreApplication>
#include "appcore.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  AppCore appCore;
  appCore.appStart();

  return a.exec();
}
