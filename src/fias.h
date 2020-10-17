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
    QTextEdit *logs;
    QMap<QString, QString> events;
    QString formatMessage(QString);
private slots:
    void onSocketReadyRead();
    void onSocketConnected();
    void onSocketDisconnected();
public:
    FIAS(QTextEdit *logsObject = nullptr);
    ~FIAS();

    void connectToHost(QString, short);
    void disconnectFromHost();
    void sendMessage(QString);
    QString getMessage(QString);
};

#endif // FIAS_H