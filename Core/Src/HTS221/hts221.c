#include "hts221.h"

struct hts221_init_config config = {0};

void    (*hts221_read_data)  (uint8_t, uint8_t*, uint8_t*, uint8_t) = 0;
int     (*hts221_write_data) (uint8_t, uint8_t, const uint8_t*, uint8_t) = 0;
uint8_t (*hts221_read_byte)  (uint8_t, uint8_t) = 0;
int     (*hts221_write_byte) (uint8_t, uint8_t, uint8_t) = 0;

int hts221_init(struct hts221_init_config init_config)
{
	config = init_config;

	hts221_read_data  = config.hts221_read_data;
	hts221_write_data = config.hts221_write_data;
	hts221_read_byte  = config.hts221_read_byte;
	hts221_write_byte = config.hts221_write_byte;

	if (hts221_read_data  == 0 ||
		hts221_write_data == 0 ||
	    hts221_read_byte  == 0 ||
		hts221_write_byte == 0)
		return HTS221_ERROR_CBPTR;

	if (hts221_read_byte(HTS221_DEVICE, HTS221_WHO_AM_I) != HTS221_WHO_AM_I_VALUE)
		return HTS221_ERROR_WHOAMI;

	hts221_write_byte(HTS221_DEVICE, HTS221_AV_CONF,   config.avconf);
	hts221_write_byte(HTS221_DEVICE, HTS221_CTRL_REG1, config.ctrl1);
	hts221_write_byte(HTS221_DEVICE, HTS221_CTRL_REG2, config.ctrl2);
	hts221_write_byte(HTS221_DEVICE, HTS221_CTRL_REG3, config.ctrl3);

	if (hts221_read_byte(HTS221_DEVICE, HTS221_AV_CONF) != config.avconf)
		return HTS221_ERROR_AV_CONF;

	if (hts221_read_byte(HTS221_DEVICE, HTS221_CTRL_REG1) != config.ctrl1)
		return HTS221_ERROR_CTRL_REG1;

	if (hts221_read_byte(HTS221_DEVICE, HTS221_CTRL_REG2) != config.ctrl2)
		return HTS221_ERROR_CTRL_REG2;

	if (hts221_read_byte(HTS221_DEVICE, HTS221_CTRL_REG3) != config.ctrl3)
		return HTS221_ERROR_CTRL_REG3;

	return HTS221_ERROR_NONE;
}

void hts221_status(uint8_t *humidity_available, uint8_t *temperature_available)
{
	uint8_t status = hts221_read_byte(HTS221_DEVICE, HTS221_STATUS_REG);
	*humidity_available    = (status & 0x2U) >> 1;
	*temperature_available = (status & 0x1U);
}

float hts221_humidity()
{
	uint8_t address[2] = {HTS221_HUMIDITY_OUT_L, HTS221_HUMIDITY_OUT_H};
	int16_t humidity;
	hts221_read_data(HTS221_DEVICE, address, (uint8_t*)&humidity, 2);
	return (float)(((humidity / -327) + 100) / 2);
}


float hts221_temperature()
{
	uint8_t address[2] = {HTS221_TEMP_OUT_L, HTS221_TEMP_OUT_H};
	int16_t temp;
	hts221_read_data(HTS221_DEVICE, address, (uint8_t*)&temp, 2);
	return (float)temp / 10.0f;
}
