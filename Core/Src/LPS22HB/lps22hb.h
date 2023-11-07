#ifndef LPS22HB_H
#define LPS22HB_H

#include "main.h"

// https://www.st.com/en/mems-and-sensors/lps22hb.html

#define LPS22HB_DEVICE                             0xBAU
#define LPS22HB_WHO_AM_I                           0x0FU
#define LPS22HB_WHO_AM_I_VALUE                     0xB1U

#define LPS22HB_INTERRUPT_CFG                      0x0BU
#define LPS22HB_THS_P_L                            0x0CU
#define LPS22HB_THS_P_H                            0x0DU
#define LPS22HB_CTRL_REG1                          0x10U
#define LPS22HB_CTRL_REG2                          0x11U
#define LPS22HB_CTRL_REG3                          0x12U
#define LPS22HB_FIFO_CTRL                          0x14U
#define LPS22HB_REF_P_XL                           0x15U
#define LPS22HB_REF_P_L                            0x16U
#define LPS22HB_REF_P_H                            0x17U
#define LPS22HB_RPDS_L                             0x18U
#define LPS22HB_RPDS_H                             0x19U
#define LPS22HB_RES_CONF                           0x1AU
#define LPS22HB_INT_SOURCE                         0x25U
#define LPS22HB_FIFO_STATUS                        0x26U
#define LPS22HB_STATUS                             0x27U
#define LPS22HB_PRESS_OUT_XL                       0x28U
#define LPS22HB_PRESS_OUT_L                        0x29U
#define LPS22HB_PRESS_OUT_H                        0x2AU
#define LPS22HB_TEMP_OUT_L                         0x2BU
#define LPS22HB_TEMP_OUT_H                         0x2CU
#define LPS22HB_LPFP_RES                           0x33U

#define LPS22HB_CTRL1_ODR_POWERDOWN                0x0U
#define LPS22HB_CTRL1_ODR_1HZ                      0x10U
#define LPS22HB_CTRL1_ODR_10HZ                     0x20U
#define LPS22HB_CTRL1_ODR_25HZ                     0x30U
#define LPS22HB_CTRL1_ODR_50HZ                     0x40U
#define LPS22HB_CTRL1_ODR_75HZ                     0x50U
#define LPS22HB_CTRL1_LPFP_DISABLE                 0x0U
#define LPS22HB_CTRL1_LPFP_ENABLE                  0x8U
#define LPS22HB_CTRL1_LPFP_CFG_DISABLE             0x0U
#define LPS22HB_CTRL1_LPFP_CFG_ENABLE              0x4U
#define LPS22HB_CTRL1_BDU_CONTINUOUS_UPDATE        0x0U
#define LPS22HB_CTRL1_BDU_NOT_UPDATE_UNTIL_READING 0x2U
#define LPS22HB_CTRL1_SPI_4WIRE                    0x0U
#define LPS22HB_CTRL1_SPI_3WIRE                    0x1U

#define LPS22HB_CTRL2_BOOT_NORMAL                  0x0U
#define LPS22HB_CTRL2_BOOT_MEMORY_CONTENT          0x80U

#define LPS22HB_CTRL2_FIFO_DISABLE                 0x0U
#define LPS22HB_CTRL2_FIFO_ENABLE                  0x40U

#define LPS22HB_CTRL2_FIFO_WATERMARK_DISABLE       0x0U
#define LPS22HB_CTRL2_FIFO_WATERMARK_ENABLE        0x20U

#define LPS22HB_CTRL2_FIFO_ADD_INC_DISABLE         0x0U
#define LPS22HB_CTRL2_FIFO_ADD_INC_ENABLE          0x10U

#define LPS22HB_CTRL2_I2C_ENABLE                   0x0U
#define LPS22HB_CTRL2_I2C_DISABLE                  0x8U

#define LPS22HB_CTRL2_SWRESET_NORMAL               0x0U
#define LPS22HB_CTRL2_SWRESET_ENABLE               0x4U

#define LPS22HB_CTRL2_ONESHOT_IDLE                 0x0U
#define LPS22HB_CTRL2_ONESHOT_NEWDATASET           0x1U

#define LPS22HB_ERROR_NONE                         0
#define LPS22HB_ERROR_CBPTR                        1
#define LPS22HB_ERROR_WHOAMI                       2

struct lps22hb_init_config
{
	void    (*lps22hb_read_data)  (uint8_t, uint8_t*, uint8_t*, uint8_t);
	int     (*lps22hb_write_data) (uint8_t, uint8_t, const uint8_t*, uint8_t);
	uint8_t (*lps22hb_read_byte)  (uint8_t, uint8_t);
	int     (*lps22hb_write_byte) (uint8_t, uint8_t, uint8_t);

	uint8_t  interrupt_cfg;
	uint16_t ths_p;
	uint8_t  ctrl1;
	uint8_t  ctrl2;
	uint8_t  ctrl3;
	uint8_t  fifo_ctrl;
	uint32_t ref_p;
	uint16_t rpds;
	uint8_t  res_conf;
};

int lps22hb_init(struct lps22hb_init_config init_config);
float lps22hb_pressure();

#endif /* LPS22HB_H */
