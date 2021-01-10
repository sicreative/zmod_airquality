#include <QFile>
#include <QDateTime>
#include <QGuiApplication>
#include <QTextStream>
#include "zmodqtdata.h"
#include "zmodhat.h"
#include "unistd.h"

#ifdef USE_INTERRUPT
bool m_int_trigger;
#endif

ZmodQtData::ZmodQtData(QObject *parent) : QObject(parent)
{



}



void ZmodQtData::readLoop(){


    if (m_sensor_progress % 25 == 0)
         sensorProgressChanged();


       ++m_sensor_progress;

        //if( m_sensor_progress == 0 || m_sensor_progress > 95 || m_sensor_progress%20 )


    if (m_sensor_progress == -1){

        i2c_openport();
        if (zmodhat_init()){
            printf("ZMOD: INIT ERR");

        }
        i2c_closeport();

        return;

    }

    if (m_sensor_progress == 99){
        sensorProgressChanged();

        i2c_openport();
        zmodhat_start_meassurement();
        zmodhat_hs300x_start_measurement();
        m_isMeasuring  = true;
#ifdef EXTERNAL_SENSOR
        m_httpsensor.getHttpSensorData(&m_sensor_map);
#endif
        i2c_closeport();

        return;

    }



    if (m_sensor_progress > 102 ){

#ifdef USE_INTERRUPT
        if (!m_int_trigger && m_sensor_progress < 150)
            return;
        m_int_trigger = false;
#endif




        i2c_openport();
        hs300x_result tempresult;
        tempresult =  zmodhat_hs300x_read_measurement();



        if (!tempresult.valid || zmodhat_read_meassurement()){
            if (m_sensor_progress>120)
                m_sensor_progress = 0;

            return;
        }
#ifdef WRITE_CSV
if (m_sensor_logging)
    gpio_active_led(true);
#endif


        iaq_2nd_gen_results_t* result = zmodhat_get_iaq_result();

#ifdef DEBUG_OUTPUT
        union Tracking{
           uint8_t u8[8];
           uint64_t u64;
        } tracking;



        zmodhat_tracking_num(tracking.u8);


#endif

        i2c_closeport();





        m_isMeasuring = false;




         char value[20];

#ifdef DEBUG_OUTPUT
        for(int i=0;i<13;i++){
            char index[10];
            sprintf(index,"rmox%d",i);
            sprintf(value,"%.2f",result->rmox[i]);
            m_sensor_map[index] = value;
        }



        sprintf(value,"%f",result->log_rcda);
        m_sensor_map["cda"] = value;


         m_sensor_map["tracking"] = tracking.u64;
#endif
        sprintf(value,"%.2f",result->iaq);
        m_sensor_map["iaq"] = value;
        sprintf(value,"%.0f",result->eco2);
        m_sensor_map["eco2"] = value;
         sprintf(value,"%.0f",result->tvoc);
        m_sensor_map["tvoc"] = value;
        sprintf(value,"%.0f", zmodhat_get_iaq_stabilization());
        m_sensor_map["stabilization"] = value;
        sprintf(value,"%.2f", tempresult.temperature);
        m_sensor_map["temperature"] = value;
        sprintf(value,"%.2f", tempresult.humidity);
        m_sensor_map["humidity"] = value;


        m_sensor_map["time"] = QDateTime::currentDateTime().toSecsSinceEpoch();

        m_sensor_progress = 0;

        sensorChanged();




#ifdef WRITE_CSV
#ifdef EXTERNAL_SENSOR
            if (m_sensor_map.size()<=8)
#else
            if (m_sensor_map.size()!=8)
#endif
                return;

       if (!m_sensor_logging)
           return;



        QFile file("./sensor.csv");
        bool fileexists = file.exists();

        if (file.open(QFile::WriteOnly|QFile::Append))
        {
            QTextStream stream(&file);



            bool first = true;
            if (!fileexists){
                for(QString key:m_sensor_map.keys()){
                    stream << (first?"":",") << key ;
                    first = false;
                }
                stream <<"\n";
            }


                first = true;
                for(QVariant value:m_sensor_map.values()){
                    stream << (first?"":",") << value.toString();
                    first = false;
                }
                stream <<"\n";





            file.close();

            gpio_active_led(false);
        }

#endif



    }


}

#ifdef USE_INTERRUPT
int8_t callback_int(int pin){

    if (pin == INT_ZMOD)
        m_int_trigger = true;


    return 0;
}
#endif

int8_t ZmodQtData::startReadSensor(QGuiApplication* app)
{


    gpio_init();
    m_timer.setSingleShot(false);
    m_timer.setInterval(20);
    m_sensor_progress = -2;
    m_isMeasuring = false;
    m_sensor_logging = false;



#ifdef USE_INTERRUPT
    m_int_trigger = false;
    gpio_int_init(callback_int);
#endif


    if (!app)
        return 0;



   // zmodhat_start_meassurement();

    connect(app,&QGuiApplication::aboutToQuit,this,&ZmodQtData::quit);


    connect(&m_timer,&QTimer::timeout,this,&ZmodQtData::readLoop,Qt::UniqueConnection);
    m_timer.start();

    return 0;

}

void ZmodQtData::sensorReset()
{

      gpio_reset();

      startReadSensor(NULL);


}

void ZmodQtData::sensorLoggingSwtich()
{
    m_sensor_logging = !m_sensor_logging;
    sensorLoggingChanged();
}

void ZmodQtData::quit()
{

    while(m_isMeasuring){
        usleep(10);
    };
      i2c_closeport();
      gpio_active_led(false);


}


