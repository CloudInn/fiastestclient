#include <QtNetwork>
#include <QWidget>
#include <QtCore>
#include <QMessageBox>

#include "mainwindow.h"
#include "src/fias.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fias = new FIAS();

    connect(fias, SIGNAL(addToLog(QString)), SLOT(addToLogsViewer(QString)));
    connect(fias, SIGNAL(socketConnected()), SLOT(socketConnected()));
    connect(fias, SIGNAL(socketDisconnected()), SLOT(socketDisconnected()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fias;
}

void MainWindow::on_connectButton_clicked()
{
    fias->connectToHost(ui->hostAddressInput->text(), ui->hostPortInput->text().toShort());
}

void MainWindow::on_sendMessageButton_clicked()
{
    fias->sendMessage(ui->messageInput->text());
}

void MainWindow::on_eventComboBox_currentTextChanged(QString currentText)
{
    ui->messageInput->setText(fias->getMessage(currentText));
}

void MainWindow::on_disconnetPushButton_clicked()
{
    fias->disconnectFromHost();
}

void MainWindow::addToLogsViewer(QString msg)
{
    ui->logsTextArea->append(msg);
}

void MainWindow::socketConnected()
{
    ui->connectButton->setDisabled(true);
    ui->disconnetPushButton->setEnabled(true);
    ui->sendMessageButton->setEnabled(true);
    ui->eventComboBox->setEnabled(true);
    ui->messageInput->setEnabled(true);
}

void MainWindow::socketDisconnected()
{
    ui->connectButton->setEnabled(true);
    ui->disconnetPushButton->setDisabled(true);
    ui->sendMessageButton->setDisabled(true);
    ui->eventComboBox->setDisabled(true);
    ui->messageInput->setDisabled(true);
}

void MainWindow::on_clearPushButton_clicked()
{
    ui->logsTextArea->clear();
}