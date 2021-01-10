#include "sensorasyncworker.h"
#include <unistd.h>
#include "zmodhat.h"

  std::string result_zmod;
  std::string result_hs300x; 




SensorAsyncWorker::SensorAsyncWorker(Function& callback)
    : AsyncWorker(callback){};
    
  


void SensorAsyncWorker::Execute() {
  

  
 
 
 
  
   
   i2c_openport();
   
 
 
  
   
       
   
   
   zmodhat_hs300x_start_measurement();
   
   
   hs300x_result hs_result;
   
  
  
  do { 
    hs_result = zmodhat_hs300x_read_measurement();
  }while(!hs_result.valid);
  
  
  
  if (zmodhat_start_meassurement()){
    printf("zmod can't start meassure");
     result_zmod = "zmod start failure";
    i2c_closeport(); 
    return;
  }
    
    usleep(50);
  
  
  if (zmodhat_read_meassurement()){
    printf("zmod can't read");
    result_zmod = "zmod read failure";
    i2c_closeport(); 
    return;
  }
  


   i2c_closeport();            
 
  
  iaq_2nd_gen_results_t* zmod_result = zmodhat_get_iaq_result();

  char buffer[100];

  sprintf(buffer,"\"temperature\":%.2f,\"humidity\":%.2f",hs_result.temperature,hs_result.humidity);
  
  result_hs300x = std::string(buffer);
  
  
   sprintf(buffer,"\"iaq\":%.2f,\"eco2\":%6.3f,\"etoh\":%.0f,\"tvoc\":%.0f,\"rcda\":%5.3f,\"stabilization\":%.0f"
                ,zmod_result->iaq,zmod_result->eco2,zmod_result->etoh,zmod_result->tvoc,zmod_result->log_rcda,zmodhat_get_iaq_stabilization());
                
  result_zmod = std::string(buffer);  
  
  
};

void SensorAsyncWorker::OnWorkComplete(Napi::Env env, napi_status status){
  
}

void SensorAsyncWorker::OnOK() {
  
  std::string msg = "{"+result_hs300x +","+ result_zmod+"}";
  Callback().Call({Env().Null(), String::New(Env(), msg)});
  
  
  
};
