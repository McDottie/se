/**
* @file		ADXL345.h
* @brief	Contains the ADXL345 API.
* @version	1.1
* @date		8 Jan 2020
* @author	Jose Filipe Cruz dos Santos
*
*/


#ifndef ADX435_H_
#define ADX435_H_

#include <stdbool.h>

/** @defgroup ADXL345_DEVID
 * @{
 */
#define ADXL345_DEVID_RESET_VALUE    (0x00E5)
#define ADXL345_REG_DEVID            (0x00)
/**
 * @}
 */

/** @defgroup ADXL345_REGISTERS
 * @{
 */

/** @defgroup ADXL345_TAP_REGISTERS
 * @{
 */
#define ADXL345_REG_THRESH_TAP       (0x1D)
#define ADXL345_REG_DUR              (0x21)
#define ADXL345_REG_LATENT           (0x22)
#define ADXL345_REG_WINDOW           (0x23)
#define ADXL345_REG_THRESH_ACT       (0x24)
#define ADXL345_REG_TAP_AXES         (0x2A)
#define ADXL345_REG_ACT_TAP_STATUS   (0x2B)
/**
 * @}
 */

/** @defgroup ADXL345_CONFIG_REGISTERS
 * @{
 */
#define ADXL345_REG_BW_RATE          (0x2C)
#define ADXL345_REG_POWER_CTL        (0x2D)
/**
 * @}
 */

/** @defgroup ADXL345_INTERRUPT_REGISTERS
 * @{
 */
#define ADXL345_REG_INT_ENABLE       (0x2E)
#define ADXL345_REG_INT_MAP          (0x2F)
#define ADXL345_REG_INT_SOURCE          (0x30)
/**
 * @}
 */

/** @defgroup ADXL345_OUTPUT_DATA_REGISTERS
 * @{
 */
#define ADXL345_REG_DATA_FORMAT      (0x31)
#define ADXL345_REG_DATAX0           (0x32)
#define ADXL345_REG_DATAX1           (0x33)
#define ADXL345_REG_DATAY0           (0x34)
#define ADXL345_REG_DATAY1           (0x35)
#define ADXL345_REG_DATAZ0           (0x36)
#define ADXL345_REG_DATAZ1           (0x37)
/**
 * @}
 */

/**
 * @}
 */

/** @defgroup GRAVITY_CONSTANTS
 * @{
 */
#define ADXL345_GRAVITY_SUN          273.95f
#define ADXL345_GRAVITY_EARTH        9.80665f
#define ADXL345_GRAVITY_MOON         1.622f
#define ADXL345_GRAVITY_MARS         3.69f
#define ADXL345_GRAVITY_NONE         1.00f
/**
 * @}
 */

/**
 * Data Rates
 * */
typedef enum
{
    ADXL345_DATARATE_3200HZ    = 0b1111,
    ADXL345_DATARATE_1600HZ    = 0b1110,
    ADXL345_DATARATE_800HZ     = 0b1101,
    ADXL345_DATARATE_400HZ     = 0b1100,
    ADXL345_DATARATE_200HZ     = 0b1011,
    ADXL345_DATARATE_100HZ     = 0b1010,
    ADXL345_DATARATE_50HZ      = 0b1001,
    ADXL345_DATARATE_25HZ      = 0b1000,
    ADXL345_DATARATE_12_5HZ    = 0b0111,
    ADXL345_DATARATE_6_25HZ    = 0b0110,
    ADXL345_DATARATE_3_13HZ    = 0b0101,
    ADXL345_DATARATE_1_56HZ    = 0b0100,
    ADXL345_DATARATE_0_78HZ    = 0b0011,
    ADXL345_DATARATE_0_39HZ    = 0b0010,
    ADXL345_DATARATE_0_20HZ    = 0b0001,
    ADXL345_DATARATE_0_10HZ    = 0b0000
} ADX345_DataRate_t;

/**
 * Data Ranges
 * */
typedef enum
{
    ADXL345_RANGE_16G          = 0b11,
    ADXL345_RANGE_8G           = 0b10,
    ADXL345_RANGE_4G           = 0b01,
    ADXL345_RANGE_2G           = 0b00
} ADX345_Range_t;

/**
 * @brief Initializes the ADXL345 Peripheral 
 **/
int ADXL345_Init(void);

/**
 * @brief Configs taping
 * @param treshold defines treshold
 * @param duration defines max duration of tap
 * @param latency defines latency for double tap
 * @param window defines window for double tap
 */
void setupTap(float threshold,float duration, float latency,float window);

/**
 * @brief Checks if it was tapped can't be use with ADXL345_isDoubleTap
 */
bool ADXL345_isTap();

/**
 * @brief Checks if it was double tapped can't be use with ADXL345_isTap
 */
bool ADXL345_isDoubleTap();

/**
 * @brief Reads ADXL345 data
 * @param[out] x_value x axis value
 * @param[out] y_value y axis value
 * @param[out] z_value z axis value
 **/
int ADXL345_Read(float * x_value, float * y_value, float * z_value);

#endif /* ADXL345_H_ */

