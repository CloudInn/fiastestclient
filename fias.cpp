#include <QtNetwork>
#include "fias.h"

FIAS::FIAS(QTextEdit *logsObject)
{
    tcpSocket = new QTcpSocket(this);
    logs = logsObject;
    events.insert("---------------", "");
    events.insert("Posting inquery PR", "PR|WSHEY|P#12|G#111|GNAhmed Azab|PI107|PMROOM|DA%1|");
    events.insert("Posting request PR", "PR|WSHEY|P#12|G#111|GNAhmed Azab|RN107|PMROOM|DA%1|TA1500|");
    events.insert("Bill view XR", "XR|RN2781|G#12345|");
    events.insert("Remote Checkout XC", "XC|RN2781|G#12345|BA13850|DA%1|TI%2|");
    events.insert("Link record LR", "LR|RI|FL|");
    events.insert("Link end LE", "LE|DA%1|TI%2|");

    connect(tcpSocket, SIGNAL(readyRead()), SLOT(onSocketReadyRead()));
    connect(tcpSocket, SIGNAL(connected()), SLOT(onSocketConnected()));
    connect(tcpSocket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
}

FIAS::~FIAS()
{
    delete tcpSocket;
}

void FIAS::connectToHost(QString host, short port)
{
    const int Timeout = 2 * 1000;
    QString leMsg;

    if (tcpSocket->isOpen())
    {
        leMsg = "LE|DA201017|TI153651|";
        this->sendMessage(leMsg);
    }
    tcpSocket->abort();
    tcpSocket->connectToHost(host, port);
    logs->append("Attempting to connect to server ...");
    if (!tcpSocket->waitForConnected(Timeout))
    {
        logs->append(tcpSocket->errorString());
    }
}
void FIAS::disconnectFromHost()
{
    this->sendMessage(events["Link end LE"]);
    tcpSocket->disconnected();
}
void FIAS::onSocketReadyRead()
{
    QByteArray data = tcpSocket->readAll();
    QList<QByteArray> dataList = data.split(STX);
    QByteArray msg;
    foreach (msg, dataList)
    {
        msg.remove(msg.indexOf(ETX), 1);
        logs->append("IN:   <---------- " + msg);
    }
}

void FIAS::onSocketConnected()
{
    QString msg = "LD|DA%1|TI%2|V#1.0|IFWW|";
    logs->append("Connected");
    logs->append("========================");
    this->sendMessage(msg);
}

void FIAS::onSocketDisconnected()
{
    logs->append("Disconnected");
    logs->append("*****************************");
}

QString FIAS::formatMessage(QString eventMessage)
{
    QDateTime now = QDateTime::currentDateTime();
    // qDebug() << eventMessage;
    if (eventMessage.indexOf("|DA%1") != -1 && eventMessage.indexOf("|TI%2") != -1)
    {
        eventMessage = QString(eventMessage).arg(now.toString("yyMMdd"), now.toString("hhmmss"));
    }
    else if (eventMessage.indexOf("|DA%1") != -1)
    {
        eventMessage = QString(eventMessage).arg(now.toString("yyMMdd"));
    }
    return eventMessage;
}

void FIAS::sendMessage(QString eventMessage)
{
    eventMessage = this->formatMessage(eventMessage);
    QString paddedMessage = STX + eventMessage + ETX;
    tcpSocket->write(paddedMessage.toUtf8());
    logs->append("OUT:  --------> " + eventMessage);
}

QString FIAS::getMessage(QString option)
{
    QString eventMessage = events[option];
    return eventMessage;
}