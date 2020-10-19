#ifndef FIAS_H
#define FIAS_H

#include <QtCore>
#include <QtNetwork>
#include <QtWidgets>

const char STX = 0x02;
const char ETX = 0x03;

class FIAS : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *tcpSocket;
    QMap<QString, QString> events;
    QString formatMessage(QString);
    QList<QString> linkRecords;
private slots:
    void onSocketReadyRead();
    void onSocketConnected();
    void onSocketDisconnected();

public:
    FIAS();
    ~FIAS();

    void connectToHost(QString, short);
    void disconnectFromHost();
    void sendMessage(QString);
    QString getMessage(QString);
signals:
    void addToLog(QString logMessage);
    void socketConnected();
    void socketDisconnected();
};

#endif // FIAS_H