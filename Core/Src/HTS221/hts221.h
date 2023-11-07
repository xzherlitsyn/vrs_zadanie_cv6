#ifndef HTS221_H
#define HTS221_H

#include "main.h"

// https://www.st.com/resource/en/datasheet/hts221.pdf

#define HTS221_DEVICE                             0xBEU
#define HTS221_WHO_AM_I                           0x0FU
#define HTS221_AV_CONF                            0x10U
#define HTS221_CTRL_REG1                          0x20U
#define HTS221_CTRL_REG2                          0x21U
#define HTS221_CTRL_REG3                          0x22U
#define HTS221_STATUS_REG                         0x27U
#define HTS221_HUMIDITY_OUT_L                     0x28U
#define HTS221_HUMIDITY_OUT_H                     0x29U
#define HTS221_TEMP_OUT_L                         0x2AU
#define HTS221_TEMP_OUT_H                         0x2BU

#define HTS221_WHO_AM_I_VALUE                     0xBCU

#define HTS221_AVGH4                              0x0U
#define HTS221_AVGH8                              0x1U
#define HTS221_AVGH16                             0x2U
#define HTS221_AVGH32                             0x3U
#define HTS221_AVGH64                             0x4U
#define HTS221_AVGH128                            0x5U
#define HTS221_AVGH256                            0x6U
#define HTS221_AVGH512                            0x7U

#define HTS221_AVGT2                              0x0U
#define HTS221_AVGT4                              0x8U
#define HTS221_AVGT8                              0x10U
#define HTS221_AVGT16                             0x18U
#define HTS221_AVGT32                             0x20U
#define HTS221_AVGT64                             0x28U
#define HTS221_AVGT128                            0x30U
#define HTS221_AVGT256                            0x38U

#define HTS221_CTRL1_POWER_UP                     0x80U
#define HTS221_CTRL1_POWER_DOWN                   0x0U
#define HTS221_CTRL1_BDU_CONTINUOUS_UPDATE        0x0U
#define HTS221_CTRL1_BDU_NOT_UPDATE_UNTIL_READING 0x4U
#define HTS221_CTRL1_OUTPUT_RATE_ONESHOT          0x0U
#define HTS221_CTRL1_OUTPUT_RATE_1HZ              0x1U
#define HTS221_CTRL1_OUTPUT_RATE_7HZ              0x2U
#define HTS221_CTRL1_OUTPUT_RATE_12_5HZ           0x3U

#define HTS221_CTRL2_BOOT_NORMAL                  0x0U
#define HTS221_CTRL2_BOOT_MEMORY_CONTENT          0x80U
#define HTS221_CTRL2_HEATER_DISABLE               0x0U
#define HTS221_CTRL2_HEATER_ENABLE                0x2U
#define HTS221_CTRL2_ONESHOT_WAIT                 0x0U
#define HTS221_CTRL2_ONESHOT_START                0x1U

#define HTS221_CTRL3_DRDY_H                       0x0U
#define HTS221_CTRL3_DRDY_L                       0x80U
#define HTS221_CTRL3_PUSHPULL                     0x0U
#define HTS221_CTRL3_OPENDRAIN                    0x40U
#define HTS221_CTRL3_DRDY_DISABLE                 0x0U
#define HTS221_CTRL3_DRDY_ENABLE                  0x4U

#define HTS221_ERROR_NONE                         0
#define HTS221_ERROR_CBPTR                        1
#define HTS221_ERROR_WHOAMI                       2
#define HTS221_ERROR_AV_CONF                      3
#define HTS221_ERROR_CTRL_REG1                    4
#define HTS221_ERROR_CTRL_REG2                    5
#define HTS221_ERROR_CTRL_REG3                    6

struct hts221_init_config
{
	void    (*hts221_read_data)  (uint8_t, uint8_t, uint8_t*, uint8_t);
	int     (*hts221_write_data) (uint8_t, uint8_t, const uint8_t*, uint8_t);
	uint8_t (*hts221_read_byte)  (uint8_t, uint8_t);
	int     (*hts221_write_byte) (uint8_t, uint8_t, uint8_t);

	uint8_t avconf;
	uint8_t ctrl1;
	uint8_t ctrl2;
	uint8_t ctrl3;
};

int  hts221_init(struct hts221_init_config init_config);
void hts221_status(uint8_t *humidity_available, uint8_t *temperature_available);
float hts221_humidity();
float hts221_temperature();

#endif /* HTS221_H */
