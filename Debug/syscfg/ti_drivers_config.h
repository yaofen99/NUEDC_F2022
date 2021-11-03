/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSP_EXP432P401R
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_MSP_EXP432P401R

#ifndef DeviceFamily_MSP432P401x
#define DeviceFamily_MSP432P401x
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== ADC ========
 */

/* P5.4 */
#define CONFIG_TSL1401_ADC          0
/* P5.5 */
#define Power_ADC                   1


/*
 *  ======== GPIO ========
 */

/* P5.2 */
#define CONFIG_TFT_RES              0
/* P5.0 */
#define CONFIG_TFT_DC               1
/* P5.1 */
#define CONFIG_EC11_A               2
/* P3.5 */
#define CONFIG_EC11_B               3
/* P3.7 */
#define CONFIG_EC11_BUTTON          4
/* P4.5 */
#define TSL1401_SI_GPIO             5
/* P4.7 */
#define TSL1401_CLK_GPIO            6
/* P6.0 */
#define ENCODER_1A                  7
/* P6.1 */
#define ENCODER_1B                  8
/* P6.6 */
#define ENCODER_2A                  9
/* P6.7 */
#define ENCODER_2B                  10
/* P4.0 */
#define ENCODER_3A                  11
/* P4.1 */
#define ENCODER_3B                  12
/* P4.2 */
#define ENCODER_4A                  13
/* P4.3 */
#define ENCODER_4B                  14
/* P8.5 */
#define AIN1                        15
/* P9.0 */
#define AIN2                        16
/* P8.4 */
#define BIN1                        17
/* P8.6 */
#define BIN2                        18
/* P8.7 */
#define CIN1                        19
/* P9.1 */
#define CIN2                        20
/* P8.3 */
#define DIN1                        21
/* P5.3 */
#define DIN2                        22
/* P3.6 */
#define CONFIG_TFT_CS               23
/* P6.2 */
#define Infr_front1                 24
/* P7.3 */
#define Infr_front2                 25
/* P7.1 */
#define Infr_front3                 26
/* P6.3 */
#define Infr_front4                 27
/* P8.0 */
#define Infr_back1                  28
/* P7.4 */
#define Infr_back2                  29
/* P7.5 */
#define Infr_back3                  30
/* P10.0 */
#define Infr_back4                  31
/* P10.1 */
#define Infr_back5                  32
/* P1.1, LaunchPad Button S1 (Left) */
#define CONFIG_GPIO_0               33
/* P1.4, LaunchPad Button S2 (Right) */
#define CONFIG_GPIO_1               34
/* P1.0, LaunchPad LED 1 Red */
#define CONFIG_LED_0_GPIO           35
/* P2.2, LaunchPad LED 2 Blue */
#define CONFIG_LED_1_GPIO           36

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== I2C ========
 */

/*
 *  SCL: P6.5
 *  SDA: P6.4
 */
#define CONFIG_I2C_0                0
/*
 *  SCL: P10.3
 *  SDA: P10.2
 */
#define CONFIG_I2C_1                1

/* ======== I2C Addresses and Speeds ======== */
#include <ti/drivers/I2C.h>

/* ---- CONFIG_I2C_0 I2C bus components ---- */

/* no components connected to CONFIG_I2C_0 */

/* max speed unspecified, defaulting to 100 Kbps */
#define CONFIG_I2C_0_MAXSPEED   (100U) /* Kbps */
#define CONFIG_I2C_0_MAXBITRATE ((I2C_BitRate)I2C_100kHz)

/* ---- CONFIG_I2C_1 I2C bus components ---- */

/* no components connected to CONFIG_I2C_1 */

/* max speed unspecified, defaulting to 100 Kbps */
#define CONFIG_I2C_1_MAXSPEED   (100U) /* Kbps */
#define CONFIG_I2C_1_MAXBITRATE ((I2C_BitRate)I2C_100kHz)


/*
 *  ======== PWM ========
 */

/* P2.0, LaunchPad LED 2 Red */
#define CONFIG_PWM_0                0
/* P2.1, LaunchPad LED 2 Green */
#define CONFIG_PWM_1                1
/* P2.3 */
#define CONFIG_PWM_2                2
/* P2.4 */
#define CONFIG_PWM_3                3
/* P5.6 */
#define CONFIG_PWM_4                4
/* P5.7 */
#define CONFIG_PWM_5                5
/* P10.5 */
#define CONFIG_PWM_6                6
/* P9.2 */
#define CONFIG_PWM_7                7
/* P9.3 */
#define CONFIG_PWM_8                8
/* P8.2 */
#define CONFIG_PWM_10               9


/*
 *  ======== SPI ========
 */

/*
 *  MOSI: P1.6
 *  MISO: P1.7
 *  SCLK: P1.5
 */
#define CONFIG_SPI_0                0
/*
 *  MOSI: P7.7
 *  MISO: P7.0
 *  SCLK: P7.6
 */
#define CONFIG_SPI_1                1


/*
 *  ======== Timer ========
 */

#define CONFIG_TIMER_0              0
#define CONFIG_TIMER_1              1

/*
 *  ======== UART ========
 */

/*
 *  TX: P2.6
 *  RX: P2.5
 */
#define CONFIG_UART_1               0
/*
 *  TX: P3.0
 *  RX: P2.7
 */
#define CONFIG_UART_2               1
/*
 *  TX: P9.7
 *  RX: P9.6
 */
#define CONFIG_UART_3               2
/*
 *  TX: P1.3
 *  RX: P1.2
 *  XDS110 UART
 */
#define CONFIG_UART_0               3


/*
 *  ======== Button ========
 */

/* P1.1, LaunchPad Button S1 (Left) */
#define CONFIG_BUTTON_0             0
/* P1.4, LaunchPad Button S2 (Right) */
#define CONFIG_BUTTON_1             1


/*
 *  ======== LED ========
 */

/* P1.0, LaunchPad LED 1 Red */
#define CONFIG_LED_0                0
/* P2.2, LaunchPad LED 2 Blue */
#define CONFIG_LED_1                1


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
