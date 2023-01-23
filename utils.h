#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <QString>
#include <Lmcons.h>
#include <QNetworkInterface>
#include <QCryptographicHash>

#include "qaesencryption.h"

QString getUsername();
QString getMac();

QByteArray encryptData(QString strToEncrypt, QString key);
QByteArray encryptData(QByteArray strToEncrypt, QString key);
QByteArray decryptData(QByteArray strToEncrypt, QString key);

QString getDataHash(QByteArray data);

#endif // UTILS_H
