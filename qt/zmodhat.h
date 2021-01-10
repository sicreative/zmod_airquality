#ifndef ZMODHAT_H
#define ZMODHAT_H

#ifdef __cplusplus
extern "C" {
#endif



#include "iaq_2nd_gen.h"

#define HS300X_ADD 0x44
#define ACTIVE_LED 5
#define LIMIT_LED 6
#define INT_ZMOD 2
#define NRST_ZMOD 25



typedef struct {
    bool valid;
    float temperature;
    float humidity;
} hs300x_result;

typedef int8_t (*zmod_int_ptr_t)(int INT);

int i2c_closeport(void);
int i2c_openport(void);
int zmodhat_init(void);
int8_t zmodhat_start_meassurement();
int8_t zmodhat_read_meassurement();
iaq_2nd_gen_results_t* zmodhat_get_iaq_result();
float zmodhat_get_iaq_stabilization();
int8_t zmodhat_hs300x_start_measurement();
hs300x_result zmodhat_hs300x_read_measurement();
int8_t zmodhat_tracking_num(uint8_t* tracking_number);

int gpio_init();
int gpio_active_led(bool status);
int gpio_limit_led(bool status);

int gpio_int_init(zmod_int_ptr_t ptr);

int gpio_reset();



#ifdef __cplusplus
}
#endif

#endif // ZMODHAT_H
