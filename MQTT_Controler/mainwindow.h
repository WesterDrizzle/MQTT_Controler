#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMqtt/QMqttClient>
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
    void LedSendState(uint8_t ledOn);

private slots:

    void on_pBConnect_clicked();

    void on_pBSendSub_clicked();

    void on_pBSendPub_clicked();

    void on_pBConfig_clicked();


private:
    Ui::MainWindow *ui;
    QMqttClient *m_client;
};
#endif // MAINWINDOW_H
