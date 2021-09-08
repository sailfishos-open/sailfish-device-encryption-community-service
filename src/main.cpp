#include <QCoreApplication>
#include <QtDBus/QtDBus>

#include <signal.h>
#include <iostream>

#include "service.h"

//#warning "SWAP THE BUS"
//#define BUS QDBusConnection::sessionBus()
#define BUS QDBusConnection::systemBus()

int main(int argc, char **argv)
{
  QCoreApplication app(argc, argv);

  if (!BUS.isConnected()) {
      std::cerr << "Cannot connect to the D-Bus session bus.\n";
      return 1;
    }

  if (!BUS.registerService(SERVICE_NAME)) {
      std::cerr << qPrintable(QDBusConnection::systemBus().lastError().message()) << "\n";
      return 2;
    }

  Service service;
  BUS.registerObject("/", &service, QDBusConnection::ExportAllSlots);

  // register signal handlers
  signal(SIGTERM, [](int /*sig*/){ qInfo("Quitting on SIGTERM"); qApp->quit(); });
  signal(SIGINT, [](int /*sig*/){ qInfo("Quitting on SIGINT"); qApp->quit(); });
  signal(SIGHUP, [](int /*sig*/){ qInfo("Quitting on SIGHUP"); qApp->quit(); });

  return app.exec();
}
