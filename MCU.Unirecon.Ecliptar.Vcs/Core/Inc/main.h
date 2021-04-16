
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

void Error_Handler(void);

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LECTOR_HS_Pin GPIO_PIN_0
#define LECTOR_HS_GPIO_Port GPIOC
#define RECTOR_HS_Pin GPIO_PIN_1
#define RECTOR_HS_GPIO_Port GPIOC
#define FPV_Pin GPIO_PIN_3
#define FPV_GPIO_Port GPIOC
#define WIFI_TX_Pin GPIO_PIN_0
#define WIFI_TX_GPIO_Port GPIOA
#define WIFI_RX_Pin GPIO_PIN_1
#define WIFI_RX_GPIO_Port GPIOA
#define SERIAL_TX_Pin GPIO_PIN_2
#define SERIAL_TX_GPIO_Port GPIOA
#define SERIAL_RX_Pin GPIO_PIN_3
#define SERIAL_RX_GPIO_Port GPIOA
#define SD_SCK_Pin GPIO_PIN_5
#define SD_SCK_GPIO_Port GPIOA
#define SD_MISO_Pin GPIO_PIN_6
#define SD_MISO_GPIO_Port GPIOA
#define SD_MOSI_Pin GPIO_PIN_7
#define SD_MOSI_GPIO_Port GPIOA
#define TSONIC_TRIG_Pin GPIO_PIN_1
#define TSONIC_TRIG_GPIO_Port GPIOB
#define TSONIC_ECHO_Pin GPIO_PIN_2
#define TSONIC_ECHO_GPIO_Port GPIOB
#define MOTOR_PWR_Pin GPIO_PIN_10
#define MOTOR_PWR_GPIO_Port GPIOB
#define FSONIC_ECHO_Pin GPIO_PIN_13
#define FSONIC_ECHO_GPIO_Port GPIOB
#define FSONIC_TRIG_Pin GPIO_PIN_14
#define FSONIC_TRIG_GPIO_Port GPIOB
#define FBEAM_Pin GPIO_PIN_7
#define FBEAM_GPIO_Port GPIOC
#define LECTOR_EMP_Pin GPIO_PIN_8
#define LECTOR_EMP_GPIO_Port GPIOA
#define RECTOR_EMP_Pin GPIO_PIN_9
#define RECTOR_EMP_GPIO_Port GPIOA
#define TBEAM_Pin GPIO_PIN_10
#define TBEAM_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define IBUS_TX_Pin GPIO_PIN_12
#define IBUS_TX_GPIO_Port GPIOC
#define IBUS_RX_Pin GPIO_PIN_2
#define IBUS_RX_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LECTOR_SRV_Pin GPIO_PIN_4
#define LECTOR_SRV_GPIO_Port GPIOB
#define RECTOR_SRV_Pin GPIO_PIN_5
#define RECTOR_SRV_GPIO_Port GPIOB
#define SD_CS_Pin GPIO_PIN_6
#define SD_CS_GPIO_Port GPIOB


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

