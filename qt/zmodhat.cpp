#include "zmodhat.h"
#include "zmod4xxx.h"
#include "zmod4410_config_iaq2.h"
#include <iostream>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <QSocketNotifier>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <wiringPi.h>
#include <softPwm.h>






static iaq_2nd_gen_handle_t zmodhat_iaq_handle;
static iaq_2nd_gen_results_t zmodhat_results;
static zmod4xxx_dev_t zmodhat_dev;
static uint8_t zmodhat_sensor_result[32] = {0} ;
static uint8_t zmodhat_prod_data[ZMOD4410_PROD_DATA_LEN];

static int i2cHandle = 0;

static uint8_t zmodhat_status;
static uint8_t eoc_flag = 0;
static uint8_t s_step_last = 0;
static uint8_t s_step_new = 0;

static uint8_t iaq_max_stable_value = 0;


static zmod_int_ptr_t zmod_int_ptr;



int8_t i2c_read(uint8_t addr, uint8_t reg_addr,uint8_t *data_buf, uint8_t len){

    i2c_rdwr_ioctl_data set[1];
    struct i2c_msg msgs[2];
    msgs[0].addr = addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = &reg_addr;

    msgs[1].addr = addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = len;
    msgs[1].buf = data_buf;

    set[0].msgs = msgs;
    set[0].nmsgs =2;

   if (ioctl(i2cHandle,I2C_RDWR,&set) < 0)
       return ERROR_I2C;

    return 0;
}

int8_t i2c_write(uint8_t addr, uint8_t reg_addr,uint8_t *data_buf, uint8_t len){

    uint8_t temp[len+1];
    temp[0]=reg_addr;
    for(int i=0;i<len;i++)
        temp[i+1]=data_buf[i];

    i2c_rdwr_ioctl_data set[1];
    struct i2c_msg msgs[1];
    msgs[0].addr = addr;
    msgs[0].flags = 0;
    msgs[0].len = len+1;
    msgs[0].buf = temp;

    set[0].msgs = msgs;
    set[0].nmsgs =1;

    if (ioctl(i2cHandle,I2C_RDWR,&set) < 0)
        return ERROR_I2C;


    return 0;
}

void delay_ms(uint32_t ms){
    usleep(ms);
}

int i2c_openport(void){

    if (i2cHandle)
        i2c_closeport();
    i2cHandle = open("/dev/i2c-1",O_RDWR);



    if (i2cHandle<0){
        std::cout << "Error of open i2c port";
        return ERROR_I2C;
    }




    return 0;
}

int i2c_closeport(void){
    if(i2cHandle>=0)
        close(i2cHandle);

    i2cHandle = 0;

    return 0;
}

int i2c_init(void){





    zmodhat_dev.read = i2c_read;
    zmodhat_dev.write = i2c_write;
    zmodhat_dev.delay_ms = delay_ms;

    return 0;

}

int i2c_deinit(){
    return i2c_closeport();
}

int gpio_init(){
    int8_t ret;
    if (ret = wiringPiSetup())
        return ret;

    pinMode(ACTIVE_LED, SOFT_PWM_OUTPUT);
    softPwmCreate(ACTIVE_LED,0,1000);
    pinMode(LIMIT_LED, SOFT_PWM_OUTPUT);
    pinMode(INT_ZMOD,INPUT);
    pinMode(NRST_ZMOD,OUTPUT);
    gpio_reset();



    gpio_active_led(false);
    gpio_limit_led(false);

    return 0;
}

int gpio_active_led(bool status){


 //  digitalWrite (ACTIVE_LED, status?HIGH:LOW);


        softPwmWrite(ACTIVE_LED,status?20:0);


   return 0;
}

int gpio_limit_led(bool status){

   digitalWrite (LIMIT_LED, status?HIGH:LOW);

   return 0;
}



int zmodhat_init(void){

     i2c_init();


       zmodhat_dev.i2c_addr = ZMOD4410_I2C_ADDR;
        zmodhat_dev.pid = ZMOD4410_PID;
        zmodhat_dev.init_conf = &zmod_sensor_type[INIT];
        zmodhat_dev.meas_conf = &zmod_sensor_type[MEASUREMENT];
        zmodhat_dev.prod_data = zmodhat_prod_data;

       int8_t ret;
     if((ret = zmod4xxx_read_sensor_info(&zmodhat_dev))){
             std::cout << "can't read sensor info "<< ret <<"\n";
             i2c_deinit();
             return ret;
        }


     if ((ret = zmod4xxx_prepare_sensor(&zmodhat_dev))){
         printf("can't prepare the sensor info %d",ret);
         //std::cout << "can't prepare sensor info "<< ret <<"\n";
         i2c_deinit();
         return ret;
     }

     if ((ret = init_iaq_2nd_gen(&zmodhat_iaq_handle))){
         i2c_deinit();
         return ret;
     }





      return ret;
}


int8_t zmodhat_hs300x_start_measurement(){


    i2c_rdwr_ioctl_data set[1];
    struct i2c_msg msgs[1];
    uint8_t data[1];

    msgs[0].addr = HS300X_ADD;
    msgs[0].flags = 0;
    msgs[0].len = 0;
    msgs[0].buf = data;


    set[0].msgs = msgs;
    set[0].nmsgs =1;



    if (ioctl(i2cHandle,I2C_RDWR,&set) < 0)
        return ERROR_I2C;



    return 0;
}

hs300x_result zmodhat_hs300x_read_measurement(){

    hs300x_result result;
    uint8_t data[4];
    i2c_rdwr_ioctl_data set[1];
    struct i2c_msg msgs[1];
    msgs[0].addr = HS300X_ADD;
    msgs[0].flags = I2C_M_RD;
    msgs[0].len = 4;
    msgs[0].buf = data;


    set[0].msgs = msgs;
    set[0].nmsgs =1;

    result.valid = false;

    //int rtl = ioctl(i2cHandle,I2C_RDWR,&set);

    if (ioctl(i2cHandle,I2C_RDWR,&set) < 0)
        return result;

    if (!(data[0] >> 6))
        result.valid = true;


    uint16_t humidity =((data[0] & 0x3F) << 8) | data[1];


            // based on equation by Renesa HS300x Datasheet part. 7

           result.humidity = static_cast<float>(humidity) / 16383.0 * 100.0;

       uint16_t temperature = (data[2] << 6) | (data[3] >> 2);


            result.temperature = static_cast<float>(temperature) / 16383.0 * 165.0 - 40.0;




    return result;
}

int8_t zmodhat_start_meassurement(){
    int8_t ret;
    if ((ret = zmod4xxx_start_measurement(&zmodhat_dev))){
        std::cout << "can't start measurement -"<< -ret <<"\n";
        i2c_deinit();
        return ret;
    }



    return 0;
}

iaq_2nd_gen_results_t* zmodhat_get_iaq_result(){
    return &zmodhat_results;
}

float zmodhat_get_iaq_stabilization(){
    uint8_t stabilization_sample = static_cast<uint8_t>(zmodhat_iaq_handle.stabilization_sample);
    if (stabilization_sample>iaq_max_stable_value)
        iaq_max_stable_value = stabilization_sample;
    return   (1 - static_cast<float>(stabilization_sample) / iaq_max_stable_value) * 100;
}

int8_t zmodhat_tracking_num(uint8_t* tracking_number){

    int8_t ret;

    if ((ret = zmod4xxx_read_tracking_number(&zmodhat_dev, tracking_number))){
        std::cout << "can't read tracking number "<< ret <<"\n";
        i2c_deinit();
        return ret;
    }

    return 0;
}

int8_t zmodhat_read_meassurement(){
    int8_t ret;


        do{

            if ((ret = zmod4xxx_read_status(&zmodhat_dev, &zmodhat_status))){
                std::cout << "can't read sensor status "<< ret <<"\n";
                i2c_deinit();
                return ret;
            }

            s_step_new = (zmodhat_status & STATUS_LAST_SEQ_STEP_MASK);

            if (s_step_new != s_step_last) {
                           if (s_step_new == ((zmodhat_dev.meas_conf->s.len / 2) - 1)) {
                               eoc_flag = 1;
                           }
            }

            s_step_last = s_step_new;
            zmodhat_dev.delay_ms(50);



        }while(!eoc_flag);

    eoc_flag = 0;


     if ((ret = zmod4xxx_read_adc_result(&zmodhat_dev, zmodhat_sensor_result))){
         std::cout << "can't read adc result "<< ret <<"\n";
         i2c_deinit();
         return ret;
     }







     if ((ret = calc_iaq_2nd_gen(&zmodhat_iaq_handle,&zmodhat_dev,zmodhat_sensor_result,&zmodhat_results))){
        if (ret!= IAQ_2ND_GEN_STABILIZATION){
         std::cout << "can't calc iaq2 "<< ret <<"\n";
         i2c_deinit();
         return ret;
        }
     }





       return 0;
}


void int_zmod_handle(void){
    zmod_int_ptr(INT_ZMOD);
}


int gpio_int_init(zmod_int_ptr_t ptr)
{

    zmod_int_ptr = ptr;

    wiringPiISR(INT_ZMOD,INT_EDGE_FALLING,&int_zmod_handle);



    return 0;
}

int gpio_reset()
{

    digitalWrite(NRST_ZMOD,0);
    usleep(500);
    digitalWrite(NRST_ZMOD,1);

    return 0;

}
