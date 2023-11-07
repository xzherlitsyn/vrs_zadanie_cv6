#include "lps22hb.h"

struct lps22hb_init_config lps22hb_config = {0};

void    (*lps22hb_read_data)  (uint8_t, uint8_t*, uint8_t*, uint8_t) = 0;
int     (*lps22hb_write_data) (uint8_t, uint8_t, const uint8_t*, uint8_t) = 0;
uint8_t (*lps22hb_read_byte)  (uint8_t, uint8_t) = 0;
int     (*lps22hb_write_byte) (uint8_t, uint8_t, uint8_t) = 0;

int lps22hb_init(struct lps22hb_init_config init_config)
{
	lps22hb_config = init_config;

	lps22hb_read_data  = lps22hb_config.lps22hb_read_data;
	lps22hb_write_data = lps22hb_config.lps22hb_write_data;
	lps22hb_read_byte  = lps22hb_config.lps22hb_read_byte;
	lps22hb_write_byte = lps22hb_config.lps22hb_write_byte;

	if (lps22hb_read_data  == 0 ||
	    lps22hb_write_data == 0 ||
		lps22hb_read_byte  == 0 ||
		lps22hb_write_byte == 0)
		return LPS22HB_ERROR_CBPTR;

	if (lps22hb_read_byte(LPS22HB_DEVICE, LPS22HB_WHO_AM_I) != LPS22HB_WHO_AM_I_VALUE)
		return LPS22HB_ERROR_WHOAMI;

	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_INTERRUPT_CFG, lps22hb_config.interrupt_cfg);
	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_THS_P_L, lps22hb_config.ths_p & 0xFFU);
	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_THS_P_H, lps22hb_config.ths_p & 0xFF00U);

	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_CTRL_REG1, lps22hb_config.ctrl1);
	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_CTRL_REG2, lps22hb_config.ctrl2);
	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_CTRL_REG3, lps22hb_config.ctrl3);
	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_FIFO_CTRL, lps22hb_config.fifo_ctrl);

	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_REF_P_XL, lps22hb_config.ref_p & 0xFFU);
	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_REF_P_L,  lps22hb_config.ref_p & 0xFF00U);
	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_REF_P_H,  lps22hb_config.ref_p & 0xFF0000U);

	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_RPDS_L, lps22hb_config.rpds & 0xFFU);
	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_RPDS_H, lps22hb_config.rpds & 0xFF00U);

	lps22hb_write_byte(LPS22HB_DEVICE, LPS22HB_RES_CONF, lps22hb_config.res_conf);

	return LPS22HB_ERROR_NONE;
}

void lps22hb_status(uint8_t *t_or, uint8_t *p_or, uint8_t *t_da, uint8_t *p_da)
{
	uint8_t status = lps22hb_read_byte(LPS22HB_DEVICE, LPS22HB_STATUS);
	*t_or = status & 0x20U;
    *p_or = status & 0x10U;
    *t_da = status & 0x2U;
    *p_da = status & 0x1U;
}

float lps22hb_pressure()
{
	uint8_t address[3] = {LPS22HB_PRESS_OUT_XL, LPS22HB_PRESS_OUT_L, LPS22HB_PRESS_OUT_H};
	uint32_t press;
	lps22hb_read_data(LPS22HB_DEVICE, address, (uint8_t*)&press, 3);
	return (float)press / 4096.0f;
}
