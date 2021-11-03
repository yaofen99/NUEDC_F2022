/*
 *  ======== ti_drivers_config.c ========
 *  Configured TI-Drivers module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSP_EXP432P401R
 *  by the SysConfig tool.
 */

#include <stddef.h>

#ifndef DeviceFamily_MSP432P401x
#define DeviceFamily_MSP432P401x
#endif

#include <ti/devices/DeviceFamily.h>

#include "ti_drivers_config.h"

/*
 *  ============================= Display =============================
 */

#include <ti/display/Display.h>
#include <ti/display/DisplayUart.h>

#define Display_UARTBUFFERSIZE 1024
static char displayUARTBuffer[Display_UARTBUFFERSIZE];

DisplayUart_Object displayUartObject;

const DisplayUart_HWAttrs displayUartHWAttrs = {
    .uartIdx      = CONFIG_UART_0,
    .baudRate     = 115200,
    .mutexTimeout = (unsigned int)(-1),
    .strBuf       = displayUARTBuffer,
    .strBufLen    = Display_UARTBUFFERSIZE
};

const Display_Config Display_config[] = {
    /* CONFIG_Display_0 */
    /* XDS110 UART */
    {
        .fxnTablePtr = &DisplayUartMin_fxnTable,
        .object      = &displayUartObject,
        .hwAttrs     = &displayUartHWAttrs
    },
};

const uint_least8_t Display_count = 1;


/*
 *  =============================== ADC ===============================
 */

#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCMSP432.h>

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/adc14.h>
#include <ti/devices/msp432p4xx/driverlib/ref_a.h>

#define CONFIG_ADC_COUNT 2

/*
 *  ======== adcMSP432Objects ========
 */
ADCMSP432_Object adcMSP432Objects[CONFIG_ADC_COUNT];

/*
 *  ======== adcMSP432HWAttrs ========
 */
const ADCMSP432_HWAttrsV1 adcMSP432HWAttrs[CONFIG_ADC_COUNT] = {
    /* CONFIG_TSL1401_ADC */
    {
        .adcPin = ADCMSP432_P5_4_A1,
        .refVoltage = ADCMSP432_REF_VOLTAGE_VDD,
        .resolution = ADC_8BIT,
        .refExtValue = 3300000
    },
    /* Power_ADC */
    {
        .adcPin = ADCMSP432_P5_5_A0,
        .refVoltage = ADCMSP432_REF_VOLTAGE_VDD,
        .resolution = ADC_14BIT,
        .refExtValue = 3300000
    },
};

/*
 *  ======== ADC_config ========
 */
const ADC_Config ADC_config[CONFIG_ADC_COUNT] = {
    /* CONFIG_TSL1401_ADC */
    {
        .fxnTablePtr = &ADCMSP432_fxnTable,
        .object = &adcMSP432Objects[CONFIG_TSL1401_ADC],
        .hwAttrs = &adcMSP432HWAttrs[CONFIG_TSL1401_ADC]
    },
    /* Power_ADC */
    {
        .fxnTablePtr = &ADCMSP432_fxnTable,
        .object = &adcMSP432Objects[Power_ADC],
        .hwAttrs = &adcMSP432HWAttrs[Power_ADC]
    },
};

const uint_least8_t ADC_count = CONFIG_ADC_COUNT;


/*
 *  =============================== DMA ===============================
 */

#include <ti/drivers/dma/UDMAMSP432.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/dma.h>

/* Ensure DMA control table is aligned as required by the uDMA Hardware */
static DMA_ControlTable dmaControlTable[16] __attribute__ ((aligned (256)));

/* This is the handler for the uDMA error interrupt. */
static void dmaErrorFxn(uintptr_t arg)
{
    int status = MAP_DMA_getErrorStatus();
    MAP_DMA_clearErrorStatus();

    /* Suppress unused variable warning */
    (void)status;

    while (1);
}

UDMAMSP432_Object udmaMSP432Object;

const UDMAMSP432_HWAttrs udmaMSP432HWAttrs = {
    .controlBaseAddr = (void *)dmaControlTable,
    .dmaErrorFxn     = (UDMAMSP432_ErrorFxn)dmaErrorFxn,
    .intNum          = INT_DMA_ERR,
    .intPriority     = (~0)
};

const UDMAMSP432_Config UDMAMSP432_config = {
    .object  = &udmaMSP432Object,
    .hwAttrs = &udmaMSP432HWAttrs
};


/*
 *  =============================== GPIO ===============================
 */

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSP432.h>

/*
 *  ======== gpioPinConfigs ========
 *  Array of Pin configurations
 */
GPIO_PinConfig gpioPinConfigs[] = {
    /* CONFIG_TFT_RES */
    GPIOMSP432_P5_2 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,
    /* CONFIG_TFT_DC */
    GPIOMSP432_P5_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* CONFIG_EC11_A */
    GPIOMSP432_P5_1 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE,
    /* CONFIG_EC11_B */
    GPIOMSP432_P3_5 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE,
    /* CONFIG_EC11_BUTTON */
    GPIOMSP432_P3_7 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE,
    /* TSL1401_SI_GPIO */
    GPIOMSP432_P4_5 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* TSL1401_CLK_GPIO */
    GPIOMSP432_P4_7 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* ENCODER_1A */
    GPIOMSP432_P6_0 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* ENCODER_1B */
    GPIOMSP432_P6_1 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* ENCODER_2A */
    GPIOMSP432_P6_6 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* ENCODER_2B */
    GPIOMSP432_P6_7 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* ENCODER_3A */
    GPIOMSP432_P4_0 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* ENCODER_3B */
    GPIOMSP432_P4_1 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* ENCODER_4A */
    GPIOMSP432_P4_2 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* ENCODER_4B */
    GPIOMSP432_P4_3 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* AIN1 */
    GPIOMSP432_P8_5 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* AIN2 */
    GPIOMSP432_P9_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,
    /* BIN1 */
    GPIOMSP432_P8_4 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* BIN2 */
    GPIOMSP432_P8_6 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,
    /* CIN1 */
    GPIOMSP432_P8_7 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* CIN2 */
    GPIOMSP432_P9_1 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,
    /* DIN1 */
    GPIOMSP432_P8_3 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* DIN2 */
    GPIOMSP432_P5_3 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,
    /* CONFIG_TFT_CS */
    GPIOMSP432_P3_6 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* Infr_front1 */
    GPIOMSP432_P6_2 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* Infr_front2 */
    GPIOMSP432_P7_3 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* Infr_front3 */
    GPIOMSP432_P7_1 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* Infr_front4 */
    GPIOMSP432_P6_3 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* Infr_back1 */
    GPIOMSP432_P8_0 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* Infr_back2 */
    GPIOMSP432_P7_4 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* Infr_back3 */
    GPIOMSP432_P7_5 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* Infr_back4 */
    GPIOMSP432_P10_0 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* Infr_back5 */
    GPIOMSP432_P10_1 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    /* CONFIG_GPIO_0 : LaunchPad Button S1 (Left) */
    GPIOMSP432_P1_1 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING,
    /* CONFIG_GPIO_1 : LaunchPad Button S2 (Right) */
    GPIOMSP432_P1_4 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING,
    /* CONFIG_LED_0_GPIO : LaunchPad LED 1 Red */
    GPIOMSP432_P1_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* CONFIG_LED_1_GPIO : LaunchPad LED 2 Blue */
    GPIOMSP432_P2_2 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
};

/*
 *  ======== gpioCallbackFunctions ========
 *  Array of callback function pointers
 *
 *  NOTE: Unused callback entries can be omitted from the callbacks array to
 *  reduce memory usage by enabling callback table optimization
 *  (GPIO.optimizeCallbackTableSize = true)
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    /* CONFIG_TFT_RES */
    NULL,
    /* CONFIG_TFT_DC */
    NULL,
    /* CONFIG_EC11_A */
    NULL,
    /* CONFIG_EC11_B */
    NULL,
    /* CONFIG_EC11_BUTTON */
    NULL,
    /* TSL1401_SI_GPIO */
    NULL,
    /* TSL1401_CLK_GPIO */
    NULL,
    /* ENCODER_1A */
    NULL,
    /* ENCODER_1B */
    NULL,
    /* ENCODER_2A */
    NULL,
    /* ENCODER_2B */
    NULL,
    /* ENCODER_3A */
    NULL,
    /* ENCODER_3B */
    NULL,
    /* ENCODER_4A */
    NULL,
    /* ENCODER_4B */
    NULL,
    /* AIN1 */
    NULL,
    /* AIN2 */
    NULL,
    /* BIN1 */
    NULL,
    /* BIN2 */
    NULL,
    /* CIN1 */
    NULL,
    /* CIN2 */
    NULL,
    /* DIN1 */
    NULL,
    /* DIN2 */
    NULL,
    /* CONFIG_TFT_CS */
    NULL,
    /* Infr_front1 */
    NULL,
    /* Infr_front2 */
    NULL,
    /* Infr_front3 */
    NULL,
    /* Infr_front4 */
    NULL,
    /* Infr_back1 */
    NULL,
    /* Infr_back2 */
    NULL,
    /* Infr_back3 */
    NULL,
    /* Infr_back4 */
    NULL,
    /* Infr_back5 */
    NULL,
    /* CONFIG_GPIO_0 : LaunchPad Button S1 (Left) */
    NULL,
    /* CONFIG_GPIO_1 : LaunchPad Button S2 (Right) */
    NULL,
    /* CONFIG_LED_0_GPIO : LaunchPad LED 1 Red */
    NULL,
    /* CONFIG_LED_1_GPIO : LaunchPad LED 2 Blue */
    NULL,
};

/*
 *  ======== GPIOMSP432_config ========
 */
const GPIOMSP432_Config GPIOMSP432_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = 37,
    .numberOfCallbacks = 37,
    .intPriority = (~0)
};


/*
 *  =============================== I2C ===============================
 */

#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CMSP432.h>

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>
#include <ti/devices/msp432p4xx/driverlib/i2c.h>

#define CONFIG_I2C_COUNT 2

/*
 *  ======== i2cMSP432Objects ========
 */
I2CMSP432_Object i2cMSP432Objects[CONFIG_I2C_COUNT];

/*
 *  ======== i2cMSP432HWAttrs ========
 */
const I2CMSP432_HWAttrsV1 i2cMSP432HWAttrs[CONFIG_I2C_COUNT] = {
    /* CONFIG_I2C_0 */
    {
        .baseAddr = EUSCI_B1_BASE,
        .intNum = INT_EUSCIB1,
        .intPriority = (~0),
        .clockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
        .dataPin = I2CMSP432_P6_4_UCB1SDA,
        .clkPin  = I2CMSP432_P6_5_UCB1SCL
    },
    /* CONFIG_I2C_1 */
    {
        .baseAddr = EUSCI_B3_BASE,
        .intNum = INT_EUSCIB3,
        .intPriority = (~0),
        .clockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
        .dataPin = I2CMSP432_P10_2_UCB3SDA,
        .clkPin  = I2CMSP432_P10_3_UCB3SCL
    },
};

/*
 *  ======== I2C_config ========
 */
const I2C_Config I2C_config[CONFIG_I2C_COUNT] = {
    /* CONFIG_I2C_0 */
    {
        .fxnTablePtr = &I2CMSP432_fxnTable,
        .object = &i2cMSP432Objects[CONFIG_I2C_0],
        .hwAttrs = &i2cMSP432HWAttrs[CONFIG_I2C_0]
    },
    /* CONFIG_I2C_1 */
    {
        .fxnTablePtr = &I2CMSP432_fxnTable,
        .object = &i2cMSP432Objects[CONFIG_I2C_1],
        .hwAttrs = &i2cMSP432HWAttrs[CONFIG_I2C_1]
    },
};

const uint_least8_t I2C_count = CONFIG_I2C_COUNT;


/*
 *  =============================== PWM ===============================
 */

#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerMSP432.h>

/* include MSP432 driverlib definitions */
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/timer_a.h>

#define CONFIG_PWM_COUNT 10

/*
 *  ======== pwmMSP432Objects ========
 */
PWMTimerMSP432_Object pwmTimerMSP432Objects[CONFIG_PWM_COUNT];

/*
 *  ======== pwmMSP432HWAttrs ========
 */
const PWMTimerMSP432_HWAttrsV2 pwmTimerMSP432HWAttrs[CONFIG_PWM_COUNT] = {
    /* CONFIG_PWM_0 */
    /* LaunchPad LED 2 Red */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P2_0_TA1CCR4A, /* P2_0 */
    },
    /* CONFIG_PWM_1 */
    /* LaunchPad LED 2 Green */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P2_1_TA1CCR3A, /* P2_1 */
    },
    /* CONFIG_PWM_2 */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P2_3_TA1CCR2A, /* P2_3 */
    },
    /* CONFIG_PWM_3 */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P2_4_TA1CCR1A, /* P2_4 */
    },
    /* CONFIG_PWM_4 */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P5_6_TA2CCR1A, /* P5_6 */
    },
    /* CONFIG_PWM_5 */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P5_7_TA2CCR2A, /* P5_7 */
    },
    /* CONFIG_PWM_6 */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P10_5_TA3CCR1A, /* P10_5 */
    },
    /* CONFIG_PWM_7 */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P9_2_TA3CCR3A, /* P9_2 */
    },
    /* CONFIG_PWM_8 */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P9_3_TA3CCR4A, /* P9_3 */
    },
    /* CONFIG_PWM_10 */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P8_2_TA3CCR2A, /* P8_2 */
    },
};

/*
 *  ======== PWM_config ========
 */
const PWM_Config PWM_config[CONFIG_PWM_COUNT] = {
    /* CONFIG_PWM_0 */
    /* LaunchPad LED 2 Red */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_0],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_0]
    },
    /* CONFIG_PWM_1 */
    /* LaunchPad LED 2 Green */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_1],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_1]
    },
    /* CONFIG_PWM_2 */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_2],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_2]
    },
    /* CONFIG_PWM_3 */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_3],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_3]
    },
    /* CONFIG_PWM_4 */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_4],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_4]
    },
    /* CONFIG_PWM_5 */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_5],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_5]
    },
    /* CONFIG_PWM_6 */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_6],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_6]
    },
    /* CONFIG_PWM_7 */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_7],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_7]
    },
    /* CONFIG_PWM_8 */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_8],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_8]
    },
    /* CONFIG_PWM_10 */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_10],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_10]
    },
};

const uint_least8_t PWM_count = CONFIG_PWM_COUNT;


/*
 *  =============================== Power ===============================
 */

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>
extern void PowerMSP432_initPolicy(void);
extern void PowerMSP432_sleepPolicy(void);

const PowerMSP432_ConfigV1 PowerMSP432_config = {
    .policyInitFxn         = PowerMSP432_initPolicy,
    .policyFxn             = PowerMSP432_sleepPolicy,
    .initialPerfLevel      = 2,
    .enablePolicy          = true,
    .enablePerf            = true,
    .enableParking         = false,
    .resumeShutdownHookFxn = NULL,
    .customPerfLevels      = NULL,
    .numCustom             = 0,
    .useExtendedPerf       = false,
    .configurePinHFXT      = false,
    .HFXTFREQ              = 0,
    .bypassHFXT            = false,
    .configurePinLFXT      = false,
    .bypassLFXT            = false,
    .LFXTDRIVE             = 0,
    .enableInterruptsCS    = false,
    .priorityInterruptsCS  = (~0),
    .isrCS                 = NULL
};


/*
 *  =============================== SPI ===============================
 */

#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPIMSP432DMA.h>

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/dma.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>
#include <ti/devices/msp432p4xx/driverlib/spi.h>

#define CONFIG_SPI_COUNT 2

/*
 *  ======== spiMSP432DMAObjects ========
 */
SPIMSP432DMA_Object spiMSP432DMAObjects[CONFIG_SPI_COUNT];

/*
 *  ======== spiMSP432DMAHWAttrs ========
 */
const SPIMSP432DMA_HWAttrsV1 spiMSP432DMAHWAttrs[CONFIG_SPI_COUNT] = {
    /* CONFIG_SPI_0 */
    {
        .baseAddr = EUSCI_B0_BASE,
        .bitOrder = EUSCI_B_SPI_MSB_FIRST,
        .clockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
        .defaultTxBufValue = 0xFF,
        .intPriority = (~0),
        .dmaIntNum = INT_DMA_INT1,
        .rxDMAChannelIndex = DMA_CH3_EUSCIB0RX1,
        .txDMAChannelIndex = DMA_CH2_EUSCIB0TX1,
        .pinMode = EUSCI_SPI_3PIN,
        .clkPin  = SPIMSP432DMA_P1_5_UCB0CLK,
        .simoPin = SPIMSP432DMA_P1_6_UCB0SIMO,
        .somiPin = SPIMSP432DMA_P1_7_UCB0SOMI,
        .stePin  = SPIMSP432DMA_PIN_NO_CONFIG,
        .minDmaTransferSize = 10,
    },
    /* CONFIG_SPI_1 */
    {
        .baseAddr = EUSCI_B2_BASE,
        .bitOrder = EUSCI_B_SPI_MSB_FIRST,
        .clockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
        .defaultTxBufValue = ~0,
        .intPriority = (~0),
        .dmaIntNum = INT_DMA_INT0,
        .rxDMAChannelIndex = DMA_CH1_EUSCIB2RX2,
        .txDMAChannelIndex = DMA_CH0_EUSCIB2TX2,
        .pinMode = EUSCI_SPI_3PIN,
        .clkPin  = SPIMSP432DMA_P7_6_UCB2CLK,
        .simoPin = SPIMSP432DMA_P7_7_UCB2SIMO,
        .somiPin = SPIMSP432DMA_P7_0_UCB2SOMI,
        .stePin  = SPIMSP432DMA_PIN_NO_CONFIG,
        .minDmaTransferSize = 10,
    },
};

/*
 *  ======== SPI_config ========
 */
const SPI_Config SPI_config[CONFIG_SPI_COUNT] = {
    /* CONFIG_SPI_0 */
    {
        .fxnTablePtr = &SPIMSP432DMA_fxnTable,
        .object = &spiMSP432DMAObjects[CONFIG_SPI_0],
        .hwAttrs = &spiMSP432DMAHWAttrs[CONFIG_SPI_0]
    },
    /* CONFIG_SPI_1 */
    {
        .fxnTablePtr = &SPIMSP432DMA_fxnTable,
        .object = &spiMSP432DMAObjects[CONFIG_SPI_1],
        .hwAttrs = &spiMSP432DMAHWAttrs[CONFIG_SPI_1]
    },
};

const uint_least8_t SPI_count = CONFIG_SPI_COUNT;


/*
 *  =============================== Timer ===============================
 */

#include <ti/drivers/Timer.h>
#include <ti/drivers/timer/TimerMSP432.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>
#include <ti/devices/msp432p4xx/driverlib/timer_a.h>
#include <ti/devices/msp432p4xx/driverlib/timer32.h>

#define CONFIG_TIMER_COUNT 2

/*
 *  ======== timerMSP432Objects ========
 */
TimerMSP432_Object timerMSP432Objects[CONFIG_TIMER_COUNT];

/*
 *  ======== timerMSP432HWAttrs ========
 */
const TimerMSP432_HWAttrs TimerMSP432HWAttrs[CONFIG_TIMER_COUNT] = {
    /* CONFIG_TIMER_0 */
    {
        .timerBaseAddress  = TIMER32_1_BASE,
        .intNum            = INT_T32_INT2,
        .intPriority       = (~0),
    },
    /* CONFIG_TIMER_1 */
    {
        .timerBaseAddress  = TIMER32_0_BASE,
        .intNum            = INT_T32_INT1,
        .intPriority       = (~0),
    },
};

/*
 *  ======== Timer_config ========
 */
const Timer_Config Timer_config[CONFIG_TIMER_COUNT] = {
    /* CONFIG_TIMER_0 */
    {
        .fxnTablePtr = &TimerMSP432_Timer32_fxnTable,
        .object      = &timerMSP432Objects[CONFIG_TIMER_0],
        .hwAttrs     = &TimerMSP432HWAttrs[CONFIG_TIMER_0]
    },
    /* CONFIG_TIMER_1 */
    {
        .fxnTablePtr = &TimerMSP432_Timer32_fxnTable,
        .object      = &timerMSP432Objects[CONFIG_TIMER_1],
        .hwAttrs     = &TimerMSP432HWAttrs[CONFIG_TIMER_1]
    },
};

const uint_least8_t Timer_count = CONFIG_TIMER_COUNT;


/*
 *  =============================== UART ===============================
 */

#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>
#include <ti/devices/msp432p4xx/driverlib/uart.h>

#define CONFIG_UART_COUNT 4

UARTMSP432_Object uartMSP432Objects[CONFIG_UART_COUNT];

static const UARTMSP432_BaudrateConfig uartMSP432Baudrates[] = {
    /* {baudrate, input clock, prescalar, UCBRFx, UCBRSx, oversampling} */
    { 9600, 32768, 3, 0, 146, 0 },
    { 115200, 3000000, 1, 10, 0, 1 },
    { 115200, 6000000, 3, 4, 2, 1 },
    { 115200, 12000000, 6, 8, 32, 1 },
    { 115200, 24000000, 13, 0, 37, 1 },
};

static unsigned char uartMSP432RingBuffer0[32];
static unsigned char uartMSP432RingBuffer1[32];
static unsigned char uartMSP432RingBuffer2[32];
static unsigned char uartMSP432RingBuffer3[32];


static const UARTMSP432_HWAttrsV1 uartMSP432HWAttrs[CONFIG_UART_COUNT] = {
  {
    .baseAddr           = EUSCI_A2_BASE,
    .intNum             = INT_EUSCIA2,
    .intPriority        = (~0),
    .clockSource        = EUSCI_A_UART_CLOCKSOURCE_ACLK,
    .bitOrder           = EUSCI_A_UART_LSB_FIRST,
    .numBaudrateEntries = sizeof(uartMSP432Baudrates) /
                          sizeof(UARTMSP432_BaudrateConfig),
    .baudrateLUT        = uartMSP432Baudrates,
    .ringBufPtr         = uartMSP432RingBuffer0,
    .ringBufSize        = sizeof(uartMSP432RingBuffer0),
    .rxPin              = UARTMSP432_P2_5_UCA2RXD,
    .txPin              = UARTMSP432_P2_6_UCA2TXD,
    .errorFxn           = NULL
  },
  {
    .baseAddr           = EUSCI_A1_BASE,
    .intNum             = INT_EUSCIA1,
    .intPriority        = (~0),
    .clockSource        = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
    .bitOrder           = EUSCI_A_UART_LSB_FIRST,
    .numBaudrateEntries = sizeof(uartMSP432Baudrates) /
                          sizeof(UARTMSP432_BaudrateConfig),
    .baudrateLUT        = uartMSP432Baudrates,
    .ringBufPtr         = uartMSP432RingBuffer1,
    .ringBufSize        = sizeof(uartMSP432RingBuffer1),
    .rxPin              = UARTMSP432_P2_7_UCA1RXD,
    .txPin              = UARTMSP432_P3_0_UCA1TXD,
    .errorFxn           = NULL
  },
  {
    .baseAddr           = EUSCI_A3_BASE,
    .intNum             = INT_EUSCIA3,
    .intPriority        = (~0),
    .clockSource        = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
    .bitOrder           = EUSCI_A_UART_LSB_FIRST,
    .numBaudrateEntries = sizeof(uartMSP432Baudrates) /
                          sizeof(UARTMSP432_BaudrateConfig),
    .baudrateLUT        = uartMSP432Baudrates,
    .ringBufPtr         = uartMSP432RingBuffer2,
    .ringBufSize        = sizeof(uartMSP432RingBuffer2),
    .rxPin              = UARTMSP432_P9_6_UCA3RXD,
    .txPin              = UARTMSP432_P9_7_UCA3TXD,
    .errorFxn           = NULL
  },
  {
    .baseAddr           = EUSCI_A0_BASE,
    .intNum             = INT_EUSCIA0,
    .intPriority        = (~0),
    .clockSource        = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
    .bitOrder           = EUSCI_A_UART_LSB_FIRST,
    .numBaudrateEntries = sizeof(uartMSP432Baudrates) /
                          sizeof(UARTMSP432_BaudrateConfig),
    .baudrateLUT        = uartMSP432Baudrates,
    .ringBufPtr         = uartMSP432RingBuffer3,
    .ringBufSize        = sizeof(uartMSP432RingBuffer3),
    .rxPin              = UARTMSP432_P1_2_UCA0RXD,
    .txPin              = UARTMSP432_P1_3_UCA0TXD,
    .errorFxn           = NULL
  },
};

const UART_Config UART_config[CONFIG_UART_COUNT] = {
    {   /* CONFIG_UART_1 */
        .fxnTablePtr = &UARTMSP432_fxnTable,
        .object      = &uartMSP432Objects[0],
        .hwAttrs     = &uartMSP432HWAttrs[0]
    },
    {   /* CONFIG_UART_2 */
        .fxnTablePtr = &UARTMSP432_fxnTable,
        .object      = &uartMSP432Objects[1],
        .hwAttrs     = &uartMSP432HWAttrs[1]
    },
    {   /* CONFIG_UART_3 */
        .fxnTablePtr = &UARTMSP432_fxnTable,
        .object      = &uartMSP432Objects[2],
        .hwAttrs     = &uartMSP432HWAttrs[2]
    },
    {   /* CONFIG_UART_0 */
        .fxnTablePtr = &UARTMSP432_fxnTable,
        .object      = &uartMSP432Objects[3],
        .hwAttrs     = &uartMSP432HWAttrs[3]
    },
};

const uint_least8_t UART_count = CONFIG_UART_COUNT;


/*
 *  =============================== Button ===============================
 */
#include <ti/drivers/apps/Button.h>

Button_Object ButtonObjects[2];

static const Button_HWAttrs ButtonHWAttrs[2] = {
    /* CONFIG_BUTTON_0 */
    /* LaunchPad Button S1 (Left) */
    {
        .gpioIndex = CONFIG_GPIO_0
    },
    /* CONFIG_BUTTON_1 */
    /* LaunchPad Button S2 (Right) */
    {
        .gpioIndex = CONFIG_GPIO_1
    },
};

const Button_Config Button_config[2] = {
    /* CONFIG_BUTTON_0 */
    /* LaunchPad Button S1 (Left) */
    {
        .object = &ButtonObjects[CONFIG_BUTTON_0],
        .hwAttrs = &ButtonHWAttrs[CONFIG_BUTTON_0]
    },
    /* CONFIG_BUTTON_1 */
    /* LaunchPad Button S2 (Right) */
    {
        .object = &ButtonObjects[CONFIG_BUTTON_1],
        .hwAttrs = &ButtonHWAttrs[CONFIG_BUTTON_1]
    },
};

const uint_least8_t Button_count = 2;


/*
 *  =============================== LED ===============================
 */
#include <ti/drivers/apps/LED.h>

LED_Object LEDObjects[2];

static const LED_HWAttrs LEDHWAttrs[2] = {
    /* CONFIG_LED_0 */
    /* LaunchPad LED 1 Red */
    {
        .type = LED_GPIO_CONTROLLED,
        .index = CONFIG_LED_0_GPIO,
    },
    /* CONFIG_LED_1 */
    /* LaunchPad LED 2 Blue */
    {
        .type = LED_GPIO_CONTROLLED,
        .index = CONFIG_LED_1_GPIO,
    },
};

const LED_Config LED_config[2] = {
    /* CONFIG_LED_0 */
    /* LaunchPad LED 1 Red */
    {
        .object = &LEDObjects[CONFIG_LED_0],
        .hwAttrs = &LEDHWAttrs[CONFIG_LED_0]
    },
    /* CONFIG_LED_1 */
    /* LaunchPad LED 2 Blue */
    {
        .object = &LEDObjects[CONFIG_LED_1],
        .hwAttrs = &LEDHWAttrs[CONFIG_LED_1]
    },
};

const uint_least8_t LED_count = 2;


#include <ti/drivers/Board.h>

/*
 *  ======== Board_initHook ========
 *  Perform any board-specific initialization needed at startup.  This
 *  function is declared weak to allow applications to override it if needed.
 */
void __attribute__((weak)) Board_initHook(void)
{
}

/*
 *  ======== Board_init ========
 *  Perform any initialization needed before using any board APIs
 */
void Board_init(void)
{
    /* ==== /ti/drivers/Power initialization ==== */
    Power_init();

    Board_initHook();
}
