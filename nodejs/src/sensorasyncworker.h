#pragma once
#include <napi.h>
using namespace Napi;



class SensorAsyncWorker : public AsyncWorker {
 public:
  SensorAsyncWorker(Function& callback);
  virtual ~SensorAsyncWorker(){};
  

  void Execute();
  void OnOK();
  void OnWorkComplete(Napi::Env, napi_status);

 private:
  

 
  
};
