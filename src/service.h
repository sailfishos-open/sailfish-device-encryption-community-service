#ifndef SERVICE_H
#define SERVICE_H

#include <QDBusMessage>
#include <QMap>
#include <QObject>
#include <QStringList>

#include <devenc/password.h>

class Service : public QObject
{
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "device.encryption.Service")

public:
  explicit Service(QObject *parent = nullptr);

public slots:

  // general properties
  QStringList Devices();
  QStringList PasswordTypes();

  // device parameters
  QString DeviceName(QString id, const QDBusMessage &message);
  bool DeviceEncrypted(QString id, const QDBusMessage &message);

  // recovery password
  QString RecoveryPassword(QString id, const QDBusMessage &message);
  bool RemoveRecoveryPasswordCopy(QString id, const QDBusMessage &message);

  // state of password slots
  int FreePasswordSlots(QString id, const QDBusMessage &message);
  int UsedPasswordSlots(QString id, const QDBusMessage &message);

  // password manipulation
  bool AddPassword(QString id,
                   QString controlPassword, QString controlType,
                   QString newPassword, QString newType,
                   const QDBusMessage &message);

signals:

private:
  DevEnc::Password *makePassword(QString description);

private:
  QMap<QString, QString> m_password_types;
};

#endif // SERVICE_H
