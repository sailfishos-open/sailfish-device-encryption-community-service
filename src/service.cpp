#include "service.h"

#include <QDBusConnection>
#include <devenc/devicelist.h>

//#warning "SWAP THE BUS"
//#define BUS QDBusConnection::sessionBus()
#define BUS QDBusConnection::systemBus()

// used to check if device was found
#define DEVCHECK(d, ret) \
  if (d == nullptr) \
    { \
      QDBusMessage error = message.createErrorReply(QDBusError::InvalidArgs, "Could not find device"); \
      BUS.send(error); \
      return ret; \
    } \


Service::Service(QObject *parent) : QObject(parent)
{
}

QStringList Service::Devices()
{
  return DevEnc::DeviceList::instance()->devices();
}

QString Service::DeviceName(QString id, const QDBusMessage &message)
{
  DevEnc::Device *d = DevEnc::DeviceList::instance()->device(id);
  DEVCHECK(d, QString());
  return d->name();
}

bool Service::DeviceEncrypted(QString id, const QDBusMessage &message)
{
  DevEnc::Device *d = DevEnc::DeviceList::instance()->device(id);
  DEVCHECK(d, false);
  return d->encrypted();
}

QString Service::RecoveryPassword(QString id, const QDBusMessage &message)
{
  DevEnc::Device *d = DevEnc::DeviceList::instance()->device(id);
  DEVCHECK(d, QString());
  return d->getRecoveryPassword();
}

bool Service::RemoveRecoveryPasswordCopy(QString id, const QDBusMessage &message)
{
  DevEnc::Device *d = DevEnc::DeviceList::instance()->device(id);
  DEVCHECK(d, false);
  return d->removeRecoveryPasswordCopy();
}
