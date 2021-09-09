#ifndef SERVICE_H
#define SERVICE_H

#include <QDBusMessage>
#include <QObject>
#include <QStringList>

class Service : public QObject
{
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "device.encryption.Service")

public:
  explicit Service(QObject *parent = nullptr);

public slots:

  // list of devices
  QStringList Devices();

  // device parameters
  QString DeviceName(QString id, const QDBusMessage &message);
  bool DeviceEncrypted(QString id, const QDBusMessage &message);

  // recovery password
  QString RecoveryPassword(QString id, const QDBusMessage &message);
  bool RemoveRecoveryPasswordCopy(QString id, const QDBusMessage &message);

  int FreePasswordSlots(QString id, const QDBusMessage &message);
  int UsedPasswordSlots(QString id, const QDBusMessage &message);

signals:

};

#endif // SERVICE_H
