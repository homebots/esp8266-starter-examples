#ifdef __cplusplus
extern "C"
{
#endif

#include "index.h"
#include "wifi.h"
#include "websocket.h"
#include "homebots.h"
#include "i2c.h"

#define WIDTH 128
#define HEIGHT 32

static bool is128_64 = HEIGHT == 64;

static char OWL[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x0A, 0x05, 0x0D, 0x01, 0x01, 0x03, 0x87, 0xFE, 0xFE, 0xFC, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0E,
  0x08, 0x0C, 0x0C, 0x0C, 0x0E, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0x30,
  0x98, 0xDE, 0xE6, 0xE7, 0xF7, 0xD7, 0xD6, 0x56, 0x56, 0xD7, 0xD7, 0x5F, 0xDF, 0x3F, 0x3F, 0x2F,
  0x9F, 0xD7, 0xDF, 0x6F, 0x6B, 0x6B, 0x7F, 0xF7, 0xF3, 0xF3, 0xE0, 0xEC, 0x98, 0x30, 0xE0, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x60, 0x70, 0x70, 0x60,
  0x40, 0x60, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xE0, 0xF0, 0xE0, 0x40, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFB, 0xE0,
  0xDF, 0xB1, 0xEF, 0x5F, 0xB9, 0xB0, 0xA0, 0xE6, 0x6E, 0x2E, 0xB6, 0xB9, 0x9F, 0xAF, 0xA0, 0xA7,
  0xBF, 0x99, 0xB6, 0xB6, 0xA6, 0xA6, 0xB0, 0xB0, 0xA9, 0xDF, 0xCF, 0xF0, 0x7F, 0x77, 0xFD, 0x01,
  0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x08, 0xEE, 0xB3, 0x7D, 0xBE, 0x7F, 0xC7, 0x87, 0xB7,
  0xB7, 0xB7, 0xCD, 0x7D, 0x83, 0x93, 0xFB, 0xCD, 0xB5, 0x35, 0xA5, 0x87, 0xCE, 0xFE, 0x1C, 0xF9,
  0xC3, 0x1C, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x3F, 0x3C, 0x73,
  0xEF, 0x9E, 0x7E, 0xFD, 0xFD, 0xED, 0xAD, 0xFD, 0xDD, 0xFF, 0xBF, 0xFF, 0x5F, 0xDF, 0xEF, 0xFF,
  0xFF, 0xFF, 0x6F, 0xFF, 0xDF, 0xEF, 0xFD, 0xDD, 0xFD, 0xBC, 0xFE, 0x7E, 0xBF, 0xEF, 0x7B, 0x3E,
  0x1F, 0x0F, 0x07, 0x00, 0x00, 0x0E, 0x1F, 0x3C, 0x77, 0x5F, 0x3D, 0x7D, 0xFB, 0xFB, 0x7A, 0xFA,
  0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0x7E, 0xFE, 0xBF, 0xFD, 0xF5, 0xF5, 0xF6, 0xFA, 0xFB, 0xDF,
  0x67, 0x78, 0x3C, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00,
  0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0xC0, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x00, 0x10, 0x38, 0x80, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x00, 0x81, 0x83, 0x83, 0x87, 0xE7, 0xEF, 0xEF, 0xEB, 0xFF, 0xF7, 0xDF,
  0xFA, 0xFE, 0xFF, 0xEB, 0xEE, 0xEE, 0xE7, 0x67, 0x63, 0x61, 0x60, 0x60, 0x60, 0xE0, 0xE0, 0xE0,
  0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x71, 0x73, 0x7B,
  0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x7F, 0x77, 0x76, 0x73, 0x73, 0x71, 0x71, 0x70, 0x71, 0x70,
  0x70, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF1, 0xF1, 0xF1, 0xE0, 0xE0,
  0xE8, 0xEC, 0xEE, 0xE7, 0xE2, 0xE4, 0xE8, 0xD0, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,

  0x10, 0x10, 0x10, 0x10, 0x1F, 0x13, 0xA1, 0xAD, 0xC8, 0x49, 0x47, 0x42, 0x40, 0xC0, 0xDC, 0x78,
  0x60, 0x60, 0x20, 0x21, 0x31, 0x30, 0x11, 0x1A, 0x1B, 0x0B, 0x0D, 0x0C, 0x04, 0x06, 0x06, 0x06,
  0x03, 0x03, 0x03, 0x13, 0x31, 0x71, 0x71, 0x61, 0x81, 0x81, 0x41, 0x20, 0x26, 0x0C, 0x1C, 0x30,
  0x78, 0x00, 0x00, 0x84, 0xC4, 0x84, 0x84, 0x0C, 0x04, 0x02, 0x02, 0x01, 0x01, 0x07, 0x0E, 0x0E,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xE0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xE0, 0xC0, 0xC0, 0x00, 0x00, 0x81, 0x81, 0x81, 0x41, 0x41,
  0x21, 0x21, 0x13, 0x13, 0x13, 0x03, 0x0B, 0x0B, 0x0B, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x07,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0x01, 0x00, 0x00, 0x06, 0x07, 0x03, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0C, 0x40, 0xEE, 0x79,
  0x35, 0x02, 0x08, 0x08, 0x04, 0x04, 0x02, 0x1F, 0x3D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
  0x04, 0x24, 0x00, 0x0D, 0x30, 0x31, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0xC6, 0xC6, 0x36, 0x68, 0x44,
  0xB4, 0xA0, 0x52, 0x62, 0x02, 0x02, 0x02, 0x00, 0x01, 0x03, 0x32, 0x7A, 0x3E, 0x1A, 0x02, 0x02,
  0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x1D, 0x3D, 0x1D, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8_t display_address;

void ICACHE_FLASH_ATTR ssd1306_writeBuffer(uint8_t* buffer, uint32_t length) {
  unsigned int i;

  i2c_start();
  i2c_writeByteAndAck(display_address);
  i2c_writeByteAndAck(buffer[0]);

  for (i = 1; i < length; i++) {
    i2c_writeByteAndAck(buffer[i]);
  }

  i2c_stop();
}

void ICACHE_FLASH_ATTR initDisplay() {
  uint8_t commands[] = {
    WRITE_COMMAND,

    DISPLAY_OFF,

    SET_CLOCK_DIVIDER,
   	0x80,

    SET_MULTIPLEX,
    HEIGHT - 1,

    SET_DISPLAY_OFFSET,
    0x00,

    SET_START_LINE | 0x0,

    CHARGE_PUMP,
    0x14,

    SET_MEMORY_MODE,
    0b10,

    X_MAP_INVERTED | 0x1,

    COM_DECREMENTAL,

    SET_LOW_COLUMN,

    SET_HIGH_COLUMN,

    SET_COM_PINS,
    is128_64 ? 0x12 : 0x02,

    SET_CONTRAST,
    is128_64 ? 0xcf : 0x8f,

    SET_PRECHARGE,
    0xf1,

    SET_VCOM_DETECT,
    0x20,

    DISPLA_SHOW_MEMORY,
    NORMAL_DISPLAY,
    DISPLAY_ON
  };

  ssd1306_writeBuffer(commands, sizeof(commands));
}

void ICACHE_FLASH_ATTR writeImage() {
  int sliceIndex, sliceStart, i;
  int slices = 8;
  int sliceLength = 128;
  uint8_t slice[sliceLength + 1];
  uint8_t pageSelect[2] = { WRITE_COMMAND, 0xb0 };
  uint8_t padding[5] = { WRITE_DATA, 0, 0, 0, 0 };

  slice[0] = WRITE_DATA;

  for (sliceIndex = 0; sliceIndex < slices; sliceIndex++) {
    sliceStart = sliceIndex * sliceLength;
    pageSelect[1] = 0xb0 + sliceIndex;

    for (i = 0; i <= sliceLength; i++) {
      slice[i + 1] = OWL[sliceStart + i];
    }

    ssd1306_writeBuffer(pageSelect, 2);
    ssd1306_writeBuffer(slice, sliceLength + 1);

    if (is128_64) {
      ssd1306_writeBuffer(padding, 5);
    }
  }
}

void setup() {
  i2c_gpio_init();

  PIN_PULLUP_EN(I2C_MASTER_SDA_MUX);
  PIN_PULLUP_EN(I2C_MASTER_SCL_MUX);

  display_address = i2c_findDevice();

  if (display_address) {
    initDisplay();
    writeImage();
  }
}

void loop() {}

#ifdef __cplusplus
}
#endif
