#include <QtNetwork>
#include <QWidget>
#include <QtCore>
#include <QMessageBox>

#include "mainwindow.h"
#include "fias.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fias = new FIAS(ui->logsTextArea);
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

void MainWindow::on_disconnetPushButton_clicked(){
    fias->disconnectFromHost();
}