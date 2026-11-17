// ============================================================================
//  KART SECIMI — M5StickC Plus / M5StickC Plus2
// ============================================================================
//  Kartinin arkasinda "PLUS2" yaziyorsa Plus2, sadece "PLUS" yaziyorsa Plus.
//    1 = M5StickC Plus   (ESP32-PICO-D4, AXP192 guc yonetimi)
//    2 = M5StickC Plus2  (ESP32-PICO-V3-02, guc tutma pini GPIO4)
#pragma once

// Arduino IDE'de Araclar > Kart menusunden "M5StickCPlus" ya da "M5StickCPlus2"
// secildiyse kart OTOMATIK belirlenir. Baska bir kart secildiyse asagidaki
// M5_BOARD_MANUAL degeri kullanilir.
#define M5_BOARD_MANUAL 1

#if defined(ARDUINO_M5STACK_STICKC_PLUS2)
  #define M5_BOARD 2
#elif defined(ARDUINO_M5STACK_STICKC_PLUS)
  #define M5_BOARD 1
#else
  #define M5_BOARD M5_BOARD_MANUAL
#endif

#if M5_BOARD == 1
  #define BOARD_NAME   "M5StickC Plus"
  #define LCD_DC       23
  #define LCD_RST      18
  #define LCD_SPI_HZ   27000000
#elif M5_BOARD == 2
  #define BOARD_NAME   "M5StickC Plus2"
  #define LCD_DC       14
  #define LCD_RST      12
  #define LCD_SPI_HZ   40000000
#else
  #error "board_config.h: M5_BOARD 1 veya 2 olmali"
#endif

// PSRAM acikken derleyici ek kod uretir ve program hizli dahili RAM'e (IRAM)
// sigmaz. Bu proje PSRAM kullanmiyor.
#if defined(BOARD_HAS_PSRAM)
  #error "Arduino IDE: Araclar > PSRAM > Disabled secin (PSRAM acikken kod IRAM'e sigmiyor)."
#endif
