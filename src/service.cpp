#include "service.h"

#include <QDBusConnection>
#include <QSharedPointer>

#include <devenc/devicelist.h>
#include <devenc/passwordmaker.h>

//#warning "SWAP THE BUS"
//#define BUS QDBusConnection::sessionBus()
#define BUS QDBusConnection::systemBus()

// used to check if device was found
#define DEVCHECK(d, ret) \
  if (d == nullptr) \
{ \
  QDBusMessage error = message.createErrorReply(QDBusError::InvalidArgs, tr("Could not find device")); \
  BUS.send(error); \
  return ret; \
  } \


Service::Service(QObject *parent) : QObject(parent)
{
  // it is expected that description is unique for each password type
  for (QString i: DevEnc::PasswordMaker::instance()->types())
    m_password_types[DevEnc::PasswordMaker::instance()->description(i)] = i;
}

QStringList Service::Devices()
{
  return DevEnc::DeviceList::instance()->devices();
}

QStringList Service::PasswordTypes()
{
  QStringList r;
  // keep the same order as a matter of preference
  for (QString i: DevEnc::PasswordMaker::instance()->types())
    r.append(DevEnc::PasswordMaker::instance()->description(i));
  return r;
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

int Service::FreePasswordSlots(QString id, const QDBusMessage &message)
{
  DevEnc::Device *d = DevEnc::DeviceList::instance()->device(id);
  DEVCHECK(d, 0);
  return d->freePasswordSlots();
}

int Service::UsedPasswordSlots(QString id, const QDBusMessage &message)
{
  DevEnc::Device *d = DevEnc::DeviceList::instance()->device(id);
  DEVCHECK(d, 0);
  return d->usedPasswordSlots();
}

DevEnc::Password *Service::makePassword(QString description)
{
  QString type = m_password_types.value(description, QString());
  return DevEnc::PasswordMaker::instance()->newPassword(type);
}

bool Service::AddPassword(QString id,
                          QString controlPassword, QString controlType,
                          QString newPassword, QString newType,
                          const QDBusMessage &message)
{
  DevEnc::Device *d = DevEnc::DeviceList::instance()->device(id);
  DEVCHECK(d, false);

  QSharedPointer<DevEnc::Password> controlPwd(makePassword(controlType));
  QSharedPointer<DevEnc::Password> newPwd(makePassword(newType));

  if (!controlPwd || !newPwd)
    {
      QDBusMessage error = message.createErrorReply(QDBusError::InvalidArgs, tr("Unknown password type"));
      BUS.send(error);
      return false;
    }

  controlPwd->setPassword(controlPassword);
  newPwd->setPassword(newPassword);

  if (!d->addPassword(controlPwd.data(), newPwd.data()))
    {
      QDBusMessage error = message.createErrorReply(QDBusError::InvalidArgs, tr("Failed to add new password. Check that the control password is correct."));
      BUS.send(error);
      return false;
    }

  return true;
}

bool Service::RemovePassword(QString id,
                             QString controlPassword, QString controlType,
                             QString removedPassword, QString removedType,
                             const QDBusMessage &message)
{
  DevEnc::Device *d = DevEnc::DeviceList::instance()->device(id);
  DEVCHECK(d, false);

  QSharedPointer<DevEnc::Password> controlPwd(makePassword(controlType));
  QSharedPointer<DevEnc::Password> removedPwd(makePassword(removedType));

  if (!controlPwd || !removedPwd)
    {
      QDBusMessage error = message.createErrorReply(QDBusError::InvalidArgs, tr("Unknown password type"));
      BUS.send(error);
      return false;
    }

  controlPwd->setPassword(controlPassword);
  removedPwd->setPassword(removedPassword);

  if (!d->removePassword(controlPwd.data(), removedPwd.data()))
    {
      QDBusMessage error = message.createErrorReply(QDBusError::InvalidArgs, tr("Failed to remove password. Check that the control and removed passwords are correct."));
      BUS.send(error);
      return false;
    }

  return true;
}

bool Service::TestPassword(QString id,
                           QString controlPassword, QString controlType,
                           const QDBusMessage &message)
{
  DevEnc::Device *d = DevEnc::DeviceList::instance()->device(id);
  DEVCHECK(d, false);

  QSharedPointer<DevEnc::Password> controlPwd(makePassword(controlType));

  if (!controlPwd)
    {
      QDBusMessage error = message.createErrorReply(QDBusError::InvalidArgs, tr("Unknown password type"));
      BUS.send(error);
      return false;
    }

  controlPwd->setPassword(controlPassword);
  return d->testPassword(controlPwd.data());
}
