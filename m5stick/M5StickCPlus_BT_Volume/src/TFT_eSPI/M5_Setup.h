// TFT_eSPI ayari — M5StickC Plus / Plus2 (ST7789V2, 135x240)
// Bu dosya yalnizca bu projedeki TFT_eSPI kopyasini ayarlar; Arduino
// kutuphanelerindeki TFT_eSPI'ye (T-Display ayarina) dokunulmaz.
#pragma once
#include "../../board_config.h"

#define USER_SETUP_ID 225
#define USER_SETUP_INFO "M5StickC_Plus"
#define DISABLE_ALL_LIBRARY_WARNINGS   // dokunmatik yok: TOUCH_CS uyarisini gizle

#define ST7789_DRIVER
#define TFT_WIDTH   135
#define TFT_HEIGHT  240
#define CGRAM_OFFSET            // 52 / 40 ofsetleri kutuphane ekler

#define TFT_MISO    -1
#define TFT_MOSI    15
#define TFT_SCLK    13
#define TFT_CS      5
#define TFT_DC      LCD_DC
#define TFT_RST     LCD_RST
// Arka isik: Plus -> AXP192 LDO2, Plus2 -> GPIO27 PWM (programda yonetilir)

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8

#define SPI_FREQUENCY       LCD_SPI_HZ
#define SPI_READ_FREQUENCY  20000000
