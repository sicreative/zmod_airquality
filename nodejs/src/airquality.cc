#include "airquality.h"
#include "zmodhat.h"
#include "sensorasyncworker.h"

#include <unistd.h>
#include <thread>  


using namespace Napi;






Airquality::Airquality(const Napi::CallbackInfo& info) : ObjectWrap(info) {
    
    
    
    Napi::Env env = info.Env();
    gpio_init();
    gpio_reset();
    

    i2c_openport();
   
    
    
    zmodhat_init();
  
    i2c_closeport();     
  
  return;
   
    
}



Napi::Value Airquality::StartSensorMeasurement(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    
    
    Function callback = info[0].As<Function>();
    sensorworker = new SensorAsyncWorker(callback);

    sensorworker->Queue();

    return Napi::Number::New(env, 0);
}

Napi::Value Airquality::Clearup(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
   return Napi::Number::New(env,i2c_closeport());
}


Napi::Function Airquality::GetClass(Napi::Env env) {
    return DefineClass(env, "Airquality", {
        Airquality::InstanceMethod("startsensormeasurement", &Airquality::StartSensorMeasurement),
        Airquality::InstanceMethod("clearup", &Airquality::Clearup)

        
    });
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::String name = Napi::String::New(env, "Airquality");
    exports.Set(name, Airquality::GetClass(env));
    return exports;
}

NODE_API_MODULE(addon, Init)
