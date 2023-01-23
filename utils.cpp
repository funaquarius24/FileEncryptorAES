#include "utils.h"
#include <QFile>



QString getUsername()
{
    wchar_t username[UNLEN+1];
    DWORD username_len = UNLEN+1;
    GetUserName((TCHAR*)username, &username_len);

    return QString::fromWCharArray(username);
}

QString getMac()
{

#ifdef Q_WS_MAC
intf = QNetworkInterface::interfaceFromName(QString("en0"));
#endif

#ifdef Q_WS_WIN
intf = QNetworkInterface::interfaceFromIndex(1);
#endif

    QNetworkInterface iface;
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();

    QString mac_address = "0";

    for (int i = 0; i < ifaces.count(); ++i)
        {
            iface = ifaces.at(i);
    //            std::cout << "ifaces: " << iface.humanReadableName().toStdString() << i << std::endl;
            if( iface.humanReadableName() == "Ethernet" ){
    //                std::cout << "ifaces: the one " << iface.isValid() << std::endl;
                mac_address = iface.hardwareAddress();
            }
        }

    return mac_address;

}

QByteArray encryptData(QString strToEncrypt, QString key = nullptr)
{
    QByteArray keyHash;

    if(key == nullptr)
        keyHash = QCryptographicHash::hash(getMac().toLocal8Bit(), QCryptographicHash::Sha256);
    else
        keyHash = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CFB);

    QByteArray iv;

    quint8 iv_16[16]     = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    for (int i=0; i<16; i++)
       iv.append(iv_16[i]);

    QByteArray encodedText = encryption.encode(strToEncrypt.toLocal8Bit(), keyHash, iv);

    return encodedText;

}

QByteArray encryptData(QByteArray strToEncrypt, QString key = nullptr)
{
    QByteArray keyHash;

    if(key == nullptr)
        keyHash = QCryptographicHash::hash(getMac().toLocal8Bit(), QCryptographicHash::Sha256);
    else
        keyHash = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CFB);

    QByteArray iv;

    quint8 iv_16[16]     = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    for (int i=0; i<16; i++)
       iv.append(iv_16[i]);

    QByteArray encodedText = encryption.encode(strToEncrypt, keyHash, iv);

    return encodedText;

}

QByteArray decryptData(QByteArray strToDecrypt, QString key = nullptr)
{
    QByteArray keyHash;

    if(key == nullptr)
        keyHash = QCryptographicHash::hash(getMac().toLocal8Bit(), QCryptographicHash::Sha256);
    else
        keyHash = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CFB);

    QByteArray iv;

    quint8 iv_16[16]     = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    for (int i=0; i<16; i++)
       iv.append(iv_16[i]);

    QByteArray decodedText = encryption.removePadding(encryption.decode(strToDecrypt, keyHash, iv));

    return decodedText;

}

QString getDataHash(QByteArray data)
{

    QByteArray dataHash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);

    return dataHash.toHex().toLower();
}
