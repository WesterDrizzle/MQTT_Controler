#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtMqtt/QMqttClient>
#include <qdebug.h>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "imagebtn.h"

static uint8_t lightState = 0;
QString clientId;
QString username;
QString mqttHostUrl;
QString passwd;
int port;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_client = new QMqttClient(this);
    connect(m_client, QMqttClient::connected, [this]()
            { ui->pBConnect->setText("Disconnect"); });
    connect(m_client, QMqttClient::disconnected, [this]()
            { ui->pBConnect->setText("Connect"); });
    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic)
            {
             
    const QString content = QDateTime::currentDateTime().toString()
                + " Received Topic: "
                + topic.name()
                + " Message: "
                + message
                + u'\n';
    ui->TextEdit->append(content); });
    connect(ui->imgbtn, &ImageBtn::imgbtnPressed, this, [this]()
            {
        if(lightState){
            LedSendState(lightState);
            ui->imgbtn->setStyleSheet("image: url(:/img/light.png);");
            lightState = !lightState;
            }

        else{
            ui->imgbtn->setStyleSheet("image: url(:/img/dark.png);");
            LedSendState(lightState);
            lightState = !lightState; 
        } });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pBConnect_clicked()
{
    m_client->setHostname(ui->lEHostName->text());
    m_client->setPort(ui->lEPort->text().toInt());
    m_client->setClientId(ui->lEClientID->text());
    m_client->setUsername(ui->lEUserName->text());
    m_client->setPassword(ui->lEPassword->text());
    if (m_client->state() == QMqttClient::Connected)
    {
        m_client->disconnectFromHost();
    }
    else
    {
        m_client->connectToHost();
    }
}

void MainWindow::on_pBSendSub_clicked()
{
    auto subscription = m_client->subscribe(ui->lEsubscribtion->text(), 1);
    if (!subscription)
    {
        QMessageBox::critical(this, "Error", "Could not subscribe. Is there a valid connection?");
        return;
    }
    else
    {
        ui->TextEdit->append(subscription->topic().filter());
    }
}
/// /sys/k1wbgzq58PT/MQTTX/thing/service/property/set
///
static uint16_t id = 0;
void MainWindow::on_pBSendPub_clicked()
{
    QString JsonDataToPub = ui->lEpublication->text();
    if (m_client->publish(QString("/sys/k1wbgzq58PT/MQTTX/thing/event/property/post"), JsonDataToPub.toUtf8()) == -1)
    {
        QMessageBox::critical(this, "Error", "Could not publish message maybe no connection");
    }
    else
    {
        ui->TextEdit->append("Pub Content: " + JsonDataToPub);
    }
}

void MainWindow::on_pBConfig_clicked()
{
    QString jsonString = ui->lEconfig->text();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (jsonDoc.isNull())
    {
        QMessageBox::critical(this, "Error", "Json Null");
        return;
    }

    // 确保是一个对象
    if (!jsonDoc.isObject())
    {
        QMessageBox::critical(this, "Error", "Json No Object");
        return;
    }
    QJsonObject jsonObj = jsonDoc.object();
    clientId = jsonObj.value("clientId").toString();
    username = jsonObj.value("username").toString();
    mqttHostUrl = jsonObj.value("mqttHostUrl").toString();
    passwd = jsonObj.value("passwd").toString();
    port = jsonObj.value("port").toInt();
    ui->lEHostName->setText(mqttHostUrl);
    ui->lEPort->setText(QString::number(port));
    ui->lEClientID->setText(clientId);
    ui->lEUserName->setText(username);
    ui->lEPassword->setText(passwd);
}
void MainWindow::LedSendState(uint8_t ledOn)
{
    QJsonDocument doc;
    QJsonObject objRoot;
    QJsonObject objParams;
    objRoot["id"] = id;
    objRoot["method"] = "thing.service.property.post";
    objParams["CurrentTemperature"] = 0;
    objParams["LightSwitch"] = ledOn;
    objRoot["params"] = objParams;
    objRoot["version"] = "1.0.0";
    doc.setObject(objRoot);
    QString SendJson = doc.toJson();
    if (m_client->publish(QString("/sys/k1wbgzq58PT/MQTTX/thing/event/property/post"), SendJson.toUtf8()) == -1)
    {
        QMessageBox::critical(this, "Error", "Could not publish message maybe no connection");
    }
    else
    {
        ui->TextEdit->append("Send Topic: " + ui->lEpublication->text());
        ui->TextEdit->append("Send Content: " + SendJson);
    }
    id++;
}
