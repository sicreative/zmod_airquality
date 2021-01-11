#ifndef ZMODQTDATA_H
#define ZMODQTDATA_H

//#define EXTERNAL_SENSOR
#define WRITE_CSV
#define USE_INTERRUPT
//#define DEBUG_OUTPUT


#ifdef EXTERNAL_SENSOR
#include "httpsensor.h"
#endif

#include <QObject>
#include <QVariantMap>
#include <QTimer>
#include <QGuiApplication>





class ZmodQtData : public QObject
{
    Q_OBJECT



    Q_PROPERTY(QVariantMap sensor_map READ sensor_map WRITE setSensor_map NOTIFY sensorChanged)
    Q_PROPERTY(int sensor_progress READ sensor_progress  NOTIFY sensorProgressChanged)
    Q_PROPERTY(int sensor_logging READ sensor_logging  NOTIFY sensorLoggingChanged)

public:
    explicit ZmodQtData(QObject *parent = nullptr);

    QVariantMap sensor_map() const
    {
        return m_sensor_map;
    }

    int sensor_progress() const
    {
        return m_sensor_progress;
    }


    int sensor_logging() const
    {
        return m_sensor_logging;
    }




signals:
     void sensorChanged();
     void sensorProgressChanged();
     void sensorLoggingChanged();

public slots:
    int8_t startReadSensor(QGuiApplication* app);



    void setSensor_map(QVariantMap sensor_map)
    {
        m_sensor_map = sensor_map;
    }

      void sensorReset();

      void sensorLoggingSwtich();

private:
    void quit();
    QTimer m_timer;
    void readLoop();
    int m_sensor_progress;
    int m_sensor_logging;
    bool m_readed;
    bool m_isMeasuring;
 #ifdef EXTERNAL_SENSOR
    HttpSensor m_httpsensor;
 #endif


    QVariantMap m_sensor_map;
};

#endif // ZMODQTDATA_H
