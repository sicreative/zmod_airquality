#pragma once

#include <napi.h>
#include "sensorasyncworker.h"



class Airquality : public Napi::ObjectWrap<Airquality>
{
public:
    Airquality(const Napi::CallbackInfo&);
   
 
    Napi::Value Clearup(const Napi::CallbackInfo&);
    
   
    
    Napi::Value StartSensorMeasurement(const Napi::CallbackInfo&);

    static Napi::Function GetClass(Napi::Env);

private:
   
    SensorAsyncWorker* sensorworker;
};
