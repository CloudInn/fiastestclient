#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include "src/fias.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FIAS *fias;
private slots:
    void on_connectButton_clicked();
    void on_sendMessageButton_clicked();
    void on_eventComboBox_currentTextChanged(QString);
    void on_disconnetPushButton_clicked();
};
#endif // MAINWINDOW_H
