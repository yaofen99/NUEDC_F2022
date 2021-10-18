#include <unistd.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/PWM.h>
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include "motor.h"

#include "includes.h"

void *speed_measure_Thread(void *arg0)
{
    static int last_encoder[4];
    static UInt32 last_time;

    while (1){
    int encoder[4];
    uint32_t time;
    time = Clock_getTicks();

    encoder[0] = get_encoder1();
    encoder[1] = get_encoder2();
    encoder[2] = get_encoder3();
    encoder[3] = get_encoder4();


    int i = 0;
    for (; i<4 ;i++){
        speed_wheel[i] = (encoder[i] - last_encoder[i])*100/(float)(time - last_time);
    }

//    DEBUG_printf("%d,%d,%d,%f\n",encoder[0],last_encoder[0],encoder[0]-last_encoder[0],speed_wheel[0]);
//    DEBUG_printf("%f,%f,%f,%f \n",speed_wheel[0],speed_wheel[1],speed_wheel[2],speed_wheel[3]);

    DEBUG_printf("%f,%f,%f",*motor_speed_pid[0].Kp,*motor_speed_pid[0].Ki,*motor_speed_pid[0].Kd);


    last_encoder[0] = encoder[0];
    last_encoder[1] = encoder[1];
    last_encoder[2] = encoder[2];
    last_encoder[3] = encoder[3];
    last_time = time;

    usleep(10000);
    }
}
