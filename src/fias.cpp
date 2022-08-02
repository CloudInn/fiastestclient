#include <QtNetwork>
#include "src/fias.h"

FIAS::FIAS()
{
    tcpSocket = new QTcpSocket(this);
    events.insert("---------------", "");
    events.insert("Posting inquery PR", "PR|WSHEY|P#12|G#111|GNJohn Smith|PI107|PMROOM|DA%1|TI%2|");
    events.insert("Posting request PR", "PR|WSHEY|P#12|G#111|GNJohn Smith|RN107|PMROOM|DA%1|TI%2|TA1500|");
    events.insert("Posting simple PS", "PS|P#12|DD004|CTInternational|PTC|RN107|DA%1|TI%2|TA1050|");
    events.insert("Bill view XR", "XR|RN2781|G#12345|DA%1|TI%2|");
    events.insert("Remote Checkout XC", "XC|RN2781|G#12345|BA13850|DA%1|TI%2|");
    events.insert("Link record LR", "LR|RI|FL|");
    events.insert("Link end LE", "LE|DA%1|TI%2|");

    linkRecords.append("LR|RIDE|FLDATI|");
    linkRecords.append("LR|RIDS|FLDATI|");
    linkRecords.append("LR|RIGI|FLRNGSG#GFGNGLGVGGGAGDGTDATINPWSA0A1A2A3A4A5A6A7A8A9|");
    linkRecords.append("LR|RIGO|FLRNG#GSDATIWSA0A1A2A3A4A5A6A7A8A9|");
    linkRecords.append("LR|RIGC|FLRORNG#GFGNGLGVGGGSGAGDGTDATINPWSA0A1A2A3A4A5A6A7A8A9|");
    linkRecords.append("LR|RIXL|FLG#MIMTRNDATI|");
    linkRecords.append("LR|RIXT|FLG#MIMTRNDATI|");
    linkRecords.append("LR|RIXD|FLG#MIRNDATI|");
    linkRecords.append("LR|RIXI|FLBDBIDCG#RNF#FDDATI|");
    linkRecords.append("LR|RIXB|FLG#RNASDATIBA|");
    linkRecords.append("LR|RIXC|FLRNG#ASCTBADATI|");
    linkRecords.append("LR|RIKR|FLKCKTRNWSCTDATIG#GAGDDTGGGNIDK#KORTSTA0|");
    linkRecords.append("LR|RIKD|FLKCRNWSDATIG#IDRT|");
    linkRecords.append("LR|RIKM|FLG#KCRNROWSDATIDTGAGDGGGNIDRT|");
    linkRecords.append("LR|RIKZ|FLKCRNWSCTDATIG#GDDTGNKOAS|");
    linkRecords.append("LR|RIPL|FLG#GNP#RNWSBADATIGAGDGFGLGVGGGTNPPMA0A1A2A3A4A5A6A7A8A9|");
    linkRecords.append("LR|RIPA|FLASRNP#DATIGNWSG#CT|");

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

    tcpSocket->connectToHost(host, port);
    emit addToLog("Attempting to connect to server ...");
    if (!tcpSocket->waitForConnected(Timeout))
    {
        emit addToLog(tcpSocket->errorString());
    }
}
void FIAS::disconnectFromHost()
{
    this->sendMessage(events["Link end LE"]);
    tcpSocket->abort();
}
void FIAS::onSocketReadyRead()
{
    QByteArray data = tcpSocket->readAll();
    QList<QByteArray> dataList = data.split(STX);
    QByteArray msg;
    QString linkRecordMsg;
    bool sendLinkRecords = false;
    foreach (msg, dataList)
    {
        if (msg.length() > 0)
        {
            msg.remove(msg.indexOf(ETX), 1);
            emit addToLog("IN:  <--------- " + msg);
            if (msg.startsWith("LD|"))
            {
                sendLinkRecords = true;
            }
            else if (msg.startsWith("LA|"))
            {
                this->sendMessage("LA|DA%1|TI%2|");
            }
        }
    }
    if (sendLinkRecords)
    {
        foreach (linkRecordMsg, this->linkRecords)
        {
            this->sendMessage(linkRecordMsg);
        }
    }
}

void FIAS::onSocketConnected()
{
    emit socketConnected();
    QString msg = "LD|DA%1|TI%2|V#1.0|IFWW|";
    emit addToLog("Connected");
    emit addToLog("========================");
    this->sendMessage(msg);
}

void FIAS::onSocketDisconnected()
{
    emit socketDisconnected();
    emit addToLog("Disconnected");
    emit addToLog("************************");
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
    emit addToLog("OUT: ---------> " + eventMessage);
}

QString FIAS::getMessage(QString option)
{
    QString eventMessage = events[option];
    return eventMessage;
}