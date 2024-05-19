#include <QCoreApplication>
#include "appcore.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  AppCore appCore;
  if ( !appCore.appStart() ) { return 1; }

  return a.exec();
}
