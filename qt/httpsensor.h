#ifndef HTTPSENSOR_H
#define HTTPSENSOR_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class HttpSensor : public QObject
{
    Q_OBJECT
public:
    explicit HttpSensor(QObject* parent = nullptr);
    int getHttpSensorData(QVariantMap* map);


signals:

public slots:



private:
    QNetworkAccessManager* m_manager;
    QNetworkReply* m_http_reply;
    void didFinished(void);
    QVariantMap* m_result_map;


};

#endif // HTTPSENSOR_H
