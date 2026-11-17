/*
 * ============================================================================
 *  M5StickC Plus / Plus2  —  Bluetooth (BLE) Ses Kumandasi   (TTGO v3.3 portu)
 * ============================================================================
 *  Bu dosya TTGO T-Display surumunun M5StickC Plus / Plus2 icin uyarlamasidir.
 *  Kart ayari: board_config.h (Arduino IDE'de M5StickCPlus/Plus2 secilirse otomatik)
 *  Ekran surucusu projenin kendi klasorunde (src/TFT_eSPI): Arduino'daki TFT_eSPI
 *  kutuphanesinin T-Display ayarina DOKUNULMAZ, iki proje birbirini etkilemez.
 *  Kart bir BLE HID "Consumer Control" (medya tusu) cihazi gibi davranir.
 *  Telefon / tablet / PC ile eslesir ve Ses+ / Ses- / Sessiz tuslarini
 *  gonderir. Telefon bir Bluetooth hoparlore bagliysa, telefonun ses
 *  seviyesi (AVRCP Absolute Volume) hoparlore de yansir.
 *
 *  v4.2-M5: OBD2 gosterge paneli: tek veri/tek ekran, Sportif ve Konfor gorunumleri
 *  v4.3-M5: OBD2 sanal kokpit temalari (Klasik kadran, Dijital koseli, Kirmizi spor), depo hacmi ayari
 *  v4.4-M5: Wi-Fi senkron ekrani logodan sonra gorunur; OBD2 baglanti yokken de gostergeler + durum etiketi
 *  v4.5-M5: Kokpit temalarina dikey yerlesim; OBD ayarlarinda ekran yonu (yatay/dikey/ters/otomatik)
 *  v4.6-M5: OBD: 0-100 / 402 m olcer, canli grafik, yolculuk gecmisi (TL maliyet), otomatik uyku/uyanma
 *  v5.0-M5: Surus asistani: ayri OBD2 modu kaldirildi, OBD2 normal moda gomuldu (BLE ELM327,
 *        ses kumandasiyla ayni anda). Koruyucu stili "OBD2": M5 tusu arayuzleri degistirir.
 *        Ariza lambasi/kodlari, hararet, sarj, hiz ve vites uyarilari her ekranin ustunde.
 *  v5.1-M5: Wi-Fi ana programdan cikarildi (-690 KB flash, -22 KB RAM). Saat iPhone'dan (BLE
 *        Current Time) + BM8563 donanim saati. Hava karti (12 saat yagmur grafigi), ust seritte
 *        hava ozeti, hava uyarilari (buzlanma/yagmur/sis/ruzgar/sicak). OBD kokpitlerinde
 *        sahte dis sicaklik yerine aracin sensoru (yoksa motor suyu). Hava verisi: v5.2 Kestirme.
 *  v5.2-M5: iPhone baglantisi: AMS ile calan sarki (kart + ses ekraninda satir) ve telefon ses
 *        seviyesi halkaya yansir; ANCS ile Kestirmeler "MGMWX" bildirimi -> hava durumu
 *        (bildirim M5 tarafindan telefondan silinir). BLE MTU 185.
 *        Duzeltme: telefon servis kesfi eslesme bitip ekran tamponu geri alindiktan sonra
 *        (titreme), tampon alinamazsa bellek bosalinca yeniden denenir; ANCS icin tarama
 *        yanitinda servis istegi + onbellek yenileme.
 *  v5.2.2-M5: Kestirme satirinda [ ] ve listeli degerlere tolerans, 400 baytlik bildirim,
 *        seri portta bildirim tanisi ("[Bildirim] yeni/okunan").
 *  v5.2.3-M5: ANCS servis istegi reklam paketinde (iOS bildirim paylasimi), servis listesi tanisi.
 *  v5.2.5-M5: ANCS karakteristikleri ilk 32 bitle eslesir; eslesmeden sonra ekran tamponu
 *        alinamazsa temiz yeniden baslatma (titreme yok).
 *  v5.2.6-M5: telefon servis kurulumu BT gorevinden ana donguye alindi (kesif sonrasi takilma).
 *  v5.8-M5: I2C enkoder unitesi (Grove, 0x40; M5 Unit Encoder veya MGM_Encoder_C3 kutusu): cevir = ses /
 *        menude gezinme, kisa bas = OBD2 sonraki sayfa, cift = oynat/durdur, uzun = menu / geri.
 *        MGM kutusuysa (0xFF = 'M') ses % ve ekran adi kutunun OLED'ine yazilir (0x40 / 0x41).
 *  v5.7.1-M5: Telefon: servis arama bitis olayi gelmezse bulunan servislerle devam; hata sonrasi
 *        baglanti kapatilmaz (telefonu dusuruyordu), ayni baglantida arama yinelenir.
 *  v5.7-M5: Yeni goz: halka goz (acik cerceve, koyu ic, renkli iris, parlama). Hareket sensoru:
 *        egim/viraj -> gozler kayar, sarsinti -> sasirir, 3 kez sallama -> parcaciklara dagilip toplanir.
 *        Sahne nesneleri: uzun surus kahve molasi, yagmur semsiye, soguk kar, sicak gunes gozlugu,
 *        gece ay, ara sira kitap/kahve, yeni sarki notalar. Baglanti yokken 60 sn sonra uyur,
 *        telefon/OBD gelince alttan bakarak uyanir. Acilista parcaciklardan toplanir.
 *  v5.6.2-M5: Duzeltme: kayitli telefonda sifreleme olayi baglanti olayindan once gelince
 *        "sifrelenmedi" sanilip telefon servisleri (saat/muzik/hava) hic acilmiyordu.
 *  v5.6-M5: Arac sagligi: aku (mars ani voltaj gecmisi), isinma/termostat, ogrenilen normal motor
 *        sicakligina gore hararet egilimi, uzun donem yakit ayari, rolanti dalgalanmasi, bakim km.
 *        "Araç sağlığı" sayfasi + erken uyari kartlari. Ayarlar > OBD2: Bakım aralığı / Bakım yapıldı.
 *  v5.5-M5: Ses halkasi ekrani kaldirildi: ana ekran her zaman OBD2 (adaptor bagli olmasa da);
 *        OBD2 kapaliysa gozler. Koruyucu yalnizca gozler (OBD2 koruyucu stili yok).
 *  v5.4-M5: OBD2 adaptoru baglaninca ana ekran OBD2 arayuzleri (ses halkasi yerine), koruyucu gozler.
 *        Tuslar (ana ekran OBD2): A kisa = sonraki sayfa, A cift tik = sayfa islemi, B kisa = onceki
 *        sayfa, A/B basili = ses +/- (altta ses cubugu). Adaptor kopunca ses ekranina doner.
 *  v5.3-M5: Maskot koruyucusu kaldirildi (koruyucu: Gozler / OBD2). Gozlerin ustunde hava ozeti
 *        (ikon, sicaklik, yagmur %); yeni hava gelince 12 sn durum satiri. iPhone Kestirmeler ile
 *        hava durumu calisiyor (ANCS), GATT islem izi seri portta.
 *  v5.2.7-M5: acilista yeniden baslama nedeni; ANCS sonrasi cokme olursa sonraki acilista
 *        bildirimler kapali (cokme dongusu yok, saat/muzik calisir).
 *  v4.1-M5: Modern gozler (hale, gradyan, isilti, yansima, tanecikler, kalp gozler);
 *        OBD2 modu: ELM327 (Bluetooth klasik) ile panel/odak/grafik/yolculuk/ariza sayfalari
 *  v4.0-M5: Turkce karakter destegi (ç ğ ı ö ş ü Ç Ğ İ Ö Ş Ü °): fontlara eklendi,
 *        tum ekran yazilari Turkce yazim kurallarina gore duzeltildi; maskot %35 buyutuldu
 *  v3.9-M5: Yeni sade acilis logosu (beyaz zemin, cizgi cizgi yazilan geometrik MGM),
 *        Ayarlar > Acilis logosu: Sade / Klasik
 *  v3.8-M5: Maskot "Pit" koruyucusu: 13 kart tarzi animasyonlu sahne (sarj, tam dolu,
 *        muzik, beyzbol, baloncuk, sans kurdelesi, uyku, soguk/sicak...) + sahne sesleri
 *  v3.7-M5: IR veritabani: 290 farkli TV guc kodu (Flipper-IRDB), 26 marka/grup
 *        (Vestel/Regal/Finlux, Grundig/Arcelik/Beko, TCL, Hisense...), rastgele sirali tarama
 *  v3.6-M5: Karaoke modu: mikrofon -> Bluetooth hoparlor (eko efekti, gurultu kapisi);
 *        ayri acilista calisir, diger ozellikler kapali
 *  v3.5-M5: TV kumanda: kendi kumandani ogret (IR alici G26) ve alicisiz kod tarama;
 *        3 kumanda yuvasi x 6 tus (Teba vb. listede olmayan markalar icin)
 *  v3.4-M5: Otomatik ekran dondurme (MPU6886, 2 adimli ayar), mikrofona tepki
 *        veren gozler (alkis/ritim/muzik), buzzer ses efektleri + goz sesleri,
 *        IR TV kumanda sayfasi (tum TV'leri ac/kapa + marka bazli guc/ses/sessiz)
 *  v3.3: Acilis logosu: kullanicinin MGM BT VOLUME gorseli (logo_img.h), animasyonlu
 *  v3.2: Yeni acilis logosu (MGM + ses dugmesi); Wi-Fi guncellemesinden sonra
 *        temiz yeniden baslatma -> Wi-Fi'in birakmadigi ~20 KB RAM geri kazanilir
 *  v3.1: Eslesme sirasindaki ekran titremesi giderildi (tamponsuz cizimde
 *        ekran bir kez cizilir, sadece kucuk animasyon guncellenir)
 *  v3.0: Eslesme varsayilan olarak yine onaysiz ("Just Works", v1.x gibi).
 *        Kartta 6 haneli kod onayi Ayarlar > "Eşleşme onayı" ile acilabilir.
 *  v2.9: Eslesme resette kayboluyordu: RAM yetersizliginden NVS kaydi basarisiz
 *        oluyordu. Varsayilan hafif BT modu (+RAM), eslesmede ekran tamponu
 *        bosaltilir, kayit dogrulanir ve basarisizsa uyarilir.
 *  v2.8: Eslesmelerin kendiliginden silinmesi duzeltildi (sifreleme bitmeden
 *        baglanti kesilmiyor), silinen eslesme ekranda bildiriliyor
 *  v2.7: Ayarlar: buyuk ikonlu, saga-sola kayan menu
 *  v2.6: Acilista "MGM", Cihaz bilgisi: Programci: MERT GUZELMANSUR
 *  v2.5: Wi-Fi yalnizca acilista (Bluetooth'tan once) -> donma/RAM sorunu yok
 *  v2.4: Wi-Fi ile saat + hava durumu (ara sira baglanir), yeni ses kapsulu
 *  v2.3: Medya tuslari (cift tik), uyku zamanlayicisi, kamera deklansoru,
 *        tercihli cihaz, etkilesimli gozler, sarj animasyonu + pil uyarilari
 *  v2.2: "Bağlantıyı kes" -> gizli mod (eski cihaz geri baglanamaz),
 *        eslesme modunda yeni cihaz her zaman baglanabilir
 *  v2.1: Ekran koruyucu — duygusal robot gozleri (Menu > Ekran koruyucu)
 *
 *  v2.0 yenilikleri
 *   - Kart uzerinden eslestirme: yakindaki cihazlari ISIMLERIYLE arar,
 *     hedef secilir, eslesme istegi kartta 6 haneli kodla ONAYLANIR
 *   - Eslesen cihazlarin isimleri kaydedilir (kayitli cihaz listesi,
 *     ust bilgi satiri)
 *   - Dikey ekran duzeni (Yatay / Yatay ters / Dikey / Dikey ters)
 *
 *  TUSLAR — Normal ekran (bagli)
 *   A (on, M5)      : Ses +       (basili tut = hizli)   cift tik: Sonraki sarki
 *   B (yan)         : Ses -       (basili tut = hizli)   cift tik: Onceki sarki
 *   GUC (sol yan)   : kisa basis = Menu ac / geri  (6 sn basili = kapat)
 *   Ikisi kisa      : Oynat/Duraklat (Ayarlar'dan "Sessiz" yapilabilir)
 *   Ikisi 1.5 sn    : SENKRON (birak)
 *   Ikisi 3 sn      : MENU
 *  TUSLAR — Bagli degilken : herhangi bir tus MENU'yu acar
 *  TUSLAR — Menu    : B sonraki | A sec | A uzun geri | GUC geri
 *  TUSLAR — Eslesme istegi : A ONAYLA | B REDDET
 *  TUSLAR — Dondurme ayari: A KAYDET | B / GUC IPTAL
 *
 *  YENI (v3.4-M5)
 *   - Ayarlar > Otomatik dondurme: ilk acista 2 adimli ayar ister (cihazi dik,
 *     sonra yan tutup A'ya basin). Masada duz yatarken yon degismez.
 *   - Ayarlar > Mikrofon tepkisi: koruyucuda gozler alkisa sasirir, ritimde
 *     ziplar, muzikte dans eder. Mikrofon yalnizca koruyucu acikken calisir.
 *   - Ayarlar > Ses efektleri (Kapali/Kisik/Normal), Goz sesleri (Acik/Kapali)
 *   - Menu > TV kumanda (IR): tum TV'leri ac/kapa + marka bazli Guc/Ses/Sessiz
 *   - Menu > Karaoke modu: yeniden baslar, BT hoparlor arar/baglanir.
 *     A/B mikrofon +/-, A+B eko, A uzun hoparlor degistir, GUC normal moda don
 *   - TV kumanda > + Yeni kumanda: "Kod tara" (ek parca yok) veya "Tuş öğret"
 *     (VS1838B/KY-022 alici: VCC->3V3, GND->GND, OUT->G26; 5V'a BAGLAMAYIN)
 *
 *  Gereksinimler
 *   - Arduino-ESP32 core 3.2.0 (test edildi) veya 2.0.17
 *   - Seri port testi (115200): "MGMWX;T=18;C=Yagmurlu;W=14;L=11;H=21;S=14;PT=18/19/..;PP=5/10/.."
 *     hava verisi yukler, "MGMTIME=<epoch UTC>" saati ayarlar
 *   - Ek kutuphane GEREKMEZ (TFT_eSPI kopyasi src/ klasorunde)
 *   - Kart (core 3.x): "M5StickCPlus" veya "M5StickCPlus2",
 *     Partition Scheme: "Huge APP (3MB No OTA/1MB SPIFFS)"
 *   - Kart (core 2.0.17): "M5Stick-C", Partition: "No OTA (Large APP)",
 *     board_config.h icinde M5_BOARD_MANUAL dogru secilmeli
 * ============================================================================
 */

#include <Arduino.h>
// v5.1: Wi-Fi/HTTP/NTP ana programdan cikarildi (saat iPhone + BM8563, hava Kestirmeler)
#include <esp_heap_caps.h>
#include <Wire.h>
#include "soc/gpio_reg.h"
#include "board_config.h"
#include "src/TFT_eSPI/TFT_eSPI.h"      // projeye ozel kopya (M5 ayarli)

// ---- Ekran ayar kontrolu ---------------------------------------------------
#if !defined(ST7789_DRIVER) || (TFT_MOSI != 15) || (TFT_SCLK != 13) || \
    (TFT_CS != 5) || (TFT_DC != LCD_DC) || (TFT_RST != LCD_RST) || (TFT_WIDTH != 135)
  #error "Ekran ayari hatali: src/TFT_eSPI/M5_Setup.h ve board_config.h dosyalarini kontrol edin."
#endif

#include <Preferences.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEHIDDevice.h>
#include <BLESecurity.h>
#include <HIDTypes.h>
#include <esp_gap_ble_api.h>
#include <esp_bt_main.h>
#include <esp_bt.h>
#include <esp_gattc_api.h>
#include <esp_gatt_common_api.h>
#include <esp_gap_bt_api.h>
#include <esp_sleep.h>
#include <nvs.h>
#include <esp_heap_caps.h>

// Menu govde satiri (Arduino IDE otomatik prototipleri icin en ustte tanimli)
struct BodyLine { char txt[40]; uint16_t col; uint8_t font; int8_t uml; };   // uml: ustune iki nokta konacak harf

// ============================================================================
//  SABIT AYARLAR
// ============================================================================
#define FW_VERSION         "5.8.1-M5"
#define DEVELOPER_TXT      "MERT GÜZELMANSUR"   // "U" uzerine iki nokta cizilir (GUZEL -> GUZEL)
#define DEVICE_NAME        "M5Stick Ses"
#define MANUFACTURER       "MGM Lab"

#define PIN_BTN_UP         37       // A tusu (on, "M5")  — tuslar ters ise degistirin
#define PIN_BTN_DOWN       39       // B tusu (yan)

// Opsiyonel KY-040 rotary encoder (1 = aktif)
#define USE_ENCODER        0
#define PIN_ENC_A          32       // Grove portu (G32 / G33)
#define PIN_ENC_B          33
#define PIN_ENC_SW         26       // HAT pini G26
#define ENC_STEPS_PER_DETENT 4

// Opsiyonel IR alici (VS1838B / KY-022) — kumanda ogrenme icin
//  VCC -> 3V3 (ust baglanti) | GND -> GND | OUT (S) -> G26    (5V'a BAGLAMAYIN)
#define PIN_IR_RX          26
#if USE_ENCODER && (PIN_ENC_SW == PIN_IR_RX)
  #error "Encoder tusu ve IR alici ayni pinde (G26): birini degistirin."
#endif

// Zamanlamalar (ms)
#define COMBO_WINDOW_MS    70
#define REPEAT_DELAY_MS    420
#define REPEAT_START_MS    140
#define REPEAT_MIN_MS      55
#define SYNC_HOLD_MS       1500
#define MENU_HOLD_MS       3000
#define MENU_LONG_MS       650
#define MENU_TIMEOUT_MS    30000
#define KEY_PRESS_MS       18
#define KEY_GAP_MS         32
#define DIM_AFTER_MS       20000
#define OFF_AFTER_MS       60000
#define LIMIT_FLASH_MS     500
#define ENC_GRACE_MS       8000     // yeni baglantida sifreleme icin taninan sure
#define DBL_CLICK_MS       260      // cift tik penceresi
#define STREAK_MS          700      // seri basista cift tik aranmaz
#define FADE_MS            90000UL  // uyku zamanlayicisi: sesi kisma suresi
#define PAIR_WINDOW_MS     90000    // Eslesme modu suresi
#define CONFIRM_TIMEOUT_MS 30000    // Kod onay suresi
#define SCAN_INQ_LEN       8        // x1.28 sn  (~10 sn arama)

#define BL_DIM             28

static const uint8_t  STEP_OPTS[]   = {15, 16, 20, 25, 30, 10};
static const uint8_t  BRIGHT_OPTS[] = {255, 190, 128, 64};
static const uint8_t  BRIGHT_PCT[]  = {100, 75, 50, 25};
static const uint16_t SLEEP_MIN[]   = {0, 5, 10, 30};
static const uint8_t  ORIENT_ROT[]  = {1, 3, 0, 2};
static const char*    ORIENT_TXT[]  = {"Yatay", "Yatay ters", "Dikey", "Dikey ters"};
static const uint16_t SAVER_SEC[]   = {0, 10, 30, 60};          // 0 = kapali
static const char*    SAVER_TXT[]   = {"Kapalı", "10 sn", "30 sn", "1 dk"};
static const char*    EYE_TXT[]     = {"Mavi", "Yeşil", "Turuncu", "Pembe", "Beyaz"};
static const char*    SND_TXT[]     = {"Kapalı", "Kısık", "Normal"};
static const char*    STYLE_TXT[]   = {"Gözler", "Gözler", "Gözler", "OBD2"};   // 1-2 eski maskot stilleri
#define COUNT_OF(a) (sizeof(a) / sizeof((a)[0]))

// ============================================================================
//  RENK PALETI (RGB565)
// ============================================================================
#define C_BG        0x0000
#define C_PANEL     0x18E3
#define C_TRACK     0x2124
#define C_TEXT      0xFFFF
#define C_TEXT2     0xBDF7
#define C_SUBTEXT   0x8C71
#define C_ACCENT    0x05FF
#define C_BT        0x1C9F
#define C_MIN       0x2D7F
#define C_OK        0x07E8
#define C_WARN      0xFE60
#define C_BAD       0xF904
#define C_MENU      0x901F

// ============================================================================
//  HID (Consumer Control)
// ============================================================================
#define REPORT_ID_CONSUMER 1

enum : uint16_t {
  CC_NONE       = 0x0000,
  CC_PLAY_PAUSE = 0x00CD,
  CC_NEXT       = 0x00B5,
  CC_PREV       = 0x00B6,
  CC_MUTE       = 0x00E2,
  CC_VOL_UP     = 0x00E9,
  CC_VOL_DOWN   = 0x00EA,
};

static const uint8_t HID_REPORT_MAP[] = {
  0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01,
  0x85, REPORT_ID_CONSUMER,
  0x15, 0x00, 0x26, 0xFF, 0x03,
  0x19, 0x00, 0x2A, 0xFF, 0x03,
  0x75, 0x10, 0x95, 0x01,
  0x81, 0x00,
  0xC0
};

// ============================================================================
//  YARDIMCI: UTF-8 -> ASCII (ekran fontlarinda Turkce karakter yok)
// ============================================================================
void toAscii(const uint8_t* in, size_t inLen, char* out, size_t outLen) {
  size_t o = 0;
  for (size_t i = 0; i < inLen && in[i] && o + 1 < outLen; ) {
    uint8_t c = in[i];
    if (c < 0x80) { out[o++] = (c >= 0x20) ? c : ' '; i++; continue; }
    if ((c & 0xE0) == 0xC0 && i + 1 < inLen) {
      uint16_t u = ((c & 0x1F) << 6) | (in[i + 1] & 0x3F);
      char r = '?';
      switch (u) {
        case 0xE7: case 0xC7: case 0x11F: case 0x11E: case 0x131: case 0x130:
        case 0xF6: case 0xD6: case 0x15F: case 0x15E: case 0xFC: case 0xDC:
          if (o + 2 < outLen) { out[o++] = in[i]; out[o++] = in[i + 1]; }   // Turkce harf: UTF-8 korunur
          i += 2; continue;
        case 0xE9: case 0xE8: case 0xEB: r = 'e'; break;
        case 0xE1: case 0xE0: case 0xE2: case 0xE4: r = 'a'; break;
      }
      out[o++] = r; i += 2; continue;
    }
    if ((c & 0xF0) == 0xE0 && i + 2 < inLen) {
      uint16_t u = ((c & 0x0F) << 12) | ((in[i + 1] & 0x3F) << 6) | (in[i + 2] & 0x3F);
      if (u == 0x2018 || u == 0x2019) out[o++] = '\'';
      else if (u == 0x201C || u == 0x201D) out[o++] = '"';
      else if (u == 0x2013 || u == 0x2014) out[o++] = '-';
      i += 3; continue;
    }
    if ((c & 0xF8) == 0xF0) { i += 4; continue; }   // emoji vb. atla
    i++;
  }
  // bas/son bosluklari kirp
  while (o > 0 && out[o - 1] == ' ') o--;
  out[o] = 0;
}

void fmtAddr(const uint8_t* a, char* out, size_t len) {
  snprintf(out, len, "%02X:%02X:%02X:%02X:%02X:%02X", a[0], a[1], a[2], a[3], a[4], a[5]);
}

bool strEqI(const char* a, const char* b) {
  for (; *a && *b; a++, b++) if (tolower(*a) != tolower(*b)) return false;
  return *a == *b;
}

// ============================================================================
//  GLOBAL NESNELER
// ============================================================================
TFT_eSPI     tft;
TFT_eSprite  spr(&tft);
Preferences  prefs;
Preferences  prefsNames;
bool         useSprite = false;
bool         directMode = false;   // eslesme sirasinda tampon bos: dogrudan, titremesiz cizim

BLEServer*         bleServer = nullptr;
BLEHIDDevice*      bleHid    = nullptr;
BLECharacteristic* bleInput  = nullptr;

// ---- Bagli merkezler (en fazla 3) -------------------------------------------
uint32_t authPendAt = 0; uint8_t authPendAddr[6] = {0};   // baglantidan ONCE gelen sifreleme olayi
struct Peer {
  bool                used = false;
  uint16_t            connId = 0;
  esp_bd_addr_t       bda = {0};
  esp_ble_addr_type_t type = BLE_ADDR_TYPE_PUBLIC;
  uint32_t            at = 0;
  bool                checked = false;   // tercih kontrolu yapildi
  bool                rejected = false;  // tercih edilmedigi icin atildi
  bool                authed = false;    // sifreleme (kimlik dogrulama) tamamlandi
  uint32_t            authAt = 0;
  esp_bd_addr_t       idAddr = {0};      // sifreleme sonrasi kimlik adresi
  bool                kick = false;      // sifreleme bitince baglantiyi kes
  bool                knownAtConn = false; // baglandiginda zaten kayitliydi
};
constexpr int MAX_PEERS = 3;
Peer peers[MAX_PEERS];

volatile bool     bleConnected  = false;
volatile bool     bleNeedAdvert = false;
volatile bool     bleNewConn    = false;
bool              bleHidden     = false;   // true: reklam kapali, kimse baglanamaz
volatile uint32_t bleConnAt     = 0;
volatile int      bleActive     = -1;      // en son baglanan peer indeksi

int peerCount() { int n = 0; for (auto& p : peers) if (p.used) n++; return n; }
int peerFindAddr(const uint8_t* a) {
  for (int i = 0; i < MAX_PEERS; i++) if (peers[i].used && !memcmp(peers[i].bda, a, 6)) return i;
  return -1;
}

// ============================================================================
//  KALICI AYARLAR
// ============================================================================
struct Config {
  uint8_t stepIdx   = 0;
  uint8_t brightIdx = 0;
  uint8_t sleepIdx  = 2;
  uint8_t orient    = 0;   // ORIENT_ROT indeksi
  uint8_t saverIdx  = 2;   // SAVER_SEC indeksi (30 sn)
  uint8_t eyeIdx    = 0;   // EYE_TXT indeksi
  uint8_t dblClick  = 1;   // cift tik medya
  uint8_t bothAct   = 0;   // iki tus kisa: 0 oynat/duraklat, 1 sessiz
  uint8_t wxAlerts  = 1;   // hava uyarilari (buzlanma, yagmur, sis, ruzgar, sicak)
  uint8_t classicScan = 0; // isimli arama (klasik BT) — cok RAM kullanir
  uint8_t pairConfirm = 0; // 1: yeni eslesmede kartta 6 haneli kod onayi
  uint8_t autoRot   = 0;   // hareket sensoruyle otomatik dondurme
  uint8_t rotCal    = 0;   // dondurme ayari (bit7 gecerli, eksen/yon bitleri)
  uint8_t micOn     = 1;   // koruyucuda mikrofona tepki veren gozler
  uint8_t sndLvl    = 2;   // ses efektleri: 0 kapali, 1 kisik, 2 normal
  uint8_t sndEyes   = 1;   // koruyucuda ara sira goz sesleri
  uint8_t saverStyle = 0;  // koruyucu: 0 gozler, 3 OBD2 arayuzleri (1-2 eski maskot -> 0)
  uint8_t logoStyle = 1;   // acilis logosu: 0 klasik (gorsel), 1 sade (beyaz + MGM)
} cfg;

static inline int      volSteps()   { return STEP_OPTS[cfg.stepIdx]; }
static inline uint8_t  blFull()     { return BRIGHT_OPTS[cfg.brightIdx]; }
static inline uint32_t sleepAfter() { return (uint32_t)SLEEP_MIN[cfg.sleepIdx] * 60000UL; }

// ============================================================================
//  EKRAN GEOMETRISI (yatay / dikey)
// ============================================================================
int  SW = 240, SH = 135;
bool portrait = false;
struct Gauge { int cx, cy, rOut, rIn, a0, a1; } gz;

void updateLayout() {
  portrait = (ORIENT_ROT[cfg.orient] % 2) == 0;
  SW = portrait ? 135 : 240;
  SH = portrait ? 240 : 135;
  if (portrait) gz = {67, 112, 64, 54, 45, 315};    // 270 derece halka
  else          gz = {120, 112, 88, 74, 90, 270};   // 180 derece yay
}

// ============================================================================
//  UYGULAMA DURUMU
// ============================================================================
struct AppState {
  int      level        = 7;
  bool     muted        = false;
  float    shownLevel   = 0;
  int      batteryPct   = -1;
  float    batteryV     = 0;
  bool     usbPower     = false;
  bool     syncing      = false;
  float    comboProgress = 0;
  uint32_t lastInput    = 0;
  uint32_t lastChange   = 0;
  bool     dirtySave    = false;
  uint32_t limitFlashAt = 0;
  bool     saver        = false;     // ekran koruyucu aktif
  bool     camera       = false;     // kamera deklansoru modu
  bool     nightOff     = false;     // uyku zamanlayicisi ekrani kapatti
  uint32_t chargeAnimUntil = 0;
  uint32_t lastVolAt    = 0;
  uint32_t volShowAt    = 0;       // OBD2 ana ekranda ses cubugu
  uint32_t lastRejectAt = 0;
  bool     classicOk    = true;      // klasik BT (isimli arama) acik mi
  uint32_t restartAt    = 0;         // >0: bu zamanda yeniden basla
  uint8_t  restartReq   = 0;         // yeniden baslama nedeni (BOOT_*)
  char     peerName[32] = {0};      // aktif bagli cihazin adi
  char     toast[48]    = {0};
  uint32_t toastUntil   = 0;
  uint16_t toastColor   = C_ACCENT;
} app;

// ============================================================================
//  CIHAZ ISIMLERI (NVS)
// ============================================================================
namespace Names {
  void key(const uint8_t* a, char* k) {
    snprintf(k, 16, "n%02x%02x%02x%02x%02x%02x", a[0], a[1], a[2], a[3], a[4], a[5]);
  }
  bool get(const uint8_t* a, char* out, size_t len) {
    char k[16]; key(a, k);
    out[0] = 0;
    if (!prefsNames.isKey(k)) return false;
    prefsNames.getString(k, out, len);
    return out[0] != 0;
  }
  void set(const uint8_t* a, const char* name) {
    if (!name || !name[0]) return;
    char k[16]; key(a, k);
    char old[32];
    if (get(a, old, sizeof(old)) && !strcmp(old, name)) return;
    prefsNames.putString(k, name);
  }
  void remove(const uint8_t* a) { char k[16]; key(a, k); prefsNames.remove(k); }
  void clearAll() { prefsNames.clear(); }
}

// ============================================================================
//  BLE
// ============================================================================
void showToast(const char* msg, uint16_t color = C_ACCENT, uint32_t ms = 900);
void startSaver();
void pairMemPrepare();
void eyesVol(bool up, bool limit);
void eyesMedia(int dir);
void eyesPlay();
void eyesMute(bool m);
void eyesSleep();
void eyesTired();

// OBD2 surus asistani (tanimi asagida, namespace Obd)
namespace Obd {
  namespace Link {
    void onGap(esp_gap_ble_cb_event_t ev, esp_ble_gap_cb_param_t* p);
    void onGattc(esp_gattc_cb_event_t e, esp_gatt_if_t ifc, esp_ble_gattc_cb_param_t* p);
  }
  extern bool menu;
  bool on(); void begin(); void loop(); void openSettings(); void settingsButtons(); void drawSettings();
  void drawSaver(); void saverKey(bool lng); bool alertActive(); bool alertBlocking(); void ackAlert();
  void drawAlert(); bool keepAwake();
  bool dashActive(); bool linked(); void prevPage(); uint32_t driveSec(); void encKey(int8_t a); const char* curPageName();   // OBD2 ana ekran
}
namespace Info { bool active(); void ack(); void draw(); void service(); }   // hava uyarilari (dosya sonunda)

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer*, esp_ble_gatts_cb_param_t* p) override {
    if (p->connect.link_role == 0) return;                // biz merkeziz (OBD adaptoru): telefon degil
    int slot = -1;
    for (int i = 0; i < MAX_PEERS; i++) if (!peers[i].used) { slot = i; break; }
    if (slot < 0) slot = 0;
    Peer& pr = peers[slot];
    pr.used = true;
    pr.connId = p->connect.conn_id;
    memcpy(pr.bda, p->connect.remote_bda, 6);
    pr.type = p->connect.ble_addr_type;
    pr.at = millis();
    pr.checked = false; pr.rejected = false;
    pr.authed = false; pr.authAt = 0; pr.kick = false; memcpy(pr.idAddr, pr.bda, 6);
    bleActive    = slot;
    bleConnAt    = millis();
    bleConnected = true;
    bleNewConn   = true;
  }
  void onDisconnect(BLEServer*, esp_ble_gatts_cb_param_t* p) override {
    bool mine = false;
    for (int i = 0; i < MAX_PEERS; i++)
      if (peers[i].used && peers[i].connId == p->disconnect.conn_id) { peers[i].used = false; mine = true; }
    if (!mine) return;                                    // OBD adaptoru koptu: telefonu etkilemez
    int newest = -1; uint32_t t = 0;
    for (int i = 0; i < MAX_PEERS; i++)
      if (peers[i].used && peers[i].at >= t) { t = peers[i].at; newest = i; }
    bleActive     = newest;
    bleConnected  = newest >= 0;
    bleNeedAdvert = true;
  }
};

static inline bool bleReady() { return bleConnected && (millis() - bleConnAt > 900); }

const uint8_t* activeAddr() { return (bleActive >= 0) ? peers[bleActive].bda : nullptr; }

void bleSendConsumer(uint16_t code) {
  if (!bleConnected || !bleInput) return;
  uint8_t rpt[2] = { (uint8_t)(code & 0xFF), (uint8_t)(code >> 8) };
  bleInput->setValue(rpt, sizeof(rpt));
  bleInput->notify();
}

void bleSetBattery(uint8_t pct) { if (bleHid) bleHid->setBatteryLevel(pct); }

// ESP-IDF 4.4: sifreleme yarida kalirsa (baglanti kopar, zaman asimi...) Bluetooth
// yigini o cihazin ESLESME ANAHTARLARINI SILER. Bu yuzden yeni baglanan ve henuz
// sifrelemeyi bitirmemis bir cihaz hemen atilmaz; kesme sifreleme bitince yapilir.
void bleDisconnectNow(int i) {
  if (i >= 0 && i < MAX_PEERS && peers[i].used && bleServer) { peers[i].kick = false; bleServer->disconnect(peers[i].connId); }
}
void bleDisconnectPeer(int i) {
  if (i < 0 || i >= MAX_PEERS || !peers[i].used) return;
  if (!peers[i].authed && millis() - peers[i].at < ENC_GRACE_MS) { peers[i].kick = true; return; }
  bleDisconnectNow(i);
}
void bleDisconnectAll() { for (int i = 0; i < MAX_PEERS; i++) bleDisconnectPeer(i); }

void bleAdvertise(bool on) {
  if (!bleServer) return;
  if (on) bleServer->getAdvertising()->start();
  else    bleServer->getAdvertising()->stop();
}

// ---- Son silinen eslesme (tani icin) ---------------------------------------
namespace LostLog {
  char name[32] = {0};
  uint8_t code = 0;
  void load() { prefs.getString("lostN", name, sizeof(name)); code = prefs.getUChar("lostC", 0); }
  void save(const char* n, uint8_t c) {
    strncpy(name, n, sizeof(name) - 1); name[sizeof(name) - 1] = 0; code = c;
    prefs.putString("lostN", name); prefs.putUChar("lostC", c);
  }
  void clear() { name[0] = 0; code = 0; prefs.remove("lostN"); prefs.remove("lostC"); }
}

// ---- Tercih edilen cihaz ----------------------------------------------------
namespace Pref {
  bool set = false;
  uint8_t addr[6] = {0};
  void load() { set = (prefs.getBytesLength("pref") == 6) && prefs.getBytes("pref", addr, 6) == 6; }
  void save(const uint8_t* a) { memcpy(addr, a, 6); set = true; prefs.putBytes("pref", addr, 6); }
  void clear() { set = false; prefs.remove("pref"); }
  bool is(const uint8_t* a) { return set && !memcmp(addr, a, 6); }
}

// ---- Kayitli (bonded) cihazlar ---------------------------------------------
namespace Bonds {
  constexpr int MAX = 15;
  esp_ble_bond_dev_t list[MAX];
  int count = 0;

  void refresh() {
    int n = esp_ble_get_bond_device_num();
    n = constrain(n, 0, MAX);
    count = n;
    if (n > 0) esp_ble_get_bond_device_list(&count, list);
  }
  bool isConnected(int i) { return peerFindAddr(list[i].bd_addr) >= 0; }
  int  indexOf(const uint8_t* a) {
    for (int i = 0; i < count; i++) if (!memcmp(list[i].bd_addr, a, 6)) return i;
    return -1;
  }
  void label(int i, char* out, size_t len) {
    if (!Names::get(list[i].bd_addr, out, len)) fmtAddr(list[i].bd_addr, out, len);
  }
  void remove(int i) {
    if (i < 0 || i >= count) return;
    int p = peerFindAddr(list[i].bd_addr);
    Names::remove(list[i].bd_addr);
    if (Pref::is(list[i].bd_addr)) Pref::clear();
    esp_ble_remove_bond_device(list[i].bd_addr);
    if (p >= 0) bleDisconnectPeer(p);
    delay(30);
    refresh();
  }
  void removeAll() {
    refresh();
    for (int i = 0; i < count; i++) esp_ble_remove_bond_device(list[i].bd_addr);
    Names::clearAll();
    Pref::clear();
    bleDisconnectAll();
    delay(50);
    refresh();
  }
}

// ============================================================================
//  TELEFON GATT ISTEMCISI (eslesmis iPhone'un servisleri)
// ============================================================================
//  CTS  (0x1805)  : saat 0x2A2B (+ bildirim), saat dilimi 0x2A0F
//  AMS  (Apple Media Service): calan sarki, oynatma durumu, telefon ses seviyesi
//  ANCS (Apple Notification Center): bildirimler -> Kestirmeler "MGMWX" hava verisi
//  Tum GATT islemleri tek sirada (Op kuyrugu) yapilir. Olaylar BT gorevinde gelir;
//  saat ayari, hava ayristirma ve ekran ana dongude (service) yapilir.
namespace Music {                        // AMS verisi (BT gorevi yazar, ana dongu okur)
  portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
  char     title[64] = {0}, artist[48] = {0};
  volatile bool  has = false, playing = false, newTrack = false, volNew = false;
  volatile float vol = -1, rate = 0, elapsed = 0, duration = 0;
  volatile uint32_t elapsedAt = 0;
  uint32_t cardAt = 0;                   // sarki karti gosterim zamani
  void clear() {
    portENTER_CRITICAL(&mux); title[0] = artist[0] = 0; portEXIT_CRITICAL(&mux);
    has = playing = false; vol = -1; duration = elapsed = 0; cardAt = 0;
  }
  float pos() {                          // tahmini simdiki konum (sn)
    float p = elapsed + (playing ? rate : 0) * (millis() - elapsedAt) / 1000.0f;
    return duration > 0 ? constrain(p, 0.0f, (float)duration) : max(0.0f, p);
  }
}

RTC_NOINIT_ATTR uint32_t ancsArm;         // 0xA5C5 = ANCS aboneligi yapildi, henuz kararli degil
bool ancsBlocked = false;                 // onceki acilista ANCS'ten sonra cokme: bu oturumda kapali
namespace Phone {
  constexpr uint16_t APP_ID = 0x5B;
  esp_gatt_if_t gif = ESP_GATT_IF_NONE;
  enum St : uint8_t { IDLE, OPENING, REFRESH, SEARCHING, READY, FAILED } ;
  volatile St st = IDLE;
  uint16_t connId = 0;
  esp_bd_addr_t addr = {0};
  uint32_t t = 0;
  uint8_t  tries = 0;
  // servis araliklari ve karakteristikler
  uint16_t ctsS = 0, ctsE = 0, amsS = 0, amsE = 0, ancS = 0, ancE = 0;
  uint16_t hTime = 0, hZone = 0, hAmsEU = 0, hAncNS = 0, hAncCP = 0, hAncDS = 0;
  // BT gorevinden ana donguye (saat)
  volatile bool   gotTime = false, gotZone = false;
  volatile int8_t zone15 = 0;
  uint8_t tbuf[10];
  // ANCS -> hava satiri
  char     wxLine[640];
  volatile bool wxReady = false;
  volatile uint32_t wxUid = 0;
  volatile bool ancsOn = false, amsOn = false;
  uint32_t readyAt = 0; uint8_t reTry = 0; bool opened = false;
  volatile bool setupPending = false;
  volatile uint8_t nSvc = 0;
  volatile uint32_t lastResAt = 0;      // son servis sonucu (arama bitis olayi gelmezse)

  // 128 bit UUID'ler (bluedroid kucuk-endian bekler)
  static void u128(const char* s, uint8_t* o) {
    uint8_t b[16]; int n = 0;
    for (const char* p = s; *p && n < 16; p++) {
      if (*p == '-') continue;
      auto hx = [](char c) { return (uint8_t)(c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10); };
      b[n++] = (uint8_t)(hx(p[0]) << 4 | hx(p[1])); p++;
    }
    for (int i = 0; i < 16; i++) o[i] = b[15 - i];
  }
  static bool isU128(const esp_bt_uuid_t& u, const char* s) {
    if (u.len != ESP_UUID_LEN_128) return false;
    uint8_t o[16]; u128(s, o); return !memcmp(u.uuid.uuid128, o, 16);
  }
  static const char* ANCS_SVC = "7905F431-B5CE-4E99-A40F-4B1E122D00D0";
  static const char* ANCS_NS  = "9FBF120D-6301-42D9-8C58-25E699A21DBD";
  static const char* ANCS_CP  = "69D1D8F3-45E1-49A8-9821-9BBDFDBAAD9D";
  static const char* ANCS_DS  = "22EAC6E9-24D6-4BB5-BE44-B36ACE7C7BFB";
  static const char* AMS_SVC  = "89D3502B-0F36-433A-8EF4-C502AD55F8DC";
  static const char* AMS_EU   = "2F7CABCE-808D-411F-9A0C-BB92BA96C102";

  // ---- Sirali GATT islemleri -------------------------------------------------------
  enum OpT : uint8_t { OP_READ, OP_WDESC, OP_WCHAR };
  struct Op { uint8_t type; uint16_t h; uint8_t len; uint8_t d[20]; };
  constexpr int QN = 16;
  Op q[QN]; volatile uint8_t qh = 0, qt = 0; volatile bool busy = false; volatile uint32_t busyAt = 0;
  volatile uint8_t curType = 0; volatile uint16_t curH = 0; volatile int lastSt = 0; volatile uint16_t nStart = 0, nDone = 0, lastH = 0;
  portMUX_TYPE qmux = portMUX_INITIALIZER_UNLOCKED;
  void kick() {
    Op o; bool go = false;
    portENTER_CRITICAL(&qmux);
    if (!busy && qh != qt) { o = q[qh]; qh = (qh + 1) % QN; busy = true; busyAt = millis(); go = true; }
    portEXIT_CRITICAL(&qmux);
    if (!go) return;
    curType = o.type; curH = o.h; nStart++;
    esp_err_t r = ESP_FAIL;
    if (st == READY || st == SEARCHING) {
      if (o.type == OP_READ) r = esp_ble_gattc_read_char(gif, connId, o.h, ESP_GATT_AUTH_REQ_NONE);
      else if (o.type == OP_WDESC) r = esp_ble_gattc_write_char_descr(gif, connId, o.h, o.len, o.d, ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_NONE);
      else r = esp_ble_gattc_write_char(gif, connId, o.h, o.len, o.d, ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_NONE);
    }
    if (r != ESP_OK) { busy = false; }
  }
  void push(uint8_t type, uint16_t h, const uint8_t* d = nullptr, uint8_t len = 0) {
    if (!h) return;
    portENTER_CRITICAL(&qmux);
    uint8_t nt = (qt + 1) % QN;
    if (nt != qh) { Op& o = q[qt]; o.type = type; o.h = h; o.len = min<uint8_t>(len, 20); if (d) memcpy(o.d, d, o.len); qt = nt; }
    portEXIT_CRITICAL(&qmux);
  }
  void done() { nDone++; lastH = curH; busy = false; kick(); }
  void qClear() { portENTER_CRITICAL(&qmux); qh = qt = 0; busy = false; portEXIT_CRITICAL(&qmux); }

  // Bildirimi ac: CCCD'ye 1 yaz (yerel kayit + sirali yazma)
  void subscribe(uint16_t h) {
    if (!h) return;
    esp_ble_gattc_register_for_notify(gif, addr, h);
    esp_gattc_descr_elem_t d; uint16_t cnt = 1;
    esp_bt_uuid_t du; du.len = ESP_UUID_LEN_16; du.uuid.uuid16 = 0x2902;
    if (esp_ble_gattc_get_descr_by_char_handle(gif, connId, h, du, &d, &cnt) == ESP_GATT_OK && cnt) {
      uint8_t en[2] = {0x01, 0x00};
      push(OP_WDESC, d.handle, en, 2);
    }
  }
  uint16_t charBy16(uint16_t s, uint16_t e, uint16_t u16) {
    if (!s) return 0;
    esp_gattc_char_elem_t el; uint16_t cnt = 1; esp_bt_uuid_t u; u.len = ESP_UUID_LEN_16; u.uuid.uuid16 = u16;
    return (esp_ble_gattc_get_char_by_uuid(gif, connId, s, e, u, &el, &cnt) == ESP_GATT_OK && cnt) ? el.char_handle : 0;
  }
  uint16_t charBy128(uint16_t s, uint16_t e, const char* str) {
    if (!s) return 0;
    esp_gattc_char_elem_t el; uint16_t cnt = 1; esp_bt_uuid_t u; u.len = ESP_UUID_LEN_128; u128(str, u.uuid.uuid128);
    return (esp_ble_gattc_get_char_by_uuid(gif, connId, s, e, u, &el, &cnt) == ESP_GATT_OK && cnt) ? el.char_handle : 0;
  }

  // ---- CTS --------------------------------------------------------------------------
  void onTime(const uint8_t* v, uint16_t len) { if (len < 7) return; memcpy(tbuf, v, min<uint16_t>(len, 10)); gotTime = true; }
  void onZone(const uint8_t* v, uint16_t len) {
    if (len < 2) return;
    int dst = v[1] == 255 ? 0 : v[1];
    zone15 = (int8_t)((int8_t)v[0] + dst); gotZone = true;
  }

  // ---- AMS --------------------------------------------------------------------------
  static float fnum(const char* s) { return (float)atof(s); }
  void onAms(const uint8_t* v, uint16_t len) {
    if (len < 3) return;
    uint8_t ent = v[0], at = v[1];
    char s[64]; uint16_t n = min<uint16_t>(len - 3, sizeof(s) - 1); memcpy(s, v + 3, n); s[n] = 0;
    if (ent == 0 && at == 1) {                                   // PlaybackInfo "durum,hiz,gecen"
      int state = atoi(s); const char* c1 = strchr(s, ','); const char* c2 = c1 ? strchr(c1 + 1, ',') : nullptr;
      Music::playing = state == 1;
      Music::rate = c1 ? fnum(c1 + 1) : 0;
      if (c2) { Music::elapsed = fnum(c2 + 1); Music::elapsedAt = millis(); }
    } else if (ent == 0 && at == 2) {                            // ses seviyesi 0..1
      Music::vol = constrain(fnum(s), 0.0f, 1.0f); Music::volNew = true;
    } else if (ent == 2) {
      portENTER_CRITICAL(&Music::mux);
      if (at == 0) { strncpy(Music::artist, s, sizeof(Music::artist) - 1); Music::artist[sizeof(Music::artist) - 1] = 0; }
      else if (at == 2) {
        if (strcmp(Music::title, s)) Music::newTrack = true;
        strncpy(Music::title, s, sizeof(Music::title) - 1); Music::title[sizeof(Music::title) - 1] = 0;
      }
      portEXIT_CRITICAL(&Music::mux);
      if (at == 3) Music::duration = fnum(s);
      if (at == 2) { Music::has = s[0] != 0; Music::elapsed = 0; Music::elapsedAt = millis(); }
    }
  }

  // ---- ANCS ---------------------------------------------------------------------------
  //  Veri kaynagi parca parca gelir: [0][UID 4][attr id][len 2][veri]... (3 ozellik)
  uint8_t  dsBuf[720]; uint16_t dsN = 0;
  volatile uint16_t nsCount = 0, dsCount = 0;                     // tani: gelen bildirim / okunan yanit
  void ancsRequest(uint32_t uid) {
    uint8_t c[12] = {0x00, (uint8_t)uid, (uint8_t)(uid >> 8), (uint8_t)(uid >> 16), (uint8_t)(uid >> 24),
                     0x00,                        // uygulama kimligi
                     0x01, 0x20, 0x00,            // baslik (32)
                     0x03, 0x58, 0x02};           // mesaj (600)
    push(OP_WCHAR, hAncCP, c, sizeof(c));
    kick();
  }
  void ancsDismiss(uint32_t uid) {                              // olumsuz eylem = bildirimi temizle
    uint8_t c[6] = {0x02, (uint8_t)uid, (uint8_t)(uid >> 8), (uint8_t)(uid >> 16), (uint8_t)(uid >> 24), 0x01};
    push(OP_WCHAR, hAncCP, c, sizeof(c));
    kick();
  }
  void onAncsNS(const uint8_t* v, uint16_t len) {
    if (len < 8) return;
    uint8_t ev = v[0], flags = v[1];
    uint32_t uid = v[4] | (v[5] << 8) | (v[6] << 16) | ((uint32_t)v[7] << 24);
    if (ev != 0 || (flags & 0x04)) return;                       // yalnizca yeni (onceden olmayan)
    nsCount++;
    ancsRequest(uid);
  }
  void onAncsDS(const uint8_t* v, uint16_t len) {
    if (dsN + len > sizeof(dsBuf)) dsN = 0;                      // tasma: bastan
    memcpy(dsBuf + dsN, v, len); dsN += len;
    while (dsN >= 5) {
      if (dsBuf[0] != 0x00) { dsN = 0; return; }                 // beklenmeyen: sifirla
      uint16_t p = 5; const uint8_t* ttl = nullptr; const uint8_t* msg = nullptr; uint16_t tl = 0, ml = 0; int got = 0;
      while (got < 3) {
        if (p + 3 > dsN) return;                                 // devami bekleniyor
        uint8_t id = dsBuf[p]; uint16_t l = dsBuf[p + 1] | (dsBuf[p + 2] << 8);
        if (p + 3 + l > dsN) return;
        if (id == 1) { ttl = dsBuf + p + 3; tl = l; }
        if (id == 3) { msg = dsBuf + p + 3; ml = l; }
        p += 3 + l; got++;
      }
      uint32_t uid = dsBuf[1] | (dsBuf[2] << 8) | (dsBuf[3] << 16) | ((uint32_t)dsBuf[4] << 24);
      // "MGMWX" basligi veya metni: hava satiri
      char tb[40]; uint16_t n = min<uint16_t>(tl, sizeof(tb) - 1); if (ttl) memcpy(tb, ttl, n); tb[n] = 0;
      bool isWx = strstr(tb, "MGMWX") != nullptr;
      if (!isWx && msg) for (uint16_t i = 0; i + 5 <= ml; i++) if (!memcmp(msg + i, "MGMWX", 5)) { isWx = true; break; }
      if (isWx && !wxReady) {
        int k = snprintf(wxLine, sizeof(wxLine), "MGMWX;");
        uint16_t m = min<uint16_t>(ml, sizeof(wxLine) - 1 - k); if (msg) memcpy(wxLine + k, msg, m); wxLine[k + m] = 0;
        wxUid = uid; wxReady = true;
      }
      dsCount++;
      memmove(dsBuf, dsBuf + p, dsN - p); dsN -= p;              // sonraki yanit
    }
  }

  // Servis kesfi bitti: karakteristikleri bul, abone ol (ana donguden cagrilir)
  void setupServices() {
        hZone = charBy16(ctsS, ctsE, 0x2A0F); hTime = charBy16(ctsS, ctsE, 0x2A2B);
        hAmsEU = charBy128(amsS, amsE, AMS_EU);
        hAncNS = charBy128(ancS, ancE, ANCS_NS); hAncCP = charBy128(ancS, ancE, ANCS_CP); hAncDS = charBy128(ancS, ancE, ANCS_DS);
        if (ancS && !(hAncNS && hAncCP && hAncDS)) {                 // UUID ile bulunamadi: hepsini tek tek dolas
          esp_gattc_char_elem_t el[8]; uint16_t cnt = 8;
          esp_gatt_status_t r = esp_ble_gattc_get_all_char(gif, connId, ancS, ancE, el, &cnt, 0);
          Serial.printf("[Telefon] bildirim servisi %u-%u: %u karakteristik (durum %d)\n", ancS, ancE, r == ESP_GATT_OK ? cnt : 0, r);
          for (int i = 0; r == ESP_GATT_OK && i < cnt; i++) {
            const esp_bt_uuid_t& u = el[i].uuid;
            if (u.len == ESP_UUID_LEN_128) Serial.printf("[Telefon]   h%u %02X%02X%02X%02X ozellik 0x%02X\n", el[i].char_handle, u.uuid.uuid128[15], u.uuid.uuid128[14], u.uuid.uuid128[13], u.uuid.uuid128[12], el[i].properties);
            else Serial.printf("[Telefon]   h%u 0x%04X ozellik 0x%02X\n", el[i].char_handle, u.uuid.uuid16, el[i].properties);
            if (u.len != ESP_UUID_LEN_128) continue;
            // Tam 128 bit karsilastirma bu iPhone'da tutmadi; ANCS servisi icinde ilk 32 bit yeterince ayirt edici
            uint32_t pre = ((uint32_t)u.uuid.uuid128[15] << 24) | ((uint32_t)u.uuid.uuid128[14] << 16) | ((uint32_t)u.uuid.uuid128[13] << 8) | u.uuid.uuid128[12];
            if (pre == 0x9FBF120DUL) hAncNS = el[i].char_handle;
            else if (pre == 0x69D1D8F3UL) hAncCP = el[i].char_handle;
            else if (pre == 0x22EAC6E9UL) hAncDS = el[i].char_handle;
            if (pre == 0x9FBF120DUL) {                                  // tani: tam UUID
              Serial.print("[Telefon]   tam: ");
              for (int k = 15; k >= 0; k--) Serial.printf("%02X", u.uuid.uuid128[k]);
              Serial.println();
            }
          }
        }
        Serial.printf("[Telefon] servisler: saat %s, muzik %s, bildirim %s\n", hTime ? "var" : "yok", hAmsEU ? "var" : "yok",
                      (hAncNS && hAncCP && hAncDS) ? "var" : "yok");
        // saat
        subscribe(hTime); push(OP_READ, hZone); push(OP_READ, hTime);
        // muzik: oynatici (oynatma bilgisi, ses) + parca (sanatci, baslik, sure)
        if (hAmsEU) {
          subscribe(hAmsEU);
          uint8_t pl[3] = {0x00, 0x01, 0x02}; push(OP_WCHAR, hAmsEU, pl, 3);
          uint8_t tr[4] = {0x02, 0x00, 0x02, 0x03}; push(OP_WCHAR, hAmsEU, tr, 4);
          amsOn = true;
        }
        // bildirimler: once veri kaynagi, sonra bildirim kaynagi (Apple onerisi)
        if (hAncNS && hAncCP && hAncDS && ancsBlocked) Serial.println("[Telefon] bildirim servisi bu oturumda KAPALI (onceki cokme)");
        else if (hAncNS && hAncCP && hAncDS) { ancsArm = 0xA5C5; subscribe(hAncDS); subscribe(hAncNS); ancsOn = true; }
    st = READY; readyAt = millis();
    kick();
  }

  void onGattc(esp_gattc_cb_event_t e, esp_gatt_if_t ifc, esp_ble_gattc_cb_param_t* p) {
    if (e == ESP_GATTC_REG_EVT) {
      if (p->reg.app_id == APP_ID && p->reg.status == ESP_GATT_OK) gif = ifc;
      return;
    }
    if (ifc != gif || gif == ESP_GATT_IF_NONE) return;
    switch (e) {
      case ESP_GATTC_OPEN_EVT:
        if (st != OPENING) break;
        Serial.printf("[Telefon] baglanti acildi: durum %d, conn %u, mtu %u\n", p->open.status, p->open.conn_id, p->open.mtu);
        if (p->open.status != ESP_GATT_OK) { st = FAILED; break; }
        connId = p->open.conn_id; opened = true;
        ctsS = ctsE = amsS = amsE = ancS = ancE = 0;
        hTime = hZone = hAmsEU = hAncNS = hAncCP = hAncDS = 0;
        qClear(); dsN = 0; nSvc = 0; setupPending = false;
        // Eslesme sirasinda (sifreleme oncesi) olusan onbellekte ANCS olmayabilir: once yeniden kesfet
        if (esp_ble_gattc_cache_refresh(addr) == ESP_OK) { st = REFRESH; t = millis(); }
        else st = esp_ble_gattc_search_service(gif, connId, nullptr) == ESP_OK ? SEARCHING : FAILED;
        break;
      case ESP_GATTC_DIS_SRVC_CMPL_EVT:
        if (st == REFRESH) Serial.printf("[Telefon] kesif bitti: durum %d, conn %u (beklenen %u)\n", p->dis_srvc_cmpl.status, p->dis_srvc_cmpl.conn_id, connId);
        if (st != REFRESH || p->dis_srvc_cmpl.conn_id != connId) break;
        ctsS = ctsE = amsS = amsE = ancS = ancE = 0; nSvc = 0; t = millis();
        st = esp_ble_gattc_search_service(gif, connId, nullptr) == ESP_OK ? SEARCHING : FAILED;   // tum servisler
        break;
      case ESP_GATTC_SEARCH_RES_EVT: {
        if (st != SEARCHING) break;
        const esp_bt_uuid_t& u = p->search_res.srvc_id.uuid;
        uint16_t s = p->search_res.start_handle, en = p->search_res.end_handle;
        nSvc++; lastResAt = millis();
        if (u.len == ESP_UUID_LEN_16 && u.uuid.uuid16 == 0x1805) { ctsS = s; ctsE = en; }
        else if (isU128(u, AMS_SVC))  { amsS = s; amsE = en; }
        else if (isU128(u, ANCS_SVC)) { ancS = s; ancE = en; }
        break;
      }
      case ESP_GATTC_SEARCH_CMPL_EVT:                              // agir is ana dongude (BT gorevi bloklanmasin)
        if (st != SEARCHING) break;
        if (p->search_cmpl.status != ESP_GATT_OK) Serial.printf("[Telefon] servis arama durumu %d\n", p->search_cmpl.status);
        setupPending = true;
        break;
      case ESP_GATTC_READ_CHAR_EVT:
        if (p->read.status == ESP_GATT_OK) {
          if (p->read.handle == hZone) onZone(p->read.value, p->read.value_len);
          else if (p->read.handle == hTime) onTime(p->read.value, p->read.value_len);
        } else Serial.printf("[Telefon] okuma hatasi %d (h %u)\n", p->read.status, p->read.handle);
        done();
        break;
      case ESP_GATTC_WRITE_DESCR_EVT:
        if (p->write.status != ESP_GATT_OK) Serial.printf("[Telefon] abonelik hatasi %d (h %u)\n", p->write.status, p->write.handle);
        done();
        break;
      case ESP_GATTC_WRITE_CHAR_EVT:
        if (p->write.status != ESP_GATT_OK) Serial.printf("[Telefon] yazma hatasi %d (h %u)\n", p->write.status, p->write.handle);
        done();
        break;
      case ESP_GATTC_NOTIFY_EVT: {
        uint16_t h = p->notify.handle;
        if (h == hTime) onTime(p->notify.value, p->notify.value_len);
        else if (h == hAmsEU) onAms(p->notify.value, p->notify.value_len);
        else if (h == hAncNS) onAncsNS(p->notify.value, p->notify.value_len);
        else if (h == hAncDS) onAncsDS(p->notify.value, p->notify.value_len);
        break;
      }
      case ESP_GATTC_CLOSE_EVT:
      case ESP_GATTC_DISCONNECT_EVT:
        if (st != IDLE) st = FAILED;
        qClear(); ancsOn = amsOn = false;
        break;
      default: break;
    }
  }

  void begin() { esp_ble_gattc_app_register(APP_ID); }

  bool request(int peer) {
    if (gif == ESP_GATT_IF_NONE || peer < 0 || !peers[peer].used) return false;
    memcpy(addr, peers[peer].bda, 6);
    if (esp_ble_gattc_open(gif, addr, peers[peer].type, true) != ESP_OK) return false;
    st = OPENING; t = millis();
    return true;
  }
  void service();                        // Net/HwRtc'den sonra tanimli
}

// ============================================================================
//  BAGLI CIHAZIN ADINI OKUMA (GATT istemci: GAP 0x1800 / Device Name 0x2A00)
// ============================================================================
namespace NameReader {
  constexpr uint16_t APP_ID = 0x5A;
  esp_gatt_if_t gif = ESP_GATT_IF_NONE;
  enum St { IDLE, OPENING, SEARCHING, READING } st = IDLE;
  uint16_t connId = 0, sh = 0, eh = 0;
  bool     found = false;
  esp_bd_addr_t addr = {0};
  uint32_t t = 0;
  volatile bool ready = false;          // sonuc hazir
  char     result[32] = {0};
  esp_bd_addr_t resultAddr = {0};

  void finish(const uint8_t* val, uint16_t len) {
    if (val && len) {
      toAscii(val, len, result, sizeof(result));
      memcpy(resultAddr, addr, 6);
      ready = result[0] != 0;
    }
    st = IDLE;
  }

  void onGattc(esp_gattc_cb_event_t e, esp_gatt_if_t ifc, esp_ble_gattc_cb_param_t* p) {
    Obd::Link::onGattc(e, ifc, p);                    // OBD2 adaptoru (ayri GATT uygulamasi)
    Phone::onGattc(e, ifc, p);                        // telefon servisleri (saat, v5.2: bildirim/muzik)
    if (e == ESP_GATTC_REG_EVT) {
      if (p->reg.app_id == APP_ID && p->reg.status == ESP_GATT_OK) gif = ifc;
      return;
    }
    if (ifc != gif || gif == ESP_GATT_IF_NONE) return;
    switch (e) {
      case ESP_GATTC_OPEN_EVT:
        if (st != OPENING) break;
        if (p->open.status != ESP_GATT_OK) { st = IDLE; break; }
        connId = p->open.conn_id;
        found = false;
        {
          esp_bt_uuid_t u; u.len = ESP_UUID_LEN_16; u.uuid.uuid16 = 0x1800;
          if (esp_ble_gattc_search_service(gif, connId, &u) != ESP_OK) st = IDLE;
          else st = SEARCHING;
        }
        break;
      case ESP_GATTC_SEARCH_RES_EVT:
        if (st == SEARCHING && p->search_res.srvc_id.uuid.len == ESP_UUID_LEN_16 &&
            p->search_res.srvc_id.uuid.uuid.uuid16 == 0x1800) {
          sh = p->search_res.start_handle; eh = p->search_res.end_handle; found = true;
        }
        break;
      case ESP_GATTC_SEARCH_CMPL_EVT: {
        if (st != SEARCHING) break;
        if (!found) { st = IDLE; break; }
        esp_bt_uuid_t cu; cu.len = ESP_UUID_LEN_16; cu.uuid.uuid16 = 0x2A00;
        esp_gattc_char_elem_t el; uint16_t cnt = 1;
        if (esp_ble_gattc_get_char_by_uuid(gif, connId, sh, eh, cu, &el, &cnt) == ESP_GATT_OK && cnt > 0) {
          if (esp_ble_gattc_read_char(gif, connId, el.char_handle, ESP_GATT_AUTH_REQ_NONE) == ESP_OK) st = READING;
          else st = IDLE;
        } else st = IDLE;
        break;
      }
      case ESP_GATTC_READ_CHAR_EVT:
        if (st != READING) break;
        if (p->read.status == ESP_GATT_OK) finish(p->read.value, p->read.value_len);
        else st = IDLE;
        break;
      default: break;
    }
  }

  void begin() {
    BLEDevice::setCustomGattcHandler(onGattc);
    esp_ble_gattc_app_register(APP_ID);
  }

  bool busy() { return st != IDLE; }

  bool request(int peer) {
    if (gif == ESP_GATT_IF_NONE || busy() || peer < 0 || !peers[peer].used) return false;
    memcpy(addr, peers[peer].bda, 6);
    ready = false;
    if (esp_ble_gattc_open(gif, addr, peers[peer].type, true) != ESP_OK) return false;
    st = OPENING; t = millis();
    return true;
  }

  void service() {
    if (st != IDLE && millis() - t > 6000) st = IDLE;     // zaman asimi
  }
}

// ============================================================================
//  CIHAZ ARAMA (Classic Bluetooth inquiry — isimli sonuc)
// ============================================================================
namespace Scan {
  struct Dev {
    esp_bd_addr_t bda;
    char    name[32];
    int8_t  rssi;
    uint32_t cod;
    bool    named;
    bool    nameTried;
  };
  constexpr int MAX = 20;
  Dev list[MAX];
  volatile int  count = 0;
  volatile bool running = false;
  volatile bool changed = false;
  bool     nameBusy = false;
  int      nameIdx = -1;
  uint32_t nameAt = 0;
  portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

  int find(const uint8_t* a) {
    for (int i = 0; i < count; i++) if (!memcmp(list[i].bda, a, 6)) return i;
    return -1;
  }

  void gapCb(esp_bt_gap_cb_event_t ev, esp_bt_gap_cb_param_t* p) {
    switch (ev) {
      case ESP_BT_GAP_DISC_RES_EVT: {
        char name[32] = {0}; int8_t rssi = 0; uint32_t cod = 0;
        for (int i = 0; i < p->disc_res.num_prop; i++) {
          esp_bt_gap_dev_prop_t& pr = p->disc_res.prop[i];
          switch (pr.type) {
            case ESP_BT_GAP_DEV_PROP_BDNAME:
              toAscii((uint8_t*)pr.val, pr.len, name, sizeof(name)); break;
            case ESP_BT_GAP_DEV_PROP_COD:  cod = *(uint32_t*)pr.val; break;
            case ESP_BT_GAP_DEV_PROP_RSSI: rssi = *(int8_t*)pr.val; break;
            case ESP_BT_GAP_DEV_PROP_EIR:
              if (!name[0]) {
                uint8_t l = 0;
                uint8_t* d = esp_bt_gap_resolve_eir_data((uint8_t*)pr.val, ESP_BT_EIR_TYPE_CMPL_LOCAL_NAME, &l);
                if (!d) d = esp_bt_gap_resolve_eir_data((uint8_t*)pr.val, ESP_BT_EIR_TYPE_SHORT_LOCAL_NAME, &l);
                if (d && l) toAscii(d, l, name, sizeof(name));
              }
              break;
            default: break;
          }
        }
        portENTER_CRITICAL(&mux);
        int i = find(p->disc_res.bda);
        if (i < 0 && count < MAX) {
          i = count++;
          memset(&list[i], 0, sizeof(Dev));
          memcpy(list[i].bda, p->disc_res.bda, 6);
        }
        if (i >= 0) {
          if (name[0]) { strcpy(list[i].name, name); list[i].named = true; }
          if (rssi) list[i].rssi = rssi;
          if (cod)  list[i].cod = cod;
          changed = true;
        }
        portEXIT_CRITICAL(&mux);
        break;
      }
      case ESP_BT_GAP_DISC_STATE_CHANGED_EVT:
        running = (p->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED);
        changed = true;
        break;
      case ESP_BT_GAP_READ_REMOTE_NAME_EVT: {
        char name[32] = {0};
        if (p->read_rmt_name.stat == ESP_BT_STATUS_SUCCESS)
          toAscii(p->read_rmt_name.rmt_name, sizeof(p->read_rmt_name.rmt_name), name, sizeof(name));
        portENTER_CRITICAL(&mux);
        int i = find(p->read_rmt_name.bda);
        if (i >= 0 && name[0]) { strcpy(list[i].name, name); list[i].named = true; }
        portEXIT_CRITICAL(&mux);
        nameBusy = false;
        changed = true;
        break;
      }
      default: break;
    }
  }

  void begin() {
    esp_bt_gap_register_callback(gapCb);
    // Kart Classic BT'de gorunmesin (sadece BLE ile eslesir)
    esp_bt_gap_set_scan_mode(ESP_BT_NON_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
  }

  void start() {
    esp_bt_gap_cancel_discovery();
    portENTER_CRITICAL(&mux);
    count = 0;
    portEXIT_CRITICAL(&mux);
    nameBusy = false; nameIdx = -1;
    running = esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, SCAN_INQ_LEN, 0) == ESP_OK;
    changed = true;
  }

  void stop() {
    if (running) esp_bt_gap_cancel_discovery();
    running = false;
  }

  // Arama bitince ismi gelmeyen cihazlarin adini tek tek sor
  void service() {
    if (running) return;
    if (nameBusy) {
      if (millis() - nameAt > 5000) nameBusy = false;
      return;
    }
    for (int i = 0; i < count; i++) {
      if (!list[i].named && !list[i].nameTried) {
        list[i].nameTried = true;
        if (esp_bt_gap_read_remote_name(list[i].bda) == ESP_OK) {
          nameBusy = true; nameIdx = i; nameAt = millis();
        }
        return;
      }
    }
  }

  const char* typeText(uint32_t cod) {
    switch (esp_bt_gap_get_cod_major_dev(cod)) {
      case ESP_BT_COD_MAJOR_DEV_PHONE:    return "Telefon";
      case ESP_BT_COD_MAJOR_DEV_COMPUTER: return "Bilgisayar";
      case ESP_BT_COD_MAJOR_DEV_AV: {
        uint32_t minor = (cod >> 2) & 0x3F;
        if (minor == 0x06) return "Kulaklık";
        if (minor == 0x01 || minor == 0x02) return "Kulaklık";
        return "Hoparlör";
      }
      case ESP_BT_COD_MAJOR_DEV_WEARABLE:   return "Saat";
      case ESP_BT_COD_MAJOR_DEV_PERIPHERAL: return "Çevre b.";
      default: return "Diğer";
    }
  }
  bool isAudio(uint32_t cod) { return esp_bt_gap_get_cod_major_dev(cod) == ESP_BT_COD_MAJOR_DEV_AV; }
  int  bars(int8_t rssi) {
    if (rssi == 0) return 0;
    if (rssi > -60) return 4;
    if (rssi > -70) return 3;
    if (rssi > -80) return 2;
    return 1;
  }
}

// ============================================================================
//  ESLESTIRME (hedef + kod onayi)
// ============================================================================
namespace Pair {
  bool     active = false;        // eslesme modu (reklam acik)
  uint32_t until = 0;
  char     target[32] = {0};      // aramadan secilen cihaz adi
  bool     hasTarget = false;

  // GAP olaylari (BT gorevinden gelir -> loop'ta islenir)
  volatile bool evNc = false, evNotif = false, evAuth = false, evAuthOk = false;
  esp_bd_addr_t evAddr = {0}, evAuthAddr = {0};
  volatile uint32_t evKey = 0;
  volatile uint8_t  evReason = 0;

  // Onay penceresi
  enum Modal { M_NONE, M_CONFIRM, M_NOTIFY } modal = M_NONE;
  esp_bd_addr_t mAddr = {0};
  uint32_t mKey = 0, mAt = 0;
  int      mPeer = -1;
  char     mName[32] = {0};
  bool     justAccepted = false;
  int      acceptedPeer = -1;

  void start(const char* tgt) {
    hasTarget = tgt && tgt[0];
    strncpy(target, hasTarget ? tgt : "", sizeof(target) - 1);
    active = true;
    until = millis() + PAIR_WINDOW_MS;
    bleHidden = false;
    bleAdvertise(true);                   // bagliyken de ikinci cihaz baglanabilsin
  }

  void stop() {
    active = false;
    if (bleConnected) bleAdvertise(false);
  }

  void gapHandler(esp_gap_ble_cb_event_t ev, esp_ble_gap_cb_param_t* p) {
    Obd::Link::onGap(ev, p);                          // OBD adaptoru taramasi
    switch (ev) {
      case ESP_GAP_BLE_NC_REQ_EVT:
        memcpy(evAddr, p->ble_security.key_notif.bd_addr, 6);
        evKey = p->ble_security.key_notif.passkey;
        evNc = true;
        break;
      case ESP_GAP_BLE_PASSKEY_NOTIF_EVT:
        memcpy(evAddr, p->ble_security.key_notif.bd_addr, 6);
        evKey = p->ble_security.key_notif.passkey;
        evNotif = true;
        break;
      case ESP_GAP_BLE_PASSKEY_REQ_EVT:              // klavyemiz yok
        esp_ble_passkey_reply(p->ble_security.ble_req.bd_addr, false, 0);
        break;
      case ESP_GAP_BLE_AUTH_CMPL_EVT:
        memcpy(evAuthAddr, p->ble_security.auth_cmpl.bd_addr, 6);
        evAuthOk = p->ble_security.auth_cmpl.success;
        evReason = p->ble_security.auth_cmpl.fail_reason;
        evAuth = true;
        break;
      default: break;
    }
  }

  void openModal(Modal m) {
    pairMemPrepare();                  // eslesme kaydi icin RAM ac (ekran tamponu)
    modal = m;
    memcpy(mAddr, (const void*)evAddr, 6);
    mKey = evKey;
    mAt = millis();
    mPeer = peerFindAddr(mAddr);
    if (mPeer < 0) mPeer = bleActive;
    mName[0] = 0;
    if (mPeer >= 0) Names::get(peers[mPeer].bda, mName, sizeof(mName));
    NameReader::request(mPeer);
  }

  void accept() {
    if (modal == M_CONFIRM) {
      esp_ble_confirm_reply(mAddr, true);
      justAccepted = true;
      acceptedPeer = mPeer;
      showToast("Telefonda da onaylayın", C_ACCENT, 2500);
    }
    modal = M_NONE;
  }

  void reject() {
    if (modal == M_CONFIRM) esp_ble_confirm_reply(mAddr, false);
    if (mPeer >= 0) bleDisconnectNow(mPeer);
    justAccepted = false;
    modal = M_NONE;
    showToast("Reddedildi", C_BAD, 1200);
  }

  bool targetMatch() { return hasTarget && mName[0] && strEqI(mName, target); }
}

// ============================================================================
//  BLE BASLAT
// ============================================================================
// Wi-Fi acikken klasik Bluetooth hafizasi birakilir (Wi-Fi'a yer acmak icin).
// Arduino'nun btStart() fonksiyonu denetleyici zaten acik ise dokunmaz.
bool bleControllerBleOnly() {
  if (esp_bt_controller_get_status() != ESP_BT_CONTROLLER_STATUS_IDLE) return false;
  esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
  esp_bt_controller_config_t c = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  c.mode = ESP_BT_MODE_BLE;
  if (esp_bt_controller_init(&c) != ESP_OK) return false;
  return esp_bt_controller_enable(ESP_BT_MODE_BLE) == ESP_OK;
}

// Eslesme guvenligi:
//  onaysiz (varsayilan): "Just Works" + bonding — v1.x'teki gibi, telefonda tek dokunus
//  onayli: Numeric Comparison — kartta 6 haneli kod IO35 ile onaylanir
void applySecurity() {
  esp_ble_auth_req_t auth = cfg.pairConfirm ? ESP_LE_AUTH_REQ_SC_MITM_BOND : ESP_LE_AUTH_REQ_SC_BOND;
  esp_ble_io_cap_t   io   = cfg.pairConfirm ? ESP_IO_CAP_IO : ESP_IO_CAP_NONE;
  uint8_t keys = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth, sizeof(auth));
  esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &io, sizeof(io));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &keys, sizeof(keys));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &keys, sizeof(keys));
}

void bleBegin() {
  uint32_t heap0 = ESP.getFreeHeap();
  // Klasik BT yalnizca isimli arama icin gerekli ve ~30 KB+ RAM tutuyor. Az RAM,
  // eslesme kaydinin NVS'ye yazilamamasina (resette eslesme kaybi) yol aciyordu.
  if (!cfg.classicScan && !prefs.getBool("bleofail", false)) {
    // Once basarisizlik isaretini koy; basarili olursa kaldir
    prefs.putBool("bleofail", true);
    app.classicOk = !bleControllerBleOnly();
  }
  BLEDevice::init(DEVICE_NAME);
  if (!app.classicOk) {
    if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
      // Hafif mod calismadi: tam modda yeniden basla
      cfg.classicScan = 1; saveConfig();
      delay(100);
      ESP.restart();
    }
  }
  prefs.remove("bleofail");
  Serial.printf("[BT] mod: %s (Ayarlar > Isimli arama: %s), BLE oncesi RAM %u, sonrasi %u\n", app.classicOk ? "tam (klasik+BLE)" : "hafif (sadece BLE)",
                cfg.classicScan ? "ACIK" : "kapali", (unsigned)heap0, (unsigned)ESP.getFreeHeap());
  BLEDevice::setCustomGapHandler(Pair::gapHandler);
  esp_ble_gatt_set_local_mtu(185);        // uzun sarki adlari / bildirimler tek parcada gelsin

  bleServer = BLEDevice::createServer();
  bleServer->setCallbacks(new ServerCallbacks());

  bleHid   = new BLEHIDDevice(bleServer);
  bleInput = bleHid->inputReport(REPORT_ID_CONSUMER);

  bleHid->manufacturer()->setValue(MANUFACTURER);
  bleHid->pnp(0x02, 0xE502, 0xA111, 0x0210);
  bleHid->hidInfo(0x00, 0x01);

  // Numeric Comparison: yeni eslesmede kartta 6 haneli kod onayi istenir
  applySecurity();

  bleHid->reportMap((uint8_t*)HID_REPORT_MAP, sizeof(HID_REPORT_MAP));
  bleHid->startServices();
  bleHid->setBatteryLevel(100);

  BLEAdvertising* adv = bleServer->getAdvertising();
  adv->setAppearance(0x03C0);
  adv->addServiceUUID(bleHid->hidService()->getUUID());
  adv->setScanResponse(true);
  {
    // Reklam paketi: bayraklar + gorunum (HID) + 0x1812 + ANCS servis istegi (iOS "Sistem Bildirimlerini
    // Paylas" icin bunu reklamda arar). 3 + 4 + 4 + 18 = 29 bayt. Isim tarama yanitinda.
    BLEAdvertisementData ad;
    ad.setFlags(ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);
    ad.setAppearance(0x03C0);
    ad.setCompleteServices(BLEUUID((uint16_t)0x1812));
    uint8_t sol[18] = {17, 0x15, 0xD0, 0x00, 0x2D, 0x12, 0x1E, 0x4B, 0x0F, 0xA4, 0x99, 0x4E, 0xCE, 0xB5, 0x31, 0xF4, 0x05, 0x79};
    String d; d.concat((const char*)sol, sizeof(sol));
    ad.addData(d);
    adv->setAdvertisementData(ad);
    BLEAdvertisementData sr;
    sr.setName(DEVICE_NAME);
    adv->setScanResponseData(sr);
  }
  adv->start();

  NameReader::begin();
  Phone::begin();
  if (app.classicOk) Scan::begin();
}

// ============================================================================
//  BLOKLAMAYAN HID TUS KUYRUGU
// ============================================================================
namespace KeyQueue {
  constexpr uint8_t SIZE = 64;
  uint16_t buf[SIZE];
  uint8_t  head = 0, tail = 0;
  enum { IDLE, PRESSED, GAP } state = IDLE;
  uint32_t t = 0;

  bool empty() { return head == tail; }
  void clear() { head = tail = 0; if (state == PRESSED) bleSendConsumer(CC_NONE); state = IDLE; }
  bool push(uint16_t c) {
    uint8_t n = (head + 1) % SIZE;
    if (n == tail) return false;
    buf[head] = c; head = n; return true;
  }
  bool busy() { return !empty() || state != IDLE; }

  void service() {
    if (!bleConnected) { if (busy()) clear(); return; }
    uint32_t now = millis();
    switch (state) {
      case IDLE:
        if (!empty() && bleReady()) {
          bleSendConsumer(buf[tail]);
          tail = (tail + 1) % SIZE;
          state = PRESSED; t = now;
        }
        break;
      case PRESSED:
        if (now - t >= KEY_PRESS_MS) { bleSendConsumer(CC_NONE); state = GAP; t = now; }
        break;
      case GAP:
        if (now - t >= KEY_GAP_MS) state = IDLE;
        break;
    }
  }
}

// ============================================================================
//  SAAT + HAVA DURUMU (v5.1: Wi-Fi YOK)
// ============================================================================
//  Saat: iPhone'un "Current Time" servisi (BLE, eslesmis telefon) + BM8563
//  donanim saati (pilli; kapatip acinca da saat kaybolmaz).
//  Hava durumu: iPhone Kestirmeler (v5.2, bildirim uzerinden) ya da seri port
//  test satiri "MGMWX;..." (bkz. Wx::parse). Veri RTC hafizasinda tutulur.
//  Wi-Fi/OTA ileride ayri "guncelleyici" programda olacak (ana programda yok).
#define RTC_MAGIC 0x5754A1C7
enum : uint8_t { BOOT_NORMAL = 0, BOOT_SYNC = 1, BOOT_PORTAL = 2, BOOT_CLEAN = 3, BOOT_KARAOKE = 4, BOOT_OBD = 5 };
constexpr uint8_t WX_N = 12;             // saatlik tahmin uzunlugu
struct RtcWx {
  uint32_t magic;
  uint8_t  bootReq;
  uint8_t  isDay;
  uint8_t  wxHour;       // saatlik listenin ilk saati (yerel, 0-23)
  uint8_t  nH;           // saatlik deger sayisi
  uint32_t clockEpoch;   // temiz yeniden baslatmada saati korumak icin
  uint32_t wxAt;         // hava verisinin alindigi an (epoch, 0 = yok)
  int16_t  temp10;
  int16_t  wcode;        // WMO benzeri kod (-1 = yok)
  uint8_t  wind;         // km/sa
  int8_t   tmin, tmax;
  uint8_t  wxHourSet;    // saatlik listenin ilk saati kestirmeden geldi (S=)
  int8_t   hT[WX_N];     // saatlik sicaklik
  uint8_t  hP[WX_N];     // saatlik yagis olasiligi %
};
RTC_NOINIT_ATTR RtcWx rtcWx;

namespace Net {
  long     tzOff = 10800;               // UTC+3 (iPhone saat dilimi gelince guncellenir)
  uint8_t  timeSrc = 0;                 // 0 yok, 1 donanim saati, 2 iPhone
  int16_t& temp10 = rtcWx.temp10;
  int16_t& wcode  = rtcWx.wcode;
  uint8_t& isDay  = rtcWx.isDay;

  void rtcInit() {
    if (rtcWx.magic != RTC_MAGIC) {     // ilk guc verilisi / yeni surum: RTC hafizasi gecersiz
      memset(&rtcWx, 0, sizeof(rtcWx));
      rtcWx.magic = RTC_MAGIC;
      rtcWx.wcode = -1;
      rtcWx.isDay = 1;
    }
    if (rtcWx.nH > WX_N) rtcWx.nH = WX_N;
  }

  bool timeValid()  { return time(nullptr) > 1700000000; }
  bool hasWeather();                    // Wx'te tanimli

  void applyTz(long off) {
    char tz[16];
    int h = off / 3600, m = abs((int)(off % 3600)) / 60;
    if (m) snprintf(tz, sizeof(tz), "UTC%+d:%02d", -h, m);
    else   snprintf(tz, sizeof(tz), "UTC%+d", -h);
    setenv("TZ", tz, 1);
    tzset();
  }

  void load() {
    tzOff = prefs.getLong("tzoff", 10800);
    applyTz(tzOff);
  }
  void setTz(long off) {
    if (off == tzOff) return;
    tzOff = off; prefs.putLong("tzoff", off); applyTz(off);
  }

  bool clock(char* out, size_t n) {
    if (!timeValid()) return false;
    time_t t = time(nullptr);
    struct tm tmv;
    localtime_r(&t, &tmv);
    snprintf(out, n, "%02d:%02d", tmv.tm_hour, tmv.tm_min);
    return true;
  }
  int localHour() {
    time_t t = time(nullptr); struct tm tmv; localtime_r(&t, &tmv); return tmv.tm_hour;
  }

  const char* wText() {
    int c = rtcWx.wcode;
    if (c == 0) return "Açık";
    if (c <= 2) return "Az bulutlu";
    if (c == 3) return "Bulutlu";
    if (c == 45 || c == 48) return "Sisli";
    if (c >= 51 && c <= 57) return "Çiseleme";
    if (c >= 61 && c <= 67) return "Yağmurlu";
    if (c >= 71 && c <= 77) return "Karlı";
    if (c >= 80 && c <= 82) return "Sağanak";
    if (c >= 85 && c <= 86) return "Kar yağışlı";
    if (c >= 95) return "Fırtına";
    return "-";
  }
}

// Takvim <-> epoch (UTC), newlib'de timegm olmadigi icin
static int64_t daysFromCivil(int y, unsigned m, unsigned d) {
  y -= m <= 2;
  const int era = (y >= 0 ? y : y - 399) / 400;
  const unsigned yoe = (unsigned)(y - era * 400);
  const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  return (int64_t)era * 146097 + (int64_t)doe - 719468;
}
static time_t civilToEpoch(int y, int mo, int d, int h, int mi, int s) {
  return (time_t)(daysFromCivil(y, mo, d) * 86400LL + h * 3600 + mi * 60 + s);
}

// ---- BM8563 donanim saati (I2C 0x51, Wire1 SDA21/SCL22) — UTC tutulur ----------
namespace HwRtc {
  constexpr uint8_t ADDR = 0x51;
  static uint8_t bcd(uint8_t v)  { return (v >> 4) * 10 + (v & 0x0F); }
  static uint8_t tobcd(int v)    { return (uint8_t)(((v / 10) << 4) | (v % 10)); }
  bool read(time_t& out) {
    Wire1.beginTransmission(ADDR); Wire1.write(0x02);
    if (Wire1.endTransmission() != 0) return false;
    if (Wire1.requestFrom((int)ADDR, 7) != 7) return false;
    uint8_t r[7]; for (int i = 0; i < 7; i++) r[i] = Wire1.read();
    if (r[0] & 0x80) return false;                        // VL: pil bitmis, saat gecersiz
    int y = bcd(r[6]) + ((r[5] & 0x80) ? 1900 : 2000);
    int mo = bcd(r[5] & 0x1F), d = bcd(r[3] & 0x3F);
    if (y < 2024 || mo < 1 || mo > 12 || d < 1 || d > 31) return false;
    out = civilToEpoch(y, mo, d, bcd(r[2] & 0x3F), bcd(r[1] & 0x7F), bcd(r[0] & 0x7F));
    return true;
  }
  bool write(time_t utc) {
    struct tm t; gmtime_r(&utc, &t);
    Wire1.beginTransmission(ADDR);
    Wire1.write(0x00); Wire1.write(0x00); Wire1.write(0x00);          // kontrol 1/2: saat calissin
    Wire1.write(tobcd(t.tm_sec)); Wire1.write(tobcd(t.tm_min)); Wire1.write(tobcd(t.tm_hour));
    Wire1.write(tobcd(t.tm_mday)); Wire1.write((uint8_t)t.tm_wday);
    Wire1.write(tobcd(t.tm_mon + 1));                                  // yuzyil biti 0 = 20xx
    Wire1.write(tobcd(t.tm_year % 100));
    return Wire1.endTransmission() == 0;
  }
  // Acilista: sistem saati yoksa donanim saatinden yukle
  void restore() {
    time_t t;
    if (Net::timeValid()) return;
    if (read(t) && t > 1700000000) {
      struct timeval tv = { t, 0 };
      settimeofday(&tv, nullptr);
      Net::timeSrc = 1;
      Serial.printf("[Saat] donanim saatinden yuklendi: %lu\n", (unsigned long)t);
    } else Serial.println("[Saat] donanim saati gecersiz (iPhone bekleniyor)");
  }
}

// ---- Hava durumu verisi ---------------------------------------------------------
namespace Wx {
  constexpr uint32_t MAX_AGE = 12UL * 3600UL;          // 12 saatten eski veri gosterilmez
  uint32_t updatedAt = 0;                               // son guncelleme (millis): koruyucuda kisa bildirim
  uint32_t ageS() { return (Net::timeValid() && rtcWx.wxAt) ? (uint32_t)time(nullptr) - rtcWx.wxAt : 0xFFFFFFFFUL; }
  bool has() { return rtcWx.wcode >= 0 && ageS() < MAX_AGE; }
  // Saatlik listede "simdi"nin indeksi (-1: yok)
  int idxNow() {
    if (!has() || !rtcWx.nH) return -1;
    int i = (Net::localHour() - rtcWx.wxHour + 24) % 24;
    return i < rtcWx.nH ? i : -1;
  }
  int tempNow() {
    int i = idxNow();
    return i > 0 ? rtcWx.hT[i] : (int)lroundf(rtcWx.temp10 / 10.0f);
  }
  // Onumuzdeki n saatteki en yuksek yagis olasiligi (hour: yerel saat)
  uint8_t rainPeak(int n, uint8_t* hour) {
    int i0 = idxNow(); uint8_t m = 0, h = 0;
    if (i0 < 0) { if (hour) *hour = 0; return 0; }
    for (int i = i0; i < i0 + n && i < rtcWx.nH; i++)
      if (rtcWx.hP[i] > m) { m = rtcWx.hP[i]; h = (rtcWx.wxHour + i) % 24; }
    if (hour) *hour = h;
    return m;
  }
  // Onumuzdeki n saatteki en dusuk sicaklik
  int minTemp(int n) {
    int i0 = idxNow(), m = tempNow();
    if (i0 < 0) return m;
    for (int i = i0; i < i0 + n && i < rtcWx.nH; i++) m = min(m, (int)rtcWx.hT[i]);
    return m;
  }

  // Metni ASCII kucuk harfe indir (Turkce harfler -> en yakin Latin harf)
  static void fold(const char* in, char* out, size_t n) {
    size_t o = 0; const uint8_t* p = (const uint8_t*)in;
    while (*p && o + 1 < n) {
      uint8_t c = *p;
      if (c < 0x80) { out[o++] = (char)tolower(c); p++; continue; }
      if ((c & 0xE0) == 0xC0 && p[1]) {
        uint16_t u = ((c & 0x1F) << 6) | (p[1] & 0x3F); char r = 0;
        switch (u) { case 0xE7: case 0xC7: r = 'c'; break; case 0x11F: case 0x11E: r = 'g'; break;
                     case 0x131: case 0x130: r = 'i'; break; case 0xF6: case 0xD6: r = 'o'; break;
                     case 0x15F: case 0x15E: r = 's'; break; case 0xFC: case 0xDC: r = 'u'; break; default: break; }
        if (r) out[o++] = r;
        p += 2; continue;
      }
      p++;
    }
    out[o] = 0;
  }
  // iPhone hava durumu metni (TR/EN) -> kod
  int16_t codeFromText(const char* s) {
    char f[48]; fold(s, f, sizeof(f));
    auto has = [&](const char* k) { return strstr(f, k) != nullptr; };
    if (has("firtina") || has("thunder") || has("gok gur") || has("simsek")) return 95;
    if (has("kar") || has("snow") || has("sleet") || has("flurr")) return 71;
    if (has("saganak") || has("shower")) return 80;
    if (has("ciseleme") || has("cisenti") || has("drizzle")) return 51;
    if (has("yagmur") || has("rain")) return 61;
    if (has("sis") || has("fog") || has("pus") || has("haze") || has("mist")) return 45;
    if (has("parcali") || has("az bulut") || has("partly") || has("mostly sunny") || has("mostly clear")) return 2;
    if (has("bulut") || has("cloud") || has("overcast") || has("kapali")) return 3;
    if (has("acik") || has("clear") || has("gunes") || has("sunny")) return 0;
    return 3;
  }
  // "18°C", "-3,5", "%70", "70%" -> sayi (yuvarlanir). false: sayi yok
  static bool num(const char*& p, const char* end, int& out) {
    while (p < end && !(isdigit((uint8_t)*p) || (*p == '-' && p + 1 < end && isdigit((uint8_t)p[1])))) p++;
    if (p >= end) return false;
    bool neg = *p == '-'; if (neg) p++;
    long v = 0; while (p < end && isdigit((uint8_t)*p)) v = v * 10 + (*p++ - '0');
    float f = v;
    if (p + 1 < end && (*p == ',' || *p == '.') && isdigit((uint8_t)p[1])) { p++; f += (*p - '0') / 10.0f; while (p < end && isdigit((uint8_t)*p)) p++; }
    out = (int)lroundf(neg ? -f : f);
    return true;
  }
  // Kestirme satiri:  MGMWX;T=18;C=Parçalı Bulutlu;W=14;L=11;H=21;S=14;PT=18/19/..;PP=5/10/..
  //   T simdiki sicaklik, C durum metni, W ruzgar km/sa, L/H gunluk en dusuk/yuksek,
  //   S saatlik listenin ilk saati, PT saatlik sicaklik, PP saatlik yagis olasiligi (/ ile ayrilir)
  bool parse(const char* s0) {
    static char buf[660];                                   // [ ] temizlenmis kopya
    size_t o = 0;
    for (const char* q = s0; *q && o + 1 < sizeof(buf); q++) if (*q != '[' && *q != ']') buf[o++] = *q;
    buf[o] = 0;
    const char* s = buf;
    const char* m = strstr(s, "MGMWX");
    if (!m) return false;
    RtcWx w = rtcWx;
    w.wcode = -1; w.nH = 0; w.wxHour = Net::timeValid() ? Net::localHour() : 0; w.wxHourSet = 0;
    bool gotT = false;
    int nT = 0, nP = 0;
    const char* p = m + 5;
    while (*p) {
      while (*p == ';' || *p == ' ' || *p == '\n' || *p == '\r') p++;
      if (!*p) break;
      const char* eq = strchr(p, '=');
      const char* end = strchr(p, ';'); if (!end) end = p + strlen(p);
      if (!eq || eq > end) { p = end; continue; }
      char key[4] = {0}; size_t kl = min<size_t>(eq - p, 3); memcpy(key, p, kl);
      const char* v = eq + 1; int x;
      if (!strcmp(key, "T") && num(v, end, x)) { w.temp10 = x * 10; gotT = true; }
      else if (!strcmp(key, "C")) {                               // ilk satir (liste gelirse)
        char t[48]; size_t l = min<size_t>(end - (eq + 1), sizeof(t) - 1); memcpy(t, eq + 1, l); t[l] = 0;
        char* nl = strpbrk(t, "\r\n"); if (nl) *nl = 0;
        w.wcode = codeFromText(t);
      }
      else if (!strcmp(key, "W") && num(v, end, x)) w.wind = (uint8_t)constrain(x, 0, 250);
      else if (!strcmp(key, "L") && num(v, end, x)) w.tmin = (int8_t)constrain(x, -60, 60);
      else if (!strcmp(key, "H") && num(v, end, x)) w.tmax = (int8_t)constrain(x, -60, 60);
      else if (!strcmp(key, "S") && num(v, end, x)) { w.wxHour = (uint8_t)(((x % 24) + 24) % 24); w.wxHourSet = 1; }
      else if (!strcmp(key, "D") && num(v, end, x)) w.isDay = x ? 1 : 0;
      else if (!strcmp(key, "PT")) {
        const char* q = v;
        while (nT < WX_N) {                               // "/" ayraci; ondalik virgul sayinin parcasi
          const char* se = q; while (se < end && *se != '/') se++;
          const char* qq = q; if (!num(qq, se, x)) break;
          w.hT[nT++] = (int8_t)constrain(x, -60, 60);
          if (se >= end) break; q = se + 1;
        }
      }
      else if (!strcmp(key, "PP")) {
        const char* q = v;
        while (nP < WX_N) {
          const char* se = q; while (se < end && *se != '/') se++;
          const char* qq = q; if (!num(qq, se, x)) break;
          w.hP[nP++] = (uint8_t)constrain(x, 0, 100);
          if (se >= end) break; q = se + 1;
        }
      }
      p = end;
    }
    if (!gotT) return false;
    if (w.wcode < 0) w.wcode = 3;
    w.nH = (uint8_t)min(nT, nP);
    for (int i = w.nH; i < WX_N; i++) { w.hT[i] = 0; w.hP[i] = 0; }
    if (!w.tmin && !w.tmax) { w.tmin = w.tmax = (int8_t)(w.temp10 / 10); for (int i = 0; i < w.nH; i++) { w.tmin = min(w.tmin, w.hT[i]); w.tmax = max(w.tmax, w.hT[i]); } }
    if (!strstr(m, "D=")) { int h = Net::timeValid() ? Net::localHour() : 12; w.isDay = (h >= 7 && h < 19); }
    w.wxAt = Net::timeValid() ? (uint32_t)time(nullptr) : 1;
    rtcWx = w;
    updatedAt = millis(); if (!updatedAt) updatedAt = 1;
    Serial.printf("[Hava] %d C kod %d, ruzgar %d, %d saat (ilk %02d:00)\n", w.temp10 / 10, w.wcode, w.wind, w.nH, w.wxHour);
    return true;
  }
}
bool Net::hasWeather() { return Wx::has(); }


// ============================================================================
//  M5StickC Plus / Plus2 DONANIM KATMANI
// ============================================================================
//  Plus : AXP192 (I2C 0x34, SDA21/SCL22) -> ekran isigi (LDO2), ekran (LDO3),
//         pil voltaji, USB, guc tusu. Kirmizi LED GPIO10 (LOW = yanar).
//  Plus2: GPIO4 guc tutma (HIGH olmazsa pilde kart kapanir), isik GPIO27 PWM,
//         pil GPIO38 (1/2 bolucu), guc tusu GPIO35. LED GPIO19 (HIGH = yanar).
namespace HW {
#if M5_BOARD == 1
  // ---------------- M5StickC Plus (AXP192) ----------------
  constexpr uint8_t AXP = 0x34;
  void axpW(uint8_t r, uint8_t v) { Wire1.beginTransmission(AXP); Wire1.write(r); Wire1.write(v); Wire1.endTransmission(); }
  uint8_t axpR(uint8_t r) {
    Wire1.beginTransmission(AXP); Wire1.write(r); Wire1.endTransmission();
    Wire1.requestFrom((int)AXP, 1);
    return Wire1.available() ? Wire1.read() : 0;
  }
  uint16_t axpR12(uint8_t r) {
    Wire1.beginTransmission(AXP); Wire1.write(r); Wire1.endTransmission();
    Wire1.requestFrom((int)AXP, 2);
    uint8_t a = Wire1.available() ? Wire1.read() : 0, b = Wire1.available() ? Wire1.read() : 0;
    return ((uint16_t)a << 4) | (b & 0x0F);
  }
  int8_t blStep = -1;               // son yazilan LDO2 kademesi (-1 = kapali)

  void begin() {
    Wire1.begin(21, 22, 400000);
    // M5StickC Plus kutuphanesiyle ayni baslangic sirasi
    axpW(0x28, 0xCC);                          // LDO2 (isik) ve LDO3 (ekran) 3.0 V
    axpW(0x82, 0xFF);                          // tum ADC'ler acik
    axpW(0x33, 0xC0);                          // sarj 4.2 V / 100 mA
    axpW(0x12, axpR(0x12) | 0x4D);             // EXTEN, LDO2, LDO3, DCDC1 acik
    axpW(0x36, 0x0C);                          // 128 ms acilis, 4 sn kapanis
    axpW(0x91, 0xF0);                          // RTC 3.3 V
    axpW(0x90, 0x02);                          // GPIO0 = LDO (mikrofon)
    axpW(0x30, 0x80);                          // VBUS akim siniri kapali
    axpW(0x39, 0xFC);                          // sicaklik korumasi
    axpW(0x35, 0xA2);                          // RTC pili sarji
    axpW(0x32, 0x46);                          // pil algilama
    axpW(0x46, 0x03);                          // bekleyen tus kesmelerini temizle
    pinMode(10, OUTPUT); digitalWrite(10, HIGH);   // kirmizi LED kapali
    blStep = 99;
  }
  // 0 = kapali, 1..255 -> LDO2 2.5 .. 3.2 V (7..14 kademe)
  void backlight(uint8_t v) {
    int8_t st = v ? (int8_t)(7 + (v * 7 + 127) / 255) : -1;
    if (st == blStep) return;
    if (st < 0) axpW(0x12, axpR(0x12) & ~0x04);
    else {
      if (blStep < 0 || blStep == 99) axpW(0x12, axpR(0x12) | 0x04);
      axpW(0x28, (axpR(0x28) & 0x0F) | (uint8_t)(st << 4));
    }
    blStep = st;
  }
  float batteryVolts() { return axpR12(0x78) * 1.1f / 1000.0f; }
  bool  usbPower(float) { return (axpR(0x00) & 0xA0) != 0; }     // VBUS veya ACIN var
  bool  powerPressed() {                                         // guc tusu kisa basis
    static uint32_t last = 0;
    if (millis() - last < 40) return false;
    last = millis();
    uint8_t s = axpR(0x46);
    if (s & 0x03) { axpW(0x46, 0x03); return (s & 0x02) != 0; }
    return false;
  }
  void powerOff() { axpW(0x32, axpR(0x32) | 0x80); }             // AXP192 tam kapanis
  int chargeMa() {                                               // pil sarj akimi (13 bit x 0.5 mA)
    Wire1.beginTransmission(AXP); Wire1.write(0x7A); Wire1.endTransmission();
    Wire1.requestFrom((int)AXP, 2);
    uint8_t a = Wire1.available() ? Wire1.read() : 0, b = Wire1.available() ? Wire1.read() : 0;
    return (((uint16_t)a << 5) | (b & 0x1F)) / 2;
  }
  float chipTemp() { return -144.7f + axpR12(0x5E) * 0.1f; }     // AXP192 ic sicakligi
  void led(bool on) { digitalWrite(10, on ? LOW : HIGH); }

#else
  // ---------------- M5StickC Plus2 ----------------
  constexpr uint8_t PIN_HOLD = 4, PIN_BL = 27, PIN_BAT = 38, PIN_PWR = 35, PIN_LED = 19;
  void begin() {
    pinMode(PIN_HOLD, OUTPUT); digitalWrite(PIN_HOLD, HIGH);   // gucu tut
    pinMode(PIN_LED, OUTPUT); digitalWrite(PIN_LED, LOW);
    pinMode(PIN_PWR, INPUT);
    analogSetPinAttenuation(PIN_BAT, ADC_11db);
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcAttach(PIN_BL, 5000, 8);
#else
    ledcSetup(0, 5000, 8); ledcAttachPin(PIN_BL, 0);
#endif
  }
  void backlight(uint8_t v) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcWrite(PIN_BL, v);
#else
    ledcWrite(0, v);
#endif
  }
  float batteryVolts() {
    uint32_t mv = 0;
    for (int i = 0; i < 8; i++) mv += analogReadMilliVolts(PIN_BAT);
    return (mv / 8.0f) * 2.0f / 1000.0f;
  }
  bool usbPower(float v) { return v > 4.35f; }   // Plus2'de USB algilama yok (tahmin)
  bool powerPressed() {
    static bool last = false; static uint32_t t = 0;
    bool d = digitalRead(PIN_PWR) == LOW;
    bool ev = d && !last && millis() - t > 150;
    if (d != last) t = millis();
    last = d;
    return ev;
  }
  void powerOff() { digitalWrite(PIN_HOLD, LOW); }   // USB takiliysa kapanmaz
  int   chargeMa() { return -1; }                    // Plus2: olcum yok
  float chipTemp() { return NAN; }
  void led(bool on) { digitalWrite(PIN_LED, on ? HIGH : LOW); }
#endif
}

// ============================================================================
//  EKRAN / ARKA ISIK
// ============================================================================
uint8_t blCurrent = 0, blTarget = 255;

void backlightWrite(uint8_t v) { HW::backlight(v); }

void backlightBegin() { backlightWrite(0); }

void backlightService() {
  static uint32_t last = 0;
  if (millis() - last < 8) return;
  last = millis();
  if (blCurrent == blTarget) return;
  int d = (int)blTarget - blCurrent;
  int step = max(1, abs(d) / 6);
  blCurrent += (d > 0) ? min(step, d) : max(-step, d);
  backlightWrite(blCurrent);
}

bool screenIsOff() { return blTarget == 0; }

bool userActivity() {
  bool wasOff = screenIsOff();
  app.lastInput = millis();
  blTarget = blFull();
  return wasOff;
}

void showToast(const char* msg, uint16_t color, uint32_t ms) {
  strncpy(app.toast, msg, sizeof(app.toast) - 1);
  app.toast[sizeof(app.toast) - 1] = 0;
  app.toastColor = color;
  app.toastUntil = millis() + ms;
}

void applyOrientation() {
  updateLayout();
  tft.setRotation(ORIENT_ROT[cfg.orient]);
  tft.fillScreen(C_BG);
  if (useSprite) spr.deleteSprite();
  useSprite = false;
  spr.setColorDepth(16);
  useSprite = spr.createSprite(SW, SH) != nullptr;
}

// ============================================================================
//  AYARLARI KAYDET / YUKLE
// ============================================================================
void saveState() {
  prefs.putInt("level", app.level);
  prefs.putBool("muted", app.muted);
  app.dirtySave = false;
}

void saveConfig() {
  prefs.putUChar("steps",  cfg.stepIdx);
  prefs.putUChar("bright", cfg.brightIdx);
  prefs.putUChar("sleep",  cfg.sleepIdx);
  prefs.putUChar("orient", cfg.orient);
  prefs.putUChar("saver",  cfg.saverIdx);
  prefs.putUChar("eye",    cfg.eyeIdx);
  prefs.putUChar("dbl",    cfg.dblClick);
  prefs.putUChar("both",   cfg.bothAct);
  prefs.putUChar("wxal",   cfg.wxAlerts);
  prefs.putUChar("cscan",  cfg.classicScan);
  prefs.putUChar("pconf",  cfg.pairConfirm);
  prefs.putUChar("arot",   cfg.autoRot);
  prefs.putUChar("rcal",   cfg.rotCal);
  prefs.putUChar("mic",    cfg.micOn);
  prefs.putUChar("snd",    cfg.sndLvl);
  prefs.putUChar("sndeye", cfg.sndEyes);
  prefs.putUChar("sstyle", cfg.saverStyle);
  prefs.putUChar("logo",   cfg.logoStyle);
}

void loadAll() {
  cfg.stepIdx   = prefs.getUChar("steps",  0) % COUNT_OF(STEP_OPTS);
  cfg.brightIdx = prefs.getUChar("bright", 0) % COUNT_OF(BRIGHT_OPTS);
  cfg.sleepIdx  = prefs.getUChar("sleep",  2) % COUNT_OF(SLEEP_MIN);
  if (prefs.isKey("orient")) cfg.orient = prefs.getUChar("orient", 0) % COUNT_OF(ORIENT_ROT);
  else cfg.orient = (prefs.getUChar("rot", 1) == 3) ? 1 : 0;      // v1.2'den gecis
  cfg.saverIdx  = prefs.getUChar("saver", 2) % COUNT_OF(SAVER_SEC);
  cfg.eyeIdx    = prefs.getUChar("eye",   0) % COUNT_OF(EYE_TXT);
  cfg.dblClick  = prefs.getUChar("dbl",   1) ? 1 : 0;
  cfg.bothAct   = prefs.getUChar("both",  0) ? 1 : 0;
  cfg.wxAlerts  = prefs.getUChar("wxal",  1) ? 1 : 0;
  cfg.classicScan = prefs.getUChar("cscan", 0) ? 1 : 0;
  cfg.pairConfirm = prefs.getUChar("pconf", 0) ? 1 : 0;
  cfg.rotCal   = prefs.getUChar("rcal", 0);
  cfg.autoRot  = (prefs.getUChar("arot", 0) && (cfg.rotCal & 0x80)) ? 1 : 0;
  cfg.micOn    = prefs.getUChar("mic", 1) ? 1 : 0;
  cfg.sndLvl   = prefs.getUChar("snd", 2) % 3;
  cfg.sndEyes  = prefs.getUChar("sndeye", 1) ? 1 : 0;
  cfg.saverStyle = 0;                                     // v5.5: koruyucu yalnizca gozler
  cfg.logoStyle  = prefs.getUChar("logo", 1) ? 1 : 0;
  app.level = constrain(prefs.getInt("level", volSteps() / 2), 0, volSteps());
  app.muted = prefs.getBool("muted", false);
}

// ============================================================================
//  BUZZER (G2) — engellemeyen nota sirasi
// ============================================================================
//  Notalar kucuk bir diziye yazilir, loop() icinde sirayla calinir: hicbir ses
//  programi bekletmez. Donanim PWM (LEDC kanal 2 / zamanlayici 1) kullanilir,
//  Plus2 ekran isigi (kanal 0 / zamanlayici 0) ile cakismaz.
namespace Snd {
  constexpr uint8_t PIN = 2, CH = 2, QMAX = 12;
  struct Note { uint16_t f; uint16_t ms; };
  Note     q[QMAX];
  uint8_t  qn = 0, qi = 0;
  uint32_t noteEnd = 0, lastEnd = 0;
  bool     ready = false;

  static void out(uint16_t f) {
    if (!ready) return;
    uint32_t duty = (cfg.sndLvl >= 2) ? 512 : 36;          // 10 bit: Normal %50, Kisik ~%3.5
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    if (!f) { ledcWrite(PIN, 0); return; }
    ledcChangeFrequency(PIN, f, 10);
    ledcWrite(PIN, duty);
#else
    if (!f) { ledcWrite(CH, 0); return; }
    ledcChangeFrequency(CH, f, 10);
    ledcWrite(CH, duty);
#endif
  }

  void begin() {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ready = ledcAttachChannel(PIN, 2000, 10, CH);
#else
    ledcSetup(CH, 2000, 10); ledcAttachPin(PIN, CH); ready = true;
#endif
    out(0);
  }

  bool muteAll = false;                     // karaoke: buzzer tamamen sessiz (mikrofona girmesin)
  bool enabled() { return ready && cfg.sndLvl && !app.nightOff && !muteAll; }
  bool busy()    { return qn || millis() - lastEnd < 160; }   // mikrofon kendi sesini duymasin

  void play(const Note* p, uint8_t n) {
    if (!enabled() || !n) return;
    qn = min(n, QMAX);
    memcpy(q, p, qn * sizeof(Note));
    qi = 0;
    out(q[0].f);
    noteEnd = millis() + q[0].ms;
  }

  void service() {
    if (!qn || (int32_t)(millis() - noteEnd) < 0) return;
    if (++qi >= qn) { out(0); qn = 0; lastEnd = millis(); return; }
    out(q[qi].f);
    noteEnd = millis() + q[qi].ms;
  }

  #define SND(...) do { static const Note N_[] = { __VA_ARGS__ }; play(N_, COUNT_OF(N_)); } while (0)
  void click()      { SND({2900, 9}); }
  void sel()        { SND({2100, 12}, {3200, 18}); }
  void back()       { SND({3000, 12}, {1900, 18}); }
  void open()       { SND({1600, 14}, {2400, 14}, {3200, 20}); }
  void limit()      { SND({220, 70}, {0, 40}, {220, 90}); }
  void error()      { SND({330, 90}, {0, 30}, {250, 140}); }
  void connect()    { SND({1319, 70}, {1760, 70}, {2637, 120}); }
  void disconnect() { SND({2637, 70}, {1760, 70}, {1175, 140}); }
  void charge()     { SND({1047, 70}, {1319, 70}, {1568, 70}, {2093, 150}); }
  void unplug()     { SND({1568, 70}, {1047, 120}); }
  void lowBat()     { SND({880, 120}, {0, 80}, {660, 180}); }
  void shutter()    { SND({4000, 10}, {0, 25}, {2500, 30}); }
  void ir()         { SND({3600, 8}); }
  void ok()         { SND({1760, 60}, {2349, 110}); }
  void boot()       { SND({1047, 60}, {1568, 60}, {2093, 110}); }
  void logo()       { SND({2093, 55}, {0, 25}, {3136, 240}); }   // sade logo: tek yumusak "ding"
  void mute(bool m) { if (m) SND({1400, 40}, {700, 90}); else SND({700, 40}, {1400, 90}); }
  void media(int d) { if (d > 0) SND({1800, 20}, {0, 15}, {2700, 30}); else if (d < 0) SND({2700, 20}, {0, 15}, {1800, 30}); else SND({2000, 25}, {2600, 40}); }
  void vol(float frac) { Note n[1] = {{(uint16_t)(900 + frac * 1900), 13}}; play(n, 1); }

  // Goz sesleri: ruh haline gore kisa "robot" cumleleri (R2 tarzi)
  void mood(uint8_t m) {                       // Eyes::Mood sirasi: NORMAL HAPPY TIRED ANGRY SURPRISED SLEEPY
    switch (m) {
      case 1: SND({1800, 40}, {2400, 40}, {3000, 40}, {3600, 70}); break;          // mutlu: yukselen tril
      case 2: SND({1400, 70}, {1100, 70}, {800, 140}); break;                      // yorgun: inen
      case 3: SND({300, 70}, {240, 70}, {300, 70}, {220, 120}); break;             // kizgin: homurdanma
      case 4: SND({1200, 15}, {1700, 15}, {2300, 15}, {3000, 20}, {3800, 45}); break; // saskin: "hop!"
      case 5: SND({520, 160}, {0, 90}, {440, 240}); break;                         // uykulu: esneme
      case 6: SND({1568, 70}, {2093, 70}, {1568, 70}, {2637, 180}); break;          // ask: kalp atisi
      case 7: SND({1000, 110}, {850, 140}, {640, 260}); break;                     // uzgun
      default: SND({2400, 30}, {0, 30}, {2400, 30}); break;
    }
  }
  // Maskot sahne sesleri
  void hi()      { SND({1568, 60}, {2093, 90}); }
  void powerUp() { SND({784, 45}, {988, 45}, {1175, 45}, {1568, 45}, {2093, 120}); }
  void fanfare() { SND({1047, 90}, {1319, 90}, {1568, 90}, {2093, 200}, {0, 60}, {1568, 70}, {2093, 260}); }
  void twinkle() { SND({2637, 40}, {0, 60}, {3136, 40}, {0, 80}, {3951, 60}); }
  void phew()    { SND({900, 120}, {700, 160}, {500, 220}); }
  void tune()    { SND({1319, 110}, {1568, 110}, {1760, 110}, {1568, 160}); }
  void magic()   { SND({1319, 50}, {1760, 50}, {2217, 50}, {2637, 50}, {3520, 120}); }
  void lullaby() { SND({1047, 200}, {880, 200}, {784, 320}); }
  void blup()    { uint16_t f = 1100 + esp_random() % 700; Note n[2] = {{f, 22}, {(uint16_t)(f + 700), 30}}; play(n, 2); }
  void chatter() {                             // rastgele gevezelik
    Note n[7];
    uint8_t k = 3 + esp_random() % 5;
    for (uint8_t i = 0; i < k; i++) {
      n[i].f  = (esp_random() % 6 == 0) ? 0 : (uint16_t)(1300 + esp_random() % 2800);
      n[i].ms = (uint16_t)(28 + esp_random() % 55);
    }
    play(n, k);
  }
  #undef SND
}

// ============================================================================
//  HAREKET SENSORU (MPU6886) + OTOMATIK EKRAN DONDURME
// ============================================================================
namespace Imu {
  constexpr uint8_t ADDR = 0x68;
  bool ok = false;

  static void w(uint8_t r, uint8_t v) {
    Wire1.beginTransmission(ADDR); Wire1.write(r); Wire1.write(v); Wire1.endTransmission();
  }
  static uint8_t rd(uint8_t r) {
    Wire1.beginTransmission(ADDR); Wire1.write(r);
    if (Wire1.endTransmission() != 0) return 0;
    Wire1.requestFrom((int)ADDR, 1);
    return Wire1.available() ? Wire1.read() : 0;
  }
  bool begin() {
#if M5_BOARD == 2
    Wire1.begin(21, 22, 400000);              // Plus2: dahili I2C (IMU + RTC)
#endif
    uint8_t id = rd(0x75);                     // WHO_AM_I (MPU6886 = 0x19)
    if (id == 0x00 || id == 0xFF) { ok = false; return false; }
    w(0x6B, 0x80); delay(10);                  // sifirla
    w(0x6B, 0x01); delay(10);                  // saat: PLL
    w(0x1C, 0x00);                             // ivme +-2 g
    w(0x1D, 0x04);                             // ivme alcak geciren (~20 Hz)
    w(0x1A, 0x01);
    w(0x19, 0x05);
    w(0x6C, 0x07);                             // jiroskop bekleme (guc tasarrufu), ivme acik
    ok = true;
    Serial.printf("[IMU] MPU6886 bulundu (id 0x%02X)\n", id);
    return true;
  }
  bool read(float& x, float& y, float& z) {
    if (!ok) return false;
    Wire1.beginTransmission(ADDR); Wire1.write(0x3B);
    if (Wire1.endTransmission() != 0) return false;
    if (Wire1.requestFrom((int)ADDR, 6) != 6) return false;
    int16_t v[3];
    for (int i = 0; i < 3; i++) { uint8_t h = Wire1.read(), l = Wire1.read(); v[i] = (int16_t)((h << 8) | l); }
    x = v[0] / 16384.0f; y = v[1] / 16384.0f; z = v[2] / 16384.0f;
    return true;
  }
}

//  Sensorun kart icindeki ekseni kesin bilinmedigi icin iki adimli ayar yapilir:
//  kullanici cihazi once "Dikey", sonra "Yatay" tutar; yercekimi ekseni ve yonu
//  kaydedilir (cfg.rotCal). Ters yonler bu ikisinin tersidir.
namespace AutoRot {
  uint8_t  calStep = 0;              // 0 yok, 1 dikey adimi, 2 yatay adimi
  uint8_t  calA0 = 0, calN0 = 0, prevOrient = 0;
  uint8_t  cand = 255;
  uint32_t candAt = 0, lastRead = 0;
  float    gx = 0, gy = 0, gz = 1;
  bool     changed = false;          // menu yeniden kurulsun

  bool valid() { return cfg.rotCal & 0x80; }

  // ORIENT indeksi: 0 Yatay, 1 Yatay ters, 2 Dikey, 3 Dikey ters
  static void expect(uint8_t o, uint8_t& axis, int8_t& sgn) {
    bool land = (o == 0 || o == 1);
    axis = land ? ((cfg.rotCal >> 2) & 1) : (cfg.rotCal & 1);
    bool neg = land ? ((cfg.rotCal >> 3) & 1) : ((cfg.rotCal >> 1) & 1);
    if (o == 1 || o == 3) neg = !neg;
    sgn = neg ? -1 : 1;
  }

  static bool capture(uint8_t& axis, uint8_t& neg) {
    float sx = 0, sy = 0, sz = 0, x, y, z;
    int n = 0;
    for (int i = 0; i < 6; i++) { if (Imu::read(x, y, z)) { sx += x; sy += y; sz += z; n++; } delay(8); }
    if (!n) return false;
    sx /= n; sy /= n; sz /= n;
    if (fabsf(sz) > 0.75f) return false;                 // masada yatiyor
    axis = fabsf(sy) > fabsf(sx) ? 1 : 0;
    float v = axis ? sy : sx;
    if (fabsf(v) < 0.55f) return false;
    neg = v < 0;
    return true;
  }

  void calStart() {
    if (!Imu::ok) { showToast("Hareket sensörü yok", C_BAD, 1800); Snd::error(); return; }
    prevOrient = cfg.orient;
    calStep = 1;
    cfg.orient = 2;                                      // Dikey
    applyOrientation();
  }
  void calCancel() {
    if (!calStep) return;
    calStep = 0;
    cfg.orient = prevOrient;
    applyOrientation();
    showToast("Ayar iptal edildi", C_SUBTEXT, 1000);
    Snd::back();
    changed = true;
  }
  void calCapture() {
    uint8_t a, n;
    if (!capture(a, n)) { showToast("Cihazı dik tutun", C_WARN, 1600); Snd::error(); return; }
    if (calStep == 1) {
      calA0 = a; calN0 = n; calStep = 2;
      cfg.orient = 0;                                    // Yatay
      applyOrientation();
      Snd::sel();
      return;
    }
    if (a == calA0) { showToast("Yana çevirip tekrar", C_WARN, 1800); Snd::error(); return; }
    cfg.rotCal = 0x80 | calA0 | (calN0 << 1) | (a << 2) | (n << 3);
    cfg.autoRot = 1;
    calStep = 0; cand = 255;
    saveConfig();
    showToast("Döndürme ayarlandı", C_OK, 1500);
    Snd::ok();
    changed = true;
  }

  void service() {
    if (!Imu::ok || !cfg.autoRot || !valid() || calStep) return;
    uint32_t now = millis();
    if (now - lastRead < 100) return;
    lastRead = now;
    float x, y, z;
    if (!Imu::read(x, y, z)) return;
    gx += (x - gx) * 0.45f; gy += (y - gy) * 0.45f; gz += (z - gz) * 0.45f;
    // Eslesme / onay / ekran kapaliyken donme yok (tampon bos olabilir)
    if (directMode || Pair::modal != Pair::M_NONE || blTarget == 0) { cand = 255; return; }
    if (fabsf(gz) > 0.80f) { cand = 255; return; }      // duz duruyor: son yon kalsin
    int best = -1; float bv = 0;
    for (uint8_t o = 0; o < 4; o++) {
      uint8_t a; int8_t s; expect(o, a, s);
      float v = s * (a ? gy : gx);
      if (v > bv) { bv = v; best = o; }
    }
    if (best < 0 || bv < 0.62f || best == cfg.orient) { cand = 255; return; }
    if (best != cand) { cand = best; candAt = now; return; }
    if (now - candAt >= 550) {                          // 0.55 sn kararli -> dondur
      cfg.orient = best;
      applyOrientation();
      cand = 255;
      changed = true;
    }
  }
}

// ============================================================================
//  MIKROFON (PDM: saat G0, veri G34) — yalnizca ekran koruyucuda acik
// ============================================================================
//  Koruyucu kapaliyken surucu tamamen kaldirilir (0 bayt RAM). Acikken ~2 KB
//  DMA tamponu + 640 bayt statik tampon kullanilir. Okuma beklemesiz yapilir.
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  #include "driver/i2s_pdm.h"
#else
  #include "driver/i2s.h"
#endif

namespace Mic {
  constexpr uint8_t PIN_CLK = 0, PIN_DAT = 34;
  constexpr int FRAMES = 160;                // 10 ms @ 16 kHz
  int16_t  buf[FRAMES * 2];
  bool     running = false;
  uint32_t retryAt = 0;
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  i2s_chan_handle_t rx = nullptr;
  constexpr int CHN = 2;                     // iki kanal okunur, sinyali olan secilir
#else
  constexpr int CHN = 1;
#endif

  // Seviye analizi
  float    fast = 0, slow = 0, floorDb = -58;
  uint8_t  hist[16] = {0};
  uint8_t  hIdx = 0;
  uint32_t lastHist = 0, lastBeat = 0, lastClap = 0, loudSince = 0, quietSince = 0;
  bool     music = false, beatFlag = false, clapFlag = false;
  float    beatAmp = 0;

  void reset() {
    fast = slow = 0; music = false; beatFlag = clapFlag = false;
    memset(hist, 0, sizeof(hist)); loudSince = quietSince = 0;
  }

  bool startCfg(uint32_t rate, int descs, int frames) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    i2s_chan_config_t cc = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    cc.dma_desc_num = descs; cc.dma_frame_num = frames;
    if (i2s_new_channel(&cc, nullptr, &rx) != ESP_OK) { rx = nullptr; return false; }
    i2s_pdm_rx_config_t pc = {
      .clk_cfg  = I2S_PDM_RX_CLK_DEFAULT_CONFIG(rate),
      .slot_cfg = I2S_PDM_RX_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
      .gpio_cfg = { .clk = (gpio_num_t)PIN_CLK, .din = (gpio_num_t)PIN_DAT, .invert_flags = { .clk_inv = false } },
    };
    if (i2s_channel_init_pdm_rx_mode(rx, &pc) != ESP_OK || i2s_channel_enable(rx) != ESP_OK) {
      i2s_del_channel(rx); rx = nullptr; return false;
    }
#else
    i2s_config_t ic = {};
    ic.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
    ic.sample_rate = rate;
    ic.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    ic.channel_format = I2S_CHANNEL_FMT_ALL_RIGHT;       // M5 ornekleriyle ayni
    ic.communication_format = I2S_COMM_FORMAT_STAND_I2S;
    ic.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
    ic.dma_buf_count = descs; ic.dma_buf_len = frames;
    if (i2s_driver_install(I2S_NUM_0, &ic, 0, nullptr) != ESP_OK) return false;
    i2s_pin_config_t pp = {};
    pp.mck_io_num = I2S_PIN_NO_CHANGE; pp.bck_io_num = I2S_PIN_NO_CHANGE;
    pp.ws_io_num = PIN_CLK; pp.data_out_num = I2S_PIN_NO_CHANGE; pp.data_in_num = PIN_DAT;
    if (i2s_set_pin(I2S_NUM_0, &pp) != ESP_OK) { i2s_driver_uninstall(I2S_NUM_0); return false; }
#endif
    running = true;
    reset();
    return true;
  }
  bool start() {
    if (ESP.getFreeHeap() < 22000) return false;       // RAM azsa hic acma (surucu ~3 KB)
    return startCfg(16000, 3, FRAMES);
  }
  // Karaoke: bekleyerek okuma (bayt)
  size_t readWait(void* dst, size_t bytes, uint32_t ms) {
    size_t got = 0;
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    if (i2s_channel_read(rx, dst, bytes, &got, ms) != ESP_OK) return got;
#else
    i2s_read(I2S_NUM_0, dst, bytes, &got, pdMS_TO_TICKS(ms));
#endif
    return got;
  }

  void stop() {
    if (!running) return;
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    i2s_channel_disable(rx); i2s_del_channel(rx); rx = nullptr;
#else
    i2s_driver_uninstall(I2S_NUM_0);
#endif
    running = false;
    reset();
  }

  static size_t readChunk() {
    size_t got = 0;
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    if (i2s_channel_read(rx, buf, sizeof(buf), &got, 0) != ESP_OK) return 0;
#else
    if (i2s_read(I2S_NUM_0, buf, FRAMES * CHN * 2, &got, 0) != ESP_OK) return 0;
#endif
    return got / 2;                                     // ornek sayisi
  }

  static void analyze(size_t n) {
    // Kanal basina ortalamadan sapma (DC kaldirilmis ortalama genlik)
    float best = 0;
    for (int c = 0; c < CHN; c++) {
      int32_t sum = 0; int cnt = 0;
      for (size_t i = c; i < n; i += CHN) { sum += buf[i]; cnt++; }
      if (!cnt) continue;
      int32_t mean = sum / cnt; uint32_t dev = 0;
      for (size_t i = c; i < n; i += CHN) dev += abs(buf[i] - mean);
      best = max(best, dev / (float)cnt);
    }
    float db = 20.0f * log10f(max(best, 1.0f) / 32768.0f);
    // Uyarlanir gurultu tabani: hizli iner, yavas cikar
    floorDb += (db - floorDb) * (db < floorDb ? 0.20f : 0.002f);
    floorDb = constrain(floorDb, -85.0f, -28.0f);
    float lvl = constrain((db - floorDb - 5.0f) / 28.0f, 0.0f, 1.0f);
    fast += (lvl - fast) * (lvl > fast ? 0.70f : 0.22f);
    slow += (lvl - slow) * 0.03f;

    uint32_t now = millis();
    if (fast > 0.85f && slow < 0.30f && now - lastClap > 1200) { clapFlag = true; lastClap = now; lastBeat = now; }
    else if (fast - slow > 0.20f && fast > 0.35f && now - lastBeat > 230) { beatFlag = true; beatAmp = fast; lastBeat = now; }

    if (slow > 0.30f) { if (!loudSince) loudSince = now; quietSince = 0; if (now - loudSince > 2500) music = true; }
    else if (slow < 0.15f) { loudSince = 0; if (!quietSince) quietSince = now; if (now - quietSince > 3000) music = false; }
  }

  void service() {
    uint32_t now = millis();
    bool want = cfg.micOn && app.saver && !directMode && !app.nightOff && blTarget > 0;
    if (want && !running && (int32_t)(now - retryAt) >= 0) {
      if (!start()) retryAt = now + 10000;               // olmadiysa 10 sn sonra tekrar
    }
    if (!want && running) stop();
    if (!running) return;

    for (int k = 0; k < 3; k++) {                        // birikenleri bosalt (en fazla 30 ms veri)
      size_t n = readChunk();
      if (n < 32) break;
      if (Snd::busy()) continue;                         // kendi buzzer sesini dinleme
      analyze(n);
    }
    if (now - lastHist >= 50) {
      lastHist = now;
      hist[hIdx] = (uint8_t)(fast * 255);
      hIdx = (hIdx + 1) % COUNT_OF(hist);
    }
  }

  bool takeBeat(float& amp) { if (!beatFlag) return false; beatFlag = false; amp = beatAmp; return true; }
  bool takeClap() { if (!clapFlag) return false; clapFlag = false; return true; }
  uint8_t histAt(int i) { return hist[(hIdx + i) % COUNT_OF(hist)]; }   // 0 = en eski
}

// ============================================================================
//  IR KUMANDA (G9 Plus / G19 Plus2) — TV ac/kapa, ses
// ============================================================================
//  Tasiyici frekans yazilimla (CPU sayaci) uretilir: ek kutuphane ve RAM yok.
//  Her "isaret" kisa bir kritik bolumde gonderilir (en fazla 9 ms), bosluklar
//  kesmeler acikken beklenir. Kod tablolari flash'ta (const).
namespace IR {
#if M5_BOARD == 1
  constexpr uint8_t PIN = 9;
#else
  constexpr uint8_t PIN = 19;                // Plus2: kirmizi LED ile ortak
#endif
  enum Proto : uint8_t { P_NONE, P_NEC, P_SAMSUNG, P_SONY12, P_RC5, P_RC6, P_PANA, P_JVC, P_SHARP, P_SONY15, P_SONY20,
                         P_RCA, P_KASE, P_RAW = 0xFF };
  // proto | aux (ham: ornek sayisi) | addr (RC5/RC6/RCA/Sharp adres, Kaseikyo ilk 16 bit, ham: IRRAW konumu) | data
  struct Code { uint8_t proto; uint8_t aux; uint16_t addr; uint32_t data; };
  typedef Code IRE;
  struct IRBrand { const char* name; uint16_t k[4]; };    // IRDB indeksleri: Guc, Ses+, Ses-, Sessiz
  #include "ir_db.h"                                     // ~290 guc kodu + 26 marka (flash'ta)
  constexpr int NBRAND = COUNT_OF(IRBRANDS);
  static const char* KEY_TXT[4] = {"Güç (aç/kapa)", "Ses +", "Ses -", "Sessiz"};
  // Kod tarama adaylari = markalar
  constexpr int NCAND = NBRAND;
  const char* candName(int i) { return IRBRANDS[i].name; }
  Code candKey(int i, int k) {
    uint16_t x = IRBRANDS[i].k[k];
    return x < IR_NDB ? IRDB[x] : Code{P_NONE, 0, 0, 0};
  }

  uint32_t cpu = 240;                        // CPU sayaci / mikrosaniye
  uint32_t cur = 0;                          // zaman imleci (cevrim)
  uint32_t mask = 1UL << PIN;
  uint16_t khz = 38;
  bool     toggle = false;
  portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

  // Tarama (tum guc kodlari; sirali veya rastgele)
  bool     sweeping = false, changed = false, randomOrder = false;
  int      sweepIdx = 0;
  uint32_t sweepNext = 0, sweepStartMs = 0;
  uint16_t rStart = 0, rStride = 1;                    // rastgele sira: (bas + i*adim) mod N, RAM gerektirmez

  void begin() {
    pinMode(PIN, OUTPUT); digitalWrite(PIN, LOW);
    cpu = getCpuFrequencyMhz();
  }

  static inline uint32_t cyc() { return ESP.getCycleCount(); }
  static void startFrame(uint16_t k) { khz = k; cur = cyc(); }
  static void mark(uint32_t us) {
    uint32_t period = cpu * 1000 / khz, on = period / 3;
    uint32_t end = cur + us * cpu, t = cur;
    portENTER_CRITICAL(&mux);
    while ((int32_t)(end - t) > 0) {
      REG_WRITE(GPIO_OUT_W1TS_REG, mask);
      while ((int32_t)(cyc() - (t + on)) < 0) {}
      REG_WRITE(GPIO_OUT_W1TC_REG, mask);
      t += period;
      while ((int32_t)(cyc() - t) < 0) {}
    }
    REG_WRITE(GPIO_OUT_W1TC_REG, mask);
    portEXIT_CRITICAL(&mux);
    cur = end;
  }
  static void space(uint32_t us) {
    cur += us * cpu;
    while ((int32_t)(cyc() - cur) < 0) {}
  }

  static void pulseDist(uint32_t data, int bits, uint16_t bm, uint16_t one, uint16_t zero, bool msb) {
    for (int i = 0; i < bits; i++) {
      bool b = msb ? (data >> (bits - 1 - i)) & 1 : (data >> i) & 1;
      mark(bm); space(b ? one : zero);
    }
  }

  static void nec(uint32_t d, bool samsung) {
    startFrame(38);
    mark(samsung ? 4500 : 9000); space(4500);
    pulseDist(d, 32, 560, 1690, 560, true);
    mark(560);
  }
  static void sony(uint32_t d, int bits = 12) {
    for (int r = 0; r < 3; r++) {
      startFrame(40);
      mark(2400); space(600);
      for (int i = bits - 1; i >= 0; i--) { mark((d >> i) & 1 ? 1200 : 600); space(600); }
      delay(26);
    }
  }
  static void rc5(uint8_t addr, uint8_t cmd) {
    uint16_t w = (1 << 13) | ((cmd < 64) << 12) | (toggle << 11) | ((addr & 0x1F) << 6) | (cmd & 0x3F);
    for (int r = 0; r < 2; r++) {
      startFrame(36);
      for (int i = 13; i >= 0; i--) {
        if ((w >> i) & 1) { space(889); mark(889); } else { mark(889); space(889); }
      }
      delay(90);
    }
  }
  static void rc6(uint8_t addr, uint8_t cmd) {
    for (int r = 0; r < 2; r++) {
      startFrame(36);
      mark(2666); space(889);
      mark(444); space(444);                                  // baslangic biti (1)
      for (int i = 0; i < 3; i++) { space(444); mark(444); }  // mod 000
      if (toggle) { mark(889); space(889); } else { space(889); mark(889); }
      uint16_t w = ((uint16_t)addr << 8) | cmd;
      for (int i = 15; i >= 0; i--) { if ((w >> i) & 1) { mark(444); space(444); } else { space(444); mark(444); } }
      space(2666);
      delay(80);
    }
  }
  static void rca(uint8_t addr, uint8_t cmd) {           // RCA (TCL/Thomson): 56 kHz
    uint32_t w = ((uint32_t)(addr & 0xF) << 20) | ((uint32_t)cmd << 12) | ((uint32_t)(~addr & 0xF) << 8) | (uint8_t)~cmd;
    for (int r = 0; r < 2; r++) {
      startFrame(56);
      mark(4000); space(4000);
      pulseDist(w, 24, 500, 2000, 1000, true);
      mark(500);
      delay(10);
    }
  }
  static void pana(uint32_t d, uint16_t hi = 0x4004) {   // Kaseikyo (Panasonic vb.)
    for (int r = 0; r < 2; r++) {
      startFrame(37);
      mark(3456); space(1728);
      pulseDist(hi, 16, 432, 1296, 432, true);
      pulseDist(d, 32, 432, 1296, 432, true);
      mark(432);
      delay(75);
    }
  }
  static void jvc(uint32_t d) {
    startFrame(38);
    mark(8400); space(4200);
    pulseDist(d, 16, 526, 1574, 526, true); mark(526);
    delay(24);
    startFrame(38);                                          // tekrar (basliksiz)
    pulseDist(d, 16, 526, 1574, 526, true); mark(526);
  }
  static void sharp(uint8_t addr, uint8_t cmd) {
    for (int r = 0; r < 2; r++) {
      uint16_t w = (addr & 0x1F) | ((uint16_t)(r ? (uint8_t)~cmd : cmd) << 5) | ((r ? 0 : 1) << 13) | ((r ? 1 : 0) << 14);
      startFrame(38);
      pulseDist(w, 15, 320, 1680, 680, false);
      mark(320);
      delay(40);
    }
  }

  void sendRaw(const uint16_t* d, int n);
  bool send(const Code& c) {
    if (c.proto == P_NONE) return false;
#if M5_BOARD == 1
    HW::led(true);                           // Plus: kirmizi LED (G10) gonderim gostergesi
#endif
    switch (c.proto) {
      case P_NEC:     nec(c.data, false); break;
      case P_SAMSUNG: nec(c.data, true);  break;
      case P_SONY12:  sony(c.data, 12);   break;
      case P_SONY15:  sony(c.data, 15);   break;
      case P_SONY20:  sony(c.data, 20);   break;
      case P_RC5:     rc5((uint8_t)c.addr, (uint8_t)c.data); break;
      case P_RCA:     rca((uint8_t)c.addr, (uint8_t)c.data); break;
      case P_KASE:    pana(c.data, c.addr); break;
      case P_RAW:     sendRaw(IRRAW + c.addr, c.aux); break;
      case P_RC6:     rc6((uint8_t)c.addr, (uint8_t)c.data); break;
      case P_PANA:    pana(c.data);       break;
      case P_JVC:     jvc(c.data);        break;
      case P_SHARP:   sharp((uint8_t)c.addr, (uint8_t)c.data); break;
    }
    digitalWrite(PIN, LOW);
#if M5_BOARD == 1
    HW::led(false);
#endif
    toggle = !toggle;
    return true;
  }
  // Ogrenilmis ham kod: sure dizisi (isaret, bosluk, isaret, ...) 38 kHz, bir kez
  void sendRaw(const uint16_t* d, int n) {
    startFrame(38);
    for (int i = 0; i < n; i++) { if (i & 1) space(d[i]); else mark(d[i]); }
    digitalWrite(PIN, LOW);
    toggle = !toggle;
  }
  bool has(int b, int k) { return b >= 0 && b < NBRAND && k >= 0 && k < 4 && IRBRANDS[b].k[k] < IR_NDB; }
  void sendKey(int b, int k) {
    if (!has(b, k)) return;
    Snd::ir();
    send(IRDB[IRBRANDS[b].k[k]]);
  }

  static uint16_t gcd(uint16_t a, uint16_t b) { while (b) { uint16_t t = a % b; a = b; b = t; } return a; }
  void sweepStart() {
    sweeping = true; sweepIdx = 0; sweepNext = millis(); sweepStartMs = millis(); changed = true;
    rStart = esp_random() % IR_NPWR;
    do { rStride = 1 + esp_random() % (IR_NPWR - 1); } while (gcd(rStride, IR_NPWR) != 1);
  }
  void sweepStop()  { if (sweeping) { sweeping = false; changed = true; } }
  int  sweepCode(int i) { return randomOrder ? (rStart + (uint32_t)i * rStride) % IR_NPWR : i; }
  uint32_t sweepEtaS() {                                   // kalan sure tahmini (sn)
    if (sweepIdx <= 0) return IR_NPWR / 4;
    uint32_t per = (millis() - sweepStartMs) / sweepIdx;
    return (uint32_t)(IR_NPWR - sweepIdx) * per / 1000;
  }
  void service() {
    if (!sweeping || (int32_t)(millis() - sweepNext) < 0) return;
    if (sweepIdx >= IR_NPWR) {
      sweeping = false; changed = true;
      showToast("Tüm kodlar gönderildi", C_OK, 1500);
      Snd::ok();
      return;
    }
    send(IRDB[sweepCode(sweepIdx)]);
    sweepIdx++;
    sweepNext = millis() + 90;
    if ((sweepIdx & 7) == 0) changed = true;               // ekran ilerlemesi (her 8 kodda)
  }
}

// ============================================================================
//  OGRENILEN KUMANDALAR (IR alici ile) + KOD TARAMA
// ============================================================================
//  3 kumanda yuvasi x 6 tus. Bilinen protokoller (NEC, Samsung, Sony, RC5, RC6)
//  cozulup 8 bayt olarak, digerleri ham sure dizisi olarak NVS'ye yazilir.
//  RAM: yalnizca ~550 bayt statik tampon; kodlar gonderirken tek tek okunur.
namespace IRL {
  constexpr int SLOTS = 3, KEYS = 6, CAPMAX = 132;
  static const char* NAMES[] = {"Teba TV", "TV", "Soundbar", "Uydu alıcı", "Projektör", "Cihaz"};
  static const char* KEYN[KEYS] = {"Güç", "Ses +", "Ses -", "Sessiz", "Kanal +", "Kanal -"};
  constexpr int NNAMES = COUNT_OF(NAMES);

  uint8_t  nameIdx[SLOTS];                  // 0xFF = yuva bos
  uint8_t  keyMask[SLOTS];                  // kayitli tuslar (bit)
  Preferences pr;

  // ---- Kalici kayit ---------------------------------------------------------
  static void keyName(char* k, int s, int key) { snprintf(k, 8, "k%d%d", s, key); }
  void load() {
    pr.begin("irlearn", true);
    for (int s = 0; s < SLOTS; s++) {
      char k[8]; snprintf(k, sizeof(k), "n%d", s);
      nameIdx[s] = pr.getUChar(k, 0xFF);
      snprintf(k, sizeof(k), "m%d", s);
      keyMask[s] = pr.getUChar(k, 0);
      if (!keyMask[s]) nameIdx[s] = 0xFF;
    }
    pr.end();
  }
  bool used(int s) { return s >= 0 && s < SLOTS && keyMask[s]; }
  int  firstEmpty() { for (int s = 0; s < SLOTS; s++) if (!used(s)) return s; return -1; }
  const char* name(int s) {
    uint8_t i = nameIdx[s] == 0xFF ? (s == 0 ? 0 : NNAMES - 1) : nameIdx[s];
    return NAMES[i % NNAMES];
  }
  static void saveMeta(int s) {
    pr.begin("irlearn", false);
    char k[8];
    snprintf(k, sizeof(k), "n%d", s); pr.putUChar(k, nameIdx[s] == 0xFF ? (s == 0 ? 0 : NNAMES - 1) : nameIdx[s]);
    snprintf(k, sizeof(k), "m%d", s); pr.putUChar(k, keyMask[s]);
    pr.end();
  }
  void cycleName(int s) {
    uint8_t cur = nameIdx[s] == 0xFF ? (s == 0 ? 0 : NNAMES - 1) : nameIdx[s];
    nameIdx[s] = (cur + 1) % NNAMES;
    if (used(s)) saveMeta(s);
  }
  static bool putKey(int s, int key, const uint8_t* blob, size_t len) {
    pr.begin("irlearn", false);
    char k[8]; keyName(k, s, key);
    bool ok = pr.putBytes(k, blob, len) == len;
    pr.end();
    if (!ok) return false;
    if (nameIdx[s] == 0xFF) nameIdx[s] = (s == 0 ? 0 : NNAMES - 1);
    keyMask[s] |= 1 << key;
    saveMeta(s);
    return true;
  }
  static bool putCode(int s, int key, const IR::Code& c, uint8_t bits = 0) {
    if (c.proto == IR::P_RAW) {                     // veritabanindaki ham kod: sureleri kopyala
      int cnt = min((int)c.aux, CAPMAX);
      uint8_t tmp[2 + 2 * CAPMAX];
      tmp[0] = IR::P_RAW; tmp[1] = (uint8_t)cnt;
      memcpy(tmp + 2, IR::IRRAW + c.addr, cnt * 2);
      return putKey(s, key, tmp, 2 + cnt * 2);
    }
    uint8_t b[8] = {c.proto, c.aux ? c.aux : bits, (uint8_t)c.addr, (uint8_t)(c.addr >> 8),
                    (uint8_t)c.data, (uint8_t)(c.data >> 8), (uint8_t)(c.data >> 16), (uint8_t)(c.data >> 24)};
    return putKey(s, key, b, 8);
  }
  void removeSlot(int s) {
    pr.begin("irlearn", false);
    for (int key = 0; key < KEYS; key++) { char k[8]; keyName(k, s, key); pr.remove(k); }
    char k[8];
    snprintf(k, sizeof(k), "n%d", s); pr.remove(k);
    snprintf(k, sizeof(k), "m%d", s); pr.remove(k);
    pr.end();
    keyMask[s] = 0; nameIdx[s] = 0xFF;
  }

  // ---- Gonderme -------------------------------------------------------------
  uint8_t blob[4 + 2 * CAPMAX];
  bool sendKey(int s, int key) {
    if (!used(s) || !(keyMask[s] & (1 << key))) return false;
    pr.begin("irlearn", true);
    char k[8]; keyName(k, s, key);
    size_t n = pr.getBytes(k, blob, sizeof(blob));
    pr.end();
    if (n < 2) return false;
    Snd::ir();
    if (blob[0] == IR::P_RAW) {
      int cnt = min((int)blob[1], CAPMAX);
      uint16_t d[CAPMAX];                    // yigitta ~260 bayt, kisa sureli
      memcpy(d, blob + 2, cnt * 2);
      IR::sendRaw(d, cnt);
    } else if (n >= 8) {
      IR::Code c{blob[0], blob[1], (uint16_t)(blob[2] | (blob[3] << 8)), (uint32_t)blob[4] | ((uint32_t)blob[5] << 8) | ((uint32_t)blob[6] << 16) | ((uint32_t)blob[7] << 24)};
      IR::send(c);
    }
    return true;
  }

  // ---- Yakalama (yoklama ile; kesme/IRAM kullanmaz) ------------------------
  //  Ogrenme ekranindayken her dongude en fazla ~120 ms pin izlenir. Pin en az
  //  3 ms bosta (HIGH) kaldiktan sonra gelen ilk dusen kenardan itibaren tum
  //  kenar sureleri CPU sayaciyla olculur; 20 ms sessizlik cerceveyi bitirir.
  uint16_t cap[CAPMAX];
  int      capN = 0;
  static inline bool rxLow() { return !((REG_READ(GPIO_IN_REG) >> PIN_IR_RX) & 1); }

  static int capture(uint32_t windowMs) {
    const uint32_t cpu = getCpuFrequencyMhz();
    uint32_t t0 = ESP.getCycleCount(), idleFrom = t0;
    bool idle = false;
    while ((ESP.getCycleCount() - t0) / cpu < windowMs * 1000) {       // kenar bekle
      uint32_t now = ESP.getCycleCount();
      if (rxLow()) {
        if (idle) break;
        idleFrom = now;                                                // cerceve ortasi: bosluk bekle
      } else if (!idle && (now - idleFrom) / cpu > 3000) idle = true;
    }
    if (!idle || !rxLow()) return 0;
    int n = 0;
    bool level = true;                                                  // true = isaret (LOW)
    uint32_t edge = ESP.getCycleCount();
    while (n < CAPMAX) {
      uint32_t now = ESP.getCycleCount();
      if (rxLow() != level) {
        uint32_t us = (now - edge) / cpu;
        cap[n++] = us > 65535 ? 65535 : us;
        edge = now; level = !level;
      } else if (!level && (now - edge) / cpu > 20000) break;           // cerceve bitti
      else if (level && (now - edge) / cpu > 20000) { n = 0; break; }   // surekli LOW: alici hatali
    }
    return n;
  }

  // ---- Cozucu ---------------------------------------------------------------
  static bool near(uint32_t v, uint32_t t) { return v > t * 6 / 10 && v < t * 14 / 10; }

  static bool decode(const uint16_t* d, int n, IR::Code& c, uint8_t& bits) {
    bits = 0;
    // NEC / Samsung: 9000|4500 isaret, 4500 bosluk, 32 bit, bitis isareti
    if (n >= 67 && near(d[1], 4500) && (near(d[0], 9000) || near(d[0], 4500))) {
      uint32_t v = 0;
      for (int i = 0; i < 32; i++) {
        uint16_t sp = d[3 + 2 * i];
        if (!near(d[2 + 2 * i], 560)) return false;
        v = (v << 1) | (sp > 1100 ? 1 : 0);
      }
      c = IR::Code{(uint8_t)(near(d[0], 9000) ? IR::P_NEC : IR::P_SAMSUNG), 0, 0, v};
      return true;
    }
    // Sony SIRC: 2400 baslik, 600 bosluk, 12/15/20 bit (isaret uzunlugu)
    if (near(d[0], 2400) && near(d[1], 600)) {
      int b = (n - 1) / 2;
      if (b != 12 && b != 15 && b != 20) return false;
      uint32_t v = 0;
      for (int i = 0; i < b; i++) v = (v << 1) | (d[2 + 2 * i] > 900 ? 1 : 0);
      c = IR::Code{(uint8_t)(b == 12 ? IR::P_SONY12 : b == 15 ? IR::P_SONY15 : IR::P_SONY20), 0, 0, v};
      return true;
    }
    // RC6 mod 0: 2666 onder isaret, 889 bosluk, 444 us birimleri
    if (near(d[0], 2666) && near(d[1], 889)) {
      uint8_t lv[64]; int u = 0;
      for (int i = 2; i < n && u < 64; i++) {
        int k = (d[i] + 222) / 444;
        if (k < 1 || k > 4) return false;
        for (int j = 0; j < k && u < 64; j++) lv[u++] = !(i & 1);   // cift indeks = isaret
      }
      while (u < 44) lv[u++] = 0;
      auto bit2 = [&](int p) -> int { return (lv[p] && !lv[p + 1]) ? 1 : (!lv[p] && lv[p + 1]) ? 0 : -1; };
      if (bit2(0) != 1) return false;                           // baslangic biti
      for (int m = 0; m < 3; m++) if (bit2(2 + 2 * m) != 0) return false;   // mod 000
      if (!((lv[8] && lv[9] && !lv[10] && !lv[11]) || (!lv[8] && !lv[9] && lv[10] && lv[11]))) return false;
      uint32_t w = 0;
      for (int i = 0; i < 16; i++) { int b = bit2(12 + 2 * i); if (b < 0) return false; w = (w << 1) | b; }
      c = IR::Code{IR::P_RC6, 0, (uint16_t)(w >> 8), w & 0xFF};
      return true;
    }
    // RC5: 889 us yarim bitler (Manchester), 14 bit
    if (near(d[0], 889) || near(d[0], 1778)) {
      uint8_t lv[32]; int u = 0;
      lv[u++] = 0;                                              // gorunmeyen ilk yarim (bosluk)
      for (int i = 0; i < n && u < 32; i++) {
        int k = (d[i] + 444) / 889;
        if (k < 1 || k > 2) return false;
        for (int j = 0; j < k && u < 32; j++) lv[u++] = !(i & 1);
      }
      while (u < 28) lv[u++] = 0;
      if (u != 28) return false;
      uint16_t w = 0;
      for (int i = 0; i < 14; i++) {
        uint8_t a = lv[2 * i], b = lv[2 * i + 1];
        if (a == b) return false;
        w = (w << 1) | (b ? 1 : 0);                             // 1 = bosluk->isaret
      }
      if (!(w & 0x2000)) return false;
      uint8_t cmd = (w & 0x3F) | ((w & 0x1000) ? 0 : 0x40);
      c = IR::Code{IR::P_RC5, 0, (uint16_t)((w >> 6) & 0x1F), cmd};
      return true;
    }
    return false;
  }

  // ---- Ogrenme / tarama ekran modlari -----------------------------------
  enum Mode : uint8_t { M_NONE, M_LEARN, M_SCAN };
  Mode     mode = M_NONE;
  int      slot = 0, key = 0, scanIdx = 0;
  uint32_t modeAt = 0;
  bool     changed = false;
  char     last[24] = {0};                  // son ogrenilen kodun turu

  void learnStart(int s, int k) {
    slot = s; key = k; mode = M_LEARN; modeAt = millis(); last[0] = 0;
    pinMode(PIN_IR_RX, INPUT_PULLUP);
  }
  static void learnEnd() { mode = M_NONE; changed = true; }

  void scanSend() {
    IR::Code c = IR::candKey(scanIdx, 0);
    Snd::ir();
    IR::send(c);
  }
  void scanStart(int s) { slot = s; scanIdx = 0; mode = M_SCAN; modeAt = millis(); scanSend(); }

  void cancel() {
    if (mode == M_LEARN) learnEnd();
    mode = M_NONE; changed = true;
    showToast("İptal", C_SUBTEXT, 900);
    Snd::back();
  }
  // A = true, B = false
  void onKey(bool a) {
    modeAt = millis();
    if (mode == M_SCAN) {
      if (!a) { scanIdx = (scanIdx + 1) % IR::NCAND; scanSend(); return; }
      int saved = 0;                                  // tepki verdi: ailenin tum tuslarini kaydet
      for (int k = 0; k < 4; k++) {
        IR::Code c = IR::candKey(scanIdx, k);
        if (c.proto != IR::P_NONE && putCode(slot, k, c)) saved++;
      }
      mode = M_NONE; changed = true;
      char t[32]; snprintf(t, sizeof(t), saved ? "Kaydedildi: %s" : "Kayıt hatası", IR::candName(scanIdx));
      showToast(t, saved ? C_OK : C_BAD, 2200);
      if (saved) Snd::ok(); else Snd::error();
      return;
    }
    if (mode == M_LEARN) {
      if (!a) cancel();
      else showToast("Kumandada tuşa basın", C_ACCENT, 900);
    }
  }

  void service() {
    if (mode != M_LEARN) return;
    if (millis() - modeAt > 15000) {
      learnEnd();
      showToast("Kod gelmedi (alıcı bağlı mı?)", C_WARN, 2500);
      Snd::error();
      return;
    }
    int n = capture(120);
    if (n < 8) return;                                 // yok / tekrar karesi / parazit: dinlemeye devam
    const uint16_t* d = cap;
    IR::Code c; uint8_t bits;
    bool ok;
    const char* kind;
    if (decode(d, n, c, bits)) {
      ok = putCode(slot, key, c, bits);
      static const char* PN[] = {"?", "NEC", "Samsung", "Sony", "RC5", "RC6", "Panasonic", "JVC", "Sharp", "Sony15", "Sony20"};
      kind = c.proto < COUNT_OF(PN) ? PN[c.proto] : "?";
    } else {
      if (n >= CAPMAX) { learnEnd(); showToast("Kod çok uzun", C_BAD, 2000); Snd::error(); return; }
      blob[0] = IR::P_RAW; blob[1] = (uint8_t)n;
      memcpy(blob + 2, d, n * 2);
      ok = putKey(slot, key, blob, 2 + n * 2);
      kind = "ham";
    }
    learnEnd();
    char t[32];
    snprintf(t, sizeof(t), ok ? "%s kaydedildi (%s)" : "Kayıt hatası", KEYN[key], kind);
    strncpy(last, kind, sizeof(last) - 1);
    showToast(t, ok ? C_OK : C_BAD, 2000);
    if (ok) Snd::ok(); else Snd::error();
  }
}

// ============================================================================
//  SES KOMUTLARI
// ============================================================================
void markChanged() { app.lastChange = millis(); app.dirtySave = true; }

void limitHit(bool top) {
  bool first = millis() - app.limitFlashAt > LIMIT_FLASH_MS;
  app.limitFlashAt = millis();
  if (first) { showToast(top ? "MAKSİMUM" : "MİNİMUM", top ? C_BAD : C_MIN, 700); Snd::limit(); }
}

void volumeUp() {
  if (!bleConnected) return;
  app.lastVolAt = app.volShowAt = millis();
  if (app.muted) {                        // sessizdeyken Ses+ = sesi geri ac
    KeyQueue::push(CC_MUTE);
    app.muted = false;
    showToast("SES AÇIK", C_OK);
    markChanged();
    eyesMute(false);
    Snd::mute(false);
    return;
  }
  if (app.level >= volSteps()) { limitHit(true); eyesVol(true, true); return; }
  KeyQueue::push(CC_VOL_UP);
  app.level++;
  markChanged();
  eyesVol(true, false);
  Snd::vol(app.level / (float)volSteps());
}

void volumeDown() {
  if (!bleConnected) return;
  app.lastVolAt = app.volShowAt = millis();
  if (app.level <= 0) { limitHit(false); eyesVol(false, true); return; }
  KeyQueue::push(CC_VOL_DOWN);
  app.muted = false;
  app.level--;
  markChanged();
  eyesVol(false, false);
  Snd::vol(app.level / (float)volSteps());
}

void toggleMute() {
  if (!bleConnected) return;
  KeyQueue::push(CC_MUTE);
  app.muted = !app.muted;
  showToast(app.muted ? "SESSİZ" : "SES AÇIK", app.muted ? C_BAD : C_OK);
  markChanged();
  eyesMute(app.muted);
  Snd::mute(app.muted);
}

// ---- Medya tuslari ---------------------------------------------------------
void mediaKey(uint16_t code, const char* msg, int dir) {
  if (!bleConnected) return;
  KeyQueue::push(code);
  showToast(msg, C_ACCENT, 900);
  Snd::media(dir);
  if (dir) eyesMedia(dir); else eyesPlay();
}
void mediaNext() { mediaKey(CC_NEXT,       "Sonraki >>",        1); }
void mediaPrev() { mediaKey(CC_PREV,       "<< Önceki",        -1); }
void playPause() { mediaKey(CC_PLAY_PAUSE, "Oynat / Duraklat",  0); }
void bothShort() { if (cfg.bothAct == 0) playPause(); else toggleMute(); }

void syncVolume() {
  if (!bleConnected) return;
  int target = app.level;
  KeyQueue::clear();
  for (int i = 0; i < volSteps() + 3; i++) KeyQueue::push(CC_VOL_DOWN);
  for (int i = 0; i < target; i++)          KeyQueue::push(CC_VOL_UP);
  app.muted   = false;
  app.syncing = true;
  showToast("SENKRON", C_ACCENT, 1500);
  Snd::sel();
}

// ============================================================================
//  UYKU ZAMANLAYICISI
// ============================================================================
//  Sure dolunca ses FADE_MS boyunca yavasca kisilir, muzik duraklatilir,
//  sonra ses eski seviyesine geri alinir (sabah sessiz kalmasin) ve ekran kapanir.
namespace SleepT {
  static const uint8_t OPTS[] = {0, 15, 30, 45, 60, 90};
  uint8_t  idx = 0;
  enum Ph { OFF, RUN, FADE, PAUSEWAIT } ph = OFF;
  uint32_t endAt = 0, stepAt = 0, stepIv = 0, t = 0;
  int      orig = 0;

  bool active() { return ph != OFF; }
  bool fading() { return ph == FADE || ph == PAUSEWAIT; }
  uint32_t minsLeft() {
    if (ph != RUN) return 0;
    int32_t ms = (int32_t)(endAt - millis());
    return ms > 0 ? ((uint32_t)ms + 59999UL) / 60000UL : 0;
  }
  void cancel() { ph = OFF; idx = 0; }

  void cycle() {
    idx = (idx + 1) % COUNT_OF(OPTS);
    if (OPTS[idx] == 0) { cancel(); showToast("Zamanlayıcı kapalı", C_SUBTEXT, 1200); return; }
    ph = RUN;
    endAt = millis() + (uint32_t)OPTS[idx] * 60000UL;
  }

  void service() {
    if (ph == OFF) return;
    if (!bleConnected) { cancel(); return; }
    uint32_t now = millis();
    switch (ph) {
      case RUN:
        if ((int32_t)(now - endAt) >= 0) {
          orig = app.level;
          ph = FADE;
          stepIv = FADE_MS / max(1, app.level);
          stepAt = now;
          showToast("İyi geceler...", C_MENU, 2500);
          eyesSleep();
        }
        break;
      case FADE:
        if ((int32_t)(now - stepAt) >= 0) {
          if (app.level > 0 && !app.muted) {
            KeyQueue::push(CC_VOL_DOWN);
            app.level--;
            markChanged();
            stepAt = now + stepIv;
          } else {
            KeyQueue::push(CC_PLAY_PAUSE);            // muzigi duraklat
            ph = PAUSEWAIT; t = now;
          }
        }
        break;
      case PAUSEWAIT:
        if (now - t > 2500 && !KeyQueue::busy()) {
          for (int i = app.level; i < orig; i++) KeyQueue::push(CC_VOL_UP);
          app.level = orig;
          markChanged();
          ph = OFF; idx = 0;
          app.nightOff = true;                        // ekran kapansin
          app.saver = false;
        }
        break;
      default: break;
    }
  }
}

// ============================================================================
//  KAMERA DEKLANSORU
// ============================================================================
//  Telefon kamerasi acikken Ses+ tusu fotograf ceker. Bu modda gonderilen
//  Ses+ ses seviyesi takibini degistirmez.
namespace Cam {
  uint32_t timerEnd = 0, flashAt = 0, lastAct = 0;
  int      shots = 0;

  void enter() {
    if (!bleConnected) { showToast("Önce telefona bağlan", C_WARN, 1500); return; }
    app.camera = true; app.saver = false;
    shots = 0; timerEnd = 0; lastAct = millis();
  }
  void exit() { app.camera = false; timerEnd = 0; }
  void shoot() {
    KeyQueue::push(CC_VOL_UP);
    flashAt = millis(); shots++; lastAct = millis();
    Snd::shutter();
  }
  void toggleTimer() { timerEnd = timerEnd ? 0 : millis() + 3000; lastAct = millis(); }

  void service() {
    if (!app.camera) return;
    if (!bleConnected) { exit(); return; }
    if (timerEnd && (int32_t)(millis() - timerEnd) >= 0) { timerEnd = 0; shoot(); }
    if (millis() - lastAct > 180000UL) exit();        // 3 dk islem yoksa cik
  }
}

// ============================================================================
//  PIL
// ============================================================================
void batteryService() {
  static uint32_t last = 0, lastCrit = 0;
  static float    filt = 0;
  static bool     lastUsb = false;
  static int      warned = 100;
  if (last && millis() - last < 2000) return;
  bool first = (last == 0);
  last = millis();

  float v = HW::batteryVolts();
  filt = (filt == 0) ? v : filt * 0.8f + v * 0.2f;

  app.batteryV = filt;
  app.usbPower = HW::usbPower(filt);
  float p = (filt - 3.30f) / (4.15f - 3.30f);
  p = constrain(p, 0.0f, 1.0f);
  p = p * p * (3 - 2 * p);
  int pct = (int)(p * 100 + 0.5f);
  if (filt < 2.5f) pct = -1;

  if (pct != app.batteryPct) {
    app.batteryPct = pct;
    if (pct >= 0) bleSetBattery(app.usbPower ? 100 : pct);
  }

  // USB takildi / cikarildi
  if (!first && app.usbPower && !lastUsb) {
    showToast("Şarj oluyor", C_OK, 1500);
    Snd::charge();
    app.lastInput = millis(); app.nightOff = false;
    blTarget = blFull();
  }
  if (!first && !app.usbPower && lastUsb) { showToast("USB çıkarıldı", C_SUBTEXT, 1200); Snd::unplug(); }
  lastUsb = app.usbPower;

  // Dusuk pil uyarilari: %20, %10, %5 (sonrasinda her dakika)
  if (app.usbPower || pct < 0 || pct >= 25) { warned = 100; return; }
  int lvl = pct <= 5 ? 5 : pct <= 10 ? 10 : pct <= 20 ? 20 : 100;
  bool crit = (pct <= 5 && millis() - lastCrit > 60000UL);
  if (lvl < warned || crit) {
    warned = lvl; lastCrit = millis();
    char b[24];
    snprintf(b, sizeof(b), "Pil zayıf %%%d", pct);
    showToast(b, C_BAD, 2500);
    app.lastInput = millis(); blTarget = blFull();
    eyesTired();
    Snd::lowBat();
  }
}

// ============================================================================
//  MENU
// ============================================================================
enum Page : uint8_t { PG_MAIN, PG_SETTINGS, PG_DEVICES, PG_DEVACT, PG_CONFIRM, PG_INFO, PG_SCAN, PG_PAIR, PG_AUDIO, PG_IR, PG_IRB, PG_IRU, PG_IRK, PG_IRBR };

enum ItemId : uint8_t {
  IT_PAIR, IT_DISCONNECT, IT_HIDE, IT_VISIBLE, IT_DEVICES, IT_CLEAR_ALL, IT_STEPS, IT_BRIGHT, IT_SLEEP,
  IT_ORIENT, IT_SAVER, IT_EYE, IT_SAVER_NOW, IT_INFO, IT_EXIT, IT_DEV, IT_BACK, IT_YES, IT_NO, IT_NONE,
  IT_SCAN_DEV, IT_RESCAN, IT_PAIR_NOTARGET, IT_PAIR_CANCEL,
  IT_SLEEPT, IT_CAMERA, IT_SETTINGS, IT_DBL, IT_BOTH, IT_PREF_SET, IT_PREF_CLEAR, IT_DEV_DEL, IT_CLASSIC, IT_PAIRCONF,
  IT_WIFI, IT_WIFI_SETUP, IT_WIFI_SYNC, IT_WXINT, IT_WXALERT,
  IT_AUTOROT, IT_ROTCAL, IT_MIC, IT_SND, IT_SNDEYE,
  IT_IR, IT_IR_ALL, IT_IR_STOP, IT_IR_BRAND, IT_IR_KEY,
  IT_IRU_OPEN, IT_IRU_SEND, IT_IRU_LEARN, IT_IRU_SCAN, IT_IRU_NAME, IT_IRU_DEL, IT_IRK, IT_KARAOKE, IT_IR_RANDOM, IT_IR_BRANDS, IT_SSTYLE, IT_LOGO, IT_OBD
};

struct MenuItem {
  char     label[32];
  char     value[20];
  uint16_t color;
  uint8_t  id;
  int8_t   arg;
  int8_t   bars;       // sinyal gucu (0 = yok)
  int8_t   optIdx;     // ayarlar kaydiricisi: secili secenek
  int8_t   optCnt;     //                      secenek sayisi (0 = yok)
};

struct MenuState {
  bool     open = false;
  Page     page = PG_MAIN;
  int      sel = 0, scroll = 0;
  int      mainSel = 0, settingsSel = 0;
  int      delIdx = -1;
  float    hlY = -1000;
  float    carPos = 0;          // ayarlar kaydirma animasyonu
  int      irBrand = 0;         // TV kumanda: secili marka
  int      irSlot = 0;          // ogrenilen kumanda yuvasi
} menu;

constexpr int MENU_MAX_ITEMS = 34;
MenuItem menuItems[MENU_MAX_ITEMS];
int      menuCount = 0;
int      scanOrder[Scan::MAX];

static void addItem(const char* label, const char* value, uint16_t color, uint8_t id, int8_t arg = 0, int8_t bars = 0) {
  if (menuCount >= MENU_MAX_ITEMS) return;
  MenuItem& it = menuItems[menuCount++];
  strncpy(it.label, label, sizeof(it.label) - 1); it.label[sizeof(it.label) - 1] = 0;
  strncpy(it.value, value, sizeof(it.value) - 1); it.value[sizeof(it.value) - 1] = 0;
  it.color = color; it.id = id; it.arg = arg; it.bars = bars;
  it.optIdx = 0; it.optCnt = 0;
}
static void setOpt(int idx, int cnt) {
  if (menuCount > 0) { menuItems[menuCount - 1].optIdx = idx; menuItems[menuCount - 1].optCnt = cnt; }
}

void buildMenu() {
  menuCount = 0;
  char v[20];
  switch (menu.page) {
    case PG_MAIN:
      addItem("Yeni cihaz eşle", "", C_ACCENT, IT_PAIR);
      if (bleConnected)    addItem("Bağlantıyı kes", "", C_WARN, IT_DISCONNECT);
      else if (bleHidden)  addItem("Görünür ol", "gizli", C_OK, IT_VISIBLE);
      else                 addItem("Gizli moda geç", "", C_TEXT, IT_HIDE);
      snprintf(v, sizeof(v), "%d", Bonds::count);
      addItem("Kayıtlı cihazlar", v, C_TEXT, IT_DEVICES);
      if (SleepT::active()) {
        if (SleepT::fading()) strcpy(v, "kısılıyor");
        else snprintf(v, sizeof(v), "%lu dk", (unsigned long)SleepT::minsLeft());
      } else strcpy(v, "Kapalı");
      addItem("Uyku zamanlayıcı", v, SleepT::active() ? C_MENU : C_TEXT, IT_SLEEPT);
      addItem("Kamera deklanşörü", "", C_TEXT, IT_CAMERA);
      addItem("TV kumanda (IR)", ">", C_TEXT, IT_IR);
      addItem("Karaoke modu", "BT hoparlör", C_MENU, IT_KARAOKE);
      addItem("OBD2 sürüş asistanı", Obd::on() ? "Açık" : "Kapalı", C_MENU, IT_OBD);
      addItem("Koruyucuyu göster", "", C_TEXT, IT_SAVER_NOW);
      addItem("Ayarlar", ">", C_TEXT, IT_SETTINGS);
      addItem("Cihaz bilgisi", "", C_TEXT, IT_INFO);
      addItem("Çıkış", "", C_SUBTEXT, IT_EXIT);
      break;

    case PG_SETTINGS:
      snprintf(v, sizeof(v), "%d", volSteps());
      addItem("Ses adımı", v, C_TEXT, IT_STEPS);            setOpt(cfg.stepIdx, COUNT_OF(STEP_OPTS));
      snprintf(v, sizeof(v), "%%%d", BRIGHT_PCT[cfg.brightIdx]);
      addItem("Parlaklık", v, C_TEXT, IT_BRIGHT);           setOpt(cfg.brightIdx, COUNT_OF(BRIGHT_OPTS));
      if (SLEEP_MIN[cfg.sleepIdx] == 0) strcpy(v, "Kapalı");
      else snprintf(v, sizeof(v), "%u dk", SLEEP_MIN[cfg.sleepIdx]);
      addItem("Otomatik uyku", v, C_TEXT, IT_SLEEP);        setOpt(cfg.sleepIdx, COUNT_OF(SLEEP_MIN));
      addItem("Ekran yönü", cfg.autoRot ? "Otomatik" : ORIENT_TXT[cfg.orient], C_TEXT, IT_ORIENT);  setOpt(cfg.orient, COUNT_OF(ORIENT_ROT));
      addItem("Otomatik döndürme", !Imu::ok ? "sensör yok" : cfg.autoRot ? "Açık" : "Kapalı", C_TEXT, IT_AUTOROT);  setOpt(cfg.autoRot, 2);
      addItem("Döndürme ayarı", AutoRot::valid() ? "ayarlı" : "gerekli", C_TEXT, IT_ROTCAL);
      addItem("Ekran koruyucu", SAVER_TXT[cfg.saverIdx], C_TEXT, IT_SAVER);      setOpt(cfg.saverIdx, COUNT_OF(SAVER_SEC));
      addItem("Açılış logosu", cfg.logoStyle ? "Sade" : "Klasik", C_TEXT, IT_LOGO); setOpt(cfg.logoStyle, 2);
      addItem("Göz rengi", EYE_TXT[cfg.eyeIdx], C_TEXT, IT_EYE);                 setOpt(cfg.eyeIdx, COUNT_OF(EYE_TXT));
      addItem("Mikrofon tepkisi", cfg.micOn ? "Açık" : "Kapalı", C_TEXT, IT_MIC);   setOpt(cfg.micOn, 2);
      addItem("Ses efektleri", SND_TXT[cfg.sndLvl], C_TEXT, IT_SND);             setOpt(cfg.sndLvl, 3);
      addItem("Göz sesleri", cfg.sndEyes ? "Açık" : "Kapalı", C_TEXT, IT_SNDEYE);  setOpt(cfg.sndEyes, 2);
      addItem("Çift tık medya", cfg.dblClick ? "Açık" : "Kapalı", C_TEXT, IT_DBL);  setOpt(cfg.dblClick, 2);
      addItem("İki tuşa kısa", cfg.bothAct == 0 ? "Oynat/Dur" : "Sessiz", C_TEXT, IT_BOTH);  setOpt(cfg.bothAct, 2);
      addItem("Hava uyarıları", cfg.wxAlerts ? "Açık" : "Kapalı", C_TEXT, IT_WXALERT);  setOpt(cfg.wxAlerts, 2);
      addItem("Eşleşme onayı", cfg.pairConfirm ? "Açık" : "Kapalı", C_TEXT, IT_PAIRCONF);  setOpt(cfg.pairConfirm, 2);
      addItem("İsimli arama", cfg.classicScan ? "Açık" : "Kapalı", C_TEXT, IT_CLASSIC);  setOpt(cfg.classicScan, 2);
      addItem("Tüm eşleşmeleri sil", "", C_BAD, IT_CLEAR_ALL);
      addItem("< Geri", "", C_SUBTEXT, IT_BACK);
      break;

    case PG_DEVICES:
      for (int i = 0; i < Bonds::count && menuCount < MENU_MAX_ITEMS - 2; i++) {
        char l[32], a[20];
        Bonds::label(i, l, sizeof(l));
        fmtAddr(Bonds::list[i].bd_addr, a, sizeof(a));
        bool on = Bonds::isConnected(i), pr = Pref::is(Bonds::list[i].bd_addr);
        const char* val = (on && pr) ? "bağlı *" : on ? "bağlı" : pr ? "tercih" : (portrait ? a : "");
        addItem(l, val, C_TEXT, IT_DEV, i);
      }
      if (Bonds::count == 0) addItem("(kayıtlı cihaz yok)", "", C_SUBTEXT, IT_NONE);
      if (Pref::set) addItem("Tercihi kaldır", "", C_WARN, IT_PREF_CLEAR);
      if (LostLog::name[0]) {
        char l[32], cv[12];
        snprintf(l, sizeof(l), "Silinen: %s", LostLog::name);
        snprintf(cv, sizeof(cv), "kod %02X", LostLog::code);
        addItem(l, cv, C_WARN, IT_NONE);
      }
      addItem("< Geri", "", C_SUBTEXT, IT_BACK);
      break;

    case PG_DEVACT:
      if (menu.delIdx >= 0 && menu.delIdx < Bonds::count && Pref::is(Bonds::list[menu.delIdx].bd_addr))
        addItem("Otomatik bağlan", "", C_TEXT, IT_PREF_CLEAR);
      else
        addItem("Bu cihaza bağlan", "", C_ACCENT, IT_PREF_SET);
      addItem("Sil", "", C_BAD, IT_DEV_DEL);
      addItem("< Geri", "", C_SUBTEXT, IT_BACK);
      break;

    case PG_CONFIRM:
      addItem("Evet, sil", "", C_BAD, IT_YES);
      addItem("İptal", "", C_TEXT, IT_NO);
      break;

    case PG_INFO:
      addItem("< Geri", "", C_SUBTEXT, IT_BACK);
      break;

    case PG_SCAN: {
      // Sinyal gucune gore sirala (isimliler once)
      int n = Scan::count;
      for (int i = 0; i < n; i++) scanOrder[i] = i;
      for (int i = 1; i < n; i++) {
        int k = scanOrder[i], j = i - 1;
        auto better = [](int a, int b) {
          if (Scan::list[a].named != Scan::list[b].named) return Scan::list[a].named;
          return Scan::list[a].rssi > Scan::list[b].rssi;
        };
        while (j >= 0 && better(k, scanOrder[j])) { scanOrder[j + 1] = scanOrder[j]; j--; }
        scanOrder[j + 1] = k;
      }
      for (int r = 0; r < n && menuCount < MENU_MAX_ITEMS - 3; r++) {
        Scan::Dev& d = Scan::list[scanOrder[r]];
        char l[32];
        if (d.named) strcpy(l, d.name); else fmtAddr(d.bda, l, sizeof(l));
        addItem(l, Scan::typeText(d.cod), d.named ? C_TEXT : C_SUBTEXT, IT_SCAN_DEV,
                scanOrder[r], Scan::bars(d.rssi));
      }
      if (Scan::running) addItem("Aranıyor...", "", C_SUBTEXT, IT_NONE);
      else               addItem("Yeniden ara", "", C_ACCENT, IT_RESCAN);
      addItem("Listede yok, yine de eşle", "", C_TEXT, IT_PAIR_NOTARGET);
      addItem("< Geri", "", C_SUBTEXT, IT_BACK);
      break;
    }

    case PG_PAIR:
      addItem("İptal", "", C_TEXT, IT_PAIR_CANCEL);
      break;

    case PG_AUDIO:
      addItem("Tamam", "", C_TEXT, IT_BACK);
      break;

    case PG_IR:
      if (IR::sweeping) addItem("Durdur", "", C_WARN, IT_IR_STOP);
      else { char n[12]; snprintf(n, sizeof(n), "%d kod", IR_NPWR); addItem("Tüm TV'leri aç/kapa", n, C_ACCENT, IT_IR_ALL); }
      addItem("Rastgele sıra", IR::randomOrder ? "Açık" : "Kapalı", C_TEXT, IT_IR_RANDOM);
      for (int sl = 0; sl < IRL::SLOTS; sl++)
        if (IRL::used(sl)) addItem(IRL::name(sl), ">", C_OK, IT_IRU_OPEN, sl);
      if (IRL::firstEmpty() >= 0) addItem("+ Yeni kumanda", "tara/öğret", C_ACCENT, IT_IRU_OPEN, IRL::firstEmpty());
      { char n[8]; snprintf(n, sizeof(n), "%d", IR::NBRAND); addItem("Markalar", n, C_TEXT, IT_IR_BRANDS); }
      addItem("< Geri", "", C_SUBTEXT, IT_BACK);
      break;

    case PG_IRBR:
      for (int b = 0; b < IR::NBRAND && menuCount < MENU_MAX_ITEMS - 1; b++)
        addItem(IR::IRBRANDS[b].name, "", C_TEXT, IT_IR_BRAND, b);
      addItem("< Geri", "", C_SUBTEXT, IT_BACK);
      break;

    case PG_IRU: {
      int sl = menu.irSlot;
      for (int k = 0; k < IRL::KEYS; k++)
        if (IRL::keyMask[sl] & (1 << k)) addItem(IRL::KEYN[k], "", k == 0 ? C_ACCENT : C_TEXT, IT_IRU_SEND, k);
      addItem("Kod tara (alıcısız)", "", C_TEXT, IT_IRU_SCAN);
      addItem("Tuş öğret (alıcı G26)", ">", C_TEXT, IT_IRU_LEARN);
      addItem("İsim", IRL::name(sl), C_TEXT, IT_IRU_NAME);
      if (IRL::used(sl)) addItem("Bu kumandayı sil", "", C_BAD, IT_IRU_DEL);
      addItem("< Geri", "", C_SUBTEXT, IT_BACK);
      break;
    }

    case PG_IRK:
      for (int k = 0; k < IRL::KEYS; k++)
        addItem(IRL::KEYN[k], (IRL::keyMask[menu.irSlot] & (1 << k)) ? "kayıtlı" : "boş", C_TEXT, IT_IRK, k);
      addItem("< Geri", "", C_SUBTEXT, IT_BACK);
      break;

    case PG_IRB:
      for (int k = 0; k < 4; k++)
        if (IR::has(menu.irBrand, k)) addItem(IR::KEY_TXT[k], "", k == 0 ? C_ACCENT : C_TEXT, IT_IR_KEY, k);
      addItem("< Geri", "", C_SUBTEXT, IT_BACK);
      break;

  }
  if (menu.sel >= menuCount) menu.sel = menuCount - 1;
  if (menu.sel < 0) menu.sel = 0;
  if (menuItems[menu.sel].id == IT_NONE && menuCount > 1) {
    for (int k = 0; k < menuCount; k++) {
      int s = (menu.sel + k) % menuCount;
      if (menuItems[s].id != IT_NONE) { menu.sel = s; break; }
    }
  }
}

void leavePage() {
  if (menu.page == PG_SCAN) Scan::stop();
  if (menu.page == PG_PAIR) Pair::stop();
  if (menu.page == PG_IR)   IR::sweepStop();
}

void menuGo(Page p, int sel = 0) {
  if (menu.page == PG_MAIN)     menu.mainSel = menu.sel;
  if (menu.page == PG_SETTINGS) menu.settingsSel = menu.sel;
  if (p != menu.page) leavePage();
  menu.page = p; menu.sel = sel; menu.scroll = 0; menu.hlY = -1000;
  buildMenu();
  menu.carPos = menu.sel;
}

void menuOpen() {
  Bonds::refresh();
  app.saver = false;
  menu.open = true;
  menu.page = PG_MAIN;
  menu.sel = 0; menu.scroll = 0; menu.hlY = -1000;
  buildMenu();
  Snd::open();
}

void menuClose() { leavePage(); menu.open = false; menu.page = PG_MAIN; }

void menuSelectItem(uint8_t id, int arg) {
  for (int i = 0; i < menuCount; i++)
    if (menuItems[i].id == id && (id == IT_IRU_LEARN || id == IT_IR_BRANDS || menuItems[i].arg == arg)) { menu.sel = i; menu.carPos = i; return; }
}

void menuBack() {
  Snd::back();
  switch (menu.page) {
    case PG_IRB:     menuGo(PG_IRBR, 0); menuSelectItem(IT_IR_BRAND, menu.irBrand); return;
    case PG_IRBR:    menuGo(PG_IR, 0); menuSelectItem(IT_IR_BRANDS, 0); return;
    case PG_IRU:     menuGo(PG_IR, 0); menuSelectItem(IT_IRU_OPEN, menu.irSlot); return;
    case PG_IRK:     menuGo(PG_IRU, 0); menuSelectItem(IT_IRU_LEARN, 0); return;
    case PG_MAIN:    menuClose(); return;
    case PG_AUDIO:   menuGo(PG_SCAN); if (!Scan::running && Scan::count == 0) Scan::start(); return;
    case PG_DEVACT:  menuGo(PG_DEVICES, max(0, menu.delIdx)); return;
    case PG_CONFIRM:
      if (menu.delIdx >= 0) menuGo(PG_DEVACT, 0);
      else                  menuGo(PG_SETTINGS, menu.settingsSel);
      return;
    default:
      leavePage();
      menu.page = PG_MAIN;
      menu.sel = menu.mainSel; menu.scroll = 0; menu.hlY = -1000;
      buildMenu();
      return;
  }
}

void menuPrev() {
  if (menuCount == 0) return;
  int s = menu.sel;
  for (int k = 0; k < menuCount; k++) {
    s = (s - 1 + menuCount) % menuCount;
    if (menuItems[s].id != IT_NONE) break;
  }
  menu.sel = s;
  Snd::click();
}

void menuNext() {
  if (menuCount == 0) return;
  int s = menu.sel;
  for (int k = 0; k < menuCount; k++) {
    s = (s + 1) % menuCount;
    if (menuItems[s].id != IT_NONE) break;
  }
  menu.sel = s;
  Snd::click();
}

void menuSelect() {
  if (menuCount == 0) return;
  MenuItem& it = menuItems[menu.sel];
  if (it.id != IT_BACK && it.id != IT_NO && it.id != IT_PAIR_CANCEL && it.id != IT_IR_KEY) Snd::sel();
  switch (it.id) {
    case IT_IR:
      menuGo(PG_IR, 0);
      return;
    case IT_IR_ALL:
      IR::sweepStart();
      break;
    case IT_IR_STOP:
      IR::sweepStop();
      showToast("Durduruldu", C_SUBTEXT, 900);
      break;
    case IT_OBD:
      // OBD2 artik normal modun parcasi: ayarlar penceresi (BLE adaptor, gostergeler, uyarilar)
      Obd::openSettings();
      break;
    case IT_KARAOKE:
      // Karaoke ayri bir acilista calisir: ses kumandasi (BLE), koruyucu, IR,
      // dondurme, buzzer kapali; tum RAM mikrofon + BT hoparlor icin kullanilir
      showToast("Karaoke için yeniden başlatılıyor", C_MENU, 3000);
      app.restartReq = BOOT_KARAOKE; app.restartAt = millis() + 1200;
      break;
    case IT_IR_RANDOM:
      IR::randomOrder = !IR::randomOrder;
      break;
    case IT_IR_BRANDS:
      menuGo(PG_IRBR, 0);
      return;
    case IT_IRU_OPEN:
      menu.irSlot = it.arg;
      menuGo(PG_IRU, 0);
      return;
    case IT_IRU_SEND:
      IRL::sendKey(menu.irSlot, it.arg);
      { char t[32]; snprintf(t, sizeof(t), "%s: %s", IRL::name(menu.irSlot), IRL::KEYN[it.arg]); showToast(t, C_ACCENT, 900); }
      return;
    case IT_IRU_SCAN:
      IRL::scanStart(menu.irSlot);
      return;
    case IT_IRU_LEARN:
      menuGo(PG_IRK, 0);
      return;
    case IT_IRK:
      IRL::learnStart(menu.irSlot, it.arg);
      return;
    case IT_IRU_NAME:
      IRL::cycleName(menu.irSlot);
      break;
    case IT_IRU_DEL:
      IRL::removeSlot(menu.irSlot);
      showToast("Kumanda silindi", C_BAD, 1200);
      menuGo(PG_IR, 0);
      return;
    case IT_IR_BRAND:
      menu.irBrand = it.arg;
      menuGo(PG_IRB, 0);
      return;
    case IT_IR_KEY: {
      char t[32];
      snprintf(t, sizeof(t), "%s: %s", IR::IRBRANDS[menu.irBrand].name, it.arg == 0 ? "Güç" : IR::KEY_TXT[it.arg]);
      IR::sendKey(menu.irBrand, it.arg);
      showToast(t, C_ACCENT, 900);
      return;
    }
    case IT_AUTOROT:
      if (!Imu::ok) { showToast("Hareket sensörü yok", C_BAD, 1500); break; }
      if (!cfg.autoRot && !AutoRot::valid()) { AutoRot::calStart(); return; }   // ilk acilista ayar
      cfg.autoRot = !cfg.autoRot;
      saveConfig();
      break;
    case IT_ROTCAL:
      AutoRot::calStart();
      return;
    case IT_MIC:
      cfg.micOn = !cfg.micOn;
      saveConfig();
      break;
    case IT_SND:
      cfg.sndLvl = (cfg.sndLvl + 1) % 3;
      saveConfig();
      Snd::ok();                                   // ornek ses
      break;
    case IT_SSTYLE:
      cfg.saverStyle = cfg.saverStyle == 3 ? 0 : 3;
      saveConfig();
      break;
    case IT_LOGO:
      cfg.logoStyle = !cfg.logoStyle;
      saveConfig();
      showToast("Sonraki açılışta görünür", C_SUBTEXT, 1400);
      break;
    case IT_SNDEYE:
      cfg.sndEyes = !cfg.sndEyes;
      saveConfig();
      if (cfg.sndEyes) Snd::chatter();
      break;
    case IT_PAIR:
      if (!app.classicOk) {                  // hafif mod: isimli arama yok
        menuGo(PG_PAIR);
        Pair::start(nullptr);
        showToast("İsimli arama kapalı (Ayarlar)", C_SUBTEXT, 2200);
        return;
      }
      menuGo(PG_SCAN);
      Scan::start();
      return;
    case IT_DISCONNECT:
    case IT_HIDE:
      // Reklami kapat: kayitli telefon otomatik geri baglanamaz
      bleHidden = true;
      bleAdvertise(false);
      bleDisconnectAll();
      showToast("Gizli mod", C_WARN, 1500);
      break;
    case IT_VISIBLE:
      bleHidden = false;
      bleAdvertise(true);
      showToast("Görünür", C_OK, 1200);
      break;
    case IT_DEVICES:
      Bonds::refresh();
      menuGo(PG_DEVICES, Bonds::count == 0 ? 1 : 0);
      return;
    case IT_SLEEPT:
      if (!bleConnected && !SleepT::active()) { showToast("Önce telefona bağlan", C_WARN, 1500); break; }
      SleepT::cycle();
      break;
    case IT_CAMERA:
      Cam::enter();
      if (app.camera) menuClose();
      return;
    case IT_SETTINGS:
      menuGo(PG_SETTINGS, menu.settingsSel);
      return;
    case IT_CLEAR_ALL:
      Bonds::refresh();
      menu.delIdx = -1;
      menuGo(PG_CONFIRM, 1);
      return;
    case IT_STEPS: {
      int old = volSteps();
      cfg.stepIdx = (cfg.stepIdx + 1) % COUNT_OF(STEP_OPTS);
      app.level = (app.level * volSteps() + old / 2) / old;
      app.level = constrain(app.level, 0, volSteps());
      app.shownLevel = app.level;
      saveConfig(); markChanged();
      break;
    }
    case IT_BRIGHT:
      cfg.brightIdx = (cfg.brightIdx + 1) % COUNT_OF(BRIGHT_OPTS);
      blTarget = blFull();
      saveConfig();
      break;
    case IT_SLEEP:
      cfg.sleepIdx = (cfg.sleepIdx + 1) % COUNT_OF(SLEEP_MIN);
      saveConfig();
      break;
    case IT_SAVER:
      cfg.saverIdx = (cfg.saverIdx + 1) % COUNT_OF(SAVER_SEC);
      saveConfig();
      break;
    case IT_EYE:
      cfg.eyeIdx = (cfg.eyeIdx + 1) % COUNT_OF(EYE_TXT);
      saveConfig();
      break;
    case IT_PAIRCONF:
      cfg.pairConfirm = !cfg.pairConfirm;
      saveConfig();
      applySecurity();
      showToast(cfg.pairConfirm ? "Yeni eşleşmede kod sorulur" : "Onaysız eşleşme", C_ACCENT, 1800);
      break;
    case IT_CLASSIC:
      // Bluetooth modu yalnizca acilista secilebilir: kaydet ve yeniden basla
      cfg.classicScan = !cfg.classicScan;
      saveConfig();
      if (app.dirtySave) saveState();
      showToast("Yeniden başlatılıyor...", C_ACCENT, 3000);
      app.restartAt = millis() + 1200; app.restartReq = BOOT_NORMAL;
      break;
    case IT_DBL:
      cfg.dblClick = !cfg.dblClick;
      saveConfig();
      break;
    case IT_BOTH:
      cfg.bothAct = cfg.bothAct ? 0 : 1;
      saveConfig();
      break;
    case IT_SAVER_NOW:
      menuClose();
      startSaver();
      return;
    case IT_ORIENT:
      if (cfg.autoRot) {                           // elle secim: otomatik dondurme kapanir
        cfg.autoRot = 0; saveConfig();
        showToast("Otomatik döndürme kapandı", C_SUBTEXT, 1500);
        break;
      }
      cfg.orient = (cfg.orient + 1) % COUNT_OF(ORIENT_ROT);
      applyOrientation();
      saveConfig();
      menu.hlY = -1000;
      break;
    case IT_INFO:
      Bonds::refresh();
      menuGo(PG_INFO);
      return;
    case IT_EXIT:
      menuClose();
      return;
    case IT_DEV:
      menu.delIdx = it.arg;
      menuGo(PG_DEVACT, 0);
      return;
    case IT_PREF_SET: {
      if (menu.delIdx < 0 || menu.delIdx >= Bonds::count) break;
      const uint8_t* a = Bonds::list[menu.delIdx].bd_addr;
      Pref::save(a);
      // Tercih edilmeyen bagli cihazlari birak, tercih edilen baglanabilsin
      for (int i = 0; i < MAX_PEERS; i++)
        if (peers[i].used && memcmp(peers[i].bda, a, 6)) bleDisconnectPeer(i);
      bleHidden = false;
      if (!bleConnected) bleAdvertise(true);
      char nm[32], t[40];
      Bonds::label(menu.delIdx, nm, sizeof(nm));
      snprintf(t, sizeof(t), "Bekleniyor: %s", nm);
      showToast(t, C_ACCENT, 2500);
      menuClose();
      return;
    }
    case IT_PREF_CLEAR:
      Pref::clear();
      showToast("Otomatik bağlantı", C_OK, 1500);
      if (menu.page == PG_DEVACT) { menuGo(PG_DEVICES, max(0, menu.delIdx)); return; }
      break;
    case IT_DEV_DEL:
      menuGo(PG_CONFIRM, 1);
      return;
    case IT_WXALERT:
      cfg.wxAlerts = !cfg.wxAlerts;
      saveConfig();
      break;
    case IT_YES:
      if (menu.delIdx < 0) {
        Bonds::removeAll(); showToast("Tümü silindi", C_BAD, 1500);
        menu.page = PG_MAIN; menu.sel = 0; buildMenu();
      } else {
        Bonds::remove(menu.delIdx); showToast("Cihaz silindi", C_BAD, 1500);
        menu.delIdx = -1;
        menuGo(PG_DEVICES, 0);
      }
      return;
    case IT_NO:
      menuBack();
      return;
    case IT_SCAN_DEV: {
      Scan::Dev& d = Scan::list[it.arg];
      if (Scan::isAudio(d.cod)) { menuGo(PG_AUDIO); return; }
      char name[32];
      if (d.named) strcpy(name, d.name); else fmtAddr(d.bda, name, sizeof(name));
      menuGo(PG_PAIR);
      Pair::start(name);
      return;
    }
    case IT_RESCAN:
      Scan::start();
      break;
    case IT_PAIR_NOTARGET:
      menuGo(PG_PAIR);
      Pair::start(nullptr);
      return;
    case IT_PAIR_CANCEL:
      Pair::stop();
      menuBack();
      return;
    case IT_BACK:
      menuBack();
      return;
    default:
      break;
  }
  buildMenu();
}

// ============================================================================
//  TUS YONETIMI
// ============================================================================
struct Btn {
  uint8_t  pin = 0;
  bool     down = false;
  bool     raw  = false;
  uint32_t changedAt = 0;
  uint32_t downAt    = 0;
  void begin(uint8_t p) { pin = p; pinMode(pin, p >= 34 ? INPUT : INPUT_PULLUP); }   // G34-39: yalniz giris, harici pull-up var
  bool update() {
    bool r = digitalRead(pin) == LOW;
    uint32_t now = millis();
    if (r != raw) { raw = r; changedAt = now; }
    if (r != down && now - changedAt > 12) {
      down = r; if (down) downAt = now;
      return true;
    }
    return false;
  }
};

Btn btnUp, btnDn;

namespace Input {
  //  Tek tus akisi (cift tik acikken):
  //   bas -> PENDING (iki tus mu?) -> HOLDWAIT (uzun basis mi?) -> birak ->
  //   DBLWAIT (ikinci tik bekle): 2. tik = medya, zaman asimi = ses adimi.
  //   Son ses adimindan STREAK_MS icinde gelen tikler bekletilmeden ses olur,
  //   boylece hizli art arda basislar sarki atlatmaz.
  enum Mode { NONE, PENDING, HOLDWAIT, DBLWAIT, SINGLE, COMBO, MENU_BTN, CAM_BTN, OBD_BTN, DASH_BTN, DASH_DBL, SWALLOW } mode = NONE;
  Btn*     active = nullptr;
  uint32_t pendingAt = 0, nextRepeat = 0, repeatIv = REPEAT_START_MS, relAt = 0;
  bool     comboFired = false, longFired = false;

  void fireSingle(Btn* b) { if (b == &btnUp) volumeUp(); else volumeDown(); }
  void fireDouble(Btn* b) { if (b == &btnUp) mediaNext(); else mediaPrev(); }
  bool inStreak() { return millis() - app.lastVolAt < STREAK_MS; }

  void startRepeat(uint32_t now) {
    mode = SINGLE;
    nextRepeat = now + repeatIv;
  }

  void service() {
    btnUp.update();
    btnDn.update();
    uint32_t now = millis();
    bool u = btnUp.down, d = btnDn.down;

    // OBD2 ayarlari penceresi acikken tuslar ona gider
    if (Obd::menu) { Obd::settingsButtons(); mode = SWALLOW; return; }

    // M5 GUC tusu (kisa basis): menu ac / geri / iptal
    if (HW::powerPressed()) {
      if (Obd::alertBlocking()) { Obd::ackAlert(); userActivity(); Snd::back(); return; }
      if (Info::active()) { Info::ack(); userActivity(); Snd::back(); return; }
      bool wasOff = userActivity();
      app.nightOff = false;
      if (!wasOff) {
        if (Pair::modal != Pair::M_NONE) Pair::reject();
        else if (AutoRot::calStep) AutoRot::calCancel();
        else if (IRL::mode) IRL::cancel();
        else if (app.camera) { Cam::exit(); Snd::back(); }
        else if (menu.open) menuBack();
        else menuOpen();
      }
    }

    switch (mode) {
      case NONE:
        if (u || d) {
          bool wasOff = userActivity();
          app.nightOff = false;
          active = u ? &btnUp : &btnDn;
          // Eslesme onay penceresi her seyin onunde
          if (Pair::modal != Pair::M_NONE) {
            if (Pair::modal == Pair::M_CONFIRM && active == &btnUp) Pair::accept();
            else if (active == &btnDn) Pair::reject();
            mode = SWALLOW; break;
          }
          if (wasOff) { mode = SWALLOW; break; }
          Music::cardAt = 0;                                                                 // sarki kartini kapat
          if (Obd::alertBlocking()) { Obd::ackAlert(); Snd::back(); mode = SWALLOW; break; }   // surus uyarisi: tamam
          if (Info::active()) { Info::ack(); Snd::back(); mode = SWALLOW; break; }             // hava uyarisi: tamam
          if (app.chargeAnimUntil > now) { app.chargeAnimUntil = 0; mode = SWALLOW; break; }
          if (IRL::mode) {                          // IR tarama / ogrenme ekrani
            IRL::onKey(active == &btnUp);
            mode = SWALLOW; break;
          }
          if (AutoRot::calStep) {                   // dondurme ayari: A kaydet, B iptal
            if (active == &btnUp) AutoRot::calCapture(); else AutoRot::calCancel();
            mode = SWALLOW; break;
          }
          if (menu.open) {
            mode = MENU_BTN; longFired = false;
            if (active == &btnDn) {
              if (menu.page == PG_SETTINGS) nextRepeat = now + MENU_LONG_MS;    // kisa/uzun ayrimi
              else { menuNext(); nextRepeat = now + REPEAT_DELAY_MS; }
            }
            break;
          }
          if (app.camera) {
            mode = CAM_BTN; longFired = false;
            if (active == &btnDn) Cam::toggleTimer();
            break;
          }
          if (app.saver && Obd::dashActive()) {                         // gozlerden OBD2 ana ekrana don
            app.saver = false; Snd::click(); mode = SWALLOW; break;
          }
          if (app.saver && cfg.saverStyle == 3 && Obd::on()) {   // OBD2 koruyucu: M5 = arayuz
            mode = OBD_BTN; longFired = false; break;
          }
          if (!app.saver && Obd::dashActive()) {               // OBD2 ana ekran: kisa = sayfa, basili = ses
            mode = DASH_BTN; pendingAt = now; break;
          }
          if (!bleConnected) { menuOpen(); mode = SWALLOW; break; }
          mode = PENDING; pendingAt = now;
        }
        break;

      case PENDING:
        if (u && d) { mode = COMBO; comboFired = false; app.comboProgress = 0; break; }
        if (!cfg.dblClick || inStreak()) {
          // Klasik davranis: basinca hemen ses
          if (!u && !d) { fireSingle(active); mode = NONE; break; }
          if (now - pendingAt >= COMBO_WINDOW_MS) {
            fireSingle(active);
            repeatIv = REPEAT_START_MS;
            mode = SINGLE;
            nextRepeat = active->downAt + REPEAT_DELAY_MS;
          }
          break;
        }
        if (!active->down) { mode = DBLWAIT; relAt = now; break; }
        if (now - pendingAt >= COMBO_WINDOW_MS) mode = HOLDWAIT;
        break;

      case HOLDWAIT:
        if ((active == &btnUp ? d : u)) { mode = SWALLOW; break; }
        if (!active->down) { mode = DBLWAIT; relAt = now; break; }
        if (now - active->downAt >= REPEAT_DELAY_MS) {          // uzun basis: ses + tekrar
          fireSingle(active);
          repeatIv = REPEAT_START_MS;
          startRepeat(now);
        }
        break;

      case DBLWAIT:
        if (active->down) {                                     // ikinci tik
          fireDouble(active);
          mode = SWALLOW; break;
        }
        if ((active == &btnUp ? d : u)) {                       // diger tus: once bekleyen sesi uygula
          fireSingle(active);
          active = (active == &btnUp) ? &btnDn : &btnUp;
          mode = PENDING; pendingAt = now;
          break;
        }
        if (now - relAt >= DBL_CLICK_MS) { fireSingle(active); mode = NONE; }
        break;

      case SINGLE:
        userActivity();
        if (u && d) { mode = SWALLOW; break; }
        if (!active->down) { mode = NONE; break; }
        if ((int32_t)(now - nextRepeat) >= 0) {
          fireSingle(active);
          nextRepeat = now + repeatIv;
          repeatIv = max((uint32_t)REPEAT_MIN_MS, repeatIv - 12);
        }
        break;

      case COMBO: {
        userActivity();
        uint32_t held = now - max(btnUp.downAt, btnDn.downAt);
        if (u && d && !comboFired) {
          app.comboProgress = min(1.0f, held / (float)MENU_HOLD_MS);
          if (held >= MENU_HOLD_MS) { comboFired = true; app.comboProgress = 0; menuOpen(); }
        }
        if (!u || !d) {
          if (!comboFired) {
            if (held >= SYNC_HOLD_MS) syncVolume();
            else                      bothShort();
          }
          app.comboProgress = 0;
          mode = SWALLOW;
        }
        break;
      }

      case MENU_BTN:
        userActivity();
        if (!menu.open) { mode = SWALLOW; break; }
        if (active == &btnUp) {
          if (btnUp.down && !longFired && now - btnUp.downAt >= MENU_LONG_MS) {
            longFired = true; menuBack();
          }
          if (!btnUp.down) {
            if (!longFired && menu.open) menuSelect();
            mode = btnDn.down ? SWALLOW : NONE;
          }
        } else if (menu.page == PG_SETTINGS) {
          // Kaydirmali ayarlar: kisa = sonraki, uzun = onceki (basili tutunca devam)
          if (!btnDn.down) {
            if (!longFired) menuNext();
            mode = btnUp.down ? SWALLOW : NONE; break;
          }
          if ((int32_t)(now - nextRepeat) >= 0) { menuPrev(); longFired = true; nextRepeat = now + 380; }
        } else {
          if (!btnDn.down) { mode = btnUp.down ? SWALLOW : NONE; break; }
          if ((int32_t)(now - nextRepeat) >= 0) { menuNext(); nextRepeat = now + 150; }
        }
        break;

      case CAM_BTN:
        userActivity();
        if (u && d) { Cam::exit(); mode = SWALLOW; break; }
        if (active == &btnUp) {
          if (btnUp.down && !longFired && now - btnUp.downAt >= MENU_LONG_MS) {
            longFired = true; Cam::exit(); showToast("Kamera kapandı", C_SUBTEXT, 900);
          }
          if (!btnUp.down) {
            if (!longFired) Cam::shoot();
            mode = NONE;
          }
        } else if (!btnDn.down) mode = NONE;
        break;

      case OBD_BTN:                                  // OBD2 koruyucu: M5 kisa/uzun, B = cikis
        userActivity();
        if (active == &btnUp) {
          if (btnUp.down && !longFired && now - btnUp.downAt >= MENU_LONG_MS) { longFired = true; Obd::saverKey(true); }
          if (!btnUp.down) { if (!longFired) Obd::saverKey(false); mode = btnDn.down ? SWALLOW : NONE; }
        } else if (!btnDn.down) { app.saver = false; Snd::back(); mode = btnUp.down ? SWALLOW : NONE; }
        break;

      case DASH_BTN:                                 // OBD2 ana ekran: A kisa sonraki / cift tik islem, B kisa onceki, basili = ses
        userActivity();
        if (u && d) { mode = COMBO; comboFired = false; app.comboProgress = 0; break; }
        if (active->down && now - active->downAt >= REPEAT_DELAY_MS) {
          if (bleConnected) { fireSingle(active); repeatIv = REPEAT_START_MS; startRepeat(now); }
          else mode = SWALLOW;
          break;
        }
        if (!active->down) {
          if (active == &btnDn) { Obd::prevPage(); mode = NONE; }
          else { mode = DASH_DBL; relAt = now; }
        }
        break;

      case DASH_DBL:                                 // A: ikinci tik bekle
        if (btnUp.down) { Obd::saverKey(true); mode = SWALLOW; break; }
        if (btnDn.down) { Obd::saverKey(false); active = &btnDn; mode = DASH_BTN; pendingAt = now; break; }
        if (now - relAt >= DBL_CLICK_MS) { Obd::saverKey(false); mode = NONE; }
        break;

      case SWALLOW:
        if (!u && !d) mode = NONE;
        break;
    }
  }
}

// ============================================================================
//  I2C ENKODER UNITESI (Grove: SDA G32 / SCL G33, adres 0x40)
// ============================================================================
//  M5Stack Unit Encoder ve ayni protokolu konusan MGM_Encoder_C3 kutusu.
//  Takili degilse 3 sn'de bir yoklanir (sonradan takilabilir).
//  Cevir: ses (menude gezinme). Kisa bas: OBD2 sonraki sayfa (gozlerde OBD2'ye don,
//  OBD2 kapaliysa oynat/durdur). Cift bas: oynat/durdur. Uzun bas: menu / geri.
#if !USE_ENCODER
namespace EncUnit {
  constexpr uint8_t  ADDR = 0x40;
  constexpr int      DIV = 1;                   // tik basina sayac artisi (orijinal unite 2 sayarsa 2)
  constexpr uint32_t LONG_MS = 650, DBL_MS = DBL_CLICK_MS + 60;
  bool     present = false;
  int16_t  lastVal = 0;
  int      acc = 0;
  uint8_t  fails = 0;
  uint32_t probeAt = 0, pollAt = 0;
  bool     btn = false, longDone = false, swallow = false;
  uint32_t downAt = 0, relAt = 0;
  uint8_t  clicks = 0;
  bool     mgm = false;                         // MGM kutusu (OLED'e ses / ekran adi yollanir)
  uint8_t  sentVol = 0xEE; char sentLbl[16] = "";
  uint32_t lblAt = 0, resendAt = 0;

  bool rd(uint8_t reg, uint8_t* b, uint8_t n) {
    Wire.beginTransmission(ADDR); Wire.write(reg);
    if (Wire.endTransmission() != 0) return false;
    if (Wire.requestFrom((int)ADDR, (int)n) != n) return false;
    for (int i = 0; i < n; i++) b[i] = Wire.read();
    return true;
  }
  void setLed(uint32_t rgb) {                   // orijinal unitede 2 LED (0 = ikisi), kutuda mavi LED
    Wire.beginTransmission(ADDR);
    Wire.write(0x30); Wire.write(0); Wire.write((rgb >> 16) & 0xFF); Wire.write((rgb >> 8) & 0xFF); Wire.write(rgb & 0xFF);
    Wire.endTransmission();
  }
  // Turkce -> ASCII (kutunun OLED yazi tipi icin)
  void ascii(const char* in, char* o, size_t n) {
    size_t k = 0; const uint8_t* p = (const uint8_t*)in;
    while (*p && k + 1 < n) {
      if (*p < 0x80) { o[k++] = *p++; continue; }
      uint16_t u = (p[1] && (*p & 0xE0) == 0xC0) ? ((*p & 0x1F) << 6) | (p[1] & 0x3F) : 0;
      char c = '?';
      switch (u) {
        case 0xE7: c = 'c'; break; case 0xC7: c = 'C'; break; case 0x11F: c = 'g'; break; case 0x11E: c = 'G'; break;
        case 0x131: c = 'i'; break; case 0x130: c = 'I'; break; case 0xF6: c = 'o'; break; case 0xD6: c = 'O'; break;
        case 0x15F: c = 's'; break; case 0x15E: c = 'S'; break; case 0xFC: c = 'u'; break; case 0xDC: c = 'U'; break;
        case 0xB0: c = 'o'; break;
      }
      o[k++] = c;
      p += (*p & 0xE0) == 0xC0 ? 2 : (*p & 0xF0) == 0xE0 ? 3 : (*p & 0xF8) == 0xF0 ? 4 : 1;
    }
    o[k] = 0;
  }
  // Kutuya ses ve ekran adini yolla (degisince; 3 sn'de bir tekrar)
  void sendInfo(uint32_t now) {
    uint8_t v = !bleConnected ? 254 : app.muted ? 255 : (uint8_t)constrain((int)lroundf(app.level * 100.0f / volSteps()), 0, 100);
    bool again = now >= resendAt;
    if (v != sentVol || again) {
      Wire.beginTransmission(ADDR); Wire.write(0x40); Wire.write(v);
      if (Wire.endTransmission() == 0) sentVol = v;
    }
    if (now - lblAt < 300 && !again) return;
    lblAt = now;
    const char* l = menu.open || Obd::menu ? "MENU" : app.saver ? "GOZLER" : Obd::dashActive() ? Obd::curPageName() : "SES";
    char a[16]; ascii(l, a, sizeof(a));
    if (strcmp(a, sentLbl) || again) {
      Wire.beginTransmission(ADDR); Wire.write(0x41); Wire.write((const uint8_t*)a, strlen(a) + 1);
      if (Wire.endTransmission() == 0) strcpy(sentLbl, a);
    }
    if (again) resendAt = now + 3000;
  }
  void begin() {
    Wire.begin(32, 33, 100000);
    Wire.setTimeOut(15);
    probeAt = millis() + 1500;
  }

  // ---- tus islemleri ----
  bool modalKey() {                             // her seyin onundeki pencereler (true = tuketildi)
    Music::cardAt = 0;
    if (Obd::alertBlocking()) { Obd::ackAlert(); Snd::back(); return true; }
    if (Info::active()) { Info::ack(); Snd::back(); return true; }
    if (app.chargeAnimUntil > millis()) { app.chargeAnimUntil = 0; return true; }
    return false;
  }
  void shortPress() {
    if (Pair::modal == Pair::M_CONFIRM) { Pair::accept(); return; }
    if (Pair::modal != Pair::M_NONE) return;
    if (modalKey()) return;
    if (Obd::menu)       { Obd::encKey(2); return; }
    if (menu.open)       { menuSelect(); return; }
    if (IRL::mode)       { IRL::onKey(true); return; }
    if (AutoRot::calStep){ AutoRot::calCapture(); return; }
    if (app.camera)      { Cam::shoot(); return; }
    if (app.saver && Obd::dashActive()) { app.saver = false; Snd::click(); return; }   // gozlerden OBD2'ye
    if (Obd::dashActive()) { Obd::saverKey(false); return; }                          // sonraki sayfa
    if (bleConnected) playPause(); else menuOpen();
  }
  void doublePress() {
    if (modalKey()) return;
    if (bleConnected) playPause(); else showToast("Telefon bağlı değil", C_WARN, 1000);
  }
  void longPress() {
    if (Pair::modal != Pair::M_NONE) { Pair::reject(); return; }
    if (Obd::menu)        { Obd::encKey(3); return; }
    if (AutoRot::calStep) { AutoRot::calCancel(); return; }
    if (IRL::mode)        { IRL::cancel(); return; }
    if (app.camera)       { Cam::exit(); Snd::back(); return; }
    if (menu.open)        { menuBack(); return; }
    if (modalKey()) return;
    app.saver = false;
    menuOpen();
  }
  // Menu / onay pencerelerinde cift tik beklenmez (hizli secim)
  bool instant() { return menu.open || Obd::menu || Pair::modal != Pair::M_NONE || IRL::mode || AutoRot::calStep || app.camera; }

  void step(int dir) {
    if (Pair::modal != Pair::M_NONE || AutoRot::calStep || IRL::mode || app.camera) return;
    if (Obd::menu)  { Obd::encKey(dir); return; }
    if (menu.open)  { if (dir > 0) menuNext(); else menuPrev(); return; }
    if (dir > 0) volumeUp(); else volumeDown();          // gozlerde: koruyucu acik kalir, gozler tepki verir
  }

  void service() {
    uint32_t now = millis();
    if (!present) {
      if (now < probeAt) return;
      probeAt = now + 3000;
      uint8_t b[2];
      if (!rd(0x10, b, 2)) return;
      present = true; fails = 0; acc = 0; btn = false; clicks = 0; swallow = false;
      lastVal = (int16_t)(b[0] | (b[1] << 8));
      uint8_t v = 0, m = 0; rd(0xFE, &v, 1); rd(0xFF, &m, 1);
      mgm = (m == 0x4D); sentVol = 0xEE; sentLbl[0] = 0; resendAt = 0;
      Serial.printf("[Enkoder] baglandi (I2C 0x40, surum %u, %s)\n", v, mgm ? "MGM kutusu, OLED" : "M5 Unit Encoder");
      showToast("Enkoder bağlandı", C_OK, 1200); Snd::connect();
      setLed(0x000000);
      return;
    }
    if (now - pollAt < 25) return;
    pollAt = now;
    uint8_t b[2], k = 1;
    if (!rd(0x10, b, 2) || !rd(0x20, &k, 1)) {
      if (++fails >= 8) {
        present = false; probeAt = now + 3000;
        Serial.println("[Enkoder] ayrildi");
        showToast("Enkoder ayrıldı", C_WARN, 1200);
      }
      return;
    }
    fails = 0;
    // donus
    int16_t v = (int16_t)(b[0] | (b[1] << 8));
    int16_t d = (int16_t)(v - lastVal); lastVal = v;
    if (d) {
      acc += d;
      int st = acc / DIV; acc -= st * DIV;
      if (st) {
        bool wasOff = userActivity(); app.nightOff = false;
        if (!wasOff) {
          st = constrain(st, -6, 6);                      // cok hizli cevirmede kuyruk tasmasin
          while (st > 0) { step(1); st--; }
          while (st < 0) { step(-1); st++; }
        }
      }
    }
    // tus (0 = basili)
    bool p = (k == 0);
    if (p && !btn) {
      btn = true; downAt = now; longDone = false;
      swallow = userActivity(); app.nightOff = false;   // ekran kapaliydi: sadece uyandir
    }
    if (p && btn && !longDone && !swallow && now - downAt >= LONG_MS) { longDone = true; clicks = 0; longPress(); }
    if (!p && btn) {
      btn = false; relAt = now;
      if (!longDone && !swallow) {
        if (instant()) shortPress();
        else if (++clicks >= 2) { clicks = 0; doublePress(); }
      }
    }
    if (clicks && !btn && now - relAt >= DBL_MS) { clicks = 0; shortPress(); }
    if (mgm) sendInfo(now);
  }
}
#endif

// ============================================================================
//  OPSIYONEL ROTARY ENCODER
// ============================================================================
#if USE_ENCODER
volatile int32_t encCount = 0;
volatile uint8_t encState = 0;
Btn btnEnc;

void IRAM_ATTR encISR() {
  static const int8_t table[16] = {0,-1,1,0, 1,0,0,-1, -1,0,0,1, 0,1,-1,0};
  encState = ((encState << 2) | (digitalRead(PIN_ENC_A) << 1) | digitalRead(PIN_ENC_B)) & 0x0F;
  encCount += table[encState];
}

void encoderBegin() {
  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
  btnEnc.begin(PIN_ENC_SW);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), encISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), encISR, CHANGE);
}

void encoderService() {
  static int32_t last = 0;
  noInterrupts(); int32_t c = encCount; interrupts();
  int32_t det = c / ENC_STEPS_PER_DETENT;
  if (det != last) {
    int32_t diff = det - last; last = det;
    if (userActivity()) return;
    if (Pair::modal != Pair::M_NONE) return;
    if (menu.open) { while (diff-- > 0) menuNext(); return; }
    while (diff > 0) { volumeUp();   diff--; }
    while (diff < 0) { volumeDown(); diff++; }
  }
  if (btnEnc.update() && btnEnc.down) {
    if (userActivity()) return;
    if (Pair::modal == Pair::M_CONFIRM) Pair::accept();
    else if (menu.open) menuSelect();
    else if (app.camera) Cam::shoot();
    else bothShort();
  }
}
#endif

// ============================================================================
//  CIZIM YARDIMCILARI
// ============================================================================
TFT_eSPI& G() { return useSprite ? (TFT_eSPI&)spr : tft; }

uint16_t levelColor(float frac) {
  uint8_t r, g, b;
  if (frac < 0.5f)      { float t = frac / 0.5f;          r = 0;       g = 200 + 55 * t;  b = 255 - 200 * t; }
  else if (frac < 0.8f) { float t = (frac - 0.5f) / 0.3f; r = 255 * t; g = 255;           b = 55 - 55 * t; }
  else                  { float t = (frac - 0.8f) / 0.2f; r = 255;     g = 255 - 190 * t; b = 0; }
  return tft.color565(r, g, b);
}

uint16_t blend565(uint16_t a, uint16_t b, float t) {
  return tft.alphaBlend((uint8_t)(constrain(t, 0.0f, 1.0f) * 255), b, a);
}

// ---- Turkce karakterler ------------------------------------------------------
//  Ekran fontlarinda (1, 2, 4) Turkce harfler nadir kullanilan ASCII kodlarinin
//  yerine cizildi (src/TFT_eSPI/Fonts). Bu fonksiyon UTF-8 metni o kodlara cevirir;
//  metindeki gercek ` { | } ~ ^ \ [ ] _ $ & # karakterleri benzerleriyle degistirilir.
size_t trMap(const char* in, char* out, size_t n) {
  size_t o = 0;
  const uint8_t* p = (const uint8_t*)in;
  while (*p && o + 1 < n) {
    uint8_t c = *p;
    char r;
    if (c < 0x80) {
      switch (c) {
        case '`': r = '\''; break; case '{': r = '('; break; case '}': r = ')'; break;
        case '|': r = 'I'; break;  case '~': r = '-'; break; case '^': r = '-'; break;
        case '\\': r = '/'; break; case '[': r = '('; break; case ']': r = ')'; break;
        case '_': r = ' '; break;  case '$': r = 'S'; break; case '&': r = '+'; break;
        case '#': r = 'N'; break;  default: r = (char)c;
      }
      p++;
    } else if ((c & 0xE0) == 0xC0 && p[1]) {
      uint16_t u = ((c & 0x1F) << 6) | (p[1] & 0x3F);
      switch (u) {
        case 0xE7:  r = '`';  break;  case 0xC7:  r = '\\'; break;   // c C (cedilla)
        case 0x11F: r = '{';  break;  case 0x11E: r = '[';  break;   // g G (breve)
        case 0x131: r = '|';  break;  case 0x130: r = ']';  break;   // noktasiz i, noktali I
        case 0xF6:  r = '}';  break;  case 0xD6:  r = '_';  break;   // o O
        case 0x15F: r = '~';  break;  case 0x15E: r = '$';  break;   // s S
        case 0xFC:  r = '^';  break;  case 0xDC:  r = '&';  break;   // u U
        case 0xB0:  r = '#';  break;                                 // derece
        case 0xE9: case 0xE8: case 0xEB: r = 'e'; break;
        case 0xE1: case 0xE0: case 0xE2: case 0xE4: r = 'a'; break;
        default: r = '?';
      }
      p += 2;
    } else {                                           // 3-4 baytlik (emoji vb.): atla
      int len = (c & 0xF0) == 0xE0 ? 3 : (c & 0xF8) == 0xF0 ? 4 : 1;
      for (int k = 0; k < len && *p; k++) p++;
      continue;
    }
    out[o++] = r;
  }
  out[o] = 0;
  return o;
}
int textW(const char* s, uint8_t font) {
  char b[64];
  trMap(s, b, sizeof(b));
  return G().textWidth(b, font);
}

// Metni genislige sigdir ("..")
void fitText(const char* s, int maxW, uint8_t font, char* out, size_t n) {
  TFT_eSPI& g = G();
  strncpy(out, s, n - 1); out[n - 1] = 0;
  if (g.textWidth(out, font) <= maxW) return;
  size_t len = strlen(out);
  char tmp[48];
  while (len > 0) {
    out[--len] = 0;
    snprintf(tmp, sizeof(tmp), "%s..", out);
    if (g.textWidth(tmp, font) <= maxW) { strncpy(out, tmp, n - 1); out[n - 1] = 0; return; }
  }
}

void drawText(const char* s, int x, int y, uint8_t datum, uint8_t font, uint16_t fg, uint16_t bg, int maxW = 0) {
  TFT_eSPI& g = G();
  char m[48], buf[48];
  trMap(s, m, sizeof(m));                              // UTF-8 Turkce -> font kodlari
  if (maxW > 0) fitText(m, maxW, font, buf, sizeof(buf));
  else { strncpy(buf, m, sizeof(buf) - 1); buf[sizeof(buf) - 1] = 0; }
  g.setTextFont(font);
  g.setTextDatum(datum);
  g.setTextColor(fg, bg);
  g.drawString(buf, x, y);
}

void drawBtIcon(int x, int y, int h, uint16_t c) {
  TFT_eSPI& g = G();
  int w = h / 4, q = h / 4;
  g.drawWideLine(x, y, x, y + h, 2, c, C_BG);
  g.drawWideLine(x, y, x + w, y + q, 2, c, C_BG);
  g.drawWideLine(x + w, y + q, x - w, y + 3 * q, 2, c, C_BG);
  g.drawWideLine(x, y + h, x + w, y + 3 * q, 2, c, C_BG);
  g.drawWideLine(x + w, y + 3 * q, x - w, y + q, 2, c, C_BG);
}

void drawSpeaker(int x, int y, uint16_t c, bool muted) {
  TFT_eSPI& g = G();
  g.fillRect(x, y + 5, 5, 8, c);
  g.fillTriangle(x + 4, y + 9, x + 12, y + 1, x + 12, y + 17, c);
  if (muted) {
    g.drawWideLine(x + 16, y + 5, x + 24, y + 13, 2, C_BAD, C_BG);
    g.drawWideLine(x + 24, y + 5, x + 16, y + 13, 2, C_BAD, C_BG);
  } else {
    g.drawArc(x + 12, y + 9, 7, 6, 210, 330, c, C_BG, true);
    g.drawArc(x + 12, y + 9, 12, 11, 215, 325, c, C_BG, true);
  }
}

void drawBattery(int x, int y) {
  TFT_eSPI& g = G();
  if (app.batteryPct < 0 && !app.usbPower) return;
  uint16_t c = app.usbPower ? C_OK
             : app.batteryPct <= 15 ? C_BAD
             : app.batteryPct <= 35 ? C_WARN : C_TEXT;
  g.drawRoundRect(x, y, 26, 13, 3, c);
  g.fillRect(x + 26, y + 4, 2, 5, c);
  if (app.usbPower) {
    int w = (int)(((millis() / 250) % 6) * 22 / 5);          // dolan sarj cubugu
    if (w > 0) g.fillRoundRect(x + 2, y + 2, w, 9, 2, blend565(C_OK, C_BG, 0.45f));
    g.fillTriangle(x + 14, y + 2, x + 8, y + 7, x + 13, y + 7, C_TEXT);
    g.fillTriangle(x + 12, y + 6, x + 18, y + 6, x + 11, y + 11, C_TEXT);
  } else {
    bool blinkOff = app.batteryPct <= 10 && (millis() / 400) % 2;
    int w = (int)(22 * app.batteryPct / 100.0f + 0.5f);
    if (w > 0 && !blinkOff) g.fillRoundRect(x + 2, y + 2, w, 9, 2, c);
  }
}

void drawBars(int x, int yBottom, int level, uint16_t on, uint16_t off) {
  TFT_eSPI& g = G();
  for (int i = 0; i < 4; i++) {
    int h = 3 + i * 2;
    g.fillRect(x + i * 4, yBottom - h, 3, h, i < level ? on : off);
  }
}

void drawPill(int cx, int cy, const char* txt, uint16_t fill, uint16_t fg, int font = 2) {
  TFT_eSPI& g = G();
  int w = textW(txt, font) + 18, h = 18;
  g.fillRoundRect(cx - w / 2, cy - h / 2, w, h, h / 2, fill);
  drawText(txt, cx, cy + 1, MC_DATUM, font, fg, fill);
}

// Yay/halka acisi (TFT_eSPI: 0 = saat 6, saat yonunde) -> ekran koordinati
void arcPoint(int a, int r, int& x, int& y) {
  float th = (90.0f + a) * DEG_TO_RAD;
  x = gz.cx + cosf(th) * r;
  y = gz.cy + sinf(th) * r;
}

// ============================================================================
//  ANA ARAYUZ
// ============================================================================
void drawMoon(int cx, int cy, int r, uint16_t c) {
  TFT_eSPI& g = G();
  g.fillCircle(cx, cy, r, c);
  g.fillCircle(cx + r / 2 + 1, cy - r / 3, r, C_BG);
}

// ---- Hava durumu simgeleri (yaklasik 18x16 px, merkez cx,cy) ----------------
void drawCloud(int cx, int cy, uint16_t c) {
  TFT_eSPI& g = G();
  g.fillCircle(cx - 4, cy + 2, 4, c);
  g.fillCircle(cx + 1, cy - 1, 5, c);
  g.fillCircle(cx + 6, cy + 2, 4, c);
  g.fillRect(cx - 4, cy + 2, 10, 5, c);
}
void drawSun(int cx, int cy, int r, uint16_t c) {
  TFT_eSPI& g = G();
  g.fillCircle(cx, cy, r, c);
  for (int i = 0; i < 8; i++) {
    float a = i * 45 * DEG_TO_RAD;
    g.drawLine(cx + cosf(a) * (r + 2), cy + sinf(a) * (r + 2), cx + cosf(a) * (r + 4), cy + sinf(a) * (r + 4), c);
  }
}
// ---- Olceklenebilir hava ikonu (s = 1: ~18x16 px; hava kartinda s = 2..3) -------
void drawCloudS(int cx, int cy, float s, uint16_t c) {
  TFT_eSPI& g = G();
  g.fillCircle(cx - 4 * s, cy + 2 * s, 4 * s, c); g.fillCircle(cx + 1 * s, cy - 1 * s, 5 * s, c);
  g.fillCircle(cx + 6 * s, cy + 2 * s, 4 * s, c); g.fillRect(cx - 4 * s, cy + 2 * s, 10 * s + 1, 5 * s, c);
}
void drawSunS(int cx, int cy, float r, uint16_t c) {
  TFT_eSPI& g = G(); g.fillSmoothCircle(cx, cy, (int)r, c, C_BG);
  float k = r / 5.0f;
  for (int i = 0; i < 8; i++) {
    float a = i * 45 * DEG_TO_RAD;
    g.drawWideLine(cx + cosf(a) * (r + 2 * k), cy + sinf(a) * (r + 2 * k), cx + cosf(a) * (r + 4 * k), cy + sinf(a) * (r + 4 * k), max(1.0f, k), c, C_BG);
  }
}
void drawMoonS(int cx, int cy, int r, uint16_t c) { G().fillCircle(cx, cy, r, c); G().fillCircle(cx + r / 2 + 1, cy - r / 3, r, C_BG); }
void drawWxIconS(int cx, int cy, int code, bool day, float s) {
  TFT_eSPI& g = G();
  const uint16_t SUN = 0xFE60, MOONC = 0xE71C, CLD = 0xBDF7, DARK = 0x8C71, RAIN = 0x3D9F, SNOW = 0xFFFF, BOLT = 0xFFE0;
  if (code == 0) { if (day) drawSunS(cx, cy, 5 * s, SUN); else drawMoonS(cx, cy, 6 * s, MOONC); return; }
  if (code <= 2) { if (day) drawSunS(cx - 4 * s, cy - 3 * s, 4 * s, SUN); else drawMoonS(cx - 4 * s, cy - 3 * s, 4 * s, MOONC); drawCloudS(cx + 2 * s, cy + 2 * s, s, CLD); return; }
  if (code == 45 || code == 48) { for (int i = 0; i < 3; i++) g.fillRoundRect(cx - (8 - (i % 2) * 2) * s, cy + (-5 + i * 5) * s, 15 * s, 2 * s, s, CLD); return; }
  bool rain = (code >= 51 && code <= 67) || (code >= 80 && code <= 82);
  bool snow = (code >= 71 && code <= 77) || code == 85 || code == 86;
  drawCloudS(cx, cy - 3 * s, s, (rain || code >= 95) ? DARK : CLD);
  if (rain) for (int i = 0; i < 3; i++) g.drawWideLine(cx + (-5 + i * 5) * s, cy + 6 * s, cx + (-7 + i * 5) * s, cy + 10 * s, max(1.0f, s * 0.8f), RAIN, C_BG);
  if (snow) for (int i = 0; i < 3; i++) g.fillCircle(cx + (-5 + i * 5) * s, cy + 8 * s, max(1.0f, s), SNOW);
  if (code >= 95) { g.fillTriangle(cx + 1 * s, cy + 3 * s, cx - 3 * s, cy + 9 * s, cx, cy + 9 * s, BOLT); g.fillTriangle(cx, cy + 8 * s, cx + 3 * s, cy + 8 * s, cx - 2 * s, cy + 13 * s, BOLT); }
}
void drawWxIcon(int cx, int cy, int code, bool day) { drawWxIconS(cx, cy, code, day, 1.0f); }
// Kucuk yagmur damlasi
void drawDrop(int cx, int cy, int r, uint16_t c) {
  G().fillCircle(cx, cy + r / 2, r, c);
  G().fillTriangle(cx - r, cy + r / 2, cx + r, cy + r / 2, cx, cy - r - 2, c);
}
// Derece isaretli metin; kullanilan genislik doner
int drawDeg(const char* t, int x, int y, uint8_t datum, uint8_t font, uint16_t c) {
  int tw = textW(t, font), r = font >= 4 ? 3 : 2, full = tw + r * 2 + 2;
  int x0 = (datum == MR_DATUM) ? x - full : (datum == MC_DATUM ? x - full / 2 : x);
  drawText(t, x0, y, ML_DATUM, font, c, C_BG);
  G().drawCircle(x0 + tw + r + 1, y - (font >= 4 ? 8 : 5), r, c);
  return full;
}
uint16_t rainCol(uint8_t p) { return blend565(tft.color565(40, 70, 110), tft.color565(70, 170, 255), p / 100.0f); }
// Ust serit hava ozeti: ikon + derece (+ >= %20 ise damla ve yuzde). Sag kenar xR
int wxMiniW(bool withRain) {
  char t[8], r[8]; snprintf(t, sizeof(t), "%d", Wx::tempNow());
  uint8_t pk = Wx::rainPeak(3, nullptr); int w = 0;
  if (withRain && pk >= 20) { snprintf(r, sizeof(r), "%d%%", pk); w += textW(r, 2) + 12; }
  return w + textW(t, 2) + 6 + 2 + 20;
}
int drawWxMini(int xR, int cy, bool withRain) {
  char t[8], r[8]; snprintf(t, sizeof(t), "%d", Wx::tempNow());
  uint8_t pk = Wx::rainPeak(3, nullptr);
  int w = 0;
  if (withRain && pk >= 20) {
    snprintf(r, sizeof(r), "%d%%", pk); int rw = textW(r, 2);
    drawText(r, xR, cy, MR_DATUM, 2, rainCol(pk), C_BG); drawDrop(xR - rw - 5, cy - 1, 3, rainCol(pk)); w += rw + 12;
  }
  w += drawDeg(t, xR - w, cy, MR_DATUM, 2, C_TEXT2) + 2;
  drawWxIconS(xR - w - 9, cy, Net::wcode, Net::isDay, 0.85f);
  return w + 20;
}
// ---- Muzik (iPhone AMS): nota ikonu, sarki satiri, sarki karti -----------------------
void drawNote(int cx, int cy, int h, uint16_t c) {
  TFT_eSPI& g = G();
  float s = h / 16.0f; int w = max(1, (int)lroundf(1.6f * s));
  int x1 = cx - 5 * s, x2 = cx + 5 * s, yt = cy - 8 * s, yb = cy + 5 * s;
  g.fillEllipse(x1 - 2 * s, yb, 3 * s, 2.3f * s, c); g.fillEllipse(x2 - 2 * s, yb - 2 * s, 3 * s, 2.3f * s, c);
  g.fillRect(x1 + s * 0.6f, yt + 2 * s, w, yb - yt - 2 * s, c); g.fillRect(x2 + s * 0.6f, yt, w, yb - yt - 2 * s, c);
  g.drawWideLine(x1 + s, yt + 2.5f * s, x2 + s, yt + 0.5f * s, 3 * s, c, C_BG);
}
void musicCopy(char* t, size_t tn, char* a, size_t an) {
  portENTER_CRITICAL(&Music::mux);
  strncpy(t, Music::title, tn - 1); t[tn - 1] = 0; strncpy(a, Music::artist, an - 1); a[an - 1] = 0;
  portEXIT_CRITICAL(&Music::mux);
}
// Yatay ses ekrani alt satiri: nota + "Baslik - Sanatci"
void drawTrackLine(int cx, int cy, int maxW) {
  char t[64], a[48], b[112], m[112], f[64];
  musicCopy(t, sizeof(t), a, sizeof(a));
  if (a[0]) snprintf(b, sizeof(b), "%s - %s", t, a); else snprintf(b, sizeof(b), "%s", t);
  trMap(b, m, sizeof(m));
  if (strlen(m) > sizeof(f) - 3) m[sizeof(f) - 3] = 0;
  fitText(m, maxW - 16, 2, f, sizeof(f));
  int tw = tft.textWidth(f, 2), x = cx - (tw + 14) / 2;
  drawNote(x + 5, cy, 11, Music::playing ? C_MENU : C_SUBTEXT);
  TFT_eSPI& g = G(); g.setTextFont(2); g.setTextDatum(ML_DATUM); g.setTextColor(C_TEXT2, C_BG); g.drawString(f, x + 14, cy);
}
void fmtMin(char* o, size_t n, float s) { int v = (int)s; snprintf(o, n, "%d:%02d", v / 60, v % 60); }
void drawCover(int x, int y, int sz) {                         // kapak yerine renkli kare + nota
  TFT_eSPI& g = G();
  for (int i = 0; i < sz; i += 2) g.fillRect(x, y + i, sz, 2, blend565(tft.color565(150, 40, 255), tft.color565(0, 190, 255), i / (float)sz));
  g.fillRect(x, y, 2, 1, C_PANEL); g.fillRect(x, y, 1, 2, C_PANEL); g.fillRect(x + sz - 2, y, 2, 1, C_PANEL); g.fillRect(x + sz - 1, y, 1, 2, C_PANEL);
  g.fillRect(x, y + sz - 1, 2, 1, C_PANEL); g.fillRect(x, y + sz - 2, 1, 2, C_PANEL); g.fillRect(x + sz - 2, y + sz - 1, 2, 1, C_PANEL); g.fillRect(x + sz - 1, y + sz - 2, 1, 2, C_PANEL);
  drawNote(x + sz / 2 + 1, y + sz / 2, sz / 2, C_TEXT);
}
constexpr uint32_t TRACK_CARD_MS = 4500;
bool trackCardActive() { return Music::cardAt && Music::has && millis() - Music::cardAt < TRACK_CARD_MS; }
void drawTrackCard() {
  TFT_eSPI& g = G();
  char t[64], ar[48], a[8], b[8];
  musicCopy(t, sizeof(t), ar, sizeof(ar));
  float d = Music::duration, p = Music::pos();
  fmtMin(a, sizeof(a), p); fmtMin(b, sizeof(b), d);
  float pr = d > 0 ? p / d : 0;
  const char* st = Music::playing ? "Şimdi çalıyor" : "Duraklatıldı";
  uint16_t sc = Music::playing ? C_MENU : C_SUBTEXT;
  if (!portrait) {
    int w = SW - 12, h = 84, x = 6, y = SH / 2 - h / 2;
    g.fillRoundRect(x, y, w, h, 12, C_PANEL);
    g.drawRoundRect(x, y, w, h, 12, blend565(C_MENU, C_PANEL, 0.3f));
    drawCover(x + 10, y + 8, 46);
    int tx = x + 70, tw = w - 78;
    drawText(st, tx, y + 14, ML_DATUM, 1, sc, C_PANEL);
    drawText(t, tx, y + 32, ML_DATUM, 2, C_TEXT, C_PANEL, tw);
    drawText(ar, tx, y + 50, ML_DATUM, 2, C_TEXT2, C_PANEL, tw);
    int by = y + h - 13, bx = x + 10, bw = w - 20;
    g.fillRoundRect(bx, by, bw, 3, 1, C_TRACK);
    if (d > 0) {
      g.fillRoundRect(bx, by, max(1, (int)(bw * pr)), 3, 1, C_MENU); g.fillCircle(bx + (int)(bw * pr), by + 1, 3, C_TEXT);
      drawText(a, bx, by - 7, ML_DATUM, 1, C_SUBTEXT, C_PANEL); drawText(b, x + w - 10, by - 7, MR_DATUM, 1, C_SUBTEXT, C_PANEL);
    }
  } else {
    int w = SW - 10, h = 170, x = 5, y = SH / 2 - h / 2;
    g.fillRoundRect(x, y, w, h, 12, C_PANEL);
    g.drawRoundRect(x, y, w, h, 12, blend565(C_MENU, C_PANEL, 0.3f));
    drawCover(SW / 2 - 32, y + 12, 64);
    drawText(st, SW / 2, y + 88, MC_DATUM, 1, sc, C_PANEL);
    drawText(t, SW / 2, y + 106, MC_DATUM, 2, C_TEXT, C_PANEL, w - 10);
    drawText(ar, SW / 2, y + 124, MC_DATUM, 2, C_TEXT2, C_PANEL, w - 10);
    int by = y + h - 26, bx = x + 10, bw = w - 20;
    g.fillRoundRect(bx, by, bw, 3, 1, C_TRACK);
    if (d > 0) {
      g.fillRoundRect(bx, by, max(1, (int)(bw * pr)), 3, 1, C_MENU); g.fillCircle(bx + (int)(bw * pr), by + 1, 3, C_TEXT);
      drawText(a, bx, by + 12, ML_DATUM, 1, C_SUBTEXT, C_PANEL); drawText(b, bx + bw, by + 12, MR_DATUM, 1, C_SUBTEXT, C_PANEL);
    }
  }
}
// "18" + derece isareti; genislik doner
int drawTemp(int x, int y, uint8_t datum, uint8_t font, uint16_t c) {
  char t[8];
  snprintf(t, sizeof(t), "%d", Wx::tempNow());
  int tw = textW(t, font);
  int x0 = (datum == MR_DATUM) ? x - tw - 6 : (datum == MC_DATUM ? x - (tw + 6) / 2 : x);
  drawText(t, x0, y, ML_DATUM, font, c, C_BG);
  int r = font >= 4 ? 3 : 2;
  G().drawCircle(x0 + tw + r + 1, y - (font >= 4 ? 8 : 5), r, c);
  return tw + 6;
}

void drawHeader(const char* title = nullptr) {
  bool conn = bleConnected;
  uint16_t btc = conn ? C_BT : bleHidden ? C_TRACK : ((millis() / 500) % 2 ? C_SUBTEXT : C_TRACK);
  drawBtIcon(10, 4, 16, btc);
  const char* txt = title;
  char nm[32];
  if (!txt) {
    if (conn && !portrait && app.peerName[0]) { snprintf(nm, sizeof(nm), "%s", app.peerName); txt = nm; }
    else if (!conn && bleHidden) txt = "Gizli mod";
    else txt = conn ? "Bağlı" : (portrait ? "Bekleniyor" : "Eşleşme bekleniyor");
  }
  int reserve = 40;
  if (SleepT::active()) {
    char m[8];
    if (SleepT::fading()) strcpy(m, "..");
    else snprintf(m, sizeof(m), "%lu", (unsigned long)SleepT::minsLeft());
    int tw = textW(m, 2);
    drawText(m, SW - 40, 12, MR_DATUM, 2, C_MENU, C_BG);
    drawMoon(SW - 46 - tw, 11, 5, C_MENU);
    reserve += tw + 18;
  }
  char clk[8];
  if (!title && Net::clock(clk, sizeof(clk)) && !(portrait && SleepT::active())) {
    int tw = textW(clk, 2);
    drawText(clk, SW - reserve, 12, MR_DATUM, 2, C_TEXT2, C_BG);
    reserve += tw + 8;
  }
  // Yatay ses ekrani: bagliyken isim yerine hava ozeti (isim menude ve alt satirda)
  int ww = (!title && conn && !portrait && Net::hasWeather()) ? wxMiniW(true) : 0;
  if (ww && ww <= SW - 24 - reserve) drawWxMini(24 + ww, 12, true);
  else drawText(txt, 22, 12, ML_DATUM, 2, (title || conn) ? C_TEXT : C_SUBTEXT, C_BG, SW - 22 - reserve);
  drawBattery(SW - 34, 5);
}

void drawGauge(bool atMax, bool atMin, float flash) {
  TFT_eSPI& g = G();
  int steps = volSteps();
  int span = gz.a1 - gz.a0;
  float frac = constrain(app.shownLevel / (float)steps, 0.0f, 1.0f);
  bool dim = app.muted || !bleConnected;

  uint16_t track = C_TRACK;
  if (atMin && !dim) track = blend565(C_TRACK, C_MIN, 0.25f + 0.45f * flash);
  g.drawSmoothArc(gz.cx, gz.cy, gz.rOut, gz.rIn, gz.a0, gz.a1, track, C_BG, true);

  uint32_t endA = gz.a0 + (uint32_t)(span * frac + 0.5f);
  if (endA > (uint32_t)gz.a0 + 1) {
    uint16_t col = dim ? C_SUBTEXT : levelColor(frac);
    if (atMax && !dim) col = blend565(C_BAD, C_TEXT, flash * 0.6f);
    g.drawSmoothArc(gz.cx, gz.cy, gz.rOut, gz.rIn, gz.a0, endA, col, C_BG, true);
  }

  if (atMax && !dim) {
    float ph = (sinf(millis() / 260.0f) + 1) * 0.5f;
    int gr = portrait ? 1 : 3;
    g.drawSmoothArc(gz.cx, gz.cy, gz.rOut + gr + 2, gz.rOut + gr, gz.a0, gz.a1,
                    blend565(C_BG, C_BAD, 0.35f + 0.65f * max(ph, flash)), C_BG, true);
  } else {
    for (int i = 1; i < steps; i++) {
      int a = gz.a0 + span * i / steps;
      int x1, y1, x2, y2;
      arcPoint(a, gz.rOut + (portrait ? 2 : 3), x1, y1);
      arcPoint(a, gz.rOut + (portrait ? 4 : 6), x2, y2);
      g.drawLine(x1, y1, x2, y2, (i <= app.level && !dim) ? C_SUBTEXT : C_TRACK);
    }
  }

  if (app.comboProgress > 0.02f) {
    float p = app.comboProgress;
    uint16_t c = (p < SYNC_HOLD_MS / (float)MENU_HOLD_MS) ? C_ACCENT : C_MENU;
    uint32_t e = gz.a0 + (uint32_t)(span * p);
    g.drawSmoothArc(gz.cx, gz.cy, gz.rIn - 4, gz.rIn - 8, gz.a0, max(e, (uint32_t)gz.a0 + 2), c, C_BG, true);
  }
}

void drawCenter(bool atMax, bool atMin, float flash) {
  TFT_eSPI& g = G();
  if (!bleConnected) {
    float ph = (sinf(millis() / 350.0f) + 1) * 0.5f;
    uint8_t v = 80 + ph * 175;
    uint16_t c = bleHidden ? C_SUBTEXT : tft.color565(v / 8, v / 2, v);
    const char* line = bleHidden ? "Gizli mod" : DEVICE_NAME;
    if (portrait) {
      drawBtIcon(gz.cx, gz.cy - 22, 40, c);
      drawText(line, SW / 2, 176, MC_DATUM, 2, C_TEXT2, C_BG, SW - 8);
      drawText("Menü için", SW / 2, 196, MC_DATUM, 2, C_SUBTEXT, C_BG);
      drawText("bir tuşa basın", SW / 2, 212, MC_DATUM, 2, C_SUBTEXT, C_BG);
    } else {
      drawBtIcon(gz.cx, 46, 36, c);
      drawText(line, gz.cx, 96, MC_DATUM, 2, C_TEXT2, C_BG);
      drawText("Menü: bir tuşa basın", gz.cx, 125, MC_DATUM, 2, C_SUBTEXT, C_BG);
    }
    return;
  }

  if (app.muted) {
    int y = portrait ? gz.cy - 26 : 56;
    drawSpeaker(gz.cx - 14, y, C_BAD, true);
    drawText("SESSİZ", gz.cx, portrait ? gz.cy + 14 : 98, MC_DATUM, 4, C_BAD, C_BG);
    return;
  }

  int pct = (int)(app.shownLevel * 100.0f / volSteps() + 0.5f);
  pct = constrain(pct, 0, 100);
  char buf[6];
  snprintf(buf, sizeof(buf), "%d", pct);

  int shake = (int)(sinf(millis() / 18.0f) * 5.0f * flash);
  uint16_t numCol = C_TEXT;
  if (atMax) numCol = blend565(C_BAD, C_TEXT, flash);
  if (atMin) numCol = blend565(C_MIN, C_TEXT, flash);
  uint16_t pctCol = atMax ? C_BAD : atMin ? C_MIN : C_SUBTEXT;

  g.setTextFont(7);
  int tw = g.textWidth(buf, 7);
  if (portrait) {
    drawText(buf, gz.cx + shake, gz.cy, MC_DATUM, 7, numCol, C_BG);
    drawText("%", gz.cx + shake, gz.cy + 38, MC_DATUM, 4, pctCol, C_BG);
  } else {
    drawText(buf, gz.cx - 6 + shake, 84, MC_DATUM, 7, numCol, C_BG);
    drawText("%", gz.cx - 6 + shake + tw / 2 + 3, 96, ML_DATUM, 4, pctCol, C_BG);
  }
}

void drawFooter(bool atMax, bool atMin, float flash) {
  char buf[24];
  uint16_t stepCol = atMax ? C_BAD : atMin ? C_MIN : C_SUBTEXT;

  bool pill = bleConnected && !app.muted && (atMax || atMin);
  if (bleConnected && Music::has && !pill) {                 // calan sarki (iPhone)
    if (portrait) {
      char t[64], a[48]; musicCopy(t, sizeof(t), a, sizeof(a));
      drawNote(12, 192, 12, Music::playing ? C_MENU : C_SUBTEXT);
      drawText(t, 22, 192, ML_DATUM, 2, C_TEXT, C_BG, SW - 26);
      if (a[0]) drawText(a, 22, 209, ML_DATUM, 2, C_SUBTEXT, C_BG, SW - 26);
    } else drawTrackLine(SW / 2, SH - 9, 150);
  }
  if (portrait && bleConnected && Net::hasWeather()) {       // dikey: isim satirinda hava ozeti
    int w = wxMiniW(true);
    drawWxMini(SW / 2 + w / 2, 33, true);
  } else if (portrait && bleConnected && app.peerName[0])
    drawText(app.peerName, SW / 2, 33, MC_DATUM, 2, C_TEXT2, C_BG, SW - 8);

  snprintf(buf, sizeof(buf), "%d/%d", app.level, volSteps());
  if (bleConnected) drawText(buf, 4, SH, BL_DATUM, 2, stepCol, C_BG);

  if (app.batteryPct >= 0 && !app.usbPower) {
    snprintf(buf, sizeof(buf), "%d%%", app.batteryPct);
    drawText(buf, SW - 4, SH, BR_DATUM, 2, C_SUBTEXT, C_BG);
  } else if (app.usbPower) {
    drawText("USB", SW - 4, SH, BR_DATUM, 2, C_SUBTEXT, C_BG);
  }

  if (bleConnected && !app.muted && (atMax || atMin)) {
    uint16_t base = atMax ? C_BAD : C_MIN;
    bool inv = flash > 0.5f;
    drawPill(SW / 2, portrait ? 184 : 125, atMax ? "MAX" : "MIN", inv ? C_TEXT : base, inv ? base : C_TEXT);
  }
}

// OBD2 ana ekraninda ses degisince 1,5 sn alt kenarda ince ses cubugu
void drawDashVolume() {
  if (!app.volShowAt || millis() - app.volShowAt > 1500 || !bleConnected) return;
  TFT_eSPI& g = G();
  int steps = volSteps();
  float f = constrain(app.shownLevel / (float)steps, 0.0f, 1.0f);
  int w = SW - 16, h = 22, x = 8, y = SH - h - 8;
  g.fillRoundRect(x, y, w, h, 11, C_PANEL);
  drawSpeaker(x + 6, y + 2, app.muted ? C_BAD : C_TEXT2, app.muted);
  int bx = x + 34, bw = w - 34 - 44;
  g.fillRoundRect(bx, y + h / 2 - 3, bw, 6, 3, C_TRACK);
  if (!app.muted) g.fillRoundRect(bx, y + h / 2 - 3, max(6, (int)(bw * f)), 6, 3, levelColor(f));
  char b[8]; snprintf(b, sizeof(b), "%d%%", (int)lroundf(f * 100));
  drawText(b, x + w - 8, y + h / 2 + 1, MR_DATUM, 2, C_TEXT, C_PANEL);
}

void drawToast() {
  if (millis() > app.toastUntil || !app.toast[0]) return;
  TFT_eSPI& g = G();
  char t[32];
  fitText(app.toast, SW - 26, 2, t, sizeof(t));
  int tw = textW(t, 2) + 20;
  int y = portrait ? 200 : 116;
  int x = SW / 2 - tw / 2;
  g.fillRoundRect(x, y, tw, 18, 9, C_PANEL);
  g.drawRoundRect(x, y, tw, 18, 9, app.toastColor);
  drawText(t, SW / 2, y + 9, MC_DATUM, 2, app.toastColor, C_PANEL);
}

// ============================================================================
//  MENU CIZIMI
// ============================================================================

int buildBody(BodyLine* L) {
  int n = 0;
  auto add = [&](const char* s, uint16_t c, uint8_t f = 2) {
    if (n >= 10) return;
    strncpy(L[n].txt, s, sizeof(L[n].txt) - 1); L[n].txt[sizeof(L[n].txt) - 1] = 0;
    L[n].col = c; L[n].font = f; L[n].uml = -1; n++;
  };
  // "GUZELMANSUR" icindeki ilk U harfinin konumu (Ü icin)
  auto umlAt = [&](const char* line) -> int8_t {
    const char* g = strstr(line, "GUZEL");
    return g ? (int8_t)(g - line + 1) : (int8_t)-1;
  };
  char b[40];
  switch (menu.page) {
    case PG_CONFIRM: {
      char nm[32] = "Tüm kayıtlı cihazlar";
      if (menu.delIdx >= 0 && menu.delIdx < Bonds::count) Bonds::label(menu.delIdx, nm, sizeof(nm));
      add(nm, C_TEXT);
      if (portrait) { add("Telefonda da", C_SUBTEXT); add("'Unut' deyin", C_SUBTEXT); }
      else add("Telefonda da 'Unut' deyin", C_SUBTEXT);
      break;
    }
    case PG_INFO: {
      if (portrait) {
        add("MAC", C_SUBTEXT);
        add(BLEDevice::getAddress().toString().c_str(), C_TEXT2, 1);
      } else {
        snprintf(b, sizeof(b), "MAC  %s", BLEDevice::getAddress().toString().c_str());
        add(b, C_TEXT2);
      }
      if (app.usbPower) snprintf(b, sizeof(b), "USB  %.2f V", app.batteryV);
      else if (app.batteryPct >= 0) snprintf(b, sizeof(b), "Pil %%%d  %.2f V", app.batteryPct, app.batteryV);
      else snprintf(b, sizeof(b), "Pil yok");
      if (!portrait) {                       // yatayda yer az: pil + kayitli ayni satirda
        size_t l = strlen(b);
        snprintf(b + l, sizeof(b) - l, "   Kayıtlı: %d", Bonds::count);
      }
      add(b, C_TEXT2);
      if (portrait) {
        snprintf(b, sizeof(b), "Kayıtlı: %d   RAM %uKB", Bonds::count, (unsigned)(ESP.getFreeHeap() / 1024));
        add(b, C_TEXT2);
      }
      {
        static const char* SRC[] = {"yok", "cihaz", "iPhone"};
        if (Net::hasWeather()) snprintf(b, sizeof(b), "Saat: %s  Hava %d°C", SRC[Net::timeSrc % 3], Wx::tempNow());
        else snprintf(b, sizeof(b), "Saat: %s  Hava: veri yok", SRC[Net::timeSrc % 3]);
        add(b, Net::timeSrc ? C_TEXT2 : C_SUBTEXT, portrait ? 1 : 2);
      }
      if (portrait) {
        add("Programcı:", C_SUBTEXT, 1);
        add(DEVELOPER_TXT, C_ACCENT, 1);
        L[n - 1].uml = umlAt(L[n - 1].txt);
        snprintf(b, sizeof(b), "v%s  BT: %s", FW_VERSION, app.classicOk ? "tam" : "hafif");
      } else {
        snprintf(b, sizeof(b), "Programcı: %s", DEVELOPER_TXT);
        add(b, C_ACCENT);
        L[n - 1].uml = umlAt(L[n - 1].txt);
        snprintf(b, sizeof(b), "v%s  BT: %s  RAM %uKB", FW_VERSION, app.classicOk ? "tam" : "hafif",
                 (unsigned)(ESP.getFreeHeap() / 1024));
      }
      add(b, C_SUBTEXT, 1);
      break;
    }
    case PG_SCAN:
      if (Scan::running) snprintf(b, sizeof(b), "Aranıyor... (%d)", (int)Scan::count);
      else snprintf(b, sizeof(b), "%d cihaz bulundu", (int)Scan::count);
      add(b, Scan::running ? C_ACCENT : C_TEXT2);
      if (!portrait) add("Telefonun BT ayarları açık olsun", C_SUBTEXT, 1);
      break;
    case PG_PAIR: {
      uint32_t left = (Pair::until > millis()) ? (Pair::until - millis()) / 1000 : 0;
      if (Pair::hasTarget) {
        add("Hedef:", C_SUBTEXT);
        add(Pair::target, C_ACCENT);
      } else add("Yeni cihaz bekleniyor", C_ACCENT);
      if (portrait) {
        add("Telefonda", C_TEXT2); add("Bluetooth menüsünden", C_TEXT2, 1);
        snprintf(b, sizeof(b), "'%s'", DEVICE_NAME); add(b, C_TEXT);
        add("seçin", C_TEXT2);
      } else {
        snprintf(b, sizeof(b), "Telefonda '%s' seçin", DEVICE_NAME); add(b, C_TEXT2);
      }
      snprintf(b, sizeof(b), "Kalan %lu sn", (unsigned long)left);
      add(b, C_SUBTEXT);
      break;
    }
    case PG_DEVACT: {
      if (menu.delIdx < 0 || menu.delIdx >= Bonds::count) break;
      char nm[32], a[20];
      Bonds::label(menu.delIdx, nm, sizeof(nm));
      fmtAddr(Bonds::list[menu.delIdx].bd_addr, a, sizeof(a));
      add(nm, C_TEXT);
      add(a, C_SUBTEXT, 1);
      bool pr = Pref::is(Bonds::list[menu.delIdx].bd_addr), on = Bonds::isConnected(menu.delIdx);
      add(pr ? (on ? "Tercih edilen - bağlı" : "Tercih edilen") : (on ? "Bağlı" : "Bağlı değil"),
          pr ? C_MENU : (on ? C_OK : C_SUBTEXT), 1);
      break;
    }
    case PG_IR:
      if (IR::sweeping) {
        snprintf(b, sizeof(b), "Gönderiliyor %d/%d", min(IR::sweepIdx, IR_NPWR), IR_NPWR);
        add(b, C_ACCENT);
        snprintf(b, sizeof(b), "Kalan ~%lu sn%s", (unsigned long)IR::sweepEtaS(), IR::randomOrder ? "  (rastgele)" : "");
        add(b, C_TEXT2, 1);
      } else {
        add(portrait ? "IR ucunu TV'ye" : "Üstteki IR ucunu TV'ye doğrultun", C_TEXT2, portrait ? 2 : 1);
        if (portrait) add("doğrultun", C_TEXT2);
      }
      break;
    case PG_IRB:
      add(portrait ? "TV'ye doğrultun" : "Üstteki IR ucunu TV'ye doğrultun", C_TEXT2, 1);
      break;
    case PG_IRU:
      if (!IRL::used(menu.irSlot)) {
        add("Kod tara: ek parça gerekmez", C_TEXT2, 1);
        add("Tuş öğret: IR alıcı gerekir", C_TEXT2, 1);
      }
      break;
    case PG_IRK:
      add(portrait ? "Alıcı: OUT->G26" : "Alıcı: OUT->G26  VCC->3V3  GND", C_SUBTEXT, 1);
      if (portrait) add("VCC->3V3  GND", C_SUBTEXT, 1);
      break;
    case PG_AUDIO:
      if (portrait) {
        add("Hoparlör/kulaklık", C_WARN);
        add("doğrudan", C_TEXT2); add("kontrol edilemez.", C_TEXT2);
        add("Telefonu hoparlöre", C_TEXT2); add("bağla, sonra", C_TEXT2);
        add("TELEFONU eşle.", C_TEXT);
      } else {
        add("Hoparlör doğrudan kontrol edilemez.", C_WARN);
        add("Telefonu hoparlöre bağlayın,", C_TEXT2);
        add("sonra TELEFONU buradan eşleyin.", C_TEXT2);
      }
      break;
    default: break;
  }
  return n;
}

// ============================================================================
//  AYARLAR — BUYUK IKONLU KAYDIRMALI MENU (carousel)
// ============================================================================
//  Ortada secili ayarin buyuk karti, iki yanda komsu kartlar (kucuk, soluk).
//  Secim degisince kartlar yumusakca kayar. Altta ad, deger ve secenek cubugu.

// Vektor ikonlar (s = olcek, ~40 px @ s=1). bg: ikonun uzerinde durdugu renk.
void drawIcon(uint8_t id, int cx, int cy, float s, uint16_t c, uint16_t bg) {
  TFT_eSPI& g = G();
  auto S = [&](float v) { return (int)(v * s + 0.5f); };
  auto pt = [&](int a, float r, int& x, int& y) {      // TFT acisi: 0 = asagi, saat yonu
    float th = (90.0f + a) * DEG_TO_RAD;
    x = cx + (int)(cosf(th) * r * s); y = cy + (int)(sinf(th) * r * s);
  };
  switch (id) {
    case IT_STEPS:                                      // artan cubuklar
      for (int k = 0; k < 5; k++) {
        int h = S(8 + 6 * k);
        g.fillRoundRect(cx - S(19) + k * S(8), cy + S(14) - h, max(2, S(5)), h, 1, c);
      }
      break;
    case IT_BRIGHT:                                     // gunes
      g.fillCircle(cx, cy, S(8), c);
      for (int a = 0; a < 360; a += 45) {
        int x1, y1, x2, y2; pt(a, 12, x1, y1); pt(a, 17, x2, y2);
        g.drawWideLine(x1, y1, x2, y2, max(2, S(3)), c, bg);
      }
      break;
    case IT_SLEEP:                                      // ay + z
      g.fillCircle(cx - S(3), cy + S(2), S(13), c);
      g.fillCircle(cx + S(5), cy - S(5), S(11), bg);
      drawText("z", cx + S(12), cy - S(12), MC_DATUM, 2, c, bg);
      break;
    case IT_ORIENT: {                                   // telefon + donme oku
      int w = S(14), h = S(24);
      g.fillRoundRect(cx - w / 2, cy - h / 2, w, h, S(3), c);
      g.fillRoundRect(cx - w / 2 + 2, cy - h / 2 + 3, w - 4, h - 7, 1, bg);
      g.drawArc(cx, cy, S(19), S(17), 200, 330, c, bg, true);
      int x, y; pt(330, 18, x, y);
      g.fillTriangle(x - S(4), y - S(3), x + S(3), y - S(4), x, y + S(4), c);
      break;
    }
    case IT_SAVER:                                      // robot gozleri
      g.fillRoundRect(cx - S(17), cy - S(9), S(14), S(18), S(4), c);
      g.fillRoundRect(cx + S(3),  cy - S(9), S(14), S(18), S(4), c);
      break;
    case IT_EYE: {                                      // renk paleti
      static const uint8_t P[][3] = {{80,200,255},{90,255,160},{255,150,40},{255,110,200},{235,240,255}};
      for (int k = 0; k < 5; k++) {
        int x, y; pt(k * 72 + 180, 13, x, y);
        uint16_t pc = tft.color565(P[k][0], P[k][1], P[k][2]);
        g.fillCircle(x, y, max(2, S(4)), c == C_ACCENT || s > 0.9f ? pc : blend565(pc, bg, 0.6f));
      }
      g.fillCircle(cx, cy, max(2, S(5)), c);
      break;
    }
    case IT_DBL:                                        // >>|  sonraki sarki
      g.fillTriangle(cx - S(16), cy - S(10), cx - S(16), cy + S(10), cx - S(4), cy, c);
      g.fillTriangle(cx - S(4),  cy - S(10), cx - S(4),  cy + S(10), cx + S(8), cy, c);
      g.fillRect(cx + S(10), cy - S(10), max(2, S(4)), S(20), c);
      break;
    case IT_BOTH:                                       // oynat / duraklat
      g.fillTriangle(cx - S(16), cy - S(10), cx - S(16), cy + S(10), cx - S(2), cy, c);
      g.fillRect(cx + S(4),  cy - S(10), max(2, S(4)), S(20), c);
      g.fillRect(cx + S(12), cy - S(10), max(2, S(4)), S(20), c);
      break;
    case IT_WIFI:
    case IT_WIFI_SETUP: {                               // wi-fi dalgalari
      int by = cy + S(10);
      g.drawArc(cx, by, S(22), S(19), 135, 225, c, bg, true);
      g.drawArc(cx, by, S(15), S(12), 135, 225, c, bg, true);
      g.fillCircle(cx, by - S(2), max(2, S(3)), c);
      if (id == IT_WIFI_SETUP) {                        // + isareti
        g.fillCircle(cx + S(14), cy + S(8), S(7), c);
        g.fillRect(cx + S(14) - S(4), cy + S(8) - 1, S(8), 2, bg);
        g.fillRect(cx + S(14) - 1, cy + S(8) - S(4), 2, S(8), bg);
      }
      break;
    }
    case IT_WXALERT: {                                  // bulut + yagmur damlalari
      g.fillCircle(cx - S(7), cy - S(2), S(7), c);
      g.fillCircle(cx + S(2), cy - S(7), S(9), c);
      g.fillCircle(cx + S(10), cy - S(1), S(6), c);
      g.fillRect(cx - S(7), cy - S(2), S(17), S(7), c);
      for (int k = -1; k <= 1; k++) g.drawWideLine(cx + k * S(7) + S(2), cy + S(9), cx + k * S(7) - S(1), cy + S(15), max(2, S(3)), c, bg);
      break;
    }
    case IT_WXINT: {                                    // saat
      g.fillCircle(cx, cy, S(15), c);
      g.fillCircle(cx, cy, S(15) - max(2, S(3)), bg);
      g.drawWideLine(cx, cy, cx, cy - S(9), 2, c, bg);
      g.drawWideLine(cx, cy, cx + S(7), cy + S(3), 2, c, bg);
      break;
    }
    case IT_WIFI_SYNC: {                                // yenile oklari
      g.drawArc(cx, cy, S(15), S(12), 40, 160, c, bg, true);
      g.drawArc(cx, cy, S(15), S(12), 220, 340, c, bg, true);
      int x, y;
      pt(160, 13.5f, x, y); g.fillTriangle(x - S(5), y, x + S(5), y, x, y + S(6), c);
      pt(340, 13.5f, x, y); g.fillTriangle(x - S(5), y, x + S(5), y, x, y - S(6), c);
      break;
    }
    case IT_CLEAR_ALL: {                                // cop kutusu
      g.fillRect(cx - S(5), cy - S(17), S(10), S(4), c);
      g.fillRect(cx - S(14), cy - S(13), S(28), max(2, S(3)), c);
      g.fillRoundRect(cx - S(11), cy - S(8), S(22), S(24), S(3), c);
      for (int k = -1; k <= 1; k++) g.fillRect(cx + k * S(6) - 1, cy - S(4), 2, S(16), bg);
      break;
    }
    case IT_PAIRCONF: {                                 // kalkan + tik
      g.fillRoundRect(cx - S(13), cy - S(16), S(26), S(18), S(3), c);
      g.fillTriangle(cx - S(13), cy + S(1), cx + S(13), cy + S(1), cx, cy + S(18), c);
      g.drawWideLine(cx - S(7), cy - S(1), cx - S(2), cy + S(5), max(2, S(3)), bg, c);
      g.drawWideLine(cx - S(2), cy + S(5), cx + S(8), cy - S(7), max(2, S(3)), bg, c);
      break;
    }
    case IT_CLASSIC: {                                  // buyutec
      int r = S(11);
      g.fillCircle(cx - S(4), cy - S(4), r, c);
      g.fillCircle(cx - S(4), cy - S(4), r - max(2, S(3)), bg);
      g.drawWideLine(cx + S(4), cy + S(4), cx + S(15), cy + S(15), max(3, S(5)), c, bg);
      break;
    }
    case IT_AUTOROT: {                                  // yatik telefon + iki donme oku
      int w = S(24), h = S(14);
      g.fillRoundRect(cx - w / 2, cy - h / 2, w, h, S(3), c);
      g.fillRoundRect(cx - w / 2 + 3, cy - h / 2 + 2, w - 7, h - 4, 1, bg);
      g.drawArc(cx, cy, S(20), S(18), 110, 160, c, bg, true);
      g.drawArc(cx, cy, S(20), S(18), 290, 340, c, bg, true);
      int x, y;
      pt(160, 19, x, y); g.fillTriangle(x - S(4), y - S(1), x + S(3), y - S(4), x + S(1), y + S(4), c);
      pt(340, 19, x, y); g.fillTriangle(x + S(4), y + S(1), x - S(3), y + S(4), x - S(1), y - S(4), c);
      break;
    }
    case IT_ROTCAL: {                                   // nisangah
      g.fillCircle(cx, cy, S(15), c);
      g.fillCircle(cx, cy, S(15) - max(2, S(3)), bg);
      g.fillCircle(cx, cy, max(2, S(4)), c);
      g.fillRect(cx - 1, cy - S(20), 2, S(9), c);
      g.fillRect(cx - 1, cy + S(11), 2, S(9), c);
      g.fillRect(cx - S(20), cy - 1, S(9), 2, c);
      g.fillRect(cx + S(11), cy - 1, S(9), 2, c);
      break;
    }
    case IT_MIC: {                                      // mikrofon
      g.fillRoundRect(cx - S(6), cy - S(17), S(12), S(22), S(6), c);
      g.drawArc(cx, cy - S(5), S(12), S(10), 270, 90, c, bg, true);
      g.fillRect(cx - 1, cy + S(7), 2, S(7), c);
      g.fillRect(cx - S(7), cy + S(14), S(14), 2, c);
      break;
    }
    case IT_SND: {                                      // hoparlor + dalgalar
      g.fillRect(cx - S(16), cy - S(5), S(7), S(10), c);
      g.fillTriangle(cx - S(10), cy - S(5), cx - S(1), cy - S(13), cx - S(1), cy + S(13), c);
      g.fillTriangle(cx - S(10), cy + S(5), cx - S(1), cy + S(13), cx - S(10), cy - S(5), c);
      g.drawArc(cx, cy, S(10), S(8), 210, 330, c, bg, true);
      g.drawArc(cx, cy, S(17), S(15), 215, 325, c, bg, true);
      break;
    }
    case IT_LOGO: {                                     // beyaz kart + siyah MGM
      g.fillRoundRect(cx - S(20), cy - S(13), S(40), S(26), S(5), c);
      drawText("MGM", cx, cy + 1, MC_DATUM, s > 0.9f ? 2 : 1, bg, c);
      break;
    }
    case IT_SSTYLE: {                                   // mini maskot
      g.fillEllipse(cx, cy + S(2), S(17), S(14), c);
      g.fillEllipse(cx, cy + S(2), S(14), S(11), bg);
      g.fillEllipse(cx - S(6), cy, max(2, S(2)), max(2, S(3.5f)), c);
      g.fillEllipse(cx + S(6), cy, max(2, S(2)), max(2, S(3.5f)), c);
      g.drawWideLine(cx + S(5), cy - S(11), cx + S(9), cy - S(19), 2, c, bg);
      g.fillCircle(cx + S(9), cy - S(19), max(2, S(3)), c);
      break;
    }
    case IT_SNDEYE: {                                   // goz + nota
      g.fillRoundRect(cx - S(17), cy - S(8), S(13), S(17), S(4), c);
      g.fillRoundRect(cx - S(1),  cy - S(8), S(13), S(17), S(4), c);
      g.fillEllipse(cx + S(15), cy - S(10), max(2, S(3)), max(2, S(2)), c);
      g.fillRect(cx + S(17), cy - S(20), 2, S(10), c);
      break;
    }
    case IT_BACK:                                       // geri oku
      g.fillTriangle(cx - S(16), cy, cx - S(4), cy - S(11), cx - S(4), cy + S(11), c);
      g.fillRect(cx - S(5), cy - S(4), S(20), max(2, S(8)), c);
      break;
    default:
      g.fillCircle(cx, cy, S(10), c);
      break;
  }
}

void renderCarousel() {
  TFT_eSPI& g = G();
  int n = menuCount;
  if (n <= 0) return;

  // Kayma animasyonu (dairesel)
  float d = menu.sel - menu.carPos;
  if (d >  n / 2.0f) d -= n;
  if (d < -n / 2.0f) d += n;
  menu.carPos += d * 0.30f;
  if (fabsf(d) < 0.01f) menu.carPos = menu.sel;
  if (menu.carPos < 0)  menu.carPos += n;
  if (menu.carPos >= n) menu.carPos -= n;

  const int cyC     = portrait ? 82 : 57;
  const int spacing = portrait ? 104 : 90;
  const int W0      = portrait ? 90 : 74;
  const int H0      = portrait ? 86 : 60;

  // Sira: once uzaktakiler, en son ortadaki (ustte kalsin)
  for (int pass = 0; pass < 2; pass++) {
    for (int i = 0; i < n; i++) {
      float di = i - menu.carPos;
      if (di >  n / 2.0f) di -= n;
      if (di < -n / 2.0f) di += n;
      float ad = fabsf(di);
      if (ad > 1.8f) continue;
      if ((pass == 0) != (ad >= 0.5f)) continue;
      float dim = min(1.0f, ad);
      float t = 1.0f - dim * 0.32f;
      int w = (int)(W0 * t), h = (int)(H0 * t);
      int x = SW / 2 + (int)(di * spacing);
      uint16_t fill = blend565(C_PANEL, C_BG, dim * 0.55f);
      g.fillRoundRect(x - w / 2, cyC - h / 2, w, h, 12, fill);
      if (ad < 0.5f) {
        uint16_t oc = blend565(C_BG, C_ACCENT, 1.0f - ad * 2);
        g.drawRoundRect(x - w / 2, cyC - h / 2, w, h, 12, oc);
        g.drawRoundRect(x - w / 2 + 1, cyC - h / 2 + 1, w - 2, h - 2, 11, oc);
      }
      MenuItem& it = menuItems[i];
      uint16_t base = (it.color == C_BAD) ? C_BAD : (it.id == IT_BACK ? C_TEXT2 : C_ACCENT);
      drawIcon(it.id, x, cyC, t * (portrait ? 1.15f : 1.0f), blend565(base, fill, dim * 0.6f), fill);
    }
  }

  // Yan oklar (kaydirilabilir oldugunu gosterir)
  uint16_t ac = C_SUBTEXT;
  g.fillTriangle(3, cyC, 9, cyC - 6, 9, cyC + 6, ac);
  g.fillTriangle(SW - 3, cyC, SW - 9, cyC - 6, SW - 9, cyC + 6, ac);

  // Sira numarasi
  char idx[8];
  snprintf(idx, sizeof(idx), "%d/%d", menu.sel + 1, n);
  drawText(idx, SW - 5, 27, TR_DATUM, 1, C_SUBTEXT, C_BG);

  // Ad, deger ve secenek cubugu (secili oge)
  MenuItem& it = menuItems[menu.sel];
  int yLabel = portrait ? 142 : 97;
  int yValue = portrait ? 162 : 113;
  int ySlide = portrait ? 180 : 126;
  drawText(it.label, SW / 2, yLabel, MC_DATUM, 2, it.color == C_BAD ? C_BAD : C_TEXT, C_BG, SW - 8);
  if (it.value[0]) {
    uint16_t vc = !strcmp(it.value, "Açık") ? C_OK : !strcmp(it.value, "Kapalı") ? C_SUBTEXT : C_ACCENT;
    drawText(it.value, SW / 2, yValue, MC_DATUM, 2, vc, C_BG, SW - 8);
  } else {
    drawText(it.id == IT_BACK ? "menüye dön" : "A ile çalıştır", SW / 2, yValue, MC_DATUM, 1, C_SUBTEXT, C_BG);
  }
  if (it.optCnt > 1) {
    int cnt = it.optCnt;
    int segW = min(18, (SW - 40) / cnt - 3);
    int total = cnt * segW + (cnt - 1) * 3;
    int x0 = SW / 2 - total / 2;
    for (int k = 0; k < cnt; k++) {
      uint16_t sc = (k == it.optIdx) ? C_ACCENT : (k < it.optIdx ? blend565(C_ACCENT, C_BG, 0.6f) : C_TRACK);
      g.fillRoundRect(x0 + k * (segW + 3), ySlide - 2, segW, 5, 2, sc);
    }
  }

  if (portrait) {
    drawText("B: sonraki  (uzun: önceki)", SW / 2, SH - 11, BC_DATUM, 1, C_SUBTEXT, C_BG);
    drawText("A: değiştir (uzun: geri)", SW / 2, SH, BC_DATUM, 1, C_SUBTEXT, C_BG);
  }
}

void renderMenu() {
  TFT_eSPI& g = G();
  const char* title = "Menü";
  switch (menu.page) {
    case PG_DEVICES: title = "Kayıtlı cihazlar"; break;
    case PG_SETTINGS: title = "Ayarlar";         break;
    case PG_DEVACT:  title = "Cihaz";            break;
    case PG_CONFIRM: title = "Silinsin mi?";     break;
    case PG_INFO:    title = "Cihaz bilgisi";    break;
    case PG_SCAN:    title = "Cihaz ara";        break;
    case PG_PAIR:    title = "Eşleşme modu";     break;
    case PG_AUDIO:   title = "Uyarı";            break;
    case PG_IR:      title = "TV kumanda";       break;
    case PG_IRB:     title = IR::IRBRANDS[menu.irBrand].name; break;
    case PG_IRBR:    title = "Markalar"; break;
    case PG_IRU:     title = IRL::name(menu.irSlot); break;
    case PG_IRK:     title = "Tuş öğret"; break;
    default: break;
  }
  drawHeader(title);
  g.drawFastHLine(0, 23, SW, C_TRACK);
  if (menu.page == PG_SETTINGS) { renderCarousel(); return; }

  // Govde
  BodyLine L[10];
  int nl = buildBody(L);
  int top = 27;
  for (int i = 0; i < nl; i++) {
    int h = (L[i].font == 1) ? 11 : 16;
    drawText(L[i].txt, 8, top, TL_DATUM, L[i].font, L[i].col, C_BG, SW - 14);
    if (L[i].uml > 0 && L[i].uml < (int)strlen(L[i].txt)) {
      // Fontta Turkce karakter yok: U harfinin ustune iki nokta ciz -> Ü
      char pre[40];
      memcpy(pre, L[i].txt, L[i].uml); pre[L[i].uml] = 0;
      int ux = 8 + g.textWidth(pre, L[i].font);
      int uw = g.textWidth("U", L[i].font);
      if (L[i].font == 1) {
        g.drawPixel(ux + 1, top - 2, L[i].col);
        g.drawPixel(ux + 3, top - 2, L[i].col);
      } else {
        int d = max(2, uw / 4);
        g.fillRect(ux + uw / 2 - d - 1, top, 2, 2, L[i].col);
        g.fillRect(ux + uw / 2 + d - 1, top, 2, 2, L[i].col);
      }
    }
    top += h;
  }
  if (nl) top += 3;

  // Eslesme modunda donen gosterge
  if (menu.page == PG_PAIR) {
    float a = (millis() % 1200) / 1200.0f * 360.0f;
    int cx = SW - 48, cy = 12;
    g.drawSmoothArc(cx, cy, 7, 5, (uint32_t)a % 360, ((uint32_t)a + 90) % 360, C_ACCENT, C_BG, true);
  }

  // Liste
  const int rowH  = portrait ? 30 : 20;
  int listBottom  = SH - (portrait ? 22 : 11);
  int visible     = max(1, (listBottom - top) / rowH);
  if (menu.sel < menu.scroll) menu.scroll = menu.sel;
  if (menu.sel >= menu.scroll + visible) menu.scroll = menu.sel - visible + 1;
  if (menu.scroll > max(0, menuCount - visible)) menu.scroll = max(0, menuCount - visible);

  float targetY = top + (menu.sel - menu.scroll) * rowH;
  if (menu.hlY < top - rowH || menu.hlY > listBottom) menu.hlY = targetY;
  menu.hlY += (targetY - menu.hlY) * 0.45f;
  if (fabsf(targetY - menu.hlY) < 0.5f) menu.hlY = targetY;

  bool scrollbar = menuCount > visible;
  int rightEdge = scrollbar ? SW - 8 : SW - 4;
  g.fillRoundRect(3, (int)menu.hlY, rightEdge - 3, rowH - 1, 6, C_PANEL);
  g.fillRoundRect(3, (int)menu.hlY + 3, 3, rowH - 7, 1, C_ACCENT);

  for (int r = 0; r < visible; r++) {
    int i = menu.scroll + r;
    if (i >= menuCount) break;
    MenuItem& it = menuItems[i];
    int y0 = top + r * rowH;
    bool selRow = (i == menu.sel);
    uint16_t bg = (fabsf(menu.hlY - y0) < rowH / 2) ? C_PANEL : C_BG;
    uint16_t lc = selRow ? (it.color == C_SUBTEXT ? C_TEXT : it.color) : blend565(it.color, C_BG, 0.3f);
    bool on = strcmp(it.value, "bağlı") == 0;
    uint16_t vc = on ? C_OK : (selRow ? C_ACCENT : C_SUBTEXT);
    int barsW = it.bars ? 18 : 0;

    if (portrait) {
      drawText(it.label, 11, y0 + 3, TL_DATUM, 2, lc, bg, rightEdge - 14);
      if (it.value[0]) drawText(it.value, 11, y0 + 19, TL_DATUM, 1, vc, bg, rightEdge - 14 - barsW);
      if (it.bars) drawBars(rightEdge - 20, y0 + 27, it.bars, selRow ? C_ACCENT : C_SUBTEXT, C_TRACK);
    } else {
      int valW = it.value[0] ? textW(it.value, 2) + 8 : 0;
      int yc = y0 + rowH / 2;
      drawText(it.label, 11, yc, ML_DATUM, 2, lc, bg, rightEdge - 16 - valW - barsW);
      if (it.value[0]) drawText(it.value, rightEdge - 5 - barsW, yc, MR_DATUM, 2, vc, bg);
      if (it.bars) drawBars(rightEdge - 19, y0 + rowH - 5, it.bars, selRow ? C_ACCENT : C_SUBTEXT, C_TRACK);
    }
  }

  if (scrollbar) {
    int trackH = visible * rowH;
    int barH = max(8, trackH * visible / menuCount);
    int barY = top + (trackH - barH) * menu.scroll / max(1, menuCount - visible);
    g.fillRoundRect(SW - 5, top, 3, trackH, 1, C_TRACK);
    g.fillRoundRect(SW - 5, barY, 3, barH, 1, C_SUBTEXT);
  }

  if (portrait) {
    drawText("B: sonraki", SW / 2, SH - 11, BC_DATUM, 1, C_SUBTEXT, C_BG);
    drawText("A: seç  uzun: geri", SW / 2, SH, BC_DATUM, 1, C_SUBTEXT, C_BG);
  } else {
    drawText("B sonraki  A seç  uzun: geri", SW / 2, SH, BC_DATUM, 1, C_SUBTEXT, C_BG);
  }
}

// ============================================================================
//  ESLESME ONAY PENCERESI
// ============================================================================
void renderPairModal() {
  TFT_eSPI& g = G();
  bool confirm = (Pair::modal == Pair::M_CONFIRM);
  uint32_t el = millis() - Pair::mAt;
  float left = 1.0f - min(1.0f, el / (float)CONFIRM_TIMEOUT_MS);

  g.fillRect(0, 0, SW, SH, C_BG);
  g.drawRoundRect(0, 0, SW, SH, 8, confirm ? C_ACCENT : C_WARN);

  const char* name = Pair::mName[0] ? Pair::mName
                   : NameReader::busy() ? "İsim okunuyor..."
                   : (Pair::hasTarget ? Pair::target : "Bilinmeyen cihaz");
  bool match = Pair::targetMatch();

  char code[8];
  snprintf(code, sizeof(code), "%06lu", (unsigned long)Pair::mKey);

  if (portrait) {
    drawText(confirm ? "Eşleşme isteği" : "Telefona girin", SW / 2, 14, MC_DATUM, 2, confirm ? C_ACCENT : C_WARN, C_BG);
    drawText(name, SW / 2, 38, MC_DATUM, 2, C_TEXT, C_BG, SW - 10);
    if (match) drawText("Seçilen cihaz", SW / 2, 56, MC_DATUM, 1, C_OK, C_BG);
    char a[4], b[4];
    memcpy(a, code, 3); a[3] = 0; memcpy(b, code + 3, 3); b[3] = 0;
    drawText(a, SW / 2, 96, MC_DATUM, 7, C_TEXT, C_BG);
    drawText(b, SW / 2, 150, MC_DATUM, 7, C_TEXT, C_BG);
    int bw = (int)((SW - 20) * left);
    g.fillRoundRect(10, 184, SW - 20, 4, 2, C_TRACK);
    if (bw > 0) g.fillRoundRect(10, 184, bw, 4, 2, confirm ? C_ACCENT : C_WARN);
    if (confirm) {
      drawPill(SW / 2, 206, "A  ONAYLA", C_OK, C_BG);
      drawPill(SW / 2, 228, "B  REDDET", C_BAD, C_TEXT);
    } else {
      drawText("Kod telefonda", SW / 2, 206, MC_DATUM, 2, C_TEXT2, C_BG);
      drawPill(SW / 2, 228, "B  İPTAL", C_BAD, C_TEXT);
    }
  } else {
    drawText(confirm ? "Eşleşme isteği" : "Bu kodu telefona girin", 10, 12, ML_DATUM, 2, confirm ? C_ACCENT : C_WARN, C_BG);
    if (match) drawText("Seçilen cihaz", SW - 10, 12, MR_DATUM, 2, C_OK, C_BG);
    drawText(name, SW / 2, 36, MC_DATUM, 4, C_TEXT, C_BG, SW - 16);
    drawText(code, SW / 2, 80, MC_DATUM, 7, C_TEXT, C_BG);
    int bw = (int)((SW - 20) * left);
    g.fillRoundRect(10, 107, SW - 20, 3, 1, C_TRACK);
    if (bw > 0) g.fillRoundRect(10, 107, bw, 3, 1, confirm ? C_ACCENT : C_WARN);
    if (confirm) {
      drawPill(62, 123, "A ONAYLA", C_OK, C_BG);
      drawPill(SW - 62, 123, "B REDDET", C_BAD, C_TEXT);
    } else {
      drawPill(SW / 2, 123, "B İPTAL", C_BAD, C_TEXT);
    }
  }
}

// ============================================================================
//  KAMERA EKRANI
// ============================================================================
void drawCameraIcon(int cx, int cy, uint16_t c) {
  TFT_eSPI& g = G();
  g.fillRoundRect(cx - 12, cy - 25, 24, 10, 3, c);
  g.fillRoundRect(cx - 30, cy - 18, 60, 38, 7, c);
  g.fillCircle(cx, cy + 1, 13, C_BG);
  g.fillCircle(cx, cy + 1, 9, c);
  g.fillCircle(cx, cy + 1, 5, C_BG);
  g.fillCircle(cx + 21, cy - 11, 2, C_BG);
}

void renderCamera() {
  TFT_eSPI& g = G();
  uint32_t now = millis();
  drawHeader("Kamera");
  int cy = portrait ? 100 : 66;
  if (Cam::timerEnd) {
    uint32_t left = Cam::timerEnd - now;
    char b[4];
    snprintf(b, sizeof(b), "%lu", (unsigned long)((left + 999) / 1000));
    float frac = (left % 1000) / 1000.0f;
    uint32_t e = (uint32_t)(360 * frac);
    g.drawSmoothArc(SW / 2, cy, 40, 35, 0, 360, C_TRACK, C_BG, false);
    if (e > 4) g.drawSmoothArc(SW / 2, cy, 40, 35, 0, e, C_WARN, C_BG, true);
    drawText(b, SW / 2, cy, MC_DATUM, 7, C_TEXT, C_BG);
  } else {
    drawCameraIcon(SW / 2, cy, C_TEXT);
  }
  char s[20];
  snprintf(s, sizeof(s), "Foto: %d", Cam::shots);
  drawText(s, SW / 2, portrait ? 158 : 116, MC_DATUM, 2, C_SUBTEXT, C_BG);
  if (portrait) {
    drawText("A: çek", SW / 2, SH - 34, BC_DATUM, 1, C_TEXT2, C_BG);
    drawText("B: 3 sn sayaç", SW / 2, SH - 22, BC_DATUM, 1, C_TEXT2, C_BG);
    drawText("A uzun: çıkış", SW / 2, SH - 10, BC_DATUM, 1, C_SUBTEXT, C_BG);
  } else {
    drawText("A çek  B 3 sn sayaç  A uzun çıkış", SW / 2, SH, BC_DATUM, 1, C_TEXT2, C_BG);
  }
  // Flas efekti
  uint32_t fl = now - Cam::flashAt;
  if (Cam::flashAt && fl < 220) g.fillRect(0, 0, SW, SH, blend565(C_TEXT, C_BG, fl / 220.0f));
}

// ============================================================================
//  DONDURME AYARI EKRANI (2 adim)
// ============================================================================
void renderCalib() {
  TFT_eSPI& g = G();
  bool st1 = AutoRot::calStep == 1;
  char h[24];
  snprintf(h, sizeof(h), "Döndürme ayarı %d/2", AutoRot::calStep);
  drawText(h, SW / 2, 10, MC_DATUM, 2, C_ACCENT, C_BG);
  // Yukari bakan buyuk ok: ekranin ustu burasi
  float bob = sinf(millis() / 260.0f) * 3.0f;
  int ay = (portrait ? 48 : 36) + (int)bob;
  g.fillTriangle(SW / 2, ay - 14, SW / 2 - 16, ay + 6, SW / 2 + 16, ay + 6, C_OK);
  g.fillRect(SW / 2 - 6, ay + 6, 12, portrait ? 22 : 12, C_OK);
  drawText("ÜST", SW / 2, ay + (portrait ? 38 : 26), MC_DATUM, 2, C_OK, C_BG);
  int y = portrait ? 118 : 86;
  if (portrait) {
    drawText(st1 ? "Cihazı DİK tutun" : "Cihazı YAN tutun", SW / 2, y, MC_DATUM, 2, C_TEXT, C_BG);
    drawText("yazılar düz", SW / 2, y + 22, MC_DATUM, 2, C_TEXT2, C_BG);
    drawText("okunsun", SW / 2, y + 40, MC_DATUM, 2, C_TEXT2, C_BG);
    drawText("A: kaydet", SW / 2, SH - 34, MC_DATUM, 2, C_ACCENT, C_BG);
    drawText("B: iptal", SW / 2, SH - 14, MC_DATUM, 2, C_SUBTEXT, C_BG);
  } else {
    drawText(st1 ? "Cihazı DİK tutun, yazılar düz okunsun" : "Cihazı YAN tutun, yazılar düz okunsun",
             SW / 2, y, MC_DATUM, 2, C_TEXT, C_BG, SW - 6);
    drawText("A: kaydet     B: iptal", SW / 2, y + 26, MC_DATUM, 2, C_ACCENT, C_BG);
  }
}

// ============================================================================
//  IR KOD TARAMA / OGRENME EKRANI
// ============================================================================
void renderIrMode() {
  TFT_eSPI& g = G();
  bool scan = IRL::mode == IRL::M_SCAN;
  drawText(scan ? "Kod tarama" : "Tuş öğrenme", SW / 2, 10, MC_DATUM, 2, C_ACCENT, C_BG);
  char b[40];
  int y = portrait ? 44 : 34;
  if (scan) {
    snprintf(b, sizeof(b), "Aday %d / %d", IRL::scanIdx + 1, IR::NCAND);
    drawText(b, SW / 2, y, MC_DATUM, 2, C_SUBTEXT, C_BG);
    drawText(IR::candName(IRL::scanIdx), SW / 2, y + 24, MC_DATUM, portrait ? 2 : 4, C_TEXT, C_BG, SW - 6);
    drawText("TV kapandı/açıldı mı?", SW / 2, y + (portrait ? 56 : 52), MC_DATUM, 2, C_TEXT2, C_BG, SW - 6);
    if (portrait) {
      drawText("A: evet, kaydet", SW / 2, SH - 34, MC_DATUM, 2, C_OK, C_BG);
      drawText("B: sonraki kod", SW / 2, SH - 14, MC_DATUM, 2, C_ACCENT, C_BG);
    } else {
      drawText("A: evet, kaydet    B: sonraki", SW / 2, SH - 10, MC_DATUM, 2, C_ACCENT, C_BG);
    }
    return;
  }
  // Ogrenme: dalga animasyonu + geri sayim
  uint32_t el = millis() - IRL::modeAt;
  int left = max(0, 15 - (int)(el / 1000));
  int cx = SW / 2, cy = portrait ? 92 : 62;
  for (int i = 0; i < 3; i++) {
    float p = ((millis() + i * 400) % 1200) / 1200.0f;
    int r = 8 + (int)(p * 26);
    g.drawCircle(cx, cy, r, blend565(C_ACCENT, C_BG, p));
  }
  g.fillCircle(cx, cy, 6, C_ACCENT);
  snprintf(b, sizeof(b), "'%s' tuşuna basın", IRL::KEYN[IRL::key]);
  drawText(b, SW / 2, portrait ? 30 : 26, MC_DATUM, 2, C_TEXT, C_BG, SW - 6);
  snprintf(b, sizeof(b), "%d sn", left);
  if (portrait) {
    drawText("Kumandayı alıcıya", SW / 2, 140, MC_DATUM, 2, C_TEXT2, C_BG);
    drawText("doğrultun", SW / 2, 158, MC_DATUM, 2, C_TEXT2, C_BG);
    drawText(b, SW / 2, 184, MC_DATUM, 2, C_SUBTEXT, C_BG);
    drawText("B: iptal", SW / 2, SH - 14, MC_DATUM, 2, C_SUBTEXT, C_BG);
  } else {
    drawText("Kumandayı alıcıya doğrultun", SW / 2, 104, MC_DATUM, 2, C_TEXT2, C_BG);
    snprintf(b + strlen(b), sizeof(b) - strlen(b), "   B: iptal");
    drawText(b, SW / 2, SH - 10, MC_DATUM, 2, C_SUBTEXT, C_BG);
  }
}

// ============================================================================
//  SARJ ANIMASYONU
// ============================================================================
void renderCharge() {
  TFT_eSPI& g = G();
  float t = (millis() % 1400) / 1400.0f;
  int bw = portrait ? 60 : 110, bh = portrait ? 110 : 54;
  int x = SW / 2 - bw / 2, y = (portrait ? 90 : 52) - bh / 2;
  for (int i = 0; i < 3; i++) g.drawRoundRect(x + i, y + i, bw - 2 * i, bh - 2 * i, 10 - i, C_TEXT);
  if (portrait) g.fillRoundRect(SW / 2 - 12, y - 8, 24, 8, 2, C_TEXT);
  else          g.fillRoundRect(x + bw, y + bh / 2 - 12, 8, 24, 2, C_TEXT);
  int in = 6;
  if (portrait) {
    int fh = (int)((bh - 2 * in) * t);
    g.fillRoundRect(x + in, y + bh - in - fh, bw - 2 * in, fh, 5, C_OK);
  } else {
    int fw = (int)((bw - 2 * in) * t);
    g.fillRoundRect(x + in, y + in, fw, bh - 2 * in, 5, C_OK);
  }
  int cx = SW / 2, cy = y + bh / 2;
  g.fillTriangle(cx + 6, cy - 20, cx - 10, cy + 3, cx + 1, cy + 3, C_TEXT);
  g.fillTriangle(cx - 1, cy - 3, cx + 10, cy - 3, cx - 6, cy + 20, C_TEXT);
  char v[16];
  snprintf(v, sizeof(v), "%.2f V", app.batteryV);
  drawText("Şarj oluyor", SW / 2, portrait ? 172 : 100, MC_DATUM, 4, C_OK, C_BG);
  drawText(v, SW / 2, portrait ? 198 : 124, MC_DATUM, 2, C_SUBTEXT, C_BG);
}

// ============================================================================
//  EKRAN KORUYUCU — DUYGUSAL ROBOT GOZLERI
// ============================================================================
//  Iki yuvarlak koseli goz; saga-sola bakar, kirpar, ruh hali degistirir.
//  Ruh halleri goz kapaklariyla cizilir (arka plan renginde ucgen/elips).
//  Cihazin durumuna tepki verir: baglaninca mutlu, kopunca uzgun, sessizde uykulu.
// <<EYEGFX  ---- Goz cizimi (saf cizim; tools/preview bu bolumu aynen kullanir) ----
//  Halka goz: acik renk kalin cerceve, koyu ic, altta renkli iris havuzu, parlak nokta.
//  Kirpinca yatay cizgiye, mutluyken yaya doner. Sahne nesneleri (fincan, kitap,
//  semsiye, ay, kar, gunes gozlugu, nota) gozlerin onunde/ustunde cizilir.
namespace EyeGfx {
  enum Prop : uint8_t { PR_NONE, PR_COFFEE, PR_BOOK, PR_UMBRELLA, PR_MOON, PR_SNOW, PR_SUN, PR_NOTE };
  struct Frame {
    uint16_t col = 0;
    float lx = 0, ly = 0, lidOut = 0, lidIn = 0, lidFlat = 0, happy = 0, size = 1, open = 1;
    float bounce = 0, peek = 0, shiver = 0;
    bool love = false, asleep = false, info = true, zzz = false, notes = false;
    uint8_t prop = PR_NONE; uint32_t propT = 0, propLeft = 0;
    uint8_t burst = 0; float burstP = 0; uint32_t seed = 1;          // burst: 1 toplan, 2 dagil
    uint32_t t = 0;
  };
  struct Geo { int cxL, cxR, cy, w, h; };
  struct RingP { float lx, ly, lidOut, lidIn, lidFlat, happy, open; };

  static inline float ease(float k) { k = constrain(k, 0.0f, 1.0f); return k * k * (3 - 2 * k); }
  static inline uint32_t hsh(uint32_t x) { x ^= x >> 16; x *= 0x7feb352dU; x ^= x >> 15; x *= 0x846ca68bU; x ^= x >> 16; return x; }
  static inline float sc() { return portrait ? 0.85f : 1.0f; }

  // Temel yerlesim (dikey / yatay)
  static void base(bool info, int& ew, int& eh, int& gap, int& mx, int& my, int& cy) {
    if (portrait) { ew = 42; eh = 60; gap = 16; mx = 10; my = 16; cy = info ? 126 : 112; }
    else          { ew = 48; eh = 70; gap = 30; mx = 24; my = 8;  cy = 66; }
  }
  Geo geo(const Frame& f) {
    int ew, eh, gap, mx, my, cy; base(f.info, ew, eh, gap, mx, my, cy);
    Geo g; g.w = (int)(ew * f.size); g.h = (int)(eh * f.size);
    int ox = (int)(f.lx * mx + f.shiver), oy = (int)(f.ly * my + f.bounce + f.peek);
    g.cxL = SW / 2 - gap / 2 - g.w / 2 + ox; g.cxR = SW / 2 + gap / 2 + g.w / 2 + ox; g.cy = cy + oy;
    return g;
  }

  // ---- Halka goz --------------------------------------------------------------
  static inline int ellHalf(int rx, int ry, int dy) {
    if (ry <= 0 || dy <= -ry || dy >= ry) return 0;
    float f = dy / (float)ry; return (int)(rx * sqrtf(max(0.0f, 1 - f * f)));
  }
  static void ringArc(int cx, int cy, int rx, int ry, float th, uint16_t c, bool up) {
    TFT_eSPI& g = G();
    const int N = 14; float px = 0, py = 0;
    for (int i = 0; i <= N; i++) {
      float a = PI * i / N, x = cx - cosf(a) * rx, y = cy + (up ? -1 : 1) * sinf(a) * ry;
      if (i) g.drawWideLine(px, py, x, y, th, c, C_BG);
      px = x; py = y;
    }
  }
  void ringEye(int cx, int cy, int w, int hFull, bool left, uint16_t col, const RingP& p) {
    TFT_eSPI& g = G();
    uint16_t ring = blend565(col, C_TEXT, 0.82f);
    float th = max(3.0f, w / 8.5f);
    int rx = w / 2, h = (int)(hFull * p.open);
    if (p.happy > 0.4f) {                                            // mutlu: ters U yay
      int ry = (int)(hFull * 0.30f);
      ringArc(cx, cy + ry / 2, rx - 2, ry, th, ring, true);
      return;
    }
    if (h < th * 2.6f) {                                             // kapali: yatay kapsul
      int ww = rx + (int)(rx * 0.12f);
      g.drawWideLine(cx - ww + th / 2, cy, cx + ww - th / 2, cy, th, ring, C_BG);
      return;
    }
    int ry = h / 2;
    int irx = rx - (int)th, iry = ry - (int)th;
    g.fillEllipse(cx, cy, rx, ry, ring);
    uint16_t dark = blend565(C_BG, col, 0.10f);
    int shiftY = (int)(p.ly * iry * 0.18f);
    for (int dy = -iry + 1; dy < iry; dy++) {
      int hw = ellHalf(irx, iry, dy);
      if (hw <= 0) continue;
      float f = (dy - shiftY + iry) / (float)(2 * iry);
      float k = f < 0.45f ? 0 : min(1.0f, (f - 0.45f) / 0.47f);
      k = k * k * (3 - 2 * k);
      uint16_t c = k > 0 ? blend565(dark, col, 0.9f * k) : dark;
      g.drawFastHLine(cx - hw, cy + dy, hw * 2 + 1, c);
    }
    int hx = cx - irx / 3 + (int)(p.lx * irx * 0.35f), hy = cy - iry / 2 + (int)(p.ly * iry * 0.25f);
    int hr = max(2, w / 11);
    g.fillCircle(hx, hy, hr, C_TEXT);
    g.fillCircle(hx + hr + 2, hy + hr + 1, max(1, hr / 2), blend565(C_TEXT, dark, 0.35f));
    float outer = max(p.lidOut, p.lidFlat), inner = max(p.lidIn, p.lidFlat);
    int dOut = (int)(outer * h), dIn = (int)(inner * h);
    int dL = left ? dOut : dIn, dR = left ? dIn : dOut;
    int y0 = cy - ry;
    if (dL > 1 || dR > 1) {
      int x0 = cx - rx - 2, x1 = cx + rx + 2, yt = y0 - 4;
      g.fillTriangle(x0, yt, x1, yt, x1, y0 + dR, C_BG);
      g.fillTriangle(x0, yt, x1, y0 + dR, x0, y0 + dL, C_BG);
      float yl = y0 + dL, yr = y0 + dR; float xa = -1, xb = -1, ya = 0, yb = 0;
      for (int i = 0; i <= 48; i++) {
        float x = cx - rx + 2 * rx * i / 48.0f, y = yl + (yr - yl) * i / 48.0f;
        float u = (x - cx) / rx, v = (y - cy) / ry;
        if (u * u + v * v <= 1.0f) { if (xa < 0) { xa = x; ya = y; } xb = x; yb = y; }
      }
      if (xa >= 0) g.drawWideLine(xa + th * 0.4f, ya + th * 0.35f, xb - th * 0.4f, yb + th * 0.35f, th * 0.9f, ring, C_BG);
    }
  }
  // Kalp goz (sevgi): halka renginde, nabiz gibi atar
  void heartEye(int cx, int cy, int w, int h, uint16_t col, uint32_t t) {
    TFT_eSPI& g = G();
    float p = 1.0f + 0.08f * sinf(t / 120.0f);
    int r = (int)(min(w, h) * 0.24f * p);
    uint16_t c = blend565(col, C_TEXT, 0.25f);
    g.fillCircle(cx - r, cy - r / 2, r + 1, c);
    g.fillCircle(cx + r, cy - r / 2, r + 1, c);
    g.fillTriangle(cx - 2 * r - 1, cy, cx + 2 * r + 1, cy, cx, cy + 2 * r + 2, c);
    g.fillCircle(cx - r - r / 3, cy - r + 1, max(2, r / 3), blend565(c, C_TEXT, 0.7f));
  }

  // ---- Parcacik gecisi (toplanma / dagilma) ----------------------------------
  void drawBurst(const Frame& f, const Geo& g) {
    TFT_eSPI& d = G();
    const int N = 22;
    float k = ease(f.burstP);
    for (int e = 0; e < 2; e++) {
      int cx = e ? g.cxR : g.cxL, rx = g.w / 2, ry = g.h / 2;
      for (int i = 0; i < N; i++) {
        uint32_t h = hsh(f.seed * 131u + e * 977u + i * 7919u);
        float a = 2 * PI * i / N;
        float tx = cx + cosf(a) * rx * 0.85f, ty = g.cy + sinf(a) * ry * 0.85f;
        float an = (h % 628) / 100.0f, dist = 28 + (h >> 10) % 64;
        float sx = tx + cosf(an) * dist, sy = ty + sinf(an) * dist;
        float x, y, fade;
        if (f.burst == 1) {                                          // girdapla toplan
          float q = 1 - k, sw = q * 1.8f, dx = sx - tx, dy = sy - ty, c = cosf(sw), s = sinf(sw);
          x = tx + (dx * c - dy * s) * q; y = ty + (dx * s + dy * c) * q; fade = 0.3f * q;
        } else { x = tx + (sx - tx) * k; y = ty + (sy - ty) * k; fade = k; }
        int r = 1 + (h >> 20) % 3;
        if (f.burst == 2) r = max(1, (int)(r * (1 - k * 0.6f)));
        uint16_t c = ((h >> 24) & 3) == 0 ? blend565(f.col, C_TEXT, 0.7f) : f.col;
        d.fillCircle((int)x, (int)y, r, blend565(c, C_BG, fade));
      }
    }
  }

  // ---- Sahne nesneleri --------------------------------------------------------
  float cupLift(uint32_t t) {                                        // fincan yudumu (0..1)
    t %= 3600;
    if (t < 1800) return 0;
    return ease((t - 1800) / 350.0f) * (1 - ease((t - 2650) / 350.0f));
  }
  void drawNote(int x, int y, uint16_t c) {
    TFT_eSPI& g = G();
    g.fillEllipse(x, y, 4, 3, c);
    g.fillRect(x + 3, y - 12, 2, 12, c);
    g.fillRect(x + 3, y - 12, 7, 2, c);
  }
  void drawZzz(int x, int y, uint16_t col, uint32_t t) {
    t %= 2400;
    for (int i = 0; i < 3; i++) {
      float p = ((t + i * 800) % 2400) / 2400.0f;
      int zx = x + (int)(p * 14) + i * 3, zy = y - (int)(p * 26);
      drawText("z", zx, zy, MC_DATUM, p < 0.5f ? 2 : 1, blend565(col, C_BG, p), C_BG);
    }
  }
  void drawCup(const Frame& f, const Geo& g, int mid, int bot) {
    TFT_eSPI& d = G();
    float s = sc(), lift = cupLift(f.propT);
    int cw = (int)(26 * s), ch = (int)(20 * s), by0 = bot + 6, by = by0 - (int)(lift * 16 * s);
    uint16_t cup = blend565(C_TEXT, f.col, 0.18f), rim = blend565(C_TEXT, f.col, 0.05f);
    d.fillRoundRect(mid - cw / 2 - 7, by0 - 1, cw + 14, 4, 2, blend565(C_TEXT2, C_BG, 0.25f));   // tabak
    d.drawArc(mid + cw / 2, by - ch / 2, (int)(8 * s), (int)(4 * s), 200, 340, cup, C_BG, true);  // kulp
    d.fillRoundRect(mid - cw / 2, by - ch, cw, ch, 6, cup);
    d.fillRect(mid - cw / 2, by - ch + ch / 2 - 2, cw, 4, f.col);                                 // renkli bant
    d.fillRoundRect(mid - cw / 2 - 1, by - ch - 2, cw + 2, 4, 2, rim);
    if (lift < 0.05f)                                                                             // buhar
      for (int j = 0; j < 3; j++)
        for (int k = 0; k < 6; k++) {
          int y = by - ch - 5 - k * 4 - (int)((f.t / 45) % 4);
          int x = mid - 6 + j * 6 + (int)(sinf(f.t / 300.0f + k * 0.9f + j * 2.1f) * 2.5f);
          d.fillCircle(x, y, 1, blend565(C_TEXT2, C_BG, 0.35f + k * 0.12f));
        }
  }
  void drawBook(const Frame& f, const Geo& g, int mid, int bot) {
    TFT_eSPI& d = G();
    float s = sc();
    int bw = (int)(72 * s) / 2, bh = (int)(24 * s), by = bot + 8, top = by - bh;
    uint16_t cover = blend565(f.col, C_BG, 0.35f), page = blend565(C_TEXT, f.col, 0.08f), ink = blend565(page, C_BG, 0.45f);
    d.fillTriangle(mid - bw - 3, top - 1, mid, top + 4, mid, by + 5, cover);                     // kapak
    d.fillTriangle(mid - bw - 3, top - 1, mid - bw - 3, by + 2, mid, by + 5, cover);
    d.fillTriangle(mid + bw + 3, top - 1, mid, top + 4, mid, by + 5, cover);
    d.fillTriangle(mid + bw + 3, top - 1, mid + bw + 3, by + 2, mid, by + 5, cover);
    for (int sd = -1; sd <= 1; sd += 2) {                                                          // sayfalar
      int xo = mid + sd * bw;
      d.fillTriangle(xo, top - 2, mid, top + 2, mid, by + 2, page);
      d.fillTriangle(xo, top - 2, xo, by - 1, mid, by + 2, page);
      for (int l = 0; l < 4; l++) {                                                                 // yazi satirlari
        float q = (l + 1) / 5.0f;
        int y1 = top + (int)(q * bh) - 1;
        d.drawLine(mid + sd * 5, y1 + 2, xo - sd * 5, y1, ink);
      }
    }
    d.drawLine(mid, top + 2, mid, by + 2, cover);
    uint32_t t = f.propT % 4000;                                                                    // sayfa cevirme
    if (t > 3200) {
      float q = ease((t - 3200) / 700.0f);
      int xe = mid + (int)(bw * (1 - 2 * q));
      int lift = (int)(sinf(q * PI) * 6);
      d.fillTriangle(mid, top + 2, xe, top - 2 - lift, xe, by - 1 - lift, page);
      d.fillTriangle(mid, top + 2, xe, by - 1 - lift, mid, by + 2, page);
      d.drawLine(xe, top - 2 - lift, xe, by - 1 - lift, ink);
    }
  }
  void drawRain(const Frame& f, int cx, int cyC, int rx, int ry) {
    TFT_eSPI& d = G();
    uint16_t dc = tft.color565(110, 170, 255);
    for (int i = 0; i < 26; i++) {
      uint32_t h = hsh(i * 2654435761U + 17);
      int x = h % SW, y = (int)((f.t * (0.10f + (h >> 12) % 5 * 0.01f) + (h >> 4)) ) % (SH + 20) - 10;
      if (abs(x - cx) < rx && y > cyC - ry - 6) continue;                                            // semsiye altinda yagmur yok
      d.drawLine(x, y, x - 2, y + 6, blend565(dc, C_BG, (h >> 8) % 3 * 0.2f));
    }
  }
  void drawUmbrella(const Frame& f, const Geo& g, int mid, int top, int bot, bool back) {
    TFT_eSPI& d = G();
    float s = sc();
    int rx = (g.cxR - g.cxL) / 2 + g.w / 2 + 8, ry = (int)(22 * s), cyC = top - 4;
    if (back) {
      drawRain(f, mid, cyC, rx, ry);
      uint16_t a = f.col, b = blend565(f.col, C_TEXT, 0.4f);
      for (int dy = -ry; dy <= 0; dy++) {
        int hw = ellHalf(rx, ry + 1, dy);
        if (hw <= 0) continue;
        for (int j = 0; j < 5; j++) {
          int xa = mid - hw + hw * 2 * j / 5, xb = mid - hw + hw * 2 * (j + 1) / 5;
          d.drawFastHLine(xa, cyC + dy, xb - xa, (j & 1) ? b : a);
        }
      }
      for (int j = 0; j < 5; j++)                                                                    // dalgali alt kenar
        d.fillEllipse(mid - rx + rx * 2 * (2 * j + 1) / 10, cyC + 2, rx / 5, (int)(ry * 0.28f), C_BG);
      d.fillCircle(mid, cyC - ry - 1, 2, b);
    } else {
      uint16_t hc = blend565(C_TEXT2, f.col, 0.2f);
      d.drawWideLine(mid, cyC - 2, mid, bot + 10, 2.5f, hc, C_BG);
      d.drawArc(mid - 4, bot + 10, 5, 3, 270, 90, hc, C_BG, true);
    }
  }
  void drawMoonStars(const Frame& f, const Geo& g, int top) {
    TFT_eSPI& d = G();
    float s = sc();
    int r = (int)(9 * s), x = min(SW - r - 3, g.cxR + g.w / 2 + (int)(10 * s)), y = max(r + 22, top + 2);
    if (portrait) y = max(y, 62);
    d.fillCircle(x, y, r, tft.color565(255, 226, 130));
    d.fillCircle(x + r / 2 + 1, y - r / 3, r, C_BG);
    for (int i = 0; i < 5; i++) {
      uint32_t h = hsh(i * 7331u + 5);
      int sx = 8 + h % (SW - 16), sy = (portrait ? 60 : 20) + (h >> 9) % (portrait ? 60 : 30);
      float tw = 0.5f + 0.5f * sinf(f.t / 400.0f + i * 1.7f);
      int l = 1 + (int)(tw * 2.5f);
      uint16_t c = blend565(C_BG, tft.color565(255, 240, 200), 0.3f + 0.7f * tw);
      d.drawFastHLine(sx - l, sy, 2 * l + 1, c); d.drawFastVLine(sx, sy - l, 2 * l + 1, c);
    }
  }
  void drawSnow(const Frame& f) {
    TFT_eSPI& d = G();
    for (int i = 0; i < 24; i++) {
      uint32_t h = hsh(i * 40503u + 3);
      float sp = 0.012f + (h >> 6) % 10 * 0.002f;
      int x = h % SW + (int)(sinf(f.t / 700.0f + i) * 4);
      int y = (int)(f.t * sp + (h >> 12)) % (SH + 10) - 5;
      uint16_t c = blend565(C_TEXT, C_BG, (h >> 3) % 3 * 0.18f);
      if ((h >> 20) & 1) { d.drawFastHLine(x - 2, y, 5, c); d.drawFastVLine(x, y - 2, 5, c); d.drawPixel(x, y, C_TEXT); }
      else d.fillCircle(x, y, 1, c);
    }
  }
  void drawShades(const Frame& f, const Geo& g) {
    TFT_eSPI& d = G();
    float q = ease(f.propT / 600.0f) * (f.propLeft < 500 ? ease(f.propLeft / 500.0f) : 1);
    int lw = g.w + 10, lh = (int)(g.h * 0.5f), ly = g.cy - (int)(g.h * 0.06f) - (int)((1 - q) * (g.cy + lh));
    uint16_t lens = tft.color565(18, 20, 30), frame = tft.color565(70, 72, 84);
    uint32_t gp = f.t % 2600;
    for (int e = 0; e < 2; e++) {
      int cx = e ? g.cxR : g.cxL, x0 = cx - lw / 2, y0 = ly - lh / 2;
      d.fillRoundRect(x0 - 2, y0 - 2, lw + 4, lh + 4, lh / 3 + 2, frame);
      d.fillRoundRect(x0, y0, lw, lh, lh / 3, lens);
      d.fillRoundRect(x0 + 4, y0 + 3, lw / 3, 3, 1, blend565(lens, C_TEXT, 0.25f));
      if (gp < 700) {                                                                                 // kayan parilti
        int gx = x0 + (int)(gp / 700.0f * (lw + 12)) - 6;
        if (gx > x0 + 4 && gx < x0 + lw - 2) d.drawWideLine(gx, y0 + 3, gx - 6, y0 + lh - 3, 3, blend565(lens, C_TEXT, 0.45f), lens);
      }
    }
    d.drawWideLine(g.cxL + lw / 2, ly - lh / 4, g.cxR - lw / 2, ly - lh / 4, 3, frame, C_BG);         // kopru
  }
  void drawNotes(const Frame& f, const Geo& g, int top) {
    for (int i = 0; i < 3; i++) {
      float p = ((f.t + i * 600) % 1800) / 1800.0f;
      bool r = i & 1;
      int x = (r ? g.cxR + g.w / 2 - 4 : g.cxL - g.w / 2 - 6) + (int)(sinf(p * 6.28f) * 3);
      if (portrait) x = r ? min(SW - 12, x) : max(4, x);
      int y = top + 10 - (int)(p * 26);
      drawNote(x, y, blend565(i == 1 ? blend565(f.col, C_TEXT, 0.4f) : f.col, C_BG, p * 0.85f));
    }
  }

  // Nesneye gore ifade (gozler fincana / kitaba bakar, yudumda mutlu kapanir ...)
  void propExpr(Frame& f) {
    if (!f.prop) return;
    float a = ease(min(f.propT, f.propLeft) / 450.0f);
    auto mix = [&](float& v, float t) { v += (t - v) * a; };
    uint32_t t = f.propT;
    switch (f.prop) {
      case PR_COFFEE: mix(f.ly, 0.45f); mix(f.lx, 0); mix(f.lidFlat, 0.15f); if (cupLift(t) > 0.4f) f.happy = max(f.happy, a); break;
      case PR_BOOK: {
        uint32_t c = t % 4000; float sx = c < 3200 ? ((c % 800) / 800.0f) * 0.4f + (c < 1600 ? -0.5f : 0.1f) : 0;
        mix(f.ly, 0.55f); mix(f.lx, sx); mix(f.lidFlat, 0.2f); break;
      }
      case PR_UMBRELLA: mix(f.ly, (t % 5000) < 1800 ? -0.45f : 0.0f); mix(f.lx, sinf(t / 900.0f) * 0.4f); break;
      case PR_MOON:  mix(f.lidFlat, 0.45f); mix(f.ly, 0.1f); f.zzz = f.zzz || t > 2500; break;
      case PR_SNOW:  f.shiver += sinf(t / 30.0f) * 1.6f * a; mix(f.lidFlat, 0.22f); mix(f.size, 0.95f); break;
      case PR_SUN:   mix(f.ly, -0.1f); break;
      case PR_NOTE:  f.bounce += -fabsf(sinf(t / 250.0f)) * 5 * a; mix(f.lx, sinf(t / 500.0f) * 0.5f); if ((t / 1500) % 2 == 0) f.happy = max(f.happy, a); f.notes = true; break;
    }
  }
  // Uyanis: gozler ekranin altindan bakar, saga sola bakip yukari cikar (t ms)
  void peekExpr(Frame& f, uint32_t t) {
    int ew, eh, gap, mx, my, cy; base(f.info, ew, eh, gap, mx, my, cy);
    float hidden = SH + eh / 2 + 6 - cy, half = SH - eh * 0.1f - cy;
    float off = t < 450 ? hidden + (half - hidden) * ease(t / 450.0f)
              : t < 1300 ? half : half * (1 - ease((t - 1300) / 500.0f));
    if (t < 1300) {
      f.ly = -0.6f; f.lx = t < 450 ? 0 : t < 850 ? -0.8f : 0.8f; f.happy = 0; f.lidFlat = 0; f.lidIn = f.lidOut = 0;
      f.peek = off - f.ly * my - f.bounce;                           // bakis kaymasi yukselmeyi bozmasin
    } else f.peek = off;
  }

  void render(const Frame& f) {
    Geo g = geo(f);
    int top = g.cy - g.h / 2, bot = g.cy + g.h / 2, mid = (g.cxL + g.cxR) / 2;
    // arka plan nesneleri
    if (f.prop == PR_UMBRELLA) drawUmbrella(f, g, mid, top, bot, true);
    if (f.prop == PR_SNOW) drawSnow(f);
    if (f.prop == PR_MOON) drawMoonStars(f, g, top);
    // gozler
    if (f.burst == 1 || f.burst == 2) drawBurst(f, g);
    float vis = 1;
    if (f.burst == 1) vis = f.burstP < 0.7f ? 0 : ease((f.burstP - 0.7f) / 0.3f);
    if (f.burst == 2) vis = f.burstP < 0.25f ? 1 - f.burstP / 0.25f : 0;
    if (vis > 0.02f) {
      RingP p = { f.lx, f.ly, f.lidOut, f.lidIn, f.lidFlat, f.happy, f.open * vis };
      if (f.asleep) { p.open = 0.04f; p.happy = 0; }
      int hL = (int)(g.h * (1 - 0.08f * f.lx)), hR = (int)(g.h * (1 + 0.08f * f.lx));   // bakis yonunde perspektif
      if (f.love && !f.asleep && p.open > 0.3f) { heartEye(g.cxL, g.cy, g.w, g.h, f.col, f.t); heartEye(g.cxR, g.cy, g.w, g.h, f.col, f.t); }
      else { ringEye(g.cxL, g.cy, g.w, hL, true, f.col, p); ringEye(g.cxR, g.cy, g.w, hR, false, f.col, p); }
    }
    // on plan nesneleri
    switch (f.prop) {
      case PR_COFFEE:   drawCup(f, g, mid, bot); break;
      case PR_BOOK:     drawBook(f, g, mid, bot); break;
      case PR_UMBRELLA: drawUmbrella(f, g, mid, top, bot, false); break;
      case PR_SUN:      drawShades(f, g); break;
      default: break;
    }
    if (f.notes) drawNotes(f, g, top);
    if (f.asleep || f.zzz) {
      int zx = f.prop == PR_MOON ? g.cxR - g.w / 4 : min(SW - 18, g.cxR + g.w / 2 - 4);   // ayin ustune binmesin
      drawZzz(zx, top - 4 + (f.asleep ? g.h / 2 - 6 : 0), f.col, f.t);
    }
  }
}
// EYEGFX>>

namespace Eyes {
  enum Mood : uint8_t { NORMAL, HAPPY, TIRED, ANGRY, SURPRISED, SLEEPY, LOVE, SAD };
  using namespace EyeGfx;

  struct Anim {
    float v = 0, t = 0;
    void set(float x) { v = t = x; }
    void step(float k) { v += (t - v) * k; if (fabsf(t - v) < 0.002f) v = t; }
  };

  Anim lookX, lookY;          // -1..1
  Anim lidOut, lidIn;         // ust kapak egimi (dis / ic kose), 0..1
  Anim lidFlat;               // duz ust kapak (uykulu), 0..1
  Anim happy;                 // mutlu yay, 0..1
  Anim size;                  // goz boyutu carpani
  Anim blink;                 // 1 = acik, ~0 = kapali
  Anim tiltX, tiltY;          // hareket sensoru: gozler yercekimi / viraj yonune kayar

  Mood     mood = NORMAL;
  uint32_t nextLook = 0, nextBlink = 0, nextMood = 0, blinkOpenAt = 0;
  uint8_t  blinksLeft = 0;
  uint32_t nextChat = 0;      // bir sonraki goz sesi
  uint32_t volUntil = 0, notesUntil = 0;
  float    bounce = 0;        // ziplama (piksel)

  // Sahne nesnesi (fincan, kitap, semsiye ...)
  uint8_t  prop = PR_NONE;
  uint32_t propAt = 0, propUntil = 0, nextProp = 0, coffeeAt = 0;
  // Parcacik gecisi
  uint8_t  burstMode = 0; uint32_t burstAt = 0, burstSeed = 1;
  // Uyku / uyanis
  bool     asleep = false; uint32_t noLinkAt = 0, peekAt = 0;
  // Hareket sensoru
  bool     imuInit = false; float gb[3] = {0, 0, 1};
  uint32_t imuAt = 0, lastJolt = 0, joltWin = 0; uint8_t joltN = 0;

  uint16_t color() {
    static const uint8_t C[][3] = {
      {80, 200, 255}, {90, 255, 160}, {255, 150, 40}, {255, 110, 200}, {235, 240, 255}
    };
    uint8_t i = cfg.eyeIdx % COUNT_OF(C);
    return tft.color565(C[i][0], C[i][1], C[i][2]);
  }

  uint32_t rnd(uint32_t a, uint32_t b) { return a + esp_random() % (b - a + 1); }

  void setMood(Mood m) {
    mood = m;
    lidOut.t = 0; lidIn.t = 0; lidFlat.t = 0; happy.t = 0; size.t = 1.0f;
    switch (m) {
      case HAPPY:     happy.t = 1.0f;                     break;
      case TIRED:     lidOut.t = 0.38f; lidIn.t = 0.06f;  break;   // dis koseler dusuk
      case ANGRY:     lidIn.t = 0.55f;                    break;   // ic koseler dusuk
      case SURPRISED: size.t = 1.16f;                     break;
      case SLEEPY:    lidFlat.t = 0.55f;                  break;
      case SAD:       lidOut.t = 0.32f; size.t = 0.94f; lookY.t = 0.5f; break;
      default: break;
    }
  }

  // Cihaz durumuna gore olasi ruh hali
  Mood pickMood() {
    if (SleepT::fading()) return SLEEPY;
    if (!bleConnected) return (esp_random() % 3) ? TIRED : SLEEPY;
    if (!app.usbPower && app.batteryPct >= 0 && app.batteryPct <= 20 && esp_random() % 100 < 60) return TIRED;
    if (app.muted)     return SLEEPY;
    uint32_t r = esp_random() % 100;
    if (app.level >= volSteps() && r < 30) return SURPRISED;
    if (r < 50) return NORMAL;
    if (r < 70) return HAPPY;
    if (r < 80) return SURPRISED;
    if (r < 90) return TIRED;
    if (r < 95) return SLEEPY;
    if (r < 98 && bleConnected) return LOVE;
    return ANGRY;
  }

  void react(Mood m, uint32_t holdMs) {
    setMood(m);
    nextMood = millis() + holdMs;
    if (m == HAPPY || m == SURPRISED) { lookX.t = 0; lookY.t = -0.2f; nextLook = millis() + 900; }
  }

  bool soundsOk() { return cfg.sndEyes && !SleepT::fading() && !app.nightOff; }

  // ---- Parcacik gecisi / nesneler / uyku ---------------------------------------
  void burst(bool scatterFirst) { burstSeed = esp_random() | 1; burstMode = scatterFirst ? 2 : 1; burstAt = millis(); }
  void startProp(uint8_t p, uint32_t dur) { prop = p; propAt = millis(); propUntil = propAt + dur; }
  void endProp() { prop = PR_NONE; nextProp = millis() + rnd(40000, 80000); }
  bool rainNow() {
    int i = Wx::idxNow(), c = rtcWx.wcode;
    return (i >= 0 && rtcWx.hP[i] >= 50) || (c >= 51 && c <= 67) || (c >= 80 && c <= 82) || c >= 95;
  }
  // Duruma uygun nesne: uzun surus -> kahve molasi, yagmur -> semsiye, soguk -> kar,
  // sicak gunduz -> gunes gozlugu, gece -> ay; yoksa ara sira kitap / kahve
  void pickProp(uint32_t now) {
    if (Obd::driveSec() > 5400 && (!coffeeAt || now - coffeeAt > 1800000UL)) { coffeeAt = now; startProp(PR_COFFEE, 12000); return; }
    uint8_t c[4]; int n = 0;
    if (Wx::has()) {
      int T = Wx::tempNow();
      if (rainNow()) c[n++] = PR_UMBRELLA;
      if (T <= 2) c[n++] = PR_SNOW;
      if (T >= 30 && rtcWx.isDay) c[n++] = PR_SUN;
    }
    if (Net::timeValid()) { int h = Net::localHour(); if (h >= 23 || h < 5) c[n++] = PR_MOON; }
    if (n && esp_random() % 100 < 75) { startProp(c[esp_random() % n], 9000); return; }
    uint32_t r = esp_random() % 100;
    if (r < 30) startProp(PR_BOOK, 9000);
    else if (r < 50) startProp(PR_COFFEE, 9000);
    else nextProp = now + rnd(20000, 40000);
  }
  void goSleep() { asleep = true; prop = PR_NONE; setMood(SLEEPY); Serial.println("[Goz] uyku (baglanti yok)"); }
  void wake(bool peek) {
    asleep = false; noLinkAt = millis();
    setMood(NORMAL);
    if (peek) { peekAt = millis(); nextMood = peekAt + 2400; }
    else react(SURPRISED, 1500);
  }

  // ---- Hareket sensoru: egim -> bakis, sarsinti -> sasirma, cok sallama -> dagilip toplanma
  //  Kalibrasyon (Ekran yonu > otomatik) varsa eksenler oradan, yoksa varsayim kullanilir.
  static void upVec(uint8_t rot, float v[3]) {
    bool land = rot & 1; uint8_t axis; bool neg;
    if (AutoRot::valid()) { axis = land ? (cfg.rotCal >> 2) & 1 : cfg.rotCal & 1; neg = land ? (cfg.rotCal >> 3) & 1 : (cfg.rotCal >> 1) & 1; }
    else { axis = land ? 0 : 1; neg = land; }
    if (rot >= 2) neg = !neg;
    v[0] = v[1] = v[2] = 0; v[axis] = neg ? -1 : 1;
  }
  void imuService(uint32_t now) {
    if (!Imu::ok || AutoRot::calStep || now - imuAt < 40) return;
    imuAt = now;
    float a[3];
    if (!Imu::read(a[0], a[1], a[2])) return;
    if (!imuInit) { for (int i = 0; i < 3; i++) gb[i] = a[i]; imuInit = true; }
    float d[3], dev = 0;
    for (int i = 0; i < 3; i++) { d[i] = a[i] - gb[i]; dev += d[i] * d[i]; gb[i] += d[i] * 0.03f; }
    dev = sqrtf(dev);
    uint8_t rot = ORIENT_ROT[cfg.orient];
    float eU[3], eR[3]; upVec(rot, eU); upVec((rot + 1) & 3, eR);
    float dx = d[0] * eR[0] + d[1] * eR[1] + d[2] * eR[2];
    float dy = d[0] * eU[0] + d[1] * eU[1] + d[2] * eU[2];
    tiltX.t = constrain(dx * 3.2f, -1.0f, 1.0f);
    tiltY.t = constrain(dy * 3.2f, -1.0f, 1.0f);
    if (dev > 0.6f && now - lastJolt > 500) {                       // sarsinti / eline alma
      lastJolt = now;
      if (now - joltWin > 1500) { joltWin = now; joltN = 0; }
      if (asleep) { wake(false); return; }
      if (++joltN >= 3) { burst(true); react(SURPRISED, 1800); joltN = 0; }
      else {
        react(SURPRISED, 1200); bounce = -8;
        if (soundsOk() && esp_random() % 100 < 50) Snd::mood(SURPRISED);
      }
    }
  }

  void start() {
    uint32_t now = millis();
    lookX.set(0); lookY.set(0); tiltX.set(0); tiltY.set(0);
    lidOut.set(0); lidIn.set(0); lidFlat.set(0); happy.set(0); size.set(1);
    blink.set(1);
    setMood(pickMood());
    nextLook  = now + 1100;
    nextBlink = now + rnd(2000, 4500);
    nextMood  = now + rnd(6000, 12000);
    nextChat  = now + rnd(8000, 20000);
    nextProp  = now + rnd(15000, 30000);
    prop = PR_NONE; peekAt = 0; asleep = false; noLinkAt = 0; imuInit = false;
    bounce    = 0;
    burst(false);                                                  // parcaciklardan toplanarak acil
  }

  // Mikrofon tepkileri: alkis -> sasirma, ritim -> ziplama, surekli muzik -> dans
  void micReact(uint32_t now) {
    if (!Mic::running) { bounce *= 0.7f; return; }
    if (Mic::takeClap()) {
      react(SURPRISED, 1700);
      lookX.t = 0; lookY.t = -0.35f; nextLook = now + 1200;
      bounce = -9;
      if (soundsOk() && esp_random() % 100 < 70) Snd::mood(SURPRISED);
    }
    float amp;
    if (Mic::takeBeat(amp)) {
      bounce = -(3.0f + 9.0f * amp);
      size.v = min(1.25f, size.v + 0.07f * amp);
      if (mood == SLEEPY && !SleepT::fading()) { setMood(NORMAL); nextMood = now + 4000; }
    }
    if (Mic::music) {
      if (mood != HAPPY && !SleepT::fading()) setMood(HAPPY);
      nextMood = now + 3000;
      notesUntil = now + 700;
      if (now >= nextLook) {                          // muzikle saga sola sallan
        lookX.t = (lookX.t > 0) ? -0.55f : 0.55f; lookY.t = -0.1f;
        nextLook = now + 520;
      }
    }
    bounce *= 0.72f;
    if (fabsf(bounce) < 0.3f) bounce = 0;
  }

  void update() {
    uint32_t now = millis();

    // Parcacik gecisi: dagil -> toplan -> bitti
    if (burstMode == 2 && now - burstAt >= 450) { burstMode = 1; burstAt = now; }
    else if (burstMode == 1 && now - burstAt >= 800) burstMode = 0;

    // Uyku: telefon ve OBD baglantisi 60 sn yoksa uyur; baglanti gelince alttan bakarak uyanir
    bool link = bleConnected || Obd::linked();
    if (link) { noLinkAt = 0; if (asleep) wake(true); }
    else {
      if (!noLinkAt) noLinkAt = now;
      if (!asleep && !peekAt && now - noLinkAt > 60000 && !Mic::music) goSleep();
    }
    if (peekAt && now - peekAt > 1900) { peekAt = 0; react(bleConnected ? LOVE : HAPPY, 2500); if (soundsOk()) Snd::mood(HAPPY); }

    imuService(now);

    if (asleep) {                                                  // uykuda: sadece nefes
      lidFlat.step(0.1f); tiltX.step(0.1f); tiltY.step(0.1f);
      bounce = sinf(now / 900.0f) * 2;
      return;
    }

    // Sahne nesnesi
    if (prop && now >= propUntil) endProp();
    if (!prop && !peekAt && !burstMode && now >= nextProp && mood != LOVE && !Mic::music) pickProp(now);

    // Kirpma
    if (blinkOpenAt && now >= blinkOpenAt) {
      blink.t = 1.0f; blinkOpenAt = 0;
      if (blinksLeft) { blinksLeft--; nextBlink = now + 140; }
    }
    if (!blinkOpenAt && now >= nextBlink) {
      blink.t = 0.04f; blinkOpenAt = now + 95;
      if (!blinksLeft) {
        if (esp_random() % 5 == 0) blinksLeft = 1;            // bazen cift kirp
        nextBlink = now + ((mood == SLEEPY) ? rnd(1500, 3000) : rnd(2500, 6500));
      }
    }

    // Bakis
    if (now >= nextLook) {
      bool calm = (mood == SLEEPY || mood == TIRED || mood == SAD);
      if (esp_random() % 100 < 35) { lookX.t = 0; lookY.t = mood == SAD ? 0.5f : 0; }
      else {
        float r = calm ? 0.55f : 1.0f;
        lookX.t = ((int)(esp_random() % 201) - 100) / 100.0f * r;
        lookY.t = ((int)(esp_random() % 201) - 100) / 100.0f * r * 0.8f;
        if (esp_random() % 3 == 0) lookY.t = 0;               // cogunlukla yatay
      }
      nextLook = now + (calm ? rnd(2200, 4500) : rnd(1100, 3200));
    }

    // Ruh hali (ara sira sesli)
    if (now >= nextMood) {
      setMood(pickMood());
      nextMood = now + rnd(6000, 13000);
      if (soundsOk() && esp_random() % 100 < 30) { Snd::mood(mood); nextChat = max(nextChat, now + 6000); }
    }
    // Rastgele gevezelik
    if (now >= nextChat) {
      if (soundsOk() && mood != SLEEPY) Snd::chatter();
      nextChat = now + rnd(25000, 70000);
    }
    micReact(now);

    lookX.step(0.16f);  lookY.step(0.16f);
    tiltX.step(0.18f);  tiltY.step(0.18f);
    lidOut.step(0.12f); lidIn.step(0.12f); lidFlat.step(0.12f);
    happy.step(0.14f);  size.step(0.15f);
    blink.step(0.55f);
  }

  // ---- Etkilesim (tus / durum tepkileri) ------------------------------------

  void onVol(bool up, bool limit) {
    uint32_t now = millis();
    volUntil = now + 1600;
    if (asleep) wake(false);
    if (limit) { react(up ? SURPRISED : TIRED, 1800); }
    else if (mood == SLEEPY || mood == ANGRY || mood == SAD) setMood(NORMAL);
    lookX.t = up ? 0.75f : -0.75f;
    lookY.t = up ? -0.55f : 0.55f;
    nextLook = now + 1200;
  }
  void onMedia(int dir) {
    uint32_t now = millis();
    setMood(SURPRISED); nextMood = now + 900;
    lookX.v = -dir * 0.9f; lookX.t = dir * 0.9f; lookY.t = 0;
    nextLook = now + 1000;
  }
  void onPlay()        { react(HAPPY, 3000); notesUntil = millis() + 3000; }
  void onMute(bool m)  { if (m) react(SLEEPY, 4000); else react(HAPPY, 1500); }
  void onSleep()       { setMood(SLEEPY); lidFlat.t = 0.8f; nextMood = millis() + 600000UL; }
  void onTrack()       { if (!asleep && !burstMode) { startProp(PR_NOTE, 5000); react(HAPPY, 2500); } }   // yeni sarki
  void onWeather()     { if (!asleep) { burst(true); nextProp = millis() + 2500; if (prop != PR_NOTE) prop = PR_NONE; } }
  void onDisconnect()  { if (!asleep) react(SAD, 6000); }

  // Mikrofon seviyesi: gozlerin altinda kucuk ekolayzer (dinlerken)
  void drawEq(uint16_t col, int base) {
    TFT_eSPI& g = G();
    const int N = 16, bw = 3, gap = 2, maxH = portrait ? 18 : 13;
    int x0 = SW / 2 - (N * (bw + gap) - gap) / 2;
    for (int i = 0; i < N; i++) {
      // ortada en yeni ornek, kenarlara dogru eskiler (simetrik)
      int k = abs(i * 2 - (N - 1)) / 2;
      float v = Mic::histAt(COUNT_OF(Mic::hist) - 1 - k) / 255.0f;
      int h = 1 + (int)(v * maxH);
      uint16_t c = blend565(col, C_BG, 0.75f - 0.5f * v);
      g.fillRect(x0 + i * (bw + gap), base - h, bw, h, c);
    }
  }

  // ---- Ust bilgi: saat + hava durumu (Wi-Fi verisi varsa) -------------------
  // Yeni hava verisi geldi: 12 sn boyunca durum satiri (son 2 sn soner)
  bool wxFresh() { return Wx::updatedAt && millis() - Wx::updatedAt < 12000; }
  float wxFade() { uint32_t t = millis() - Wx::updatedAt; return t < 10000 ? 0.0f : (t - 10000) / 2000.0f; }
  const char* wxLine() {
    static char b[40];
    uint8_t h; uint8_t pk = Wx::rainPeak(12, &h);
    if (pk >= 40) snprintf(b, sizeof(b), "%s  %d°/%d°  yağmur %02d:00", Net::wText(), rtcWx.tmin, rtcWx.tmax, h);
    else snprintf(b, sizeof(b), "%s  %d°/%d°", Net::wText(), rtcWx.tmin, rtcWx.tmax);
    return b;
  }
  void drawTopInfo(uint16_t col) {
    char clk[8];
    bool hasClk = Net::clock(clk, sizeof(clk));
    bool hasWx  = Net::hasWeather();
    uint16_t dim = blend565(col, C_TEXT, 0.35f);
    if (portrait) {
      if (hasClk) drawText(clk, SW / 2, 17, MC_DATUM, 4, C_TEXT, C_BG);
      int y = 45;
      int right = SW - 4;
      if (SleepT::active()) {
        char m[10];
        if (SleepT::fading()) strcpy(m, "..");
        else snprintf(m, sizeof(m), "%lu", (unsigned long)SleepT::minsLeft());
        drawText(m, right, y, MR_DATUM, 2, C_MENU, C_BG);
        drawMoon(right - textW(m, 2) - 7, y - 1, 4, C_MENU);
      }
      if (hasWx) {
        int w = wxMiniW(true);
        drawWxMini(SW / 2 + w / 2, y, true);
        if (wxFresh()) drawText(wxLine(), SW / 2, y + 17, MC_DATUM, 1, blend565(C_TEXT2, C_BG, wxFade()), C_BG, SW - 6);
      }
    } else {
      if (hasClk) drawText(clk, 4, 9, ML_DATUM, 2, C_TEXT2, C_BG);
      if (hasWx) {
        drawWxMini(SW - 4, 9, true);
        if (wxFresh()) drawText(wxLine(), SW / 2, 26, MC_DATUM, 1, blend565(C_TEXT2, C_BG, wxFade()), C_BG, SW - 8);
      }
      if (SleepT::active()) {
        char m[16];
        if (SleepT::fading()) strcpy(m, "uyku..");
        else snprintf(m, sizeof(m), "%lu dk", (unsigned long)SleepT::minsLeft());
        int tw = textW(m, 2);
        drawMoon(SW / 2 - tw / 2 - 6, 8, 4, C_MENU);
        drawText(m, SW / 2 + 5, 9, MC_DATUM, 2, C_MENU, C_BG);
      }
    }
  }

  // ---- Alt: gosterisli ses kapsulu -------------------------------------------
  void drawVolPill(uint16_t col) {
    TFT_eSPI& g = G();
    uint32_t now = millis();
    bool fresh = now < volUntil;                         // az once degisti
    int ph = portrait ? 34 : 25, pw = portrait ? SW - 8 : 176;
    int py = SH - ph - 2, px = SW / 2 - pw / 2;

    // Limitte basinca kapsul titrer
    uint32_t since = now - app.limitFlashAt;
    float flash = (app.limitFlashAt && since < LIMIT_FLASH_MS) ? 1.0f - since / (float)LIMIT_FLASH_MS : 0.0f;
    px += (int)(sinf(now / 18.0f) * 4.0f * flash);

    bool conn = bleConnected;
    int  pct  = (int)(app.shownLevel * 100.0f / volSteps() + 0.5f);
    bool top  = conn && !app.muted && app.level >= volSteps();
    bool bot  = conn && !app.muted && app.level <= 0;
    uint16_t acc = !conn ? C_SUBTEXT : app.muted ? C_BAD : top ? C_BAD : bot ? C_MIN : col;

    float glow = fresh ? 0.5f + 0.5f * (volUntil - now) / 1600.0f : 0.0f;
    uint16_t bgc = blend565(acc, C_BG, 0.86f - 0.14f * glow);
    uint16_t brd = blend565(acc, C_BG, 0.55f - 0.45f * glow);
    g.fillRoundRect(px, py, pw, ph, ph / 2, bgc);
    g.drawRoundRect(px, py, pw, ph, ph / 2, brd);
    if (fresh) g.drawRoundRect(px + 1, py + 1, pw - 2, ph - 2, ph / 2 - 1, blend565(acc, bgc, 0.5f));

    int midY = py + (portrait ? 14 : ph / 2);
    if (!conn) {
      drawBtIcon(px + 16, midY - 7, 14, C_SUBTEXT);
      drawText(bleHidden ? "Gizli mod" : "Bağlı değil", px + 30, midY + 1, ML_DATUM, 2, C_TEXT2, bgc, pw - 40);
      return;
    }
    if (app.muted) {
      drawSpeaker(px + 10, midY - 9, C_BAD, true);
      drawText("SESSİZ", px + pw / 2 + 12, midY + 1, MC_DATUM, 4, C_BAD, bgc);
      return;
    }
    drawSpeaker(px + 10, midY - 9, acc, false);

    char num[8];
    if (top) strcpy(num, "MAX"); else if (bot) strcpy(num, "MIN"); else snprintf(num, sizeof(num), "%d", pct);
    int nx = px + 40;
    int nw = textW(num, 4);
    drawText(num, nx, midY + 1, ML_DATUM, 4, top || bot ? acc : C_TEXT, bgc);
    if (!top && !bot) drawText("%", nx + nw + 2, midY + 4, ML_DATUM, 2, acc, bgc);

    float frac = constrain(app.shownLevel / (float)volSteps(), 0.0f, 1.0f);
    if (portrait) {
      // kapsulun altinda ince seviye cizgisi
      int bx = px + 14, bw = pw - 28, by = py + ph - 7;
      g.fillRoundRect(bx, by, bw, 3, 1, blend565(acc, bgc, 0.7f));
      int fw = (int)(bw * frac);
      if (fw > 0) g.fillRoundRect(bx, by, fw, 3, 1, acc);
    } else {
      // sagda 8 bolmeli seviye gostergesi (yukselen cubuklar)
      const int N = 8, sw = 5, gap = 2;
      int bx = px + pw - 14 - N * (sw + gap) + gap;
      int lit = (int)lroundf(frac * N);
      for (int i = 0; i < N; i++) {
        int h = 5 + i * 2;
        uint16_t c = i < lit ? (top ? C_BAD : levelColor((i + 1) / (float)N)) : blend565(acc, bgc, 0.75f);
        if (i < lit && fresh && i == lit - 1) c = C_TEXT;             // son eklenen cubuk parlar
        g.fillRoundRect(bx + i * (sw + gap), midY + 8 - h, sw, h, 1, c);
      }
    }
  }

  uint16_t moodColor(uint16_t col) {
    if (mood == ANGRY) return blend565(col, tft.color565(255, 60, 60), 0.55f);
    if (mood == SAD)   return blend565(col, tft.color565(90, 130, 255), 0.35f);
    if (mood == SLEEPY || mood == TIRED || asleep) return blend565(col, C_BG, 0.22f);
    if (mood == LOVE) return tft.color565(255, 90, 150);
    if (prop == PR_SNOW) return blend565(col, tft.color565(160, 215, 255), 0.35f);
    return col;
  }

  void draw() {
    uint32_t now = millis();
    uint16_t col = color();
    Frame f;
    f.t = now;
    f.col = moodColor(col);
    f.love = mood == LOVE;
    f.lx = constrain(lookX.v + tiltX.v * 0.9f, -1.3f, 1.3f);
    f.ly = constrain(lookY.v + tiltY.v * 0.9f, -1.3f, 1.3f);
    f.lidOut = lidOut.v; f.lidIn = lidIn.v; f.lidFlat = lidFlat.v; f.happy = happy.v; f.size = size.v;
    f.open = blink.v * (1.0f + 0.02f * sinf(now / 700.0f));
    f.bounce = bounce;
    f.asleep = asleep;
    f.info = Net::timeValid() || Net::hasWeather();
    f.zzz = mood == SLEEPY && lidFlat.v > 0.3f;
    f.notes = now < notesUntil;
    if (prop) { f.prop = prop; f.propT = now - propAt; f.propLeft = propUntil > now ? propUntil - now : 0; }
    if (burstMode) { f.burst = burstMode; f.burstP = (now - burstAt) / (burstMode == 2 ? 450.0f : 800.0f); f.seed = burstSeed; }
    propExpr(f);
    if (peekAt) peekExpr(f, now - peekAt);
    render(f);

    if (Mic::running) drawEq(col, portrait ? 198 : 104);
    drawTopInfo(col);
    if ((!prop && !peekAt) || now < volUntil) drawVolPill(col);    // nesne / uyanis sirasinda ses kapsulu gizli
  }
}


void eyesVol(bool up, bool limit) { if (app.saver) Eyes::onVol(up, limit); }
void eyesMedia(int dir)           { if (app.saver) Eyes::onMedia(dir); }
void eyesPlay()                   { if (app.saver) Eyes::onPlay(); }
void eyesMute(bool m)             { if (app.saver) Eyes::onMute(m); }
void eyesSleep()                  { if (app.saver) Eyes::onSleep(); }
void eyesTired()                  { if (app.saver) Eyes::react(Eyes::TIRED, 5000); }

void startSaver() {
  app.saver = true;
  blTarget = blFull();
  Eyes::start();
}

// ============================================================================
//  RENDER
// ============================================================================
// ---- Tamponsuz cizim (eslesme sirasinda RAM icin ekran tamponu bos) ---------
// Tum ekran sadece icerik degisince BIR KEZ cizilir; her karede yalnizca kucuk
// bir alan (bekleme halkasi / geri sayim cubugu) guncellenir -> titreme yok.
void drawModalBar() {
  TFT_eSPI& g = G();
  bool confirm = (Pair::modal == Pair::M_CONFIRM);
  float left = 1.0f - min(1.0f, (millis() - Pair::mAt) / (float)CONFIRM_TIMEOUT_MS);
  int y = portrait ? 184 : 107, h = portrait ? 4 : 3;
  int bw = (int)((SW - 20) * left);
  g.fillRect(10, y, SW - 20, h, C_BG);
  g.fillRoundRect(10, y, SW - 20, h, 1, C_TRACK);
  if (bw > 0) g.fillRoundRect(10, y, bw, h, 1, confirm ? C_ACCENT : C_WARN);
}

void renderDirect() {
  static uint32_t sig = 0xFFFFFFFF;
  static uint32_t lastAnim = 0;
  TFT_eSPI& g = G();
  // Icerik imzasi: degisirse tam cizim
  uint32_t h = (uint32_t)Pair::modal * 131u + (portrait ? 7u : 0u) + (bleConnected ? 3u : 0u);
  for (const char* c = Pair::mName; *c; c++) h = h * 31u + (uint8_t)*c;
  for (const char* c = app.peerName; *c; c++) h = h * 17u + (uint8_t)*c;
  bool full = (h != sig);
  sig = h;
  if (Pair::modal != Pair::M_NONE) {
    if (full) renderPairModal();
    drawModalBar();
    return;
  }
  int cy = portrait ? 120 : 78;
  if (full) {
    g.fillRect(0, 0, SW, SH, C_BG);
    drawBtIcon(SW / 2, portrait ? 34 : 10, 22, C_BT);
    drawText("Eşleşiyor...", SW / 2, portrait ? 80 : 46, MC_DATUM, 4, C_TEXT, C_BG);
    const char* nm = app.peerName[0] ? app.peerName : "Telefonda onaylayın";
    drawText(nm, SW / 2, portrait ? 170 : 112, MC_DATUM, 2, C_TEXT2, C_BG, SW - 10);
    drawText("Lütfen bekleyin", SW / 2, portrait ? 190 : 127, MC_DATUM, 1, C_SUBTEXT, C_BG);
  }
  // Donen halka (yalnizca kendi alani silinir)
  if (full || millis() - lastAnim >= 60) {
    lastAnim = millis();
    uint32_t a = (millis() / 4) % 360;
    g.drawSmoothArc(SW / 2, cy, 16, 12, 0, 360, C_TRACK, C_BG, false);   // silmeden ustune ciz
    g.drawSmoothArc(SW / 2, cy, 16, 12, a, (a + 100) % 360, C_ACCENT, C_BG, true);
  }
}

void render() {
  static uint32_t last = 0;
  uint32_t now = millis();
  if (now - last < (uint32_t)(useSprite ? 33 : 60)) return;
  last = now;
  if (blCurrent == 0 && blTarget == 0) return;
  if (directMode) { renderDirect(); return; }   // eslesme: tamponsuz, titremesiz

  float target = app.level;
  app.shownLevel += (target - app.shownLevel) * 0.28f;
  if (fabsf(target - app.shownLevel) < 0.01f) app.shownLevel = target;

  if (app.syncing && !KeyQueue::busy()) { app.syncing = false; showToast("SENKRON OK", C_OK); }

  TFT_eSPI& g = G();
  g.fillRect(0, 0, SW, SH, C_BG);

  if (Pair::modal != Pair::M_NONE) {
    renderPairModal();
  } else if (AutoRot::calStep) {
    renderCalib();
    drawToast();
  } else if (IRL::mode) {
    renderIrMode();
    drawToast();
  } else if (app.camera) {
    renderCamera();
    drawToast();
  } else if (Obd::menu) {
    Obd::drawSettings();
    drawToast();
  } else if (app.saver) {
    Eyes::update(); Eyes::draw();
  } else if (menu.open) {
    renderMenu();
    drawToast();
  } else if (Obd::dashActive()) {
    Obd::drawSaver();                                             // ana ekran: OBD2 arayuzleri
    drawDashVolume();
  } else {                                                        // OBD2 kapali: ana ekran gozler (ses halkasi yok)
    Eyes::update(); Eyes::draw();
    drawDashVolume();
    drawToast();
  }
  if (trackCardActive() && !Obd::alertBlocking() && !Info::active() && Pair::modal == Pair::M_NONE && !AutoRot::calStep && !menu.open && !Obd::menu)
    drawTrackCard();                                              // sarki degisti: her ekranin ustunde ~4 sn
  if (Obd::alertActive() && Pair::modal == Pair::M_NONE && !AutoRot::calStep) Obd::drawAlert();   // surus uyarisi: her ekranin ustunde
  else if (Info::active() && Pair::modal == Pair::M_NONE && !AutoRot::calStep) Info::draw();       // hava uyarisi

  if (useSprite) spr.pushSprite(0, 0);
}

// ============================================================================
//  ESLESME / BAGLANTI OLAYLARI (loop icinde)
// ============================================================================
void refreshPeerName() {
  const uint8_t* a = activeAddr();
  app.peerName[0] = 0;
  if (a) Names::get(a, app.peerName, sizeof(app.peerName));
}

// ---- Eslesme kaydi: RAM ve dogrulama ---------------------------------------
namespace BondSnap {                     // baglanti anindaki kayitli adresler
  uint8_t addr[15][6];
  int n = 0;
  void take() { n = min(Bonds::count, 15); for (int i = 0; i < n; i++) memcpy(addr[i], Bonds::list[i].bd_addr, 6); }
  bool has(const uint8_t* a) { for (int i = 0; i < n; i++) if (!memcmp(addr[i], a, 6)) return true; return false; }
}

namespace BondGuard {
  bool     suspended = false;       // ekran tamponu gecici kapali
  uint32_t resumeAt = 0;
  bool     verifyPending = false;
  uint32_t verifyAt = 0;
  uint8_t  verifyAddr[6] = {0};
  char     verifyName[32] = {0};

  // Bluedroid eslesmeleri NVS'de "bt_config.conf" / "bt_cfg_keyN" bloblarinda,
  // bolum adi "aa:bb:cc:dd:ee:ff" olarak tutar.
  bool inNvs(const uint8_t* a) {
    nvs_handle_t h;
    if (nvs_open("bt_config.conf", NVS_READONLY, &h) != ESP_OK) return false;
    char want[20];
    snprintf(want, sizeof(want), "%02x:%02x:%02x:%02x:%02x:%02x", a[0], a[1], a[2], a[3], a[4], a[5]);
    bool found = false;
    for (int i = 0; i < 8 && !found; i++) {
      char key[16]; snprintf(key, sizeof(key), "bt_cfg_key%d", i);
      size_t len = 0;
      if (nvs_get_blob(h, key, NULL, &len) != ESP_OK || len == 0) break;
      char* b = (char*)malloc(len + 1);
      if (!b) break;
      if (nvs_get_blob(h, key, b, &len) == ESP_OK) { b[len] = 0; found = strstr(b, want) != nullptr; }
      free(b);
    }
    nvs_close(h);
    return found;
  }

  void logMem(const char* tag) {
    nvs_stats_t st = {};
    nvs_get_stats(NULL, &st);
    Serial.printf("[MEM] %s: bos RAM %u, en buyuk blok %u, NVS %u/%u kayit dolu\n", tag,
                  (unsigned)ESP.getFreeHeap(), (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_8BIT),
                  (unsigned)st.used_entries, (unsigned)st.total_entries);
  }

  void suspend() {
    if (!useSprite) return;
    spr.deleteSprite();
    useSprite = false; suspended = true; directMode = true;
    tft.fillScreen(C_BG);
    logMem("eslesme basladi, ekran tamponu bosaltildi");
  }
  void resume() {
    if (!suspended) return;
    suspended = false; directMode = false;
    spr.setColorDepth(16);
    useSprite = spr.createSprite(SW, SH) != nullptr;
    if (!useSprite) {
      // Bellek parcalandi: tamponsuz cizim titrer. Eslesme kayitli; kisa temiz yeniden baslatma
      // (telefon birkac saniyede kendiliginden yeniden baglanir).
      Serial.println("[MEM] ekran tamponu geri alinamadi -> temiz yeniden baslatma");
      showToast("Eşleşme kaydedildi, yeniden başlıyor", C_OK, 3000);
      app.restartReq = BOOT_CLEAN; app.restartAt = millis() + 2500;
    }
  }

  void service() {
    uint32_t now = millis();
    static uint32_t retryAt = 0;
    if (!useSprite && !suspended && !app.camera && now - retryAt > 3000) {     // tamponsuz kaldiysa (titreme) yeniden dene
      retryAt = now;
      if (heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) > (size_t)SW * SH * 2 + 1024) {
        spr.setColorDepth(16);
        useSprite = spr.createSprite(SW, SH) != nullptr;
        if (useSprite) logMem("ekran tamponu geri alindi");
      }
    }
    if (suspended && Pair::modal == Pair::M_NONE && !Pair::justAccepted && !verifyPending &&
        resumeAt && (int32_t)(now - resumeAt) >= 0) { resumeAt = 0; resume(); }
    if (verifyPending && (int32_t)(now - verifyAt) >= 0) {
      verifyPending = false;
      bool ok = inNvs(verifyAddr);
      logMem(ok ? "eslesme kaydi DOGRULANDI" : "eslesme kaydi NVS'de YOK");
      if (!ok) {
        char t[40];
        snprintf(t, sizeof(t), "KAYDEDİLEMEDİ: %s", verifyName[0] ? verifyName : "cihaz");
        showToast(t, C_BAD, 6000);
        app.lastInput = now; blTarget = blFull();
      }
      resumeAt = now + 500;
    }
  }
}

void pairMemPrepare() {
  BondGuard::resumeAt = millis() + 40000;            // olur da tamamlanmazsa geri al
  BondGuard::suspend();
}

void peerService() {
  uint32_t now = millis();
  for (int i = 0; i < MAX_PEERS; i++) {
    Peer& p = peers[i];
    if (!p.used) continue;
    bool settled = (p.authed && now - p.authAt > 400) || (now - p.at > ENC_GRACE_MS);
    if (!settled) continue;
    if (p.kick) { bleDisconnectNow(i); continue; }
    if (!p.checked) {
      p.checked = true;
      if (Pref::set && !Pair::active && !Pref::is(p.bda) && !Pref::is(p.idAddr) &&
          (Bonds::indexOf(p.bda) >= 0 || Bonds::indexOf(p.idAddr) >= 0)) {
        p.rejected = true;
        app.lastRejectAt = now;
        bleDisconnectNow(i);
      }
    }
  }
}

void pairingService() {
  uint32_t now = millis();

  // Onay / kod istegi geldi
  if (Pair::evNc || Pair::evNotif) {
    bool nc = Pair::evNc;
    Pair::evNc = Pair::evNotif = false;
    Pair::openModal(nc ? Pair::M_CONFIRM : Pair::M_NOTIFY);
    app.lastInput = now; blTarget = blFull();
  }

  // Isim okundu
  if (NameReader::ready) {
    NameReader::ready = false;
    Names::set(NameReader::resultAddr, NameReader::result);
    int np = peerFindAddr(NameReader::resultAddr);
    if (np >= 0 && peers[np].authed && memcmp(peers[np].idAddr, NameReader::resultAddr, 6))
      Names::set(peers[np].idAddr, NameReader::result);
    if (Pair::modal != Pair::M_NONE || Pair::justAccepted) {
      strncpy(Pair::mName, NameReader::result, sizeof(Pair::mName) - 1);
      Pair::mName[sizeof(Pair::mName) - 1] = 0;
    }
    refreshPeerName();
    if (menu.open) buildMenu();
  }

  // Kimlik dogrulama sonucu
  if (Pair::evAuth) {
    Pair::evAuth = false;
    if (Pair::evAuthOk) {
      int pa = peerFindAddr(Pair::evAuthAddr);
      if (pa < 0) {                                   // RPA ile baglandiysa: en yeni sifresiz peer
        uint32_t t = 0;
        for (int i = 0; i < MAX_PEERS; i++)
          if (peers[i].used && !peers[i].authed && peers[i].at >= t) { t = peers[i].at; pa = i; }
      }
      if (pa >= 0) { peers[pa].authed = true; peers[pa].authAt = now; memcpy(peers[pa].idAddr, Pair::evAuthAddr, 6); }
      else { authPendAt = now; memcpy(authPendAddr, Pair::evAuthAddr, 6); }   // baglanti olayi henuz gelmedi: sonra uygula
      Serial.printf("[BT] sifreleme tamam (cihaz %d)\n", pa);
      // Yeni eslesme mi? (onayli modda kod onayindan, onaysiz modda kayit listesinden anlasilir)
      bool isNewBond = Pair::justAccepted ||
                       (pa >= 0 && !peers[pa].knownAtConn && !BondSnap::has(Pair::evAuthAddr));
      if (isNewBond && !Pair::justAccepted) {
        Pair::acceptedPeer = pa;
        if (!Pair::mName[0] && pa >= 0) Names::get(peers[pa].bda, Pair::mName, sizeof(Pair::mName));
      }
      if (isNewBond) {
        // Yeni eslesme: ismi kimlik adresine de kaydet
        const char* nm = Pair::mName[0] ? Pair::mName : (Pair::hasTarget ? Pair::target : "");
        if (nm[0]) {
          Names::set(Pair::evAuthAddr, nm);
          if (Pair::acceptedPeer >= 0) Names::set(peers[Pair::acceptedPeer].bda, nm);
        }
        char t[32];
        snprintf(t, sizeof(t), "Eşleşti: %s", nm[0] ? nm : "yeni cihaz");
        Snd::ok();
        showToast(t, C_OK, 2500);
        memcpy(BondGuard::verifyAddr, Pair::evAuthAddr, 6);
        strncpy(BondGuard::verifyName, nm, sizeof(BondGuard::verifyName) - 1);
        BondGuard::verifyPending = true;
        BondGuard::verifyAt = now + 2000;
        Pair::mName[0] = 0;
        // Eski baglantilari kapat, yeni cihaz aktif olsun
        for (int i = 0; i < MAX_PEERS; i++)
          if (peers[i].used && i != Pair::acceptedPeer) bleDisconnectPeer(i);
        if (Pair::acceptedPeer >= 0) bleActive = Pair::acceptedPeer;
        Pair::justAccepted = false;
        if (Pref::set) Pref::clear();          // yeni cihaz eklendi: otomatik baglantiya don
        LostLog::clear();
        if (Pair::active) Pair::stop();
        if (menu.open && menu.page == PG_PAIR) menuClose();
      }
      Bonds::refresh();
      refreshPeerName();
    } else {
      // Sifreleme basarisiz: Bluetooth yigini bu cihazin eslesmesini siler.
      bool wasBonded = Bonds::indexOf(Pair::evAuthAddr) >= 0;
      bool wasNew = Pair::justAccepted || Pair::modal != Pair::M_NONE;
      Pair::justAccepted = false;
      if (Pair::modal != Pair::M_NONE) Pair::modal = Pair::M_NONE;
      BondGuard::resumeAt = now + 800;
      Bonds::refresh();
      bool lost = wasBonded && Bonds::indexOf(Pair::evAuthAddr) < 0;
      char nm[32];
      if (!Names::get(Pair::evAuthAddr, nm, sizeof(nm))) fmtAddr(Pair::evAuthAddr, nm, sizeof(nm));
      Serial.printf("[BT] sifreleme basarisiz: %s  kod 0x%02X  eslesme %s\n", nm, Pair::evReason,
                    lost ? "SİLİNDİ" : (wasNew ? "yeni cihaz" : "yoktu"));
      if (lost) {
        LostLog::save(nm, Pair::evReason);
        char t[40];
        snprintf(t, sizeof(t), "Eşleşme silindi: %s", nm);
        showToast(t, C_BAD, 4000);
        if (menu.open) buildMenu();
      } else { showToast("Eşleşme başarısız", C_BAD, 2500); Snd::error(); }
      app.lastInput = now; blTarget = blFull();
    }
  }

  // Onay suresi doldu
  if (Pair::modal != Pair::M_NONE && now - Pair::mAt > CONFIRM_TIMEOUT_MS) Pair::reject();
  // Istek sahibi baglantiyi kopardi
  if (Pair::modal != Pair::M_NONE && Pair::mPeer >= 0 && !peers[Pair::mPeer].used) {
    Pair::modal = Pair::M_NONE;
    showToast("İstek iptal edildi", C_WARN, 1500);
  }

  // Eslesme modu suresi
  if (Pair::active && (int32_t)(now - Pair::until) >= 0) {
    Pair::stop();
    if (menu.open && menu.page == PG_PAIR) menuBack();
    showToast("Süre doldu", C_WARN, 1500);
  }

  // Bagli cihazin adi bilinmiyorsa oku (baglandiktan 2 sn sonra)
  static uint32_t lastTry = 0;
  if (bleConnected && !app.peerName[0] && bleActive >= 0 && Pair::modal == Pair::M_NONE &&
      now - bleConnAt > 2000 && now - lastTry > 10000 && !NameReader::busy()) {
    lastTry = now;
    NameReader::request(bleActive);
  }

  // Arama listesini canli guncelle
  if (menu.open && menu.page == PG_SCAN && Scan::changed) {
    Scan::changed = false;
    buildMenu();
  }
}

// ============================================================================
//  GUC YONETIMI
// ============================================================================
void goDeepSleep() {
  if (app.dirtySave) saveState();
  blTarget = 0; backlightWrite(0);
  tft.writecommand(0x28);
  tft.writecommand(0x10);
  delay(50);
  HW::powerOff();                       // tam kapanis: GUC tusuyla acilir
  delay(500);
  // USB takiliysa kart kapanmaz: A tusuyla uyanan derin uykuya gec
  esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_BTN_UP, 0);
  esp_deep_sleep_start();
}

void powerService() {
  uint32_t now = millis();
  // Arama / eslesme / onay sirasinda ekran acik kalsin
  bool busy = Pair::modal != Pair::M_NONE || Pair::active || Scan::running || app.camera ||
              IR::sweeping || AutoRot::calStep || IRL::mode;
  if (busy) app.lastInput = now;

  uint32_t idle = now - app.lastInput;
  uint32_t saverMs = (uint32_t)SAVER_SEC[cfg.saverIdx] * 1000UL;

  // Ekran koruyucu: ayar aciksa ve bosta kalindiysa gozler baslar
  if (saverMs && !app.saver && !menu.open && !Obd::menu && !app.camera && !app.nightOff &&
      Pair::modal == Pair::M_NONE && app.chargeAnimUntil <= now && idle > saverMs)
    startSaver();
  if (busy) app.saver = false;

  if (app.nightOff)             { blTarget = 0; app.saver = false; }   // uyku zamanlayicisi bitti
  else if (app.saver)           blTarget = blFull();          // koruyucu acikken ekran kararmaz
  else if (idle > OFF_AFTER_MS) blTarget = 0;
  else if (idle > DIM_AFTER_MS) blTarget = min((uint8_t)BL_DIM, blFull());

  if (menu.open && idle > MENU_TIMEOUT_MS) menuClose();

  uint32_t sl = sleepAfter();
  if (sl && !bleConnected && idle > sl && !Obd::keepAwake()) goDeepSleep();

  if (app.dirtySave && now - app.lastChange > 3000) saveState();
}

// ============================================================================
//  ACILIS: WI-FI SENKRON EKRANI + KURULUM MODU
// ============================================================================
void drawWifiIcon(int cx, int cy, int lit, uint16_t on, uint16_t off) {
  TFT_eSPI& g = G();
  // lit: 0..3 yanan yay sayisi (animasyon)
  for (int i = 0; i < 3; i++) {
    int r = 12 + i * 11;
    g.drawSmoothArc(cx, cy, r, r - 5, 135, 225, i < lit ? on : off, C_BG, true);
  }
  g.fillCircle(cx, cy - 2, 4, lit > 0 ? on : off);
}

void rebootFor(uint8_t req) {
  rtcWx.bootReq = req;
  if (app.dirtySave) saveState();
  delay(50);
  ESP.restart();
}

// Bekleyen yeniden baslatma istegi (karaoke vb.)
void wifiService() {
  if (app.restartAt && (int32_t)(millis() - app.restartAt) >= 0) rebootFor(app.restartReq);
}

// ============================================================================
//  SETUP / LOOP
// ============================================================================
// ============================================================================
//  ACILIS LOGOSU — MGM BT VOLUME (kullanicinin logo gorseli, logo_img.h)
// ============================================================================
//  Bitmap RGB565 olarak flash'ta. Animasyon piksel bazli "gorunme" ile:
//   0.0-1.1 sn  renkli yay + olcek + dugme acisiyla soldan saga acilir
//   0.25-0.7 sn Bluetooth simgesi belirir
//   0.45-1.35   MGM harfleri soldan saga yumusak kenarla yazilir
//   1.45-1.95   "BT VOLUME" + dalga formu belirir
//   1.9-2.8     dalga formu cubuklari muzik varmis gibi oynar
//  Dikey ekranda logo ustte, 5 ikon (AUDIO, WEATHER, CLOCK, CAMERA, SHUTTER) altta.
#include "logo_img.h"

// Geometri: yatay (240x135) logo koordinatlari
static const float LG_CX = 116.73f, LG_CY = 120.29f, LG_R = 110.62f;
static const float LG_A0 = -178.85f, LG_A1 = -3.83f;           // yay uclari (derece)
static const float LG_MGM_X0 = 36.13f, LG_MGM_X1 = 200.0f, LG_TEXT_Y = 102.58f;
static const float LG_WX0 = 52.90f, LG_WY0 = 117.42f, LG_WX1 = 181.29f, LG_WY1 = 133.55f;

static inline float clamp01(float v) { return v < 0 ? 0 : (v > 1 ? 1 : v); }
static float easeOut(float t) { t = clamp01(t); return 1 - (1 - t) * (1 - t) * (1 - t); }

static inline uint16_t scale565(uint16_t c, float a) {
  if (a >= 1.0f) return c;
  if (a <= 0.0f) return 0;
  uint32_t k = (uint32_t)(a * 256.0f);
  uint32_t r = (((c >> 11) & 31) * k) >> 8, g = (((c >> 5) & 63) * k) >> 8, b = ((c & 31) * k) >> 8;
  return (uint16_t)((r << 11) | (g << 5) | b);
}

// Ekran tamponuna (sprite) dogrudan yaz (sprite 16 bit veriyi bayt-ters tutar)
static inline void putPx(uint16_t* buf, int x, int y, uint16_t c) {
  if (x < 0 || y < 0 || x >= SW || y >= SH) return;
  buf[y * SW + x] = (uint16_t)((c >> 8) | (c << 8));
}

// img: w x h bitmap; s: logo olcegi (1 = yatay 240x135); (ox, oy): ekrandaki konum
void drawLogoImg(uint16_t* buf, const uint16_t* img, int w, int h, int ox, int oy, float s, float t) {
  float sw   = easeOut(t / 1100.0f);
  float pm   = easeOut((t - 450.0f) / 900.0f);
  float edge = LG_MGM_X0 + (LG_MGM_X1 - LG_MGM_X0 + 14) * pm;
  float aBt  = clamp01((t - 250.0f) / 450.0f);
  float aTx  = clamp01((t - 1450.0f) / 500.0f);
  float env  = sinf(PI * clamp01((t - 1900.0f) / 900.0f));
  float rIn2 = (LG_R - 9) * (LG_R - 9);
  float wcy  = (LG_WY0 + LG_WY1) / 2;
  float inv  = 1.0f / s;
  for (int y = 0; y < h; y++) {
    float Y = y * inv;
    for (int x = 0; x < w; x++) {
      uint16_t c = pgm_read_word(&img[y * w + x]);
      if (!c) { putPx(buf, ox + x, oy + y, 0); continue; }
      float X = x * inv, dx = X - LG_CX, dy = Y - LG_CY, a;
      if (dx * dx + dy * dy > rIn2) {                         // yay / olcek / dugme
        float ang = atan2f(dy, dx) * RAD_TO_DEG;
        if (ang > 90) ang -= 360;
        float u = (ang - LG_A0) / (LG_A1 - LG_A0);
        a = clamp01((sw * 1.03f - u) * 25.0f);
      } else if (Y < 48) {                                    // Bluetooth simgesi
        a = aBt;
      } else if (Y < LG_TEXT_Y) {                             // MGM harfleri
        a = clamp01((edge - X) / 14.0f);
      } else {                                                // BT VOLUME + dalga
        a = aTx;
        if (env > 0 && Y >= LG_WY0 && X >= LG_WX0 && X <= LG_WX1) {
          float f  = 1.0f - 0.6f * env * (0.5f + 0.5f * sinf(X * 0.9f + t * 0.018f));
          float sy = wcy + (Y - wcy) / f;
          if (sy < LG_WY0 || sy > LG_WY1) c = 0;
          else { int yy = constrain((int)lroundf(sy * s), 0, h - 1); c = pgm_read_word(&img[yy * w + x]); }
        }
      }
      putPx(buf, ox + x, oy + y, scale565(c, a));
    }
  }
}

static const uint16_t* const LOGO_ICONS[5] = { LOGO_ICON0, LOGO_ICON1, LOGO_ICON2, LOGO_ICON3, LOGO_ICON4 };
static const char* const LOGO_ICON_TXT[5] = { "AUDIO", "WEATHER", "CLOCK", "CAMERA", "SHUTTER" };

void drawLogoFrame(float t) {
  TFT_eSPI& g = G();
  if (!useSprite) {                          // tampon yoksa: animasyonsuz son hal
    g.fillRect(0, 0, SW, SH, C_BG);
    g.setSwapBytes(true);                    // bitmap normal RGB565 sirasinda
    if (portrait) g.pushImage(0, 30, LOGOP_W, LOGOP_H, LOGO_PORT);
    else          g.pushImage(0, 0, LOGO_W, LOGO_H, LOGO_LAND);
    g.setSwapBytes(false);
    return;
  }
  uint16_t* buf = (uint16_t*)spr.getPointer();
  if (!portrait) {
    drawLogoImg(buf, LOGO_LAND, LOGO_W, LOGO_H, 0, 0, 1.0f, t);
    return;
  }
  g.fillRect(0, 0, SW, SH, C_BG);
  drawLogoImg(buf, LOGO_PORT, LOGOP_W, LOGOP_H, 0, 30, 135.0f / 240.0f, t);
  static const uint8_t PX[5] = {22, 67, 112, 45, 90}, PY[5] = {140, 140, 140, 195, 195};
  static const uint8_t PC[5][3] = {{0, 170, 255}, {0, 230, 150}, {160, 230, 0}, {255, 170, 0}, {255, 90, 20}};
  for (int k = 0; k < 5; k++) {
    float a = clamp01((t - 1500.0f - 150.0f * k) / 350.0f);
    if (a <= 0) continue;
    for (int y = 0; y < LOGO_ICON; y++)
      for (int x = 0; x < LOGO_ICON; x++)
        putPx(buf, PX[k] - 15 + x, PY[k] - 15 + y, scale565(pgm_read_word(&LOGO_ICONS[k][y * LOGO_ICON + x]), a));
    drawText(LOGO_ICON_TXT[k], PX[k], PY[k] + 24, MC_DATUM, 1,
             blend565(C_BG, tft.color565(PC[k][0], PC[k][1], PC[k][2]), a), C_BG);
  }
}

// quick: yeniden baslatmalarda (Wi-Fi guncellemesi vb.) animasyonsuz son kare
// ============================================================================
//  ACILIS LOGOSU 2 — "SADE": beyaz zemin, siyah geometrik MGM
// ============================================================================
//  Harfler hazir font degil: kalemle yaziliyormus gibi cizgi cizgi cizilen
//  geometrik bir logotip (yuvarlak uclu tek kalinlik). Akis:
//   0.0-0.35 sn  ekran beyaza acilir
//   0.25-0.7     ortadan yana ince ufuk cizgisi
//   0.6-1.6      M, G, M sirayla cizilir; harf araligi genisten dara kapanir
//   1.5-2.0      ufuk cizgisi soner, altta seyrek aralikli "BT VOLUME"
//   2.6-3.0      ekran yumusakca kararir (uygulama koyu temaya gecer)
namespace Logo2 {
  constexpr uint16_t WHITE = 0xFFFF, INK = 0x0000;

  static inline float cl(float x) { return x < 0 ? 0 : x > 1 ? 1 : x; }
  static inline float eo(float x) { x = cl(x); return 1 - (1 - x) * (1 - x) * (1 - x); }   // ease-out
  static inline float eio(float x) { x = cl(x); return x < 0.5f ? 4 * x * x * x : 1 - powf(-2 * x + 2, 3) / 2; }

  struct Geo { float H, w; int baseY, subY; };
  Geo geo() {
    if (portrait) return {30.0f, 5.0f, 116, 142};
    return {46.0f, 7.0f, 76, 104};
  }

  // Yuvarlak uclu kalin cizgi (kismi)
  static void seg(float x0, float y0, float x1, float y1, float f, float w) {
    if (f <= 0) return;
    float x = x0 + (x1 - x0) * cl(f), y = y0 + (y1 - y0) * cl(f);
    G().drawWideLine(x0, y0, x, y, w, INK, WHITE);
  }

  // M: sol dikme yukari, capraz asagi, capraz yukari, sag dikme asagi (tek kalem)
  static void drawM(float x, float top, float W, float H, float w, float p) {
    float h = w / 2, bot = top + H - h, tp = top + h, midY = top + H * 0.64f;
    float xl = x + h, xr = x + W - h, xm = x + W / 2;
    float L[4] = {bot - tp, hypotf(xm - xl, midY - tp), hypotf(xr - xm, midY - tp), bot - tp};
    float tot = L[0] + L[1] + L[2] + L[3], d = p * tot;
    seg(xl, bot, xl, tp, d / L[0], w);                 d -= L[0];
    seg(xl, tp, xm, midY, d / L[1], w);                d -= L[1];
    seg(xm, midY, xr, tp, d / L[2], w);                d -= L[2];
    seg(xr, tp, xr, bot, d / L[3], w);
  }

  // G: sag ustten saat yonunun tersine yay (310 derece) + ic yatay cizgi
  static void drawG(float x, float top, float H, float w, float p) {
    TFT_eSPI& g = G();
    float r = (H - w) / 2, cx = x + H / 2, cy = top + H / 2;
    const float A0 = 48, SPAN = 312;                   // matematik acisi (0 = sag, saat tersi)
    float arcLen = SPAN * DEG_TO_RAD * r, barLen = r * 0.95f;
    float tot = arcLen + barLen, d = p * tot;
    float fa = cl(d / arcLen);
    if (fa > 0) {
      float th = A0 + SPAN * fa;                       // kalemin su anki acisi
      int a0 = ((int)lroundf(270 - th) % 360 + 360) % 360;
      int a1 = ((int)lroundf(270 - A0) % 360 + 360) % 360;
      if (a0 != a1) g.drawArc((int)cx, (int)cy, (int)(r + w / 2), (int)(r - w / 2), a0, a1, INK, WHITE, true);
      // yuvarlak uclar
      float sx = cx + cosf(A0 * DEG_TO_RAD) * r, sy = cy - sinf(A0 * DEG_TO_RAD) * r;
      g.fillSmoothCircle((int)sx, (int)sy, (int)(w / 2), INK, WHITE);
      float ex = cx + cosf(th * DEG_TO_RAD) * r, ey = cy - sinf(th * DEG_TO_RAD) * r;
      g.fillSmoothCircle((int)ex, (int)ey, (int)(w / 2), INK, WHITE);
    }
    float fb = (d - arcLen) / barLen;                  // yatay cizgi: sagdan merkeze
    seg(cx + r, cy, cx + r - barLen, cy, fb, w);
  }

  // Seyrek aralikli kucuk yazi
  static void spaced(const char* s, int cx, int y, int track, uint16_t c) {
    TFT_eSPI& g = G();
    int tw = 0;
    for (const char* p = s; *p; p++) { char b[2] = {*p, 0}; tw += g.textWidth(b, 2) + track; }
    tw -= track;
    int x = cx - tw / 2;
    for (const char* p = s; *p; p++) {
      char b[2] = {*p, 0};
      drawText(b, x, y, TL_DATUM, 2, c, c);
      x += g.textWidth(b, 2) + track;
    }
  }

  void frame(float ms) {
    TFT_eSPI& g = G();
    g.fillRect(0, 0, SW, SH, WHITE);
    Geo o = geo();
    float H = o.H, w = o.w;
    float Wm = H * 0.98f, Wg = H;
    float gap0 = H * 0.20f, extra = H * (portrait ? 0.25f : 0.55f) * (1 - eo((ms - 600) / 1000.0f));
    float gap = gap0 + extra;
    float total = Wm * 2 + Wg + gap * 2;
    float x0 = SW / 2.0f - total / 2, top = o.baseY - H;

    // ufuk cizgisi: buyur, sonra soner
    float grow = eo((ms - 250) / 450.0f), fade = cl((ms - 1500) / 450.0f);
    if (grow > 0 && fade < 1) {
      int half = (int)(grow * (portrait ? 58 : 100));
      g.drawFastHLine(SW / 2 - half, o.baseY + (int)(H * 0.28f), half * 2, blend565(INK, WHITE, 0.55f + 0.45f * fade));
    }
    // harfler (kademeli)
    const float START = 600, STAG = 160, DUR = 620;
    drawM(x0, top, Wm, H, w, eio((ms - START) / DUR));
    drawG(x0 + Wm + gap, top, H, w, eio((ms - START - STAG) / DUR));
    drawM(x0 + Wm + gap + Wg + gap, top, Wm, H, w, eio((ms - START - 2 * STAG) / DUR));

    // alt yazi: seyrek harf araligi daralarak belirir
    float sub = eo((ms - 1550) / 600.0f);
    if (sub > 0) {
      int track = (int)(3 + 6 * (1 - sub));
      spaced("BT VOLUME", SW / 2, o.subY, track, blend565(WHITE, 0x8410, sub));
    }
    // imza: cok acik gri surum
    float sig = cl((ms - 2000) / 500.0f);
    if (sig > 0) {
      char v[20]; snprintf(v, sizeof(v), "v%s", FW_VERSION);
      drawText(v, SW / 2, SH - 6, BC_DATUM, 1, blend565(WHITE, 0xBDF7, sig), blend565(WHITE, 0xBDF7, sig));
    }
  }
}

void splash(bool quick) {
  if (cfg.logoStyle == 1) {                              // Sade: beyaz zemin, cizilen MGM
    if (quick) {
      Logo2::frame(4000);
      if (useSprite) spr.pushSprite(0, 0);
      blCurrent = blFull(); backlightWrite(blCurrent);
      delay(300);
      return;
    }
    uint32_t t0 = millis();
    bool chimed = false;
    for (;;) {
      uint32_t ms = millis() - t0;
      Logo2::frame((float)ms);
      if (useSprite) spr.pushSprite(0, 0);
      float bl = ms < 350 ? ms / 350.0f : ms > 2650 ? max(0.0f, 1 - (ms - 2650) / 350.0f) : 1.0f;
      blCurrent = (uint8_t)(bl * blFull()); backlightWrite(blCurrent);
      if (!chimed && ms > 1480) { chimed = true; Snd::logo(); }
      Snd::service();
      if (ms > 3000) break;
      delay(2);
    }
    Snd::service();
    return;                                              // isik 0: uygulama ekrani yumusakca acilir
  }
  if (quick) {
    drawLogoFrame(4000);
    if (useSprite) spr.pushSprite(0, 0);
    blCurrent = blFull(); backlightWrite(blCurrent);
    delay(250);
    return;
  }
  uint32_t t0 = millis();
  for (;;) {
    uint32_t ms = millis() - t0;
    drawLogoFrame((float)ms);
    if (useSprite) spr.pushSprite(0, 0);
    blCurrent = (uint8_t)(min(1.0f, ms / 300.0f) * blFull()); backlightWrite(blCurrent);
    if (ms > 2900) break;
    delay(2);
  }
  delay(400);
}

// ============================================================================
//  KARAOKE MODU — mikrofon -> Bluetooth hoparlor (A2DP kaynak)
// ============================================================================
//  Ayri bir acilista calisir (BOOT_KARAOKE): BLE ses kumandasi, ekran tamponu,
//  koruyucu, IR, otomatik dondurme ve buzzer KAPALI. Klasik Bluetooth yalnizca
//  A2DP icin baslatilir. Ses zinciri (ayri gorev, cekirdek 1):
//    PDM mikrofon 44.1 kHz -> DC filtresi -> gurultu kapisi -> kazanc -> eko
//    -> yumusak sinirlayici -> halka tampon -> A2DP (SBC, hoparlor)
//  Not: Bluetooth hoparlorlerin dogal gecikmesi ~150-300 ms'dir.
#include <esp_a2dp_api.h>
#include <esp_avrc_api.h>
#include <esp_bt_device.h>

namespace Kara {
  constexpr uint32_t RATE = 44100;
  constexpr int FR = 256;                        // mikrofon blogu (~5.8 ms)
  constexpr int RING = 4096;                     // halka tampon (~93 ms)
  constexpr int MAXSPK = 8;
  constexpr int ECHO_MAX_MS = 260;

  struct Spk { uint8_t bda[6]; char name[26]; int8_t rssi; };
  Spk          list[MAXSPK];
  volatile int nList = 0;
  volatile bool discovering = false;

  enum St : uint8_t { S_SCAN, S_CONNECTING, S_STREAM, S_LOST, S_ERROR };
  St       st = S_SCAN;
  uint32_t stAt = 0;
  int      sel = 0;
  uint8_t  target[6] = {0};
  char     targetName[26] = {0};
  bool     haveTarget = false;
  uint8_t  startTries = 0;
  char     errMsg[32] = {0};

  volatile bool evConnected = false, evDisconnected = false, evSrcRdy = false, evStarted = false, evStartFail = false;

  // Ses durumu
  int16_t*          ring = nullptr;        // karaokede ayrilir (normal modda 0 bayt)
  volatile uint32_t wIdx = 0, rIdx = 0;
  int16_t*          dly = nullptr;
  int               dlyLen = 0;
  volatile uint8_t  micLvl = 5, echoIdx = 2;
  volatile int      peak = 0;
  volatile uint32_t underruns = 0;
  static const uint16_t ECHO_MS[] = {0, 110, 180, ECHO_MAX_MS};
  static const uint8_t  ECHO_FB[] = {0, 25, 35, 45};      // geri besleme %
  static const char*    ECHO_TXT[] = {"Kapalı", "Hafif", "Orta", "Yoğun"};
  Preferences kp;
  bool     dirty = false;
  uint32_t dirtyAt = 0;

  // ---- Ses isleme gorevi ----------------------------------------------------
  void micTask(void*) {
    int16_t in[FR * 2];                                  // gorev yiginda (normal modda 0 bayt)
    const int CH = Mic::CHN;
    float px = 0, py = 0, env = 0, gate = 0, e0 = 0, e1 = 0;
    int ch = 0, dPos = 0;
    for (;;) {
      size_t got = Mic::readWait(in, FR * CH * 2, 30);
      int n = got / (2 * CH);
      if (n <= 0) continue;
      // Stereo okumada sinyal hangi kanaldaysa onu kullan (histerezisli)
      if (CH == 2) {
        float a0 = 0, a1 = 0;
        for (int i = 0; i < n; i += 4) { a0 += abs(in[2 * i]); a1 += abs(in[2 * i + 1]); }
        e0 += (a0 - e0) * 0.05f; e1 += (a1 - e1) * 0.05f;
        if (e1 > e0 * 1.5f + 10) ch = 1; else if (e0 > e1 * 1.5f + 10) ch = 0;
      }
      float gain = powf(1.38f, micLvl);                  // 1 .. ~25
      uint8_t ei = echoIdx;
      int len = ei ? min(dlyLen, (int)(ECHO_MS[ei] * RATE / 1000)) : 0;
      float fb = ECHO_FB[ei] / 100.0f;
      if (dPos >= len) dPos = 0;
      int pk = 0;
      for (int i = 0; i < n; i++) {
        float x = in[i * CH + ch];
        float y = x - px + 0.995f * py; px = x; py = y;           // DC / ugultu filtresi
        float a = fabsf(y);
        env = (a > env) ? env + (a - env) * 0.3f : env * 0.9997f;
        float gt = (env * gain > 900.0f) ? 1.0f : 0.08f;          // gurultu kapisi
        gate += (gt - gate) * (gt > gate ? 0.01f : 0.0004f);
        float v = y * gain * gate;
        if (len > 0) {                                           // eko (geri beslemeli gecikme)
          v += dly[dPos] * fb;
          float c = constrain(v, -32000.0f, 32000.0f);
          dly[dPos] = (int16_t)c;
          if (++dPos >= len) dPos = 0;
        }
        float av = fabsf(v);                                     // yumusak sinirlayici
        if (av > 20000.0f) v = (v > 0 ? 1 : -1) * (20000.0f + (av - 20000.0f) * 0.25f);
        int16_t o = (int16_t)constrain(v, -32767.0f, 32767.0f);
        ring[wIdx % RING] = o;
        wIdx++;
        if (abs(o) > pk) pk = abs(o);
      }
      peak = max(pk, peak * 7 / 8);
    }
  }

  // ---- A2DP veri istegi (BT gorevi) ---------------------------------------
  int32_t dataCb(uint8_t* buf, int32_t len) {
    if (!buf || len <= 0) return 0;
    int frames = len / 4;
    if (!ring) { memset(buf, 0, frames * 4); return frames * 4; }
    int16_t* o = (int16_t*)buf;
    uint32_t w = wIdx;
    if (w - rIdx > 2600) rIdx = w - 1200;                        // gecikme birikmesin
    for (int i = 0; i < frames; i++) {
      int16_t v = 0;
      if (rIdx != w) { v = ring[rIdx % RING]; rIdx++; } else underruns++;
      o[2 * i] = v; o[2 * i + 1] = v;
    }
    return frames * 4;
  }

  // ---- Bluetooth olaylari -------------------------------------------------
  void gapCb(esp_bt_gap_cb_event_t ev, esp_bt_gap_cb_param_t* p) {
    switch (ev) {
      case ESP_BT_GAP_DISC_RES_EVT: {
        uint32_t cod = 0; int8_t rssi = -127; char nm[26] = {0};
        for (int i = 0; i < p->disc_res.num_prop; i++) {
          esp_bt_gap_dev_prop_t& pr = p->disc_res.prop[i];
          if (pr.type == ESP_BT_GAP_DEV_PROP_COD && pr.len >= 4) cod = *(uint32_t*)pr.val;
          else if (pr.type == ESP_BT_GAP_DEV_PROP_RSSI) rssi = *(int8_t*)pr.val;
          else if (pr.type == ESP_BT_GAP_DEV_PROP_BDNAME && pr.len > 0) toAscii((uint8_t*)pr.val, pr.len, nm, sizeof(nm));
          else if (pr.type == ESP_BT_GAP_DEV_PROP_EIR && !nm[0]) {
            uint8_t l = 0;
            uint8_t* d = esp_bt_gap_resolve_eir_data((uint8_t*)pr.val, ESP_BT_EIR_TYPE_CMPL_LOCAL_NAME, &l);
            if (!d) d = esp_bt_gap_resolve_eir_data((uint8_t*)pr.val, ESP_BT_EIR_TYPE_SHORT_LOCAL_NAME, &l);
            if (d && l) toAscii(d, l, nm, sizeof(nm));
          }
        }
        bool audio = esp_bt_gap_get_cod_major_dev(cod) == ESP_BT_COD_MAJOR_DEV_AV ||
                     (esp_bt_gap_get_cod_srvc(cod) & ESP_BT_COD_SRVC_RENDERING);
        if (!audio) break;
        for (int i = 0; i < nList; i++)
          if (!memcmp(list[i].bda, p->disc_res.bda, 6)) {
            if (nm[0] && !list[i].name[0]) strcpy(list[i].name, nm);
            list[i].rssi = rssi; return;
          }
        if (nList >= MAXSPK) break;
        Spk& s = list[nList];
        memcpy(s.bda, p->disc_res.bda, 6); strcpy(s.name, nm); s.rssi = rssi;
        nList = nList + 1;
        break;
      }
      case ESP_BT_GAP_DISC_STATE_CHANGED_EVT:
        discovering = (p->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED);
        break;
      case ESP_BT_GAP_PIN_REQ_EVT: {                            // eski hoparlorler: 0000
        esp_bt_pin_code_t pin = {'0', '0', '0', '0'};
        esp_bt_gap_pin_reply(p->pin_req.bda, true, 4, pin);
        break;
      }
      case ESP_BT_GAP_CFM_REQ_EVT:
        esp_bt_gap_ssp_confirm_reply(p->cfm_req.bda, true);
        break;
      default: break;
    }
  }

  void a2dCb(esp_a2d_cb_event_t ev, esp_a2d_cb_param_t* p) {
    if (ev == ESP_A2D_CONNECTION_STATE_EVT) {
      if (p->conn_stat.state == ESP_A2D_CONNECTION_STATE_CONNECTED) evConnected = true;
      else if (p->conn_stat.state == ESP_A2D_CONNECTION_STATE_DISCONNECTED) evDisconnected = true;
    } else if (ev == ESP_A2D_MEDIA_CTRL_ACK_EVT) {
      bool ok = p->media_ctrl_stat.status == ESP_A2D_MEDIA_CTRL_ACK_SUCCESS;
      if (p->media_ctrl_stat.cmd == ESP_A2D_MEDIA_CTRL_CHECK_SRC_RDY) { if (ok) evSrcRdy = true; else evStartFail = true; }
      else if (p->media_ctrl_stat.cmd == ESP_A2D_MEDIA_CTRL_START)   { if (ok) evStarted = true; else evStartFail = true; }
    }
  }
  void avrcCb(esp_avrc_ct_cb_event_t, esp_avrc_ct_cb_param_t*) {}

  bool btStart() {
    esp_bt_controller_mem_release(ESP_BT_MODE_BLE);             // BLE bellegi karaokede gereksiz
    esp_bt_controller_config_t bc = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    bc.mode = ESP_BT_MODE_CLASSIC_BT;
    if (esp_bt_controller_init(&bc) != ESP_OK) return false;
    if (esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT) != ESP_OK) return false;
    if (esp_bluedroid_init() != ESP_OK || esp_bluedroid_enable() != ESP_OK) return false;
#if ESP_IDF_VERSION_MAJOR >= 5
    esp_bt_gap_set_device_name(DEVICE_NAME " Karaoke");
#else
    esp_bt_dev_set_device_name(DEVICE_NAME " Karaoke");
#endif
    esp_bt_gap_register_callback(gapCb);
    esp_bt_sp_param_t pt = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t io = ESP_BT_IO_CAP_NONE;
    esp_bt_gap_set_security_param(pt, &io, sizeof(io));
    esp_avrc_ct_init();
    esp_avrc_ct_register_callback(avrcCb);
    esp_a2d_register_callback(a2dCb);
    esp_a2d_source_register_data_callback(dataCb);
    if (esp_a2d_source_init() != ESP_OK) return false;
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
    return true;
  }

  void setSt(St s) { st = s; stAt = millis(); }
  void startScan() {
    nList = 0; sel = 0;
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);   // ~13 sn
    setSt(S_SCAN);
  }
  void connectTo(const uint8_t* bda, const char* nm) {
    if (discovering) { esp_bt_gap_cancel_discovery(); delay(250); }
    memcpy(target, bda, 6); haveTarget = true;
    strncpy(targetName, nm && nm[0] ? nm : "Hoparlör", sizeof(targetName) - 1);
    startTries = 0;
    esp_a2d_source_connect(target);
    setSt(S_CONNECTING);
  }
  void saveTarget() {
    kp.begin("karaoke", false);
    kp.putBytes("spk", target, 6);
    kp.putString("spkn", targetName);
    kp.end();
  }
  void saveLevels() {
    kp.begin("karaoke", false);
    kp.putUChar("mic", micLvl); kp.putUChar("echo", echoIdx);
    kp.end();
    dirty = false;
  }
  void loadPrefs() {
    kp.begin("karaoke", true);
    micLvl  = min((int)kp.getUChar("mic", 5), 10);
    echoIdx = kp.getUChar("echo", 2) % 4;
    haveTarget = kp.getBytes("spk", target, 6) == 6;
    String n = kp.getString("spkn", "");
    strncpy(targetName, n.c_str(), sizeof(targetName) - 1);
    kp.end();
  }

  // ---- Olaylari isle (ana dongu) --------------------------------------------
  void service() {
    uint32_t now = millis();
    if (evConnected) {
      evConnected = false;
      saveTarget();
      esp_a2d_media_ctrl(ESP_A2D_MEDIA_CTRL_CHECK_SRC_RDY);
      stAt = now;
    }
    if (evSrcRdy)   { evSrcRdy = false; esp_a2d_media_ctrl(ESP_A2D_MEDIA_CTRL_START); }
    if (evStarted)  { evStarted = false; setSt(S_STREAM); Serial.printf("[Karaoke] yayin basladi, bos RAM %u\n", (unsigned)ESP.getFreeHeap()); }
    if (evStartFail) {
      evStartFail = false;
      if (++startTries <= 4) { delay(300); esp_a2d_media_ctrl(ESP_A2D_MEDIA_CTRL_CHECK_SRC_RDY); }
    }
    if (evDisconnected) {
      evDisconnected = false;
      if (st == S_STREAM || st == S_CONNECTING) setSt(S_LOST);
    }
    if (st == S_CONNECTING && now - stAt > 15000) startScan();      // olmadi: hoparlor listesi
    if (st == S_LOST && now - stAt > 3000 && haveTarget) {         // koptu: tekrar dene
      startTries = 0; esp_a2d_source_connect(target); setSt(S_CONNECTING);
    }
    if (dirty && now - dirtyAt > 2000) saveLevels();
  }

  void changeLevel(int d) {
    micLvl = constrain((int)micLvl + d, 0, 10);
    dirty = true; dirtyAt = millis();
  }

  // ---- Tuslar -----------------------------------------------------------------
  void exitToNormal() {
    if (dirty) saveLevels();
    tft.fillScreen(C_BG);
    drawText("Normal moda dönülüyor...", SW / 2, SH / 2, MC_DATUM, 2, C_TEXT2, C_BG);
    if (st == S_STREAM || st == S_CONNECTING) esp_a2d_source_disconnect(target);
    delay(400);
    rebootFor(BOOT_NORMAL);
  }

  void buttons() {
    static bool pu = false, pd = false, combo = false;
    btnUp.update(); btnDn.update();
    bool u = btnUp.down, d = btnDn.down;
    uint32_t now = millis();
    if (HW::powerPressed()) { userActivity(); exitToNormal(); }
    if (u || d) userActivity();
    if (u && d) combo = true;
    bool relU = pu && !u, relD = pd && !d;
    pu = u; pd = d;
    if (!relU && !relD) return;
    if (combo) {                                                  // iki tus: eko
      if (!u && !d) {
        combo = false;
        if (st != S_SCAN) { echoIdx = (echoIdx + 1) % 4; dirty = true; dirtyAt = now; }
      }
      return;
    }
    bool lng = relU ? (now - btnUp.downAt > 700) : (now - btnDn.downAt > 700);
    if (st == S_SCAN || st == S_ERROR) {
      if (relD) { if (nList) sel = (sel + 1) % nList; }
      else if (lng) startScan();
      else if (nList && sel < nList) connectTo(list[sel].bda, list[sel].name);
      return;
    }
    if (st == S_CONNECTING && relD) {                              // iptal -> liste
      esp_a2d_source_disconnect(target);
      startScan(); return;
    }
    if (relU && lng) {                                            // hoparlor degistir
      esp_a2d_source_disconnect(target);
      startScan(); return;
    }
    changeLevel(relU ? +1 : -1);
  }

  // ---- Ekran (tamponsuz; yalnizca degisince cizilir) ------------------------
  void drawVu() {
    TFT_eSPI& g = G();
    int x = 10, w = SW - 20, h = portrait ? 12 : 10;
    int y = portrait ? 150 : 88;
    float f = min(1.0f, peak / 26000.0f);
    int fw = (int)(w * f);
    g.fillRoundRect(x, y, w, h, 3, C_TRACK);
    if (fw > 2) g.fillRoundRect(x, y, fw, h, 3, f > 0.85f ? C_BAD : f > 0.6f ? C_WARN : C_OK);
  }

  void drawFull() {
    TFT_eSPI& g = G();
    g.fillScreen(C_BG);
    drawText("KARAOKE", 8, 12, ML_DATUM, 2, C_MENU, C_BG);
    drawBattery(SW - 34, 5);
    g.drawFastHLine(0, 23, SW, C_TRACK);
    char b[40];
    switch (st) {
      case S_SCAN: {
        snprintf(b, sizeof(b), discovering ? "Hoparlör aranıyor (%d)" : "%d hoparlör bulundu", nList);
        drawText(b, 8, 34, ML_DATUM, 2, discovering ? C_ACCENT : C_TEXT2, C_BG, SW - 12);
        int rowH = portrait ? 22 : 18, top = 48;
        int vis = max(1, ((SH - (portrait ? 40 : 14)) - top) / rowH);
        int first = max(0, sel - vis + 1);
        for (int r = 0; r < vis && first + r < nList; r++) {
          int i = first + r, y = top + r * rowH;
          bool s = (i == sel);
          if (s) g.fillRoundRect(3, y, SW - 6, rowH - 2, 5, C_PANEL);
          char nm[26];
          if (list[i].name[0]) strcpy(nm, list[i].name); else fmtAddr(list[i].bda, nm, sizeof(nm));
          drawText(nm, 10, y + rowH / 2 - 1, ML_DATUM, 2, s ? C_TEXT : C_TEXT2, s ? C_PANEL : C_BG, SW - 20);
        }
        if (!nList && !discovering) drawText("Hoparlörü eşleşme moduna alın", SW / 2, 70, MC_DATUM, 1, C_SUBTEXT, C_BG);
        if (portrait) {
          drawText("B: sonraki  A: bağlan", SW / 2, SH - 24, MC_DATUM, 1, C_SUBTEXT, C_BG);
          drawText("A uzun: yeniden ara", SW / 2, SH - 13, MC_DATUM, 1, C_SUBTEXT, C_BG);
          drawText("GÜÇ: çıkış", SW / 2, SH - 3, BC_DATUM, 1, C_SUBTEXT, C_BG);
        } else drawText("B sonraki  A bağlan (uzun: ara)  GÜÇ çıkış", SW / 2, SH, BC_DATUM, 1, C_SUBTEXT, C_BG);
        return;
      }
      case S_ERROR:
        drawText("Bluetooth başlatılamadı", SW / 2, SH / 2 - 10, MC_DATUM, 2, C_BAD, C_BG, SW - 8);
        drawText(errMsg, SW / 2, SH / 2 + 12, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 8);
        drawText("GÜÇ: çıkış", SW / 2, SH - 3, BC_DATUM, 1, C_SUBTEXT, C_BG);
        return;
      default: break;
    }
    // Baglaniyor / yayin / koptu
    uint16_t sc = st == S_STREAM ? C_OK : st == S_LOST ? C_WARN : C_ACCENT;
    const char* stTxt = st == S_STREAM ? "Yayında" : st == S_LOST ? "Bağlantı koptu..." : "Bağlanıyor...";
    drawText(targetName, SW / 2, portrait ? 40 : 36, MC_DATUM, 2, C_TEXT, C_BG, SW - 8);
    drawText(stTxt, SW / 2, portrait ? 60 : 54, MC_DATUM, 2, sc, C_BG);
    // Mikrofon seviyesi (buyuk) + eko
    snprintf(b, sizeof(b), "%d", micLvl);
    int my = portrait ? 104 : 72;
    drawText("MİK", portrait ? SW / 2 : 30, portrait ? 80 : my, MC_DATUM, 2, C_SUBTEXT, C_BG);
    drawText(b, portrait ? SW / 2 : 70, portrait ? my + 8 : my, MC_DATUM, 4, C_TEXT, C_BG);
    snprintf(b, sizeof(b), "Eko: %s", ECHO_TXT[echoIdx]);
    drawText(b, portrait ? SW / 2 : SW - 50, portrait ? 176 : my, MC_DATUM, 2, echoIdx ? C_MENU : C_SUBTEXT, C_BG);
    drawVu();
    if (portrait) {
      drawText("A/B: mikrofon +/-", SW / 2, SH - 34, MC_DATUM, 1, C_SUBTEXT, C_BG);
      drawText("A+B: eko", SW / 2, SH - 23, MC_DATUM, 1, C_SUBTEXT, C_BG);
      drawText("A uzun: hoparlör  GÜÇ: çıkış", SW / 2, SH - 12, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 4);
    } else {
      drawText("A/B mik. +/-   A+B eko", SW / 2, SH - 20, MC_DATUM, 1, C_SUBTEXT, C_BG);
      drawText("A uzun: hoparlör değiştir   GÜÇ: çıkış", SW / 2, SH - 8, MC_DATUM, 1, C_SUBTEXT, C_BG);
    }
  }

  void ui() {
    static uint32_t sig = 0xFFFFFFFF, lastVu = 0, lastBatt = 0;
    static int bp = 0;
    uint32_t now = millis();
    if (now - lastBatt > 20000) { lastBatt = now; bp = app.batteryPct; }   // pil: 20 sn'de bir
    uint32_t h = st * 131u + nList * 17u + sel * 7u + micLvl * 1009u + echoIdx * 3u + (discovering ? 5u : 0u) + bp * 7919u;
    for (const char* c = targetName; *c; c++) h = h * 31u + (uint8_t)*c;
    if (h != sig) { sig = h; drawFull(); }
    if (st != S_SCAN && st != S_ERROR && now - lastVu >= 40) { lastVu = now; drawVu(); }
  }

  void run() {
    Snd::muteAll = true;                       // buzzer sessiz: mikrofona girmesin
    spr.deleteSprite(); useSprite = false;     // ekran tamponu yok -> +64 KB RAM
    tft.fillScreen(C_BG);
    blTarget = blFull();
    loadPrefs();
    Serial.printf("[Karaoke] basliyor, bos RAM %u\n", (unsigned)ESP.getFreeHeap());
    batteryService();

    if (!btStart()) { strcpy(errMsg, "(yeniden deneyin)"); setSt(S_ERROR); }
    else {
      ring = (int16_t*)calloc(RING, sizeof(int16_t));
      dlyLen = ECHO_MAX_MS * RATE / 1000;
      dly = (int16_t*)calloc(dlyLen, sizeof(int16_t));
      if (!dly) { dlyLen = 0; echoIdx = 0; }
      if (!ring) { strcpy(errMsg, "RAM yetersiz"); setSt(S_ERROR); }
      else if (!Mic::startCfg(RATE, 4, FR)) { strcpy(errMsg, "Mikrofon açılamadı"); setSt(S_ERROR); }
      else {
        xTaskCreatePinnedToCore(micTask, "karaoke", 6144, nullptr, 5, nullptr, 1);
        if (haveTarget) connectTo(target, targetName); else startScan();
      }
      Serial.printf("[Karaoke] hazir, bos RAM %u (eko %s)\n", (unsigned)ESP.getFreeHeap(), dly ? "var" : "yok");
    }
    app.lastInput = millis();
    for (;;) {
      buttons();
      if (st != S_ERROR) service();
      batteryService();
      uint32_t idle = millis() - app.lastInput;
      blTarget = idle > 30000 ? min((uint8_t)BL_DIM, blFull()) : blFull();   // yayindayken ekran kisilir
      backlightService();
      ui();
      delay(5);
    }
  }
}

// ============================================================================
//  OBD2 MODU — ELM327 (Bluetooth Klasik / SPP) arac verileri
// ============================================================================
//  Ayri bir acilista calisir (BOOT_OBD): ses kumandasi (BLE), koruyucu, IR,
//  mikrofon, Wi-Fi kapali. Klasik Bluetooth yalnizca ELM327 icin acilir.
//  Istek/yanit tamamen beklemesiz: ekran ve tuslar hep akici kalir.
//
//  Sayfalar (B: sonraki sayfa, B uzun: onceki):
//   1 Panel     : 4 kutu (Ayarlar'dan secilir)
//   2 Odak      : tek buyuk deger + cubuk + min/maks (A: sonraki veri)
//   3 Grafik    : odak verisinin son ~60 sn grafigi   (A: sonraki veri)
//   4 Yolculuk  : sure, mesafe, ortalama/maks hiz, yakit (A uzun: sifirla)
//   5 Ariza     : MIL lambasi + ariza kodlari (A: oku, A uzun x2: sil)
//  GUC tusu: OBD ayarlari (kutular, yakit tipi, alarmlar, adaptor, cikis)
#define OBD_SUPPORTED 1

namespace Obd {
  // ---- BLE GATT istemcisi (ELM327 BLE: Vgate iCar Pro 2S, vLinker, OBDII...) --------
  //  Ses kumandasi (BLE HID sunucu) ile ayni anda calisir: ek klasik BT yok, az RAM.
  //  Servis/karakteristik sabit degil: bildirim (notify) + yazma ozelligi olan ilk
  //  standart disi servis secilir (FFF0/FFF1/FFF2, FFE0/FFE1, 18F0/2AF0/2AF1 ...).
  struct DevT { uint8_t mac[6]; char name[24]; int8_t rssi; bool elm; uint8_t atype; };
  void onFound(const uint8_t* mac, const char* name, int8_t rssi, uint8_t atype);
  namespace Link {
    constexpr uint16_t APP_ID = 0x0B;
    enum St : uint8_t { L_IDLE, L_CONNECTING, L_DISCOVER, L_OPEN, L_FAIL };
    volatile St link = L_IDLE;
    volatile bool scanning = false, wantScan = false;
    esp_gatt_if_t gif = ESP_GATT_IF_NONE;
    uint16_t conn = 0xFFFF, hNotify = 0, hWrite = 0; bool writeNr = false, indicate = false;
    uint8_t  peer[6] = {0};
    struct Rng { uint16_t s, e, u16; } svc[10]; uint8_t nSvc = 0;
    uint8_t* rb = nullptr; volatile uint16_t rbH = 0, rbT = 0;   // 512 baytlik halka tampon
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

    bool stdSvc(uint16_t u) { return u == 0x1800 || u == 0x1801 || u == 0x180A || u == 0x180F || u == 0x1812 || u == 0xFE59; }
    bool prefSvc(uint16_t u) { return u == 0xFFF0 || u == 0xFFE0 || u == 0x18F0; }
    // Veri kanalini sec: ayni serviste bildirim + yazma
    bool pickChars() {
      for (int pass = 0; pass < 2; pass++)
        for (int i = 0; i < nSvc; i++) {
          if (stdSvc(svc[i].u16) || (pass == 0) != prefSvc(svc[i].u16)) continue;
          uint16_t cnt = 0;
          esp_ble_gattc_get_attr_count(gif, conn, ESP_GATT_DB_CHARACTERISTIC, svc[i].s, svc[i].e, 0, &cnt);
          if (!cnt) continue;
          esp_gattc_char_elem_t el[8]; if (cnt > 8) cnt = 8;
          if (esp_ble_gattc_get_all_char(gif, conn, svc[i].s, svc[i].e, el, &cnt, 0) != ESP_GATT_OK) continue;
          uint16_t n = 0, w = 0; bool nr = false, ind = false;
          for (int k = 0; k < cnt; k++) {
            uint8_t pr = el[k].properties;
            if (!n && (pr & (ESP_GATT_CHAR_PROP_BIT_NOTIFY | ESP_GATT_CHAR_PROP_BIT_INDICATE))) { n = el[k].char_handle; ind = !(pr & ESP_GATT_CHAR_PROP_BIT_NOTIFY); }
            if (!w && (pr & (ESP_GATT_CHAR_PROP_BIT_WRITE_NR | ESP_GATT_CHAR_PROP_BIT_WRITE))) { w = el[k].char_handle; nr = pr & ESP_GATT_CHAR_PROP_BIT_WRITE_NR; }
          }
          if (n && w) { hNotify = n; hWrite = w; writeNr = nr; indicate = ind; return true; }
        }
      return false;
    }
    void onGattc(esp_gattc_cb_event_t e, esp_gatt_if_t ifc, esp_ble_gattc_cb_param_t* p) {
      if (e == ESP_GATTC_REG_EVT) { if (p->reg.app_id == APP_ID && p->reg.status == ESP_GATT_OK) gif = ifc; return; }
      if (ifc != gif || gif == ESP_GATT_IF_NONE) return;
      switch (e) {
        case ESP_GATTC_OPEN_EVT:
          if (memcmp(p->open.remote_bda, peer, 6)) break;
          if (p->open.status != ESP_GATT_OK) { link = L_FAIL; break; }
          conn = p->open.conn_id; nSvc = 0; link = L_DISCOVER;
          if (esp_ble_gattc_search_service(gif, conn, nullptr) != ESP_OK) link = L_FAIL;
          break;
        case ESP_GATTC_SEARCH_RES_EVT:
          if (nSvc < 10) {
            svc[nSvc].s = p->search_res.start_handle; svc[nSvc].e = p->search_res.end_handle;
            svc[nSvc].u16 = p->search_res.srvc_id.uuid.len == ESP_UUID_LEN_16 ? p->search_res.srvc_id.uuid.uuid.uuid16 : 0;
            nSvc++;
          }
          break;
        case ESP_GATTC_SEARCH_CMPL_EVT:
          if (link != L_DISCOVER) break;
          if (!pickChars()) { link = L_FAIL; esp_ble_gattc_close(gif, conn); break; }
          esp_ble_gattc_register_for_notify(gif, peer, hNotify);
          break;
        case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
          esp_bt_uuid_t u; u.len = ESP_UUID_LEN_16; u.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
          esp_gattc_descr_elem_t d; uint16_t c = 1;
          if (esp_ble_gattc_get_descr_by_char_handle(gif, conn, p->reg_for_notify.handle, u, &d, &c) == ESP_GATT_OK && c) {
            uint8_t v[2] = {(uint8_t)(indicate ? 2 : 1), 0};
            esp_ble_gattc_write_char_descr(gif, conn, d.handle, 2, v, ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_NONE);
          } else link = L_OPEN;
          break;
        }
        case ESP_GATTC_WRITE_DESCR_EVT: if (link == L_DISCOVER) link = L_OPEN; break;
        case ESP_GATTC_NOTIFY_EVT:
          if (rb && p->notify.handle == hNotify) {
            portENTER_CRITICAL(&mux);
            for (int i = 0; i < p->notify.value_len; i++) {
              uint16_t n = (rbH + 1) & 511;
              if (n == rbT) break;                                   // dolu: fazlasini at
              rb[rbH] = p->notify.value[i]; rbH = n;
            }
            portEXIT_CRITICAL(&mux);
          }
          break;
        case ESP_GATTC_DISCONNECT_EVT:
        case ESP_GATTC_CLOSE_EVT:
          if (link != L_IDLE) link = (link == L_OPEN) ? L_IDLE : L_FAIL;
          conn = 0xFFFF;
          break;
        default: break;
      }
    }
    // BLE tarama sonuclari (Pair::gapHandler'dan gelir)
    void onGap(esp_gap_ble_cb_event_t ev, esp_ble_gap_cb_param_t* p) {
      switch (ev) {
        case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
          if (wantScan) { wantScan = false; esp_ble_gap_start_scanning(8); scanning = true; }
          break;
        case ESP_GAP_BLE_SCAN_RESULT_EVT:
          if (!scanning) break;
          if (p->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT) {
            uint8_t l = 0; char nm[24] = {0};
            uint8_t* d = esp_ble_resolve_adv_data(p->scan_rst.ble_adv, ESP_BLE_AD_TYPE_NAME_CMPL, &l);
            if (!d) d = esp_ble_resolve_adv_data(p->scan_rst.ble_adv, ESP_BLE_AD_TYPE_NAME_SHORT, &l);
            if (d && l) { toAscii(d, l, nm, sizeof(nm)); onFound(p->scan_rst.bda, nm, p->scan_rst.rssi, p->scan_rst.ble_addr_type); }
          } else if (p->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_CMPL_EVT) scanning = false;
          break;
        case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT: scanning = false; break;
        default: break;
      }
    }
    bool begin() {
      if (!rb) rb = (uint8_t*)malloc(512);
      if (!rb) return false;
      if (gif == ESP_GATT_IF_NONE) esp_ble_gattc_app_register(APP_ID);
      return true;
    }
    void scan(bool on) {
      if (on) {
        esp_ble_scan_params_t sp = {};
        sp.scan_type = BLE_SCAN_TYPE_ACTIVE; sp.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
        sp.scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL; sp.scan_interval = 0x50; sp.scan_window = 0x30;
        sp.scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE;
        wantScan = true;
        if (esp_ble_gap_set_scan_params(&sp) != ESP_OK) wantScan = false;
      } else if (scanning) { esp_ble_gap_stop_scanning(); scanning = false; }
    }
    void connect(const uint8_t* mac, uint8_t atype) {
      if (scanning) { esp_ble_gap_stop_scanning(); scanning = false; delay(100); }
      memcpy(peer, mac, 6);
      rbH = rbT = 0; hNotify = hWrite = 0;
      link = L_CONNECTING;
      if (gif == ESP_GATT_IF_NONE || esp_ble_gattc_open(gif, peer, (esp_ble_addr_type_t)atype, true) != ESP_OK) link = L_FAIL;
    }
    void close() { if (conn != 0xFFFF && gif != ESP_GATT_IF_NONE) esp_ble_gattc_close(gif, conn); link = L_IDLE; }
    bool open() { return link == L_OPEN; }
    int  available() { return (rbH - rbT) & 511; }
    int  read() {
      if (rbH == rbT) return -1;
      portENTER_CRITICAL(&mux); uint8_t c = rb[rbT]; rbT = (rbT + 1) & 511; portEXIT_CRITICAL(&mux);
      return c;
    }
    void write(const char* s) {
      if (conn == 0xFFFF || !hWrite) return;
      esp_ble_gattc_write_char(gif, conn, hWrite, strlen(s), (uint8_t*)s,
                               writeNr ? ESP_GATT_WRITE_TYPE_NO_RSP : ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_NONE);
    }
  }

  // ---- Veri katalogu ------------------------------------------------------------
  enum Pid : uint8_t {
    V_RPM, V_SPEED, V_COOLANT, V_BATT, V_LOAD, V_THROTTLE, V_IAT, V_MAP, V_BOOST, V_MAF,
    V_FUELLVL, V_LPH, V_L100, V_TIMING, V_OIL, V_AMBIENT, V_ECUV, V_STFT, V_LTFT, V_BARO,
    V_RUNTIME, V_DISTCLR, V_COUNT
  };
  struct Def { uint8_t pid; const char* name; const char* unit; uint8_t dec; float lo, hi; uint16_t periodMs; };
  //  pid: 0 = hesaplanan/AT komutu
  static const Def DEFS[V_COUNT] = {
    {0x0C, "Devir",        "rpm",   0,    0, 7000,  120},
    {0x0D, "Hız",          "km/sa", 0,    0, 200,   200},
    {0x05, "Su sıcaklığı", "°C",    0,   40, 120,  2000},
    {0x00, "Akü",          "V",     1,   11, 15,   3000},
    {0x04, "Motor yükü",   "%",     0,    0, 100,   250},
    {0x11, "Gaz pedalı",   "%",     0,    0, 100,   200},
    {0x0F, "Emme havası",  "°C",    0,  -10, 80,   2500},
    {0x0B, "Emme basıncı", "kPa",   0,    0, 250,   250},
    {0x00, "Turbo",        "bar",   2, -0.9f, 1.8f,  250},
    {0x10, "Hava akışı",   "g/s",   1,    0, 200,   250},
    {0x2F, "Yakıt seviyesi","%",    0,    0, 100,  5000},
    {0x00, "Yakıt anlık",  "L/sa",  1,    0, 20,    300},
    {0x00, "Tüketim",      "L/100", 1,    0, 20,    300},
    {0x0E, "Ateşleme avansı","°",   1,  -10, 50,    400},
    {0x5C, "Yağ sıcaklığı","°C",    0,   40, 140,  2500},
    {0x46, "Dış hava",     "°C",    0,  -20, 50,   5000},
    {0x42, "ECU voltajı",  "V",     1,   11, 15,   3000},
    {0x06, "Kısa yakıt ay.","%",    1,  -25, 25,    500},
    {0x07, "Uzun yakıt ay.","%",    1,  -25, 25,   2000},
    {0x33, "Atm. basınç",  "kPa",   0,   70, 105,  8000},
    {0x1F, "Motor süresi", "dk",    0,    0, 600,  5000},
    {0x31, "Silmeden beri","km",    0,    0, 9999, 8000},
  };
  static const char* FUEL_TXT[] = {"Benzin", "Dizel", "LPG"};
  static const uint8_t DISP10[] = {10, 12, 13, 14, 15, 16, 18, 20, 25, 30};   // motor hacmi x0.1 L
  static const uint8_t HOT_C[]   = {0, 100, 105, 110};
  static const uint16_t SHIFT[]  = {0, 3000, 3500, 4000, 5000, 6000};
  static const uint8_t SPD[]     = {0, 50, 82, 90, 110, 120, 140};
  static const uint8_t TANK_L[]  = {30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80};   // depo hacmi (L)
  static const uint8_t SLEEP_OBD[] = {0, 2, 5, 10};                   // otomatik uyku (dk), 0 = kapali
  float best100 = 0, best402 = 0;                                      // en iyi hizlanma dereceleri (sn)
  static const char* OSTYLE_TXT[] = {"Sportif", "Konfor", "Klasik kadran", "Dijital köşeli", "Kırmızı spor"};

  // ---- Ayarlar (NVS "obd") ------------------------------------------------------
  struct OCfg {
    uint8_t tile[4] = {V_SPEED, V_RPM, V_COOLANT, V_BATT};
    uint8_t focus = V_RPM;
    uint8_t fuel = 0, disp = 5, hot = 2, shift = 0, spd = 0;
    uint8_t style = 0;                   // 0 Sportif, 1 Konfor, 2-4 kokpit (K1 Klasik, K2 Dijital, K3 Kirmizi)
    uint8_t tank = 4;                    // TANK_L indeksi (50 L)
    uint8_t sleep = 0;                   // SLEEP_OBD indeksi
    uint8_t price = 50;                  // yakit fiyati (TL/L)
    uint32_t mask = (1UL << V_SPEED) | (1UL << V_RPM) | (1UL << V_COOLANT) | (1UL << V_BOOST) |
                    (1UL << V_L100) | (1UL << V_BATT) | (1UL << V_OIL) | (1UL << V_FUELLVL);
    uint8_t mac[6] = {0}; bool haveMac = false; uint8_t atype = 0;
    bool enabled = false;                // OBD2 surus asistani acik
    char    name[24] = {0};
  } oc;
  Preferences op;
  namespace Health { bool fastVolt(); void onVolt(float v); void onKm(float d); void load(); void save(); void service(); }
  void loadCfg() {
    op.begin("obd", true);
    op.getBytes("tile", oc.tile, 4);
    oc.focus = op.getUChar("focus", V_RPM) % V_COUNT;
    oc.fuel = op.getUChar("fuel", 0) % 3;  oc.disp = op.getUChar("disp", 5) % COUNT_OF(DISP10);
    oc.hot = op.getUChar("hot", 2) % 4;    oc.shift = op.getUChar("shift", 0) % COUNT_OF(SHIFT);
    oc.spd = op.getUChar("spd", 0) % COUNT_OF(SPD);
    oc.style = op.getUChar("style", 0) % 5;
    oc.tank = op.getUChar("tank", 4) % COUNT_OF(TANK_L);
    oc.sleep = op.getUChar("sleep", 0) % COUNT_OF(SLEEP_OBD);
    oc.price = constrain(op.getUChar("price", 50), 20, 150);
    best100 = op.getFloat("b100", 0); best402 = op.getFloat("b402", 0);
    oc.mask = op.getUInt("mask", oc.mask);
    if (!oc.mask) oc.mask = 1UL << V_SPEED;
    oc.haveMac = op.getBytes("bmac", oc.mac, 6) == 6;          // BLE adresi (eski klasik "mac" kullanilmaz)
    oc.atype = op.getUChar("atype", 0); oc.enabled = op.getUChar("en", 0);
    String n = op.getString("name", ""); strncpy(oc.name, n.c_str(), sizeof(oc.name) - 1);
    op.end();
    for (int i = 0; i < 4; i++) if (oc.tile[i] >= V_COUNT) oc.tile[i] = V_SPEED;
  }
  void saveCfg() {
    op.begin("obd", false);
    op.putBytes("tile", oc.tile, 4); op.putUChar("focus", oc.focus); op.putUChar("fuel", oc.fuel);
    op.putUChar("disp", oc.disp); op.putUChar("hot", oc.hot); op.putUChar("shift", oc.shift); op.putUChar("spd", oc.spd);
    op.putUChar("style", oc.style); op.putUChar("tank", oc.tank);
    op.putUChar("sleep", oc.sleep); op.putUChar("price", oc.price);
    op.putFloat("b100", best100); op.putFloat("b402", best402); op.putUInt("mask", oc.mask);
    op.putUChar("en", oc.enabled ? 1 : 0);
    if (oc.haveMac) { op.putBytes("bmac", oc.mac, 6); op.putString("name", oc.name); op.putUChar("atype", oc.atype); }
    else { op.remove("bmac"); op.remove("name"); }
    op.end();
  }

  // ---- Canli degerler -----------------------------------------------------------
  float*   val = nullptr;               // canli degerler (OBD modunda ayrilir)
  bool     has[V_COUNT];                // en az bir gecerli okuma var
  bool     sup[V_COUNT];                // arac destekliyor (bitmap'e gore)
  uint32_t* lastAt = nullptr;
  float    *vMin = nullptr, *vMax = nullptr;
  uint8_t  supBits[12];                 // PID 0x01..0x60 destek bitleri
  float*   hist = nullptr;              // 120 ornek, OBD modunda ayrilir
  uint8_t  histN = 0, histI = 0; uint32_t histAt = 0;
  uint8_t  histFor = 255;

  // Yolculuk
  float    tripKm = 0, tripL = 0, tripMaxV = 0; uint32_t tripMs = 0, lastTrip = 0;
  // Hizlanma olcumu (0-100 km/sa, 402 m)
  enum : uint8_t { A_WAIT, A_READY, A_RUN, A_DONE };
  uint8_t  aSt = A_WAIT; uint32_t aSeen = 0, aT0 = 0, aPrevT = 0; float aPrevV = 0, aDist = 0, aMax = 0;
  float    a100 = 0, a402 = 0, a402v = 0;

  // Ariza kodlari
  char     dtc[8][6]; uint8_t dtcN = 0; bool mil = false; int8_t milCount = -1;
  bool     dtcRead = false; uint32_t clearArmAt = 0;

  // ---- Baglanti durumu ----------------------------------------------------------
  enum St : uint8_t { S_SCAN, S_CONNECT, S_INIT, S_RUN, S_NOECU, S_ERROR };
  St       st = S_SCAN;
  uint32_t stAt = 0;
  char     msg[40] = "";
  char     proto[20] = "";
  bool     isCan = false;

  // Tarama sonuclari
#if OBD_SUPPORTED
  typedef DevT Dev;
#else
  struct Dev { uint8_t mac[6]; char name[24]; int8_t rssi; bool elm; };
#endif
  Dev*     devs = nullptr;              // 10 cihaz, OBD modunda ayrilir
  volatile int nDev = 0; volatile bool scanning = false; int dsel = 0;

  // ---- Istek / yanit (beklemesiz) ----------------------------------------------
  char*    rx = nullptr; int rxN = 0;   // 256 bayt, OBD modunda ayrilir
  bool     waiting = false; uint32_t sentAt = 0, timeoutMs = 1500;
  char     lastCmd[16] = "";
  uint8_t  curVar = 255;                // sorgulanan veri (V_*), 250 = ozel komut
  uint8_t  noData = 0;

  void send(const char* cmd, uint32_t tmo) {
#if OBD_SUPPORTED
    while (Link::available()) Link::read();
    char b[20]; snprintf(b, sizeof(b), "%s\r", cmd);
    Link::write(b);
#endif
    strncpy(lastCmd, cmd, sizeof(lastCmd) - 1);
    rxN = 0; rx[0] = 0; waiting = true; sentAt = millis(); timeoutMs = tmo;
  }
  // true: yanit tamam ('>' geldi) veya zaman asimi (rx bos)
  bool poll() {
#if OBD_SUPPORTED
    while (Link::available()) {
      char c = (char)Link::read();
      if (c == '>') { rx[rxN] = 0; waiting = false; return true; }
      if (c == 0) continue;
      if (rxN < 255) rx[rxN++] = c;
    }
#endif
    if (millis() - sentAt > timeoutMs) { rx[rxN] = 0; waiting = false; rxN = -1; return true; }
    return false;
  }
  // Yanitten hex baytlari cikar (bosluk/satir/eko/SEARCHING temizlenir)
  int hexBytes(const char* s, uint8_t* out, int maxN, const char* hdr) {
    // hdr ("410C") iceren satiri bul
    char clean[256]; int n = 0;
    for (const char* p = s; *p && n < 255; p++) {
      char c = toupper(*p);
      if (c == '\r' || c == '\n') { clean[n++] = '\n'; continue; }
      if (isxdigit((unsigned char)c) || c == ':') clean[n++] = c;
    }
    clean[n] = 0;
    char* line = strtok(clean, "\n");
    while (line) {
      char* colon = strchr(line, ':');                   // coklu cerceve "0:" oneki
      if (colon) line = colon + 1;
      char* h = hdr ? strstr(line, hdr) : line;
      if (h) {
        int k = 0;
        for (const char* q = h; q[0] && q[1] && k < maxN; q += 2) {
          char b[3] = {q[0], q[1], 0}; out[k++] = (uint8_t)strtol(b, nullptr, 16);
        }
        return k;
      }
      line = strtok(nullptr, "\n");
    }
    return 0;
  }
  bool contains(const char* s, const char* w) { return strstr(s, w) != nullptr; }

  // ---- Destek bitleri --------------------------------------------------------------
  bool pidSup(uint8_t pid) {
    if (pid == 0) return true;
    if (pid > 0x60) return false;
    int i = pid - 1;
    return (supBits[i / 8] >> (7 - i % 8)) & 1;
  }
  void computeSupport() {
    for (int v = 0; v < V_COUNT; v++) sup[v] = pidSup(DEFS[v].pid);
    sup[V_BATT] = true;
    sup[V_BOOST] = pidSup(0x0B);
    bool maf = pidSup(0x10), sd = pidSup(0x0B) && pidSup(0x0C) && pidSup(0x0F);
    sup[V_LPH] = pidSup(0x5E) || ((oc.fuel != 1) && (maf || sd));
    sup[V_L100] = sup[V_LPH] && pidSup(0x0D);
  }

  // ---- Deger cozumleme -----------------------------------------------------------
  void setVal(uint8_t v, float x) {
    val[v] = x; lastAt[v] = millis();
    if (!has[v]) { vMin[v] = vMax[v] = x; has[v] = true; }
    vMin[v] = min(vMin[v], x); vMax[v] = max(vMax[v], x);
  }
  // Mode 01 yaniti: A B
  void decode01(uint8_t pid, const uint8_t* d, int n) {
    if (n < 1) return;
    float A = d[0], B = n > 1 ? d[1] : 0;
    switch (pid) {
      case 0x04: setVal(V_LOAD, A * 100 / 255); break;
      case 0x05: setVal(V_COOLANT, A - 40); break;
      case 0x06: setVal(V_STFT, A * 100 / 128 - 100); break;
      case 0x07: setVal(V_LTFT, A * 100 / 128 - 100); break;
      case 0x0B: setVal(V_MAP, A);
                 setVal(V_BOOST, (A - (has[V_BARO] ? val[V_BARO] : 101.3f)) / 100.0f); break;
      case 0x0C: setVal(V_RPM, (256 * A + B) / 4); break;
      case 0x0D: setVal(V_SPEED, A); break;
      case 0x0E: setVal(V_TIMING, A / 2 - 64); break;
      case 0x0F: setVal(V_IAT, A - 40); break;
      case 0x10: setVal(V_MAF, (256 * A + B) / 100); break;
      case 0x11: setVal(V_THROTTLE, A * 100 / 255); break;
      case 0x1F: setVal(V_RUNTIME, (256 * A + B) / 60); break;
      case 0x2F: setVal(V_FUELLVL, A * 100 / 255); break;
      case 0x31: setVal(V_DISTCLR, 256 * A + B); break;
      case 0x33: setVal(V_BARO, A); break;
      case 0x42: setVal(V_ECUV, (256 * A + B) / 1000); break;
      case 0x46: setVal(V_AMBIENT, A - 40); break;
      case 0x5C: setVal(V_OIL, A - 40); break;
      case 0x5E: setVal(V_LPH, (256 * A + B) / 20); break;
    }
  }
  // Yakit tuketimi: 5E yoksa hava akisindan (MAF) veya hiz-yogunluk yontemiyle
  void computeFuel() {
    if (pidSup(0x5E)) { /* dogrudan olculdu */ }
    else if (oc.fuel == 1) return;                        // dizel: MAF ile hesaplanamaz
    else {
      float maf = -1;
      if (has[V_MAF] && millis() - lastAt[V_MAF] < 2000) maf = val[V_MAF];
      else if (has[V_MAP] && has[V_RPM] && has[V_IAT]) {    // hiz-yogunluk (VE ~0.85)
        float disp = DISP10[oc.disp] / 10.0f / 1000.0f;       // m3
        float kg = (val[V_MAP] * 1000 * disp) / (287.05f * (val[V_IAT] + 273.15f));
        maf = val[V_RPM] / 120.0f * kg * 0.85f * 1000.0f;     // g/s
      }
      if (maf < 0) return;
      float afr = oc.fuel == 2 ? 15.5f : 14.7f, dens = oc.fuel == 2 ? 540.0f : 745.0f;
      float lph = maf / afr * 3600.0f / dens;
      if (has[V_RPM] && val[V_RPM] < 100) lph = 0;
      setVal(V_LPH, lph);
    }
    if (has[V_LPH] && has[V_SPEED]) {
      float sp = val[V_SPEED];
      val[V_L100] = sp > 3 ? val[V_LPH] / sp * 100.0f : 0; has[V_L100] = true; lastAt[V_L100] = millis();
    }
  }

  // ---- Zamanlayici: hangi veri siradaki? ---------------------------------------
  uint8_t curParam();
  void buildPages();
  enum : uint8_t { P_TRIP = 100, P_DTC = 101, P_COCKPIT = 102, P_CONN = 103, P_GRAPH = 104, P_ACCEL = 105, P_HIST = 106, P_WX = 107, P_HEALTH = 108 };
  uint8_t plist[V_COUNT + 10]; int pn = 0;
  float   shown = 0; uint32_t pageAt = 0, sweepAt = 0; uint8_t shownFor = 255;
  bool needed(uint8_t v, uint8_t cur) {
    if (!sup[v]) return false;
    if (v == V_SPEED || v == V_RPM) return true;                   // yolculuk / vites uyarisi icin hep
    return v == cur;
  }
  // hesaplanan veriler icin ham PID'ler
  void expand(bool* want, uint8_t cur) {
    if (cur == P_GRAPH) cur = oc.focus;                            // grafik: secili veri
    for (int v = 0; v < V_COUNT; v++) want[v] = needed(v, cur);
    if (cur == P_COCKPIT) {                                        // kokpit: yan bilgiler
      want[V_COOLANT] = sup[V_COOLANT]; want[V_FUELLVL] = sup[V_FUELLVL]; want[V_AMBIENT] = sup[V_AMBIENT];
    }
    if (want[V_BOOST]) { want[V_MAP] = sup[V_MAP]; }
    if (want[V_LPH] || want[V_L100] || cur == V_SPEED || cur == P_TRIP || cur == P_COCKPIT) {
      if (pidSup(0x5E)) want[V_LPH] = true;
      else if (oc.fuel != 1) {
        if (pidSup(0x10)) want[V_MAF] = true;
        else { want[V_MAP] = sup[V_MAP]; want[V_IAT] = sup[V_IAT]; }
      }
    }
    if (oc.hot) want[V_COOLANT] = sup[V_COOLANT];
    want[V_COOLANT] = sup[V_COOLANT];                              // saglik: isinma / hararet egilimi
    if (has[V_COOLANT] && val[V_COOLANT] >= 80) want[V_LTFT] = sup[V_LTFT];   // saglik: yakit ayari (seyrek)
  }
  uint8_t pickNext() {
    if (curParam() == P_ACCEL && sup[V_SPEED]) return V_SPEED;    // hizlanma: yalnizca hiz, en sik
    bool want[V_COUNT]; expand(want, curParam());
    uint32_t now = millis(); int best = -1; float bestScore = 0;
    for (int v = 0; v < V_COUNT; v++) {
      if (!want[v]) continue;
      if (v == V_L100) continue;                                   // hesaplanir
      if (v == V_LPH && !pidSup(0x5E)) continue;
      if (v == V_BOOST) continue;                                   // MAP ile gelir
      uint32_t per = DEFS[v].periodMs;
      if (v == V_LTFT && curParam() != V_LTFT) per = 15000;         // saglik icin seyrek
      if (v == V_COOLANT && per < 4000 && curParam() != V_COOLANT) per = 4000;
      float over = (now - lastAt[v]) / (float)per;
      if (over > bestScore) { bestScore = over; best = v; }
    }
    return (best >= 0 && bestScore >= 1.0f) ? best : 255;
  }

  // ---- Baglanti ------------------------------------------------------------------
  static const char* INIT_CMDS[] = {"ATZ", "ATE0", "ATL0", "ATS0", "ATH0", "ATAT1", "ATSP0"};
  int initStep = 0;
  bool wantConnPage = false;             // baglanti sayfasina gec (tarama basladi)
  void setSt(St s, const char* m = "") {
    if (s == S_ERROR || s == S_SCAN) memset(has, 0, sizeof(has));   // eski degerler kalmasin
    st = s; stAt = millis(); strncpy(msg, m, sizeof(msg) - 1);
  }

#if OBD_SUPPORTED
  bool looksElm(const char* n);
  void onFound(const uint8_t* mac, const char* name, int8_t rssi, uint8_t atype) {
    if (!devs || nDev >= 10) return;
    for (int i = 0; i < nDev; i++) if (!memcmp(devs[i].mac, mac, 6)) { if (name[0] && !devs[i].name[0]) strcpy(devs[i].name, name); return; }
    Dev& x = devs[nDev];
    memcpy(x.mac, mac, 6);
    if (name[0]) strncpy(x.name, name, sizeof(x.name) - 1);
    else snprintf(x.name, sizeof(x.name), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    x.rssi = rssi; x.elm = looksElm(x.name); x.atype = atype;
    nDev = nDev + 1;
  }
  bool looksElm(const char* n) {
    char u[24]; int i = 0;
    for (; n[i] && i < 23; i++) u[i] = toupper(n[i]);
    u[i] = 0;
    return strstr(u, "OBD") || strstr(u, "ELM") || strstr(u, "VLINK") || strstr(u, "V-LINK") ||
           strstr(u, "VGATE") || strstr(u, "KONNWEI") || strstr(u, "VIECAR") || strstr(u, "CAR");
  }
  void startScan() {
    nDev = 0; dsel = 0; scanning = true;
    Link::scan(true);
    setSt(S_SCAN);
  }
  void stopScan() { if (scanning) { Link::scan(false); scanning = false; } }
  uint32_t connAt = 0; uint8_t connTry = 0;
  bool connectTo(const uint8_t* mac, const char* name, uint8_t atype) {
    stopScan();
    memcpy(oc.mac, mac, 6); strncpy(oc.name, name, sizeof(oc.name) - 1); oc.atype = atype;
    setSt(S_CONNECT, "Adaptöre bağlanılıyor");
    connTry = 0; connAt = millis();
    Link::connect(mac, atype);
    return true;
  }
  bool connectSaved() { return oc.haveMac ? connectTo(oc.mac, oc.name, oc.atype) : false; }
#endif

  // ---- Ana durum makinesi ------------------------------------------------------
  uint8_t page = 0;
  uint32_t lastVolt = 0, lastRetry = 0;
  bool     dtcBusy = false, clearBusy = false, pendingDtc = false;

  void parseDtc(const char* r) {
    dtcN = 0;
    char clean[256]; int n = 0;
    for (const char* p = r; *p && n < 255; p++) { char c = toupper(*p); if (isxdigit((unsigned char)c) || c == '\n' || c == '\r' || c == ':') clean[n++] = c; }
    clean[n] = 0;
    // "43" iceren ilk satirdan itibaren tum baytlari topla
    uint8_t b[64]; int nb = 0; bool started = false;
    char* line = strtok(clean, "\r\n");
    while (line && nb < 60) {
      char* c = strchr(line, ':'); if (c) line = c + 1;
      char* h = started ? line : strstr(line, "43");
      if (h) {
        if (!started) { h += 2; started = true; if (isCan && h[0] && h[1]) h += 2; }   // CAN: kod sayisi baytini atla
        for (; h[0] && h[1] && nb < 60; h += 2) { char t[3] = {h[0], h[1], 0}; b[nb++] = (uint8_t)strtol(t, nullptr, 16); }
      }
      line = strtok(nullptr, "\r\n");
    }
    static const char L[] = {'P', 'C', 'B', 'U'};
    for (int i = 0; i + 1 < nb && dtcN < 8; i += 2) {
      uint16_t c = (b[i] << 8) | b[i + 1];
      if (!c) continue;
      snprintf(dtc[dtcN++], 6, "%c%01X%03X", L[c >> 14], (c >> 12) & 3, c & 0xFFF);
    }
  }

  void service() {
#if OBD_SUPPORTED
    uint32_t now = millis();
    if (st == S_SCAN) {
      if (!Link::scanning && !Link::wantScan && scanning && now - stAt > 1500) scanning = false;
      return;
    }
    if (st == S_ERROR) return;
    if (st == S_CONNECT) {
      if (Link::open()) {                                          // baglandi: ELM327'yi hazirla
        oc.haveMac = true; saveCfg();
        initStep = 0; setSt(S_INIT, "Adaptör hazırlanıyor");
        delay(300);
        send(INIT_CMDS[0], 2500);
        showToast("OBD2 bağlandı", C_OK, 1200);
      } else if (Link::link == Link::L_FAIL || now - connAt > 15000) {
        if (++connTry < 3) {                                      // tekrar dene
          Link::close(); connAt = now;
          Link::connect(oc.mac, oc.atype);
        } else { Link::close(); setSt(S_ERROR, "Adaptöre bağlanılamadı"); }
      }
      return;
    }
    if (!Link::open()) { setSt(S_ERROR, "Bağlantı koptu"); return; }
    if (waiting && !poll()) return;
    bool timeout = (rxN < 0);

    if (st == S_INIT) {
      if (initStep < (int)COUNT_OF(INIT_CMDS) - 1) {
        initStep++;
        send(INIT_CMDS[initStep], initStep == 0 ? 2500 : 1200);
        return;
      }
      if (initStep == (int)COUNT_OF(INIT_CMDS) - 1) {            // araca ilk sorgu (protokol arama)
        initStep++;
        setSt(S_INIT, "Araç aranıyor (kontak açık olmalı)");
        send("0100", 12000);
        return;
      }
      if (initStep >= (int)COUNT_OF(INIT_CMDS) && initStep < (int)COUNT_OF(INIT_CMDS) + 3) {
        uint8_t d[8]; int k = timeout ? 0 : hexBytes(rx, d, 8, initStep == (int)COUNT_OF(INIT_CMDS) ? "4100" : initStep == (int)COUNT_OF(INIT_CMDS) + 1 ? "4120" : "4140");
        int blk = initStep - COUNT_OF(INIT_CMDS);
        if (k >= 6) memcpy(supBits + blk * 4, d + 2, 4);
        else if (blk == 0) {                                     // arac yanit vermedi
          setSt(S_NOECU, contains(rx, "UNABLE") ? "Araç yanıt vermiyor" : "Kontak kapalı olabilir");
          lastRetry = now;
          return;
        }
        bool more = k >= 6 && (d[5] & 1);                        // sonraki blok destekleniyor mu
        initStep++;
        if (more && blk < 2) { send(blk == 0 ? "0120" : "0140", 2000); return; }
        send("ATDPN", 1500);
        initStep = 100;
        return;
      }
      if (initStep == 100) {
        const char* p = rx; while (*p && !isalnum((unsigned char)*p)) p++;
        char c = p[0] == 'A' ? p[1] : p[0];
        isCan = (c >= '6' && c <= '9') || (c >= 'A' && c <= 'C');
        static const char* PN[] = {"Otomatik", "J1850 PWM", "J1850 VPW", "ISO 9141", "KWP 5bd", "KWP hızlı",
                                   "CAN 11/500", "CAN 29/500", "CAN 11/250", "CAN 29/250", "J1939", "CAN", "CAN"};
        int idx = isdigit((unsigned char)c) ? c - '0' : (c >= 'A' && c <= 'C') ? 10 + c - 'A' : 0;
        strncpy(proto, PN[min(idx, 12)], sizeof(proto) - 1);
        computeSupport();
        setSt(S_RUN, "");
        sweepAt = millis();                                      // gosterge "selamlama" turu
        Snd::ok();
        return;
      }
    }
    if (st == S_NOECU) {
      if (now - lastRetry > 4000) {                              // tekrar dene
        lastRetry = now;
        initStep = COUNT_OF(INIT_CMDS);
        setSt(S_INIT, "Araç aranıyor (kontak açık olmalı)");
        send("0100", 12000);
      }
      return;
    }
    if (st != S_RUN) return;

    // ---- Calisma: gelen yaniti isle ----
    if (curVar != 255) {
      if (curVar == 200) {                                       // ATRV
        const char* p = rx; while (*p && !(isdigit((unsigned char)*p))) p++;
        float v = atof(p); if (v > 5 && v < 20) { setVal(V_BATT, v); Health::onVolt(v); }
      } else if (curVar == 201) {                                // 03: ariza kodlari
        parseDtc(rx); dtcRead = true; dtcBusy = false;
        Snd::sel();
      } else if (curVar == 202) {                                // 0101: MIL
        uint8_t d[6]; int k = hexBytes(rx, d, 6, "4101");
        if (k >= 3) { mil = d[2] & 0x80; milCount = d[2] & 0x7F; }
      } else if (curVar == 203) {                                // 04: sil
        clearBusy = false;
        bool ok = contains(rx, "44");
        showToast(ok ? "Arıza kodları silindi" : "Silinemedi (motor çalışıyor olabilir)", ok ? C_OK : C_BAD, 2500);
        if (ok) { dtcN = 0; mil = false; milCount = 0; Snd::ok(); } else Snd::error();
      } else {
        uint8_t pid = DEFS[curVar].pid;
        char hdr[5]; snprintf(hdr, sizeof(hdr), "41%02X", pid);
        uint8_t d[8]; int k = timeout ? 0 : hexBytes(rx, d, 8, hdr);
        if (k >= 3) { decode01(pid, d + 2, k - 2); noData = 0; if (curVar == V_MAF || curVar == V_MAP || curVar == V_IAT || curVar == V_RPM || curVar == V_LPH) computeFuel(); }
        else {
          lastAt[curVar] = now;                                   // basarisiz: tekrar hemen sorma
          if (++noData > 12) { setSt(S_NOECU, "Araç yanıt vermiyor"); lastRetry = now; noData = 0; }
        }
      }
      curVar = 255;
    }
    // ---- Siradaki istek ----
    if (dtcBusy) { curVar = 202; send("0101", 2000); dtcBusy = false; pendingDtc = true; return; }
    if (pendingDtc) { pendingDtc = false; curVar = 201; send("03", 4000); return; }
    if (clearBusy) { curVar = 203; send("04", 4000); clearBusy = false; return; }
    if (now - lastVolt > (Health::fastVolt() ? 250UL : 3000UL) && aSt != A_RUN) { lastVolt = now; curVar = 200; send("ATRV", 1000); return; }
    uint8_t v = pickNext();
    if (v != 255) {
      char c[6]; snprintf(c, sizeof(c), "01%02X", DEFS[v].pid);
      curVar = v; send(c, 1000);
    }
#endif
  }

  // ---- Hizlanma olcumu: her yeni hiz orneginde ---------------------------------
  void accelReset() { aSt = A_WAIT; a100 = a402 = a402v = 0; aDist = 0; aMax = 0; }
  void accelTick() {
    uint32_t now = millis();
    if (aSt == A_RUN && now - aSeen > 3000) { aSt = A_DONE; return; }  // veri kesildi
    if (!has[V_SPEED] || lastAt[V_SPEED] == aSeen) return;
    uint32_t t = lastAt[V_SPEED], pt = aPrevT; float v = val[V_SPEED], pv = aPrevV;
    aSeen = t; aPrevT = t; aPrevV = v;
    if (!pt || t <= pt) return;
    float dt = (t - pt) / 1000.0f;
    switch (aSt) {
      case A_WAIT:  if (v < 1) aSt = A_READY; break;
      case A_READY:
        if (v >= 1) {                                             // kalkis: iki ornegin ortasi
          aSt = A_RUN; aT0 = pt + (t - pt) / 2; aMax = v;
          aDist = v / 3.6f * (t - aT0) / 1000.0f / 2; a100 = a402 = a402v = 0;
        }
        break;
      case A_RUN: {
        float d0 = aDist; aDist += (pv + v) / 2 / 3.6f * dt;
        float base = (pt - aT0) / 1000.0f;
        if (!a100 && v >= 100) a100 = base + (100 - pv) / max(0.1f, v - pv) * dt;          // dogrusal ara deger
        if (!a402 && aDist >= 402) { float f = (402 - d0) / max(0.01f, aDist - d0); a402 = base + f * dt; a402v = pv + (v - pv) * f; }
        aMax = max(aMax, v);
        bool done = a100 > 0 && a402 > 0, abort = v < aMax - 8 || v < 1 || (t - aT0) > 60000;
        if (done || abort) {
          aSt = A_DONE;
          bool rec = false;
          if (a100 > 0 && (!best100 || a100 < best100)) { best100 = a100; rec = true; }
          if (a402 > 0 && (!best402 || a402 < best402)) { best402 = a402; rec = true; }
          if (rec) { saveCfg(); showToast("Yeni rekor!", C_OK, 1500); }
          if (a100 > 0) Snd::ok(); else Snd::error();
        }
      } break;
      default: break;
    }
  }

  // ---- Yolculuk ve grafik ------------------------------------------------------
  void tick() {
    uint32_t now = millis();
    uint32_t dt = lastTrip ? now - lastTrip : 0; lastTrip = now;
    if (st == S_RUN && dt < 2000) {
      bool fresh = has[V_SPEED] && now - lastAt[V_SPEED] < 3000;
      if (fresh) { float dk = val[V_SPEED] * dt / 3600000.0f; tripKm += dk; Health::onKm(dk); tripMaxV = max(tripMaxV, val[V_SPEED]); }
      if (has[V_LPH] && now - lastAt[V_LPH] < 3000) tripL += val[V_LPH] * dt / 3600000.0f;
      if (has[V_RPM] && val[V_RPM] > 300) tripMs += dt;
    }
    accelTick();
    if (histFor != oc.focus) { histFor = oc.focus; histN = 0; histI = 0; }
    if (now - histAt >= 500) {
      histAt = now;
      if (has[oc.focus]) { hist[histI] = val[oc.focus]; histI = (histI + 1) % 120; if (histN < 120) histN++; }
    }
  }

  // ---- Alarmlar -------------------------------------------------------------------
  uint8_t alarm = 0;                   // 1 hararet, 2 hiz, 3 vites
  uint32_t lastBeep = 0;
  void alarms() {
    uint32_t now = millis();
    alarm = 0;
    if (st != S_RUN) return;
    if (oc.hot && has[V_COOLANT] && val[V_COOLANT] >= HOT_C[oc.hot]) alarm = 1;
    else if (oc.spd && has[V_SPEED] && val[V_SPEED] > SPD[oc.spd]) alarm = 2;
    else if (oc.shift && has[V_RPM] && val[V_RPM] >= SHIFT[oc.shift]) alarm = 3;
    if (!alarm) return;
    uint32_t every = alarm == 1 ? 5000 : alarm == 2 ? 8000 : 900;
    if (now - lastBeep > every) {
      lastBeep = now;
      if (alarm == 1) Snd::lowBat(); else if (alarm == 2) Snd::limit(); else Snd::click();
      app.lastInput = now;                                        // ekran aydinlansin
    }
  }

  // ---- Cizim -----------------------------------------------------------------------
  void fmt(uint8_t v, char* b, size_t n) {
    if (!sup[v]) { snprintf(b, n, "yok"); return; }
    if (!has[v] || millis() - lastAt[v] > 8000) { snprintf(b, n, "--"); return; }
    float x = val[v];
    switch (DEFS[v].dec) {
      case 0: snprintf(b, n, "%d", (int)lroundf(x)); break;
      case 1: snprintf(b, n, "%.1f", x); break;
      default: snprintf(b, n, "%.2f", x); break;
    }
  }
  uint16_t valColor(uint8_t v) {
    if (!has[v]) return C_SUBTEXT;
    float x = val[v];
    switch (v) {
      case V_COOLANT: return x >= (oc.hot ? HOT_C[oc.hot] : 105) ? C_BAD : x < 70 ? C_MIN : C_OK;
      case V_OIL:     return x >= 130 ? C_BAD : x < 70 ? C_MIN : C_OK;
      case V_BATT:    return x < 12.0f ? C_BAD : x < 12.4f ? C_WARN : x > 14.8f ? C_WARN : C_OK;
      case V_RPM:     return oc.shift && x >= SHIFT[oc.shift] ? C_BAD : C_TEXT;
      case V_SPEED:   return oc.spd && x > SPD[oc.spd] ? C_WARN : C_TEXT;
      case V_BOOST:   return x > 0.05f ? C_ACCENT : C_TEXT;
      default:        return C_TEXT;
    }
  }
  void topBar(const char* title) {
    TFT_eSPI& g = G();
    uint16_t dc = st == S_RUN ? C_OK : st == S_NOECU ? C_WARN : st == S_ERROR ? C_BAD : ((millis() / 400) % 2 ? C_ACCENT : C_TRACK);
    g.fillCircle(7, 8, 4, dc);
    drawText(title, 16, 8, ML_DATUM, 2, C_TEXT, C_BG, SW - 60);
    char b[12];
    if (st != S_RUN) drawText("Bağlı değil", SW - 4, 8, MR_DATUM, 1, C_WARN, C_BG);
    else if (has[V_BATT]) { snprintf(b, sizeof(b), "%.1fV", val[V_BATT]); drawText(b, SW - 4, 8, MR_DATUM, 2, valColor(V_BATT), C_BG); }
    // sayfa noktalari
    buildPages();                                                 // sayfa noktalari (altta)
    for (int i = 0; i < pn; i++) g.fillCircle(SW / 2 + (i - pn / 2) * 6, SH - 3, i == page ? 2 : 1, i == page ? C_TEXT : C_TRACK);
  }
  // ---- Olceklenebilir vektor rakamlar (buyuk, keskin, premium) ------------------
  uint16_t vbg = C_BG;                      // kenar yumusatma icin zemin rengi
  static void vSeg(float x0, float y0, float x1, float y1, float w, uint16_t c) {
    G().drawWideLine(x0, y0, x1, y1, w, c, vbg);
  }
  // matematik acisi (0 = sag, saat yonunun tersi) ile yay
  static void vArc(float cx, float cy, float r, float w, float th0, float th1, uint16_t c) {
    int a0 = ((int)lroundf(270 - th1) % 360 + 360) % 360, a1 = ((int)lroundf(270 - th0) % 360 + 360) % 360;
    if (a0 == a1) return;
    G().drawSmoothArc((int)cx, (int)cy, (int)(r + w / 2), (int)(r - w / 2), a0, a1, c, vbg, true);
  }
  static void vCirc(float cx, float cy, float r, float w, uint16_t c) {
    G().drawArc((int)cx, (int)cy, (int)(r + w / 2), (int)(r - w / 2), 0, 360, c, vbg, true);
  }
  // Tek rakam; (x, y0) sol ust, H yukseklik, w cizgi kalinligi. Donus: ilerleme
  float vDigit(char ch, float x, float y0, float H, float w, uint16_t c) {
    float W = H * 0.56f, h = w / 2, gap = H * 0.2f;
    float L = x + h, R = x + W - h, T = y0 + h, B = y0 + H - h, cx = x + W / 2, r = (W - w) / 2;
    switch (ch) {
      case '0': vArc(cx, T + r, r, w, 0, 180, c); vArc(cx, B - r, r, w, 180, 360, c);
                vSeg(L, T + r, L, B - r, w, c); vSeg(R, T + r, R, B - r, w, c); break;
      case '1': vSeg(cx + W * 0.08f, T, cx + W * 0.08f, B, w, c); vSeg(cx - W * 0.28f, T + H * 0.2f, cx + W * 0.08f, T, w, c); break;
      case '2': { vArc(cx, T + r, r, w, -28, 180, c);
                  float ex = cx + r * cosf(-28 * DEG_TO_RAD), ey = T + r + r * sinf(28 * DEG_TO_RAD);
                  vSeg(ex, ey, L, B, w, c); vSeg(L, B, R, B, w, c); break; }
      case '3': { float r1 = (H * 0.5f - w / 2) / 2 + w / 4, r2 = r1 * 1.08f;
                  vArc(cx, T + r1, r1, w, -90, 150, c); vArc(cx, B - r2, r2, w, -150, 90, c); break; }
      case '4': { float yb = y0 + H * 0.68f, xs = x + W * 0.74f;
                  vSeg(xs, T, L, yb, w, c); vSeg(L, yb, R + h * 0.5f, yb, w, c); vSeg(xs, T + H * 0.3f, xs, B, w, c); break; }
      case '5': { float rb = H * 0.31f, cyb = B - rb;
                  vSeg(R, T, L + W * 0.06f, T, w, c); vSeg(L + W * 0.06f, T, L + W * 0.02f, cyb - rb * 0.55f, w, c);
                  vArc(cx - W * 0.02f, cyb, rb, w, -140, 145, c); break; }
      case '6': { float rb = r; vCirc(cx, B - rb, rb, w, c); vSeg(L, B - rb, cx + W * 0.28f, T, w, c); break; }
      case '7': vSeg(L, T, R, T, w, c); vSeg(R, T, cx - W * 0.12f, B, w, c); break;
      case '8': { float r1 = H * 0.24f - w * 0.25f, r2 = H * 0.27f - w * 0.25f;
                  vCirc(cx, T + r1, r1, w, c); vCirc(cx, B - r2, r2, w, c); break; }
      case '9': { float rt = r; vCirc(cx, T + rt, rt, w, c); vSeg(R, T + rt, cx - W * 0.28f, B, w, c); break; }
      case '.': G().fillSmoothCircle((int)(x + w), (int)B, (int)(w * 0.75f), c, vbg); return w * 2 + gap * 0.6f;
      case '-': vSeg(L + W * 0.1f, y0 + H / 2, R - W * 0.1f, y0 + H / 2, w, c); break;
      case '/': vSeg(R, T, L, B, w, c); break;
      case '%': { float rr = W * 0.18f; vCirc(L + rr, T + rr, rr, w * 0.6f, c); vCirc(R - rr, B - rr, rr, w * 0.6f, c);
                  vSeg(R, T, L, B, w * 0.7f, c); break; }
      case ' ': break;
      default: break;
    }
    return W + gap;
  }
  float vWidth(const char* s, float H, float w) {
    float t = 0, gap = H * 0.2f;
    for (const char* p = s; *p; p++) t += (*p == '.') ? w * 2 + gap * 0.6f : H * 0.56f + gap;
    return t - gap;
  }
  // Ortalanmis sayi; H: hedef yukseklik, maxW'ye sigmazsa kuculur
  float vNumber(const char* s, float cx, float cy, float H, float wRatio, float maxW, uint16_t c) {
    float w = max(2.5f, H * wRatio);
    float tw = vWidth(s, H, w);
    if (tw > maxW) { H *= maxW / tw; w = max(2.5f, H * wRatio); tw = vWidth(s, H, w); }
    float x = cx - tw / 2, y0 = cy - H / 2;
    for (const char* p = s; *p; p++) x += vDigit(*p, x, y0, H, w, c);
    return cx + tw / 2;                                             // sayinin sag kenari
  }

  // ---- Gosterge sayfalari -----------------------------------------------------
  static const uint8_t ORDER[] = {V_SPEED, V_RPM, V_COOLANT, V_BOOST, V_L100, V_LPH, V_BATT, V_OIL, V_FUELLVL,
                                  V_IAT, V_LOAD, V_THROTTLE, V_MAP, V_MAF, V_TIMING, V_AMBIENT, V_ECUV,
                                  V_STFT, V_LTFT, V_BARO, V_RUNTIME, V_DISTCLR};
  // (plist, pn, shown, sweepAt yukarida tanimli)
  void buildPages() {
    pn = 0;
    if (oc.style >= 2) plist[pn++] = P_COCKPIT;                    // kokpit stili: ilk sayfa
    plist[pn++] = P_WX;                                             // hava karti
    plist[pn++] = P_HEALTH;                                         // arac sagligi
    for (uint8_t v : ORDER) if ((oc.mask >> v) & 1) if (st != S_RUN || sup[v]) plist[pn++] = v;
    plist[pn++] = P_GRAPH; plist[pn++] = P_ACCEL;
    plist[pn++] = P_TRIP; plist[pn++] = P_HIST; plist[pn++] = P_DTC;
    if (st != S_RUN) plist[pn++] = P_CONN;                          // bagli degilken: baglanti sayfasi (son)
    if (wantConnPage) { wantConnPage = false; page = pn - 1; }
    if (page >= pn) page = 0;
  }
  uint8_t curParam() { buildPages(); return plist[page]; }

  // Deger icin gosterge araligi ve kirmizi bolge
  void range(uint8_t v, float& lo, float& hi, float& red) {
    lo = DEFS[v].lo; hi = DEFS[v].hi; red = 2;                     // red: 0..1 (2 = yok)
    if (v == V_RPM) { hi = 7000; red = (oc.shift ? SHIFT[oc.shift] : 6000) / hi; }
    else if (v == V_SPEED) { hi = 220; if (oc.spd) red = SPD[oc.spd] / hi; }
    else if (v == V_COOLANT) { red = ((oc.hot ? HOT_C[oc.hot] : 105) - lo) / (hi - lo); }
    else if (v == V_OIL) red = (130 - lo) / (hi - lo);
    else if (v == V_BOOST) red = (1.4f - lo) / (hi - lo);
    else if (v == V_BATT) red = 2;
  }
  // Tick etiketi: devirde binlik
  void tickLabel(uint8_t v, float x, char* b, size_t n) {
    if (v == V_RPM) snprintf(b, n, "%d", (int)lroundf(x / 1000));
    else if (DEFS[v].dec >= 2) snprintf(b, n, "%.1f", x);
    else snprintf(b, n, "%d", (int)lroundf(x));
  }
  void valueText(uint8_t v, float x, char* b, size_t n) {
    switch (DEFS[v].dec) {
      case 0: snprintf(b, n, "%d", (int)lroundf(x)); break;
      case 1: snprintf(b, n, "%.1f", x); break;
      default: snprintf(b, n, "%.2f", x); break;
    }
  }
  // Alt satirdaki ikincil bilgiler
  void secondary(uint8_t v, char* l, char* r) {
    if (v == V_SPEED) {
      snprintf(l, 24, "Yol %.1f km", tripKm);
      if (sup[V_LPH] && tripKm > 0.5f) snprintf(r, 24, "%.1f L/100", tripL / tripKm * 100); else snprintf(r, 24, "maks %.0f", tripMaxV);
    } else if (has[v]) {
      char a[10], c[10]; valueText(v, vMin[v], a, 10); valueText(v, vMax[v], c, 10);
      snprintf(l, 24, "min %s", a); snprintf(r, 24, "maks %s", c);
    } else { l[0] = r[0] = 0; }
  }

  // ---- SPORTIF: LED segmentli 240 derece gosterge, kalin rakamlar -------------
  void dashSport(uint8_t v, float f, float red, uint16_t vc) {
    TFT_eSPI& g = G();
    vbg = C_BG;
    int cx = SW / 2, cy = portrait ? 112 : 74, R = portrait ? 62 : 64, th = 10;
    // zemin: altta kirmizi bir parilti
    uint16_t glowC = tft.color565(255, 40, 30);
    for (int i = 0; i < 4; i++) g.fillEllipse(cx, cy + R - 4, R - i * 10, 10 - i * 2, blend565(C_BG, glowC, 0.05f + 0.04f * i));
    // dis ince cember
    g.drawArc(cx, cy, R + 4, R + 3, 55, 305, blend565(C_BG, C_TEXT, 0.25f), C_BG, true);
    // LED segmentleri
    const int N = 30;
    for (int i = 0; i < N; i++) {
      float a0 = 60 + 240.0f * i / N + 1.2f, a1 = 60 + 240.0f * (i + 1) / N - 1.2f;
      float mid = (i + 0.5f) / N;
      uint16_t c = mid >= red ? tft.color565(255, 45, 45) : mid >= red - 0.15f && red < 1.5f ? tft.color565(255, 170, 40) : tft.color565(235, 245, 255);
      bool lit = mid <= f;
      g.drawArc(cx, cy, R, R - th, (int)a0, (int)a1, lit ? c : blend565(C_BG, c, 0.13f), C_BG, false);
    }
    // ucta parlayan nokta
    float ae = (60 + 240 * f + 90) * DEG_TO_RAD;
    int ex = cx + (int)(cosf(ae) * (R - th / 2)), ey = cy + (int)(sinf(ae) * (R - th / 2));
    g.fillSmoothCircle(ex, ey, 7, blend565(C_BG, vc, 0.35f), C_BG);
    g.fillSmoothCircle(ex, ey, 4, C_TEXT, C_BG);
    // ince tik cizgileri + yalnizca uclarda etiket (sade)
    float lo, hi, rr; range(v, lo, hi, rr);
    for (int k = 0; k <= 8; k++) {
      float t = k / 8.0f, a = (60 + 240 * t + 90) * DEG_TO_RAD;
      int r1 = R - th - 3, r2 = R - th - (k % 2 ? 5 : 8);
      g.drawLine(cx + (int)(cosf(a) * r1), cy + (int)(sinf(a) * r1), cx + (int)(cosf(a) * r2), cy + (int)(sinf(a) * r2),
                 t >= red ? C_BAD : blend565(C_BG, C_TEXT, 0.45f));
    }
    for (int k = 0; k <= 1; k++) {
      float t = k, a = (60 + 240 * t + 90) * DEG_TO_RAD;
      char b[8]; tickLabel(v, lo + (hi - lo) * t, b, sizeof(b));
      drawText(b, cx + (int)(cosf(a) * (R - th - 14)), cy + (int)(sinf(a) * (R - th - 14)), MC_DATUM, 1,
               t >= red ? C_BAD : C_SUBTEXT, C_BG);
    }
    // etiket, deger, birim
    drawText(DEFS[v].name, cx, cy - (portrait ? 30 : 30), MC_DATUM, 2, C_SUBTEXT, C_BG, 2 * (R - th - 18));
    char b[12]; bool ok = has[v] && millis() - lastAt[v] < 8000;
    if (ok) valueText(v, shown, b, sizeof(b)); else strcpy(b, "-");
    vNumber(b, cx, cy + 3, portrait ? 38 : 40, 0.15f, 2 * (R - th - 16), ok ? vc : C_SUBTEXT);
    drawText(DEFS[v].unit, cx, cy + (portrait ? 30 : 31), MC_DATUM, 2, tft.color565(255, 70, 60), C_BG);
  }

  // ---- KONFOR: genis ince yay, yumusak renkler, zarif ince rakamlar -------------
  void dashComfort(uint8_t v, float f, float red, uint16_t vc) {
    TFT_eSPI& g = G();
    uint16_t teal = tft.color565(60, 200, 210), blue = tft.color565(90, 140, 255);
    // zemin: lacivert gradyan
    uint16_t top = C_BG, bot = tft.color565(8, 22, 40);
    for (int y = 0; y < SH; y += 3) g.fillRect(0, y, SW, 3, blend565(top, bot, y / (float)SH));
    vbg = blend565(top, bot, 0.5f);
    int cx = SW / 2, cy = portrait ? 116 : 142, R = portrait ? 58 : 112;
    int a0 = portrait ? 45 : 112, a1 = portrait ? 315 : 248;                 // TFT acilari (dikey: halka, yatay: ust yay)
    float span = a1 - a0;
    uint16_t bgA = blend565(top, bot, (cy - R * 0.5f) / SH);
    g.drawSmoothArc(cx, cy, R, R - 4, a0, a1, blend565(bgA, teal, 0.18f), bgA, true);
    // dolu kisim: renk gecisli
    const int N = 24; int lit = (int)(N * f);
    for (int i = 0; i < lit; i++) {
      float s0 = a0 + span * i / N, s1 = a0 + span * (i + 1) / N;
      g.drawArc(cx, cy, R + 1, R - 5, (int)s0, (int)s1 + 1, blend565(teal, blue, i / (float)N), bgA, true);
    }
    if (red < 1.5f) g.drawSmoothArc(cx, cy, R + 6, R + 4, a0 + (int)(span * red), a1, blend565(bgA, C_BAD, 0.7f), bgA, true);
    // parlayan uc
    float ae = (a0 + span * f + 90) * DEG_TO_RAD;
    int ex = cx + (int)(cosf(ae) * (R - 2)), ey = cy + (int)(sinf(ae) * (R - 2));
    for (int i = 3; i >= 1; i--) g.fillSmoothCircle(ex, ey, 3 + i * 3, blend565(bgA, blue, 0.12f * (4 - i)), bgA);
    g.fillSmoothCircle(ex, ey, 3, C_TEXT, bgA);
    // etiket (seyrek harf aralikli his icin kucuk)
    drawText(DEFS[v].name, cx, portrait ? 34 : 44, MC_DATUM, 2, blend565(C_TEXT, C_BG, 0.45f), C_BG, portrait ? SW - 8 : 110);
    char b[12]; bool ok = has[v] && millis() - lastAt[v] < 8000;
    if (ok) valueText(v, shown, b, sizeof(b)); else strcpy(b, "-");
    float ny = portrait ? 112 : 82, nh = portrait ? 38 : 46;
    uint16_t nc = ok ? (vc == C_TEXT ? C_TEXT : vc) : C_SUBTEXT;
    vbg = blend565(top, bot, ny / SH);
    float right = vNumber(b, cx - (portrait ? 0 : 14), ny, nh, 0.085f, portrait ? 86 : 150, nc);
    if (portrait) drawText(DEFS[v].unit, cx, (int)(ny + nh / 2 + 14), MC_DATUM, 2, teal, C_BG);
    else drawText(DEFS[v].unit, (int)right + 6, (int)(ny + nh / 2 - 6), BL_DATUM, 2, teal, C_BG);
  }


  static inline uint16_t c565(uint8_t r, uint8_t g, uint8_t b) { return tft.color565(r, g, b); }
  void icFuel(int x, int y, uint16_t c) {                          // benzin pompasi ikonu
    TFT_eSPI& g = G();
    g.fillRoundRect(x - 7, y - 8, 10, 16, 2, c);
    g.fillRect(x - 5, y - 6, 6, 4, C_BG);
    g.drawLine(x + 3, y - 4, x + 7, y - 1, c); g.drawLine(x + 7, y - 1, x + 7, y + 5, c);
    g.drawLine(x + 7, y + 5, x + 5, y + 5, c);
  }
  float kS = 0, kR = 0;                                            // yumusatilmis hiz / devir
  static inline float rad(float tftDeg) { return (tftDeg + 90) * DEG_TO_RAD; }   // TFT acisi -> radyan
  float spdV() { return kS; }
  float rpmV() { return kR; }
  int   rangeKm() {
    float pct = has[V_FUELLVL] ? val[V_FUELLVL] : 0;
    float l100 = (tripKm > 1 && tripL > 0) ? tripL / tripKm * 100 : 7.0f;
    return (int)(TANK_L[oc.tank] * pct / 100.0f / l100 * 100);
  }
  void rangeStr(char* b, size_t n, const char* unit) {            // yakit seviyesi yoksa "--"
    if (has[V_FUELLVL]) snprintf(b, n, "%d%s", rangeKm(), unit); else snprintf(b, n, "--%s", unit);
  }
  void clockStr(char* b, size_t n) { if (!Net::clock(b, n)) snprintf(b, n, "--:--"); }
  // Kokpit sicakligi: aracin dis sensoru; yoksa motor suyu ("Su 88°C"); veri yoksa "--°C"
  void ambStr(char* b, size_t n, bool sign) {
    if (has[V_AMBIENT]) snprintf(b, n, sign ? "%+d°C" : "%d°C", (int)val[V_AMBIENT]);
    else if (has[V_COOLANT]) snprintf(b, n, "Su %d°C", (int)val[V_COOLANT]);
    else snprintf(b, n, "--°C");
  }

  // Bagli degilken ustte durum etiketi (gostergeler yine de gorunur)
  void offBanner() {
    if (st == S_RUN) return;
    const char* t; uint16_t c;
    switch (st) {
      case S_SCAN:    t = "Bağlı değil"; c = C_WARN; break;
      case S_CONNECT: t = "Adaptöre bağlanılıyor..."; c = C_ACCENT; break;
      case S_INIT:    t = "Adaptör hazırlanıyor..."; c = C_ACCENT; break;
      case S_NOECU:   t = "Araç yanıt vermiyor"; c = C_WARN; break;
      default:        t = msg[0] ? msg : "Bağlı değil"; c = C_BAD; break;
    }
    if (st == S_SCAN) t = "Bağlı değil";
    int w = min(SW - 8, textW(t, 1) + 22), x = SW / 2 - w / 2;
    TFT_eSPI& g = G();
    g.fillRoundRect(x, 1, w, 13, 6, C_BG);
    g.drawRoundRect(x, 1, w, 13, 6, c);
    bool blink = st == S_ERROR || ((millis() / 500) % 2);
    g.fillCircle(x + 8, 7, 2, blink ? c : C_TRACK);
    drawText(t, x + 14, 8, ML_DATUM, 1, c, C_BG, w - 18);
  }

  // ---- K1: Klasik kadran -------------------------------------------------------
  void dial(int cx, int cy, int R, float f, int nLab, float lo, float step, int redFrom, bool rpmDial) {
    TFT_eSPI& g = G();
    uint16_t yel = c565(255, 205, 40), ring = c565(150, 160, 175);
    // cam zemin: koyu laciverten siyaha
    for (int r = R - 3; r > 0; r -= 3) g.fillCircle(cx, cy, r, blend565(c565(12, 30, 52), C_BG, 1 - r / (float)R * 0.9f));
    // metal cerceve
    g.drawArc(cx, cy, R, R - 2, 0, 360, ring, C_BG, true);
    g.drawArc(cx, cy, R - 3, R - 4, 0, 360, c565(40, 55, 75), C_BG, true);
    const float A0 = 45, A1 = 315;                              // 270 derece
    // ince ve kalin cizgiler
    int nMin = (nLab - 1) * 4;
    for (int i = 0; i <= nMin; i++) {
      float a = rad(A0 + (A1 - A0) * i / nMin);
      bool major = i % 4 == 0;
      bool red = rpmDial && i >= redFrom * 4;
      float r1 = R - 5, r2 = R - (major ? 11 : 8);
      g.drawWideLine(cx + cosf(a) * r1, cy + sinf(a) * r1, cx + cosf(a) * r2, cy + sinf(a) * r2, major ? 2 : 1,
                     red ? C_BAD : (major ? yel : c565(120, 110, 60)), C_BG);
    }
    if (rpmDial) g.drawArc(cx, cy, R - 5, R - 8, (int)(A0 + (A1 - A0) * redFrom / (float)(nLab - 1)), (int)A1, C_BAD, C_BG, true);
    // rakamlar (sari)
    for (int k = 0; k < nLab; k++) {
      float a = rad(A0 + (A1 - A0) * k / (nLab - 1));
      char b[6]; snprintf(b, sizeof(b), "%d", (int)(lo + step * k));
      drawText(b, cx + (int)(cosf(a) * (R - 19)), cy + (int)(sinf(a) * (R - 19)), MC_DATUM, 1, rpmDial && k >= redFrom ? C_BAD : yel, yel);
    }
    // ibre izi: arkasinda yumusak turuncu isik
    float an = A0 + (A1 - A0) * constrain(f, 0.0f, 1.0f);
    for (int i = 0; i < 6; i++) {
      float a0 = max(A0, an - 4 - i * 5), a1 = an - i * 5;
      if (a1 <= A0) break;
      g.drawArc(cx, cy, R - 12, 10, (int)a0, (int)a1, blend565(C_BG, c565(255, 70, 20), 0.28f - i * 0.045f), C_BG, false);
    }
    float a = rad(an);
    g.drawWideLine(cx + cosf(a) * 8, cy + sinf(a) * 8, cx + cosf(a) * (R - 7), cy + sinf(a) * (R - 7), 5, blend565(C_BG, c565(255, 60, 20), 0.45f), C_BG);
    g.drawWideLine(cx + cosf(a) * 8, cy + sinf(a) * 8, cx + cosf(a) * (R - 7), cy + sinf(a) * (R - 7), 2, c565(255, 150, 40), C_BG);
    // gobek
    g.fillSmoothCircle(cx, cy, 8, c565(20, 26, 36), C_BG);
    g.fillSmoothCircle(cx - 2, cy - 2, 3, c565(90, 110, 140), c565(20, 26, 36));
  }
  // Dikey: ustte buyuk hiz kadrani, altta kucuk devir kadrani
  void k1p() {
    TFT_eSPI& g = G();
    g.fillRect(0, 0, SW, SH, C_BG);
    uint16_t yel = c565(255, 205, 40);
    float s = spdV(), r = rpmV();
    char b[16], c[8];
    clockStr(c, sizeof(c)); drawText(c, 4, 6, ML_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    ambStr(b, sizeof(b), false);
    drawText(b, SW - 4, 6, MR_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    dial(67, 76, 60, s / 160.0f, 9, 0, 20, 99, false);
    snprintf(b, sizeof(b), "%d", (int)s); vbg = C_BG;
    vNumber(b, 67, 106, 13, 0.16f, 38, yel);
    drawText("km/sa", 67, 121, MC_DATUM, 1, c565(150, 130, 60), c565(150, 130, 60));
    // orta serit: menzil ve su sicakligi
    rangeStr(b, sizeof(b), "km");
    icFuel(12, 146, C_TEXT); drawText(b, 22, 147, ML_DATUM, 1, C_TEXT, C_TEXT);
    float ct = has[V_COOLANT] ? val[V_COOLANT] : 0, ff = constrain((ct - 50) / 70.0f, 0.0f, 1.0f);
    for (int i = 0; i < 5; i++) g.fillRoundRect(92 + i * 8, 145, 6, 3, 1, i / 5.0f < ff ? (i == 4 ? C_BAD : C_TEXT) : c565(50, 55, 65));
    dial(67, 197, 41, r / 8000.0f, 9, 0, 1, 7, true);
    snprintf(b, sizeof(b), "%.1f", r / 1000.0f);
    vNumber(b, 67, 221, 10, 0.16f, 30, C_TEXT);
  }
  void k1() {
    if (portrait) { k1p(); return; }
    TFT_eSPI& g = G();
    g.fillRect(0, 0, SW, SH, C_BG);
    // orta: mavi perspektif isik cizgileri
    for (int i = 0; i < 4; i++) {
      uint16_t c = blend565(C_BG, c565(40, 170, 255), 0.35f - i * 0.07f);
      g.drawLine(SW / 2 - 6 - i * 3, 70, SW / 2 - 28 - i * 10, SH, c);
      g.drawLine(SW / 2 + 6 + i * 3, 70, SW / 2 + 28 + i * 10, SH, c);
    }
    float s = spdV(), r = rpmV();
    dial(58, 68, 57, s / 160.0f, 9, 0, 20, 99, false);
    dial(182, 68, 57, r / 8000.0f, 9, 0, 1, 7, true);
    uint16_t yel = c565(255, 205, 40);
    // alt icerik: dijital hiz, menzil, su sicakligi cubugu
    char b[16];
    snprintf(b, sizeof(b), "%d", (int)s); vbg = C_BG;
    vNumber(b, 58, 103, 12, 0.16f, 36, yel);
    drawText("km/sa", 58, 116, MC_DATUM, 1, c565(150, 130, 60), c565(150, 130, 60));
    snprintf(b, sizeof(b), "%.1f", r / 1000.0f);
    vNumber(b, 182, 103, 12, 0.16f, 36, C_TEXT);
    drawText("x1000/dk", 182, 116, MC_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    // ust orta: saat ve dis sicaklik
    char c[8]; clockStr(c, sizeof(c));
    drawText(c, SW / 2, 6, MC_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    ambStr(b, sizeof(b), false);
    drawText(b, SW / 2, 16, MC_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    rangeStr(b, sizeof(b), "km");
    icFuel(SW / 2 - 16, 125, C_TEXT); drawText(b, SW / 2 + 8, 126, MC_DATUM, 1, C_TEXT, C_TEXT);
    // su sicakligi: sag kadranin altinda C..H cubugu
    float ct = has[V_COOLANT] ? val[V_COOLANT] : 0, ff = constrain((ct - 50) / 70.0f, 0.0f, 1.0f);
    for (int i = 0; i < 5; i++) g.fillRoundRect(103 + i * 7, 27, 6, 3, 1, i / 5.0f < ff ? (i == 4 ? C_BAD : C_TEXT) : c565(50, 55, 65));
  }

  // ---- K2: Dijital koseli ------------------------------------------------------
  //  Pahli yol: noktalar arasi dogrusal; t (0..1) boyunca konum ve dis normal
  struct Pt { float x, y; };
  void pathAt(const Pt* p, int n, float t, float& x, float& y, float& nx, float& ny) {
    float L = 0, seg[8];
    for (int i = 0; i < n - 1; i++) { seg[i] = hypotf(p[i + 1].x - p[i].x, p[i + 1].y - p[i].y); L += seg[i]; }
    float d = t * L;
    for (int i = 0; i < n - 1; i++) {
      if (d <= seg[i] || i == n - 2) {
        float u = seg[i] > 0 ? d / seg[i] : 0;
        x = p[i].x + (p[i + 1].x - p[i].x) * u; y = p[i].y + (p[i + 1].y - p[i].y) * u;
        float dx = (p[i + 1].x - p[i].x) / seg[i], dy = (p[i + 1].y - p[i].y) / seg[i];
        nx = -dy; ny = dx;                                           // sola dik (disa)
        return;
      }
      d -= seg[i];
    }
  }
  void angScale(const Pt* p, int n, float f, int nLab, float lo, float step, int redFrom, bool flip, uint16_t lit) {
    TFT_eSPI& g = G();
    const int N = 48;
    float sgn = flip ? -1 : 1;
    // ic cizgi (cerceve)
    for (int i = 0; i < n - 1; i++) g.drawWideLine(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y, 2, c565(70, 80, 100), C_BG);
    for (int i = 0; i <= N; i++) {
      float t = i / (float)N, x, y, nx, ny; pathAt(p, n, t, x, y, nx, ny);
      nx *= sgn; ny *= sgn;
      bool major = i % (N / (nLab - 1)) == 0;
      bool on = t <= f;
      bool red = t * (nLab - 1) >= redFrom;
      uint16_t c = red ? C_BAD : (on ? lit : c565(90, 100, 120));
      float l = major ? 7 : 4;
      g.drawWideLine(x + nx * 2, y + ny * 2, x + nx * (2 + l), y + ny * (2 + l), major ? 2 : 1, c, C_BG);
      if (major) {
        int k = (int)lroundf(t * (nLab - 1));
        char b[6]; snprintf(b, sizeof(b), "%d", (int)(lo + step * k));
        drawText(b, (int)(x + nx * 15), (int)(y + ny * 15), MC_DATUM, 1, red ? C_BAD : C_TEXT, red ? C_BAD : C_TEXT);
      }
    }
    // deger isaretcisi: parlak cubuk
    float x, y, nx, ny; pathAt(p, n, constrain(f, 0.0f, 1.0f), x, y, nx, ny); nx *= sgn; ny *= sgn;
    g.drawWideLine(x - nx * 3, y - ny * 3, x + nx * 11, y + ny * 11, 3, c565(255, 60, 50), C_BG);
  }
  void infoCard(int x, int y, int w, int h, const char* top, const char* big, const char* unit) {
    TFT_eSPI& g = G();
    g.fillRoundRect(x, y, w, h, 4, c565(14, 18, 26));
    g.drawRoundRect(x, y, w, h, 4, c565(60, 70, 90));
    drawText(top, x + 4, y + 7, ML_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    vbg = c565(14, 18, 26);
    float r = vNumber(big, x + w / 2 - 6, y + h - 12, 14, 0.15f, w - 24, C_TEXT);
    drawText(unit, (int)r + 3, y + h - 6, BL_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
  }
  // Dikey: ustte devir, altta hiz olcegi (pahli kemerler), ortada buyuk hiz
  void k2p() {
    TFT_eSPI& g = G();
    g.fillRect(0, 0, SW, SH, C_BG);
    for (int y = 30; y < SH - 30; y += 2) g.fillRect(24, y, SW - 48, 2, blend565(C_BG, c565(20, 40, 70), 0.5f - fabsf(y - 120) / 180.0f));
    float s = spdV(), r = rpmV();
    static const Pt T[] = {{10, 74}, {10, 32}, {30, 12}, {105, 12}, {125, 32}, {125, 74}};
    static const Pt B[] = {{10, 166}, {10, 208}, {30, 228}, {105, 228}, {125, 208}, {125, 166}};
    angScale(T, 6, r / 8000.0f, 9, 0, 1, 7, false, C_TEXT);
    angScale(B, 6, s / 240.0f, 5, 0, 60, 99, true, C_TEXT);
    char a[12];
    float l100 = (tripKm > 1 && tripL > 0) ? tripL / tripKm * 100 : 0;
    auto row = [&](const char* l, const char* v, int y) {           // "Etiket deger" ortali
      int wl = textW(l, 1), w = wl + 4 + textW(v, 1), x = SW / 2 - w / 2;
      drawText(l, x, y, ML_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
      drawText(v, x + wl + 4, y, ML_DATUM, 1, C_TEXT, C_TEXT);
    };
    snprintf(a, sizeof(a), "%.1fL", l100); row("Tüketim", a, 46);
    rangeStr(a, sizeof(a), "km"); row("Menzil", a, 60);
    char b[12]; snprintf(b, sizeof(b), "%d", (int)s);
    vbg = C_BG;
    vNumber(b, SW / 2, 106, 36, 0.14f, 74, C_TEXT);
    drawText("km/sa", SW / 2, 134, MC_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    char t[20]; snprintf(t, sizeof(t), "Yol %.0f km", tripKm);
    drawText(t, SW / 2, 152, MC_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    for (int x = 42; x < 94; x++) g.drawPixel(x, 176, blend565(C_BG, C_BAD, 1 - fabsf(x - 67) / 26.0f));
    char c[8]; clockStr(c, sizeof(c)); drawText(c, SW / 2, 192, MC_DATUM, 2, C_TEXT, C_BG);
  }
  void k2() {
    if (portrait) { k2p(); return; }
    TFT_eSPI& g = G();
    g.fillRect(0, 0, SW, SH, C_BG);
    // orta: koyu mavi cam panel
    for (int y = 14; y < SH - 12; y += 2) g.fillRect(66, y, SW - 132, 2, blend565(C_BG, c565(20, 40, 70), 0.5f - fabsf(y - 70) / 140.0f));
    float s = spdV(), r = rpmV();
    static const Pt L[] = {{40, 124}, {10, 96}, {10, 40}, {30, 16}, {62, 12}};
    static const Pt Rr[] = {{200, 124}, {230, 96}, {230, 40}, {210, 16}, {178, 12}};
    angScale(L, 5, r / 8000.0f, 9, 0, 1, 7, false, C_TEXT);
    angScale(Rr, 5, s / 240.0f, 7, 0, 40, 99, true, C_TEXT);
    // orta: buyuk dijital hiz
    char b[16]; snprintf(b, sizeof(b), "%d", (int)s);
    vbg = C_BG;
    float rr = vNumber(b, SW / 2 - 10, 88, 30, 0.14f, 56, C_TEXT);
    drawText("km/sa", (int)rr + 3, 104, BL_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    // bilgi kartlari
    char a[10]; float l100 = (tripKm > 1 && tripL > 0) ? tripL / tripKm * 100 : 0;
    snprintf(a, sizeof(a), "%.1f", l100);
    infoCard(70, 22, 48, 34, "Tüketim", a, "L");
    rangeStr(a, sizeof(a), "");
    infoCard(122, 22, 48, 34, "Menzil", a, "km");
    // alt: yolculuk, saat + kirmizi isik cizgisi
    char t[20]; snprintf(t, sizeof(t), "Yol %.0f km", tripKm);
    drawText(t, 70, SH - 8, ML_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    char c[8]; clockStr(c, sizeof(c)); drawText(c, SW - 70, SH - 8, MR_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    for (int x = 90; x < 150; x++) g.drawPixel(x, SH - 16, blend565(C_BG, C_BAD, 1 - fabsf(x - 120) / 30.0f));
  }

  // ---- K3: Kirmizi spor ------------------------------------------------------------
  void redScale(const Pt* p, int n, float f, int nLab, float lo, float step, int warnFrom, bool flip) {
    TFT_eSPI& g = G();
    float sgn = flip ? -1 : 1;
    // kirmizi isik bandi (icte): birkac kat, iceri dogru sonuk
    for (int k = 4; k >= 0; k--) {
      float off = -3 - k * 2.2f;
      uint16_t c = blend565(C_BG, c565(255, 20, 30), k == 0 ? 0.95f : 0.30f - k * 0.05f);
      for (int i = 0; i < n - 1; i++) {
        float x0, y0, nx0, ny0, x1, y1, nx1, ny1;
        pathAt(p, n, i / (float)(n - 1), x0, y0, nx0, ny0);
        float ex = p[i + 1].x, ey = p[i + 1].y, sx = p[i].x, sy = p[i].y;
        float dx = ex - sx, dy = ey - sy, L = hypotf(dx, dy); float nx = -dy / L * sgn, ny = dx / L * sgn;
        g.drawWideLine(sx + nx * off, sy + ny * off, ex + nx * off, ey + ny * off, k == 0 ? 2 : 3, c, C_BG);
      }
    }
    // beyaz dis cizgi + sik tikler
    for (int i = 0; i < n - 1; i++) g.drawWideLine(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y, 2, C_TEXT, C_BG);
    const int N = 64;
    for (int i = 0; i <= N; i++) {
      float t = i / (float)N, x, y, nx, ny; pathAt(p, n, t, x, y, nx, ny); nx *= sgn; ny *= sgn;
      bool major = i % (N / (nLab - 1)) == 0;
      g.drawLine(x + nx * 1, y + ny * 1, x + nx * (major ? 6 : 3), y + ny * (major ? 6 : 3), C_TEXT);
      if (major) {
        int k = (int)lroundf(t * (nLab - 1));
        char b[6]; snprintf(b, sizeof(b), "%d", (int)(lo + step * k));
        uint16_t c = k >= warnFrom + 1 ? C_BAD : k == warnFrom ? c565(255, 150, 40) : C_TEXT;
        drawText(b, (int)(x - nx * 11), (int)(y - ny * 11), MC_DATUM, 1, c, c);
      }
    }
    // ibre: kirmizi parlak kama
    float x, y, nx, ny; pathAt(p, n, constrain(f, 0.0f, 1.0f), x, y, nx, ny); nx *= sgn; ny *= sgn;
    for (int k = 3; k >= 1; k--) g.fillSmoothCircle(x - nx * 7, y - ny * 7, 3 + k * 2, blend565(C_BG, c565(255, 20, 30), 0.14f * (4 - k)), C_BG);
    g.drawWideLine(x + nx * 2, y + ny * 2, x - nx * 16, y - ny * 16, 3, c565(255, 60, 60), C_BG);
  }
  // Dikey: ustte hiz, altta devir olcegi (kirmizi isikli), ortada buyuk rakamlar
  void k3p() {
    TFT_eSPI& g = G();
    g.fillRect(0, 0, SW, SH, C_BG);
    float s = spdV(), r = rpmV();
    static const Pt T[] = {{20, 80}, {20, 40}, {40, 20}, {95, 20}, {115, 40}, {115, 80}};
    static const Pt B[] = {{20, 160}, {20, 200}, {40, 220}, {95, 220}, {115, 200}, {115, 160}};
    redScale(T, 6, s / 240.0f, 9, 0, 30, 99, false);
    redScale(B, 6, r / 8000.0f, 9, 0, 1, 6, true);
    char t[16];
    rangeStr(t, sizeof(t), "km"); icFuel(34, 48, C_TEXT); drawText(t, 42, 49, ML_DATUM, 1, C_TEXT, C_TEXT);
    ambStr(t, sizeof(t), true);
    drawText(t, SW / 2, 64, MC_DATUM, 1, C_TEXT2, C_TEXT2);
    char b[12]; snprintf(b, sizeof(b), "%d", (int)s);
    vbg = C_BG;
    vNumber(b, SW / 2, 98, 30, 0.13f, 70, C_TEXT);
    g.drawFastHLine(32, 117, 71, C_TEXT);
    drawText("km/sa", SW / 2, 125, MC_DATUM, 1, C_TEXT2, C_TEXT2);
    snprintf(b, sizeof(b), "%.1f", r / 1000.0f);
    vNumber(b, SW / 2, 150, 22, 0.13f, 60, C_TEXT);
    drawText("x1000/dk", SW / 2, 169, MC_DATUM, 1, C_TEXT2, C_TEXT2);
    char c[8]; clockStr(c, sizeof(c)); drawText(c, SW / 2, 188, MC_DATUM, 2, C_TEXT, C_BG);
    drawText("SPORT", SW / 2, 206, MC_DATUM, 1, C_BAD, C_BAD);
  }
  void k3() {
    if (portrait) { k3p(); return; }
    TFT_eSPI& g = G();
    g.fillRect(0, 0, SW, SH, C_BG);
    float s = spdV(), r = rpmV();
    static const Pt L[] = {{62, 118}, {28, 102}, {20, 64}, {36, 32}, {76, 20}, {112, 18}};
    static const Pt Rr[] = {{178, 118}, {212, 102}, {220, 64}, {204, 32}, {164, 20}, {128, 18}};
    redScale(L, 6, s / 240.0f, 9, 0, 30, 99, false);
    redScale(Rr, 6, r / 8000.0f, 9, 0, 1, 6, true);
    // buyuk dijital hiz (sol) ve devir (sag), alt cizgili
    char b[12]; snprintf(b, sizeof(b), "%d", (int)s);
    vbg = C_BG;
    vNumber(b, 80, 56, 26, 0.13f, 50, C_TEXT);
    drawText("km/sa", 80, 84, MC_DATUM, 1, C_TEXT2, C_TEXT2);
    g.drawFastHLine(52, 76, 56, C_TEXT);
    snprintf(b, sizeof(b), "%.1f", r / 1000.0f);
    vNumber(b, 160, 56, 26, 0.13f, 50, C_TEXT);
    drawText("x1000/dk", 160, 84, MC_DATUM, 1, C_TEXT2, C_TEXT2);
    g.drawFastHLine(132, 76, 56, C_TEXT);
    // alt satir: menzil, saat, SPORT, dis sicaklik
    char t[16];
    rangeStr(t, sizeof(t), "km"); icFuel(66, 101, C_TEXT); drawText(t, 74, 102, ML_DATUM, 1, C_TEXT, C_TEXT);
    char c[8]; clockStr(c, sizeof(c)); drawText(c, SW / 2, 122, MC_DATUM, 2, C_TEXT, C_TEXT);
    drawText("SPORT", 124, 102, MC_DATUM, 1, C_BAD, C_BAD);
    ambStr(t, sizeof(t), true);
    drawText(t, 176, 102, MR_DATUM, 1, C_TEXT, C_TEXT);
  }

  // Kokpit sayfasi (secili ekran yonune gore yatay veya dikey yerlesim)
  void pageCockpit() {
    uint32_t now = millis();
    float ts = has[V_SPEED] ? val[V_SPEED] : 0, tr = has[V_RPM] ? val[V_RPM] : 0;
    kS += (ts - kS) * 0.3f; kR += (tr - kR) * 0.3f;
    if (sweepAt && now - sweepAt < 1400) {                         // baglanti sonrasi ibre selamlamasi
      float p = (now - sweepAt) / 1400.0f, f = p < 0.5f ? p * 2 : (1 - p) * 2;
      kS = max(kS, 240 * f); kR = max(kR, 8000 * f);
    }
    switch (oc.style) { case 2: k1(); break; case 3: k2(); break; default: k3(); }   // her tema yatay + dikey
    offBanner();
    if (alarm == 3 && (now / 110) % 2) G().fillRoundRect(SW / 2 - 50, 0, 100, 4, 2, C_BAD);   // vites isigi
    for (int i = 0; i < pn; i++) G().fillCircle(SW / 2 + (i - pn / 2) * 6, SH - 2, i == page ? 1 : 0, i == page ? C_TEXT : C_TRACK);
  }

  void pageGauge(uint8_t v) {
    uint32_t now = millis();
    float lo, hi, red; range(v, lo, hi, red);
    if (shownFor != v) { shownFor = v; shown = lo; pageAt = now; }
    float target = has[v] ? val[v] : lo;
    // sayfa acilisinda ibre gibi supurme, sonra yumusak takip
    float sw = (now - pageAt) / 650.0f;
    if (sw < 1) shown = lo + (target - lo) * (1 - (1 - sw) * (1 - sw) * (1 - sw));
    else shown += (target - shown) * 0.3f;
    // baglanti sonrasi tam tur "selamlama"
    float f = constrain((shown - lo) / (hi - lo), 0.0f, 1.0f);
    if (sweepAt && now - sweepAt < 1400) { float p = (now - sweepAt) / 1400.0f; f = p < 0.5f ? p * 2 : (1 - p) * 2 + f * (p - 0.5f) * 2; f = constrain(f, 0.0f, 1.0f); }
    uint16_t vc = valColor(v);
    if (oc.style == 0 || oc.style == 4) dashSport(v, f, red, vc); else dashComfort(v, f, red, vc);
    // ust satir: baglanti + aku, alt satir: ikincil bilgiler + sayfa noktalari
    TFT_eSPI& g = G();
    uint16_t dc = st == S_RUN ? C_OK : C_WARN;
    g.fillCircle(6, 6, 3, dc);
    if (has[V_BATT] && v != V_BATT) { char bb[10]; snprintf(bb, sizeof(bb), "%.1fV", val[V_BATT]); drawText(bb, SW - 3, 7, MR_DATUM, 1, valColor(V_BATT), valColor(V_BATT)); }
    char l[24], r[24]; secondary(v, l, r);
    if (portrait) {                                               // dikey: alt bosluga iki satir
      drawText(l, SW / 2, 196, MC_DATUM, 2, C_TEXT2, C_TEXT2, SW - 6);
      drawText(r, SW / 2, 216, MC_DATUM, 2, C_SUBTEXT, C_SUBTEXT, SW - 6);
    } else {
      drawText(l, 4, SH - 5, BL_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
      drawText(r, SW - 4, SH - 5, BR_DATUM, 1, C_SUBTEXT, C_SUBTEXT);
    }
    for (int i = 0; i < pn; i++) g.fillCircle(SW / 2 + (i - pn / 2) * 6, SH - 3, i == page ? 2 : 1, i == page ? C_TEXT : C_TRACK);
    // vites isigi
    if (alarm == 3 && (now / 110) % 2) { g.fillRoundRect(SW / 2 - 50, 0, 100, 5, 2, C_BAD); }
  }

  // ---- Yolculuk gecmisi (son 10, NVS) ------------------------------------------
  struct TripRec { uint32_t at; float km, l, maxV; uint32_t sec; };
  TripRec trips[10]; uint8_t nTrips = 0; bool tripsLoaded = false; int histOff = 0;
  void loadTrips() {
    if (tripsLoaded) return;
    tripsLoaded = true;
    op.begin("obd", true);
    size_t n = op.getBytesLength("trips");
    if (n && n % sizeof(TripRec) == 0 && n <= sizeof(trips)) { op.getBytes("trips", trips, n); nTrips = n / sizeof(TripRec); }
    op.end();
  }
  void saveTrips() {
    op.begin("obd", false);
    if (nTrips) op.putBytes("trips", trips, nTrips * sizeof(TripRec)); else op.remove("trips");
    op.end();
  }
  // Mevcut yolculugu gecmise yaz ve sifirla (0.3 km altindakiler kaydedilmez). true: kaydedildi
  bool closeTrip() {
    bool saved = false;
    if (tripKm >= 0.3f) {
      loadTrips();
      memmove(&trips[1], &trips[0], sizeof(TripRec) * 9);
      trips[0].at = Net::timeValid() ? (uint32_t)time(nullptr) : 0;
      trips[0].km = tripKm; trips[0].l = tripL; trips[0].maxV = tripMaxV; trips[0].sec = tripMs / 1000;
      if (nTrips < 10) nTrips++;
      saveTrips(); saved = true;
    }
    tripKm = tripL = tripMaxV = 0; tripMs = 0;
    Health::save();
    return saved;
  }
  void pageHist() {
    loadTrips();
    topBar("Geçmiş");
    TFT_eSPI& g = G();
    if (!nTrips) {
      drawText("Henüz kayıt yok", SW / 2, SH / 2 - 6, MC_DATUM, 2, C_SUBTEXT, C_BG);
      drawText("Motor durunca kaydedilir", SW / 2, SH / 2 + 12, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 6);
      return;
    }
    int top = 19, rowH = portrait ? 36 : 25, vis = (SH - top - 7) / rowH;
    if (histOff > max(0, nTrips - vis)) histOff = 0;
    for (int r = 0; r < vis && histOff + r < nTrips; r++) {
      const TripRec& t = trips[histOff + r];
      int y = top + r * rowH;
      uint16_t bg = r % 2 ? C_BG : C_PANEL;
      g.fillRoundRect(2, y, SW - 4, rowH - 2, 4, bg);
      char d[16], a[24], c[28];
      if (t.at > 1700000000UL) {
        time_t tt = t.at; struct tm tmv; localtime_r(&tt, &tmv);
        snprintf(d, sizeof(d), "%02d.%02d %02d:%02d", tmv.tm_mday, tmv.tm_mon + 1, tmv.tm_hour, tmv.tm_min);
      } else snprintf(d, sizeof(d), "#%d", histOff + r + 1);
      snprintf(a, sizeof(a), "%.1f km  %lu dk", t.km, (unsigned long)((t.sec + 30) / 60));
      if (t.l > 0.05f && t.km > 0.5f) snprintf(c, sizeof(c), "%.1f L/100  %d TL", t.l / t.km * 100, (int)lroundf(t.l * oc.price));
      else strcpy(c, "tüketim yok");
      if (portrait) {
        drawText(d, 6, y + 6, ML_DATUM, 1, C_SUBTEXT, bg);
        drawText(a, 6, y + 17, ML_DATUM, 1, C_TEXT, bg, SW - 10);
        drawText(c, 6, y + 28, ML_DATUM, 1, C_ACCENT, bg, SW - 10);
      } else {
        drawText(d, 6, y + 6, ML_DATUM, 1, C_SUBTEXT, bg);
        drawText(a, SW - 6, y + 6, MR_DATUM, 1, C_TEXT, bg);
        drawText(c, 6, y + 17, ML_DATUM, 1, C_ACCENT, bg, SW - 12);
        char m[14]; snprintf(m, sizeof(m), "maks %.0f", t.maxV); drawText(m, SW - 6, y + 17, MR_DATUM, 1, C_SUBTEXT, bg);
      }
    }
  }

  // ---- Hizlanma sayfasi ------------------------------------------------------------
  void pageAccel() {
    topBar("Hızlanma");
    uint32_t now = millis();
    const char* stx; uint16_t sc;
    switch (aSt) {
      case A_WAIT:  stx = "Aracı durdurun"; sc = C_WARN; break;
      case A_READY: stx = "Hazır: kalkışta başlar"; sc = C_OK; break;
      case A_RUN:   stx = "Ölçülüyor..."; sc = C_ACCENT; break;
      default:      stx = "Bitti (M5 uzun: yeni)"; sc = C_SUBTEXT; break;
    }
    if (st != S_RUN) { stx = "Araç bağlı değil"; sc = C_WARN; }
    char b[16], r1[20], r2[24], r3[16], r4[16];
    float tm = aSt == A_RUN ? (now - aT0) / 1000.0f : a100;
    if (aSt == A_DONE && a100 <= 0) strcpy(b, "--"); else snprintf(b, sizeof(b), "%.2f", tm);
    snprintf(r1, sizeof(r1), "%d km/sa", has[V_SPEED] ? (int)val[V_SPEED] : 0);
    if (a402 > 0) snprintf(r2, sizeof(r2), "%.2f sn  %.0f", a402, a402v);
    else if (aSt == A_RUN) snprintf(r2, sizeof(r2), "%.0f m", aDist); else strcpy(r2, "--");
    if (best100 > 0) snprintf(r3, sizeof(r3), "%.2f sn", best100); else strcpy(r3, "--");
    if (best402 > 0) snprintf(r4, sizeof(r4), "%.2f sn", best402); else strcpy(r4, "--");
    uint16_t nc = aSt == A_RUN ? C_ACCENT : (aSt == A_DONE && a100 > 0 && a100 <= best100 + 0.001f) ? C_OK : C_TEXT;
    vbg = C_BG;
    static const char* L[] = {"Hız", "402 m", "En iyi 0-100", "En iyi 402 m"};
    const char* V[] = {r1, r2, r3, r4};
    if (portrait) {
      vNumber(b, SW / 2, 62, 36, 0.14f, SW - 16, nc);
      drawText("saniye", SW / 2, 90, MC_DATUM, 1, C_SUBTEXT, C_BG);
      for (int i = 0; i < 4; i++) {
        int y = 110 + i * 24;
        drawText(L[i], 6, y, ML_DATUM, 1, C_SUBTEXT, C_BG);
        drawText(V[i], SW - 6, y + 11, MR_DATUM, 2, i >= 2 ? C_OK : C_TEXT, C_BG, SW - 12);
      }
      drawText(stx, SW / 2, SH - 12, MC_DATUM, 1, sc, C_BG, SW - 4);
    } else {
      vNumber(b, 66, 58, 38, 0.14f, 116, nc);
      drawText("saniye", 66, 88, MC_DATUM, 1, C_SUBTEXT, C_BG);
      for (int i = 0; i < 4; i++) {
        int y = 24 + i * 23;
        drawText(L[i], 134, y + 3, ML_DATUM, 1, C_SUBTEXT, C_BG);
        drawText(V[i], 134, y + 14, ML_DATUM, 2, i >= 2 ? C_OK : C_TEXT, C_BG, SW - 138);
      }
      drawText(stx, 66, 108, MC_DATUM, 1, sc, C_BG, 126);
    }
  }

  // ---- Canli grafik (son 60 sn) ----------------------------------------------------
  uint8_t nextGraph(uint8_t cur) {
    int k = 0; while (k < (int)COUNT_OF(ORDER) && ORDER[k] != cur) k++;
    for (int i = 1; i <= (int)COUNT_OF(ORDER); i++) {
      uint8_t v = ORDER[(k + i) % COUNT_OF(ORDER)];
      if (v == V_RUNTIME || v == V_DISTCLR) continue;
      if (st != S_RUN || sup[v]) return v;
    }
    return cur;
  }
  void pageGraph() {
    uint8_t v = oc.focus;
    topBar("Grafik");
    TFT_eSPI& g = G();
    char b[16], u[24];
    if (has[v]) valueText(v, val[v], b, sizeof(b)); else strcpy(b, "--");
    uint16_t vc = has[v] ? valColor(v) : C_SUBTEXT;
    int x0 = 6, x1 = SW - 6, y0, y1;
    if (portrait) {
      drawText(DEFS[v].name, SW / 2, 28, MC_DATUM, 2, C_TEXT2, C_BG, SW - 6);
      vbg = C_BG; float r = vNumber(b, SW / 2, 56, 28, 0.14f, SW - 30, vc);
      drawText(DEFS[v].unit, SW / 2, 80, MC_DATUM, 1, C_SUBTEXT, C_BG); (void)r;
      y0 = 94; y1 = SH - 30;
    } else {
      drawText(DEFS[v].name, 6, 29, ML_DATUM, 2, C_TEXT2, C_BG, 120);
      snprintf(u, sizeof(u), "%s %s", b, DEFS[v].unit);
      drawText(u, SW - 6, 29, MR_DATUM, 2, vc, C_BG, 110);
      y0 = 40; y1 = SH - 16;
    }
    g.drawRoundRect(x0 - 2, y0 - 2, x1 - x0 + 4, y1 - y0 + 4, 4, C_TRACK);
    if (histN < 2) { drawText("Veri bekleniyor", SW / 2, (y0 + y1) / 2, MC_DATUM, 1, C_SUBTEXT, C_BG); }
    else {
      float lo = 1e9f, hi = -1e9f;
      for (int i = 0; i < histN; i++) { float x = hist[(histI - histN + i + 120) % 120]; lo = min(lo, x); hi = max(hi, x); }
      float pad = max((hi - lo) * 0.12f, (DEFS[v].hi - DEFS[v].lo) * 0.02f);
      lo -= pad; hi += pad;
      for (int k = 1; k < 4; k++) {                                   // kesikli yatay cizgiler
        int y = y0 + (y1 - y0) * k / 4;
        for (int x = x0; x < x1; x += 4) g.drawPixel(x, y, C_TRACK);
      }
      int px = -1, py = -1;
      uint16_t fillc = blend565(C_BG, C_ACCENT, 0.22f);
      for (int i = 0; i < histN; i++) {
        float x = hist[(histI - histN + i + 120) % 120];
        int sx = x1 - (histN - 1 - i) * (x1 - x0) / 119;
        int sy = y1 - (int)((x - lo) / (hi - lo) * (y1 - y0));
        sy = constrain(sy, y0, y1);
        if (px >= 0) {
          for (int xx = px; xx <= sx; xx++) { int yy = py + (sy - py) * (xx - px) / max(1, sx - px); g.drawFastVLine(xx, yy + 1, y1 - yy, fillc); }
          g.drawWideLine(px, py, sx, sy, 2, C_ACCENT, fillc);
        }
        px = sx; py = sy;
      }
      g.fillCircle(px, py, 3, C_TEXT);
      char a[12], c[12], m[28];
      valueText(v, hi - pad, a, sizeof(a)); valueText(v, lo + pad, c, sizeof(c));
      drawText(a, x0 + 2, y0 + 5, ML_DATUM, 1, C_SUBTEXT, C_BG);
      drawText(c, x0 + 2, y1 - 5, ML_DATUM, 1, C_SUBTEXT, C_BG);
      if (portrait) {
        snprintf(m, sizeof(m), "min %s  maks %s", c, a);
        drawText(m, SW / 2, SH - 21, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 4);
      }
    }
    drawText(portrait ? "M5 uzun: veri" : "60 sn   M5 uzun: veri değiştir", SW / 2, SH - 10, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 4);
  }

  void pageTrip() {
    topBar("Yolculuk");
    char b[6][24];
    uint32_t s = tripMs / 1000;
    snprintf(b[0], 24, "%lu:%02lu", (unsigned long)(s / 3600), (unsigned long)(s / 60 % 60));
    snprintf(b[1], 24, "%.1f km", tripKm);
    float avg = tripMs > 60000 ? tripKm / (tripMs / 3600000.0f) : 0;
    snprintf(b[2], 24, "%.0f km/sa", avg);
    snprintf(b[3], 24, "%.0f km/sa", tripMaxV);
    if (sup[V_LPH]) {
      snprintf(b[4], 24, "%.2f L", tripL);
      if (tripKm > 0.5f) snprintf(b[5], 24, "%.1f L/100", tripL / tripKm * 100); else strcpy(b[5], "--");
    } else { strcpy(b[4], "yok"); strcpy(b[5], "yok"); }
    static const char* L[] = {"Motor süresi", "Mesafe", "Ort. hız", "Maks. hız", "Yakıt", "Ort. tüketim"};
    int rows = 3, cols = 2, top = 22;
    int w = (SW - 12) / cols, h = (SH - top - 4) / rows;
    if (portrait) { rows = 6; cols = 1; w = SW - 12; h = (SH - top - 4) / rows; }
    for (int i = 0; i < 6; i++) {
      int x = 6 + (i % cols) * w, y = top + (i / cols) * h;
      drawText(L[i], x + 2, y + 7, ML_DATUM, 1, C_SUBTEXT, C_BG);
      drawText(b[i], x + 2, y + h / 2 + 5, ML_DATUM, 2, i == 5 ? C_ACCENT : C_TEXT, C_BG, w - 6);
    }
  }
  // ---- Hava karti (iPhone Kestirmeler verisi + aracin dis sicaklik sensoru) --------
  bool ambOk() { return st == S_RUN && has[V_AMBIENT]; }
  void wxChart(int x0, int yB, int w, int hMax) {
    TFT_eSPI& g = G();
    int i0 = Wx::idxNow(), n = WX_N;
    float slot = w / (float)n; int bw = max(3, (int)(slot * 0.62f));
    g.drawFastHLine(x0, yB + 1, w, C_TRACK);
    for (int k = 0; k < n; k++) {
      int i = i0 + k, cx = x0 + (int)(slot * k + slot / 2);
      bool ok = i0 >= 0 && i < rtcWx.nH;
      int p = ok ? rtcWx.hP[i] : 0, h = max(2, p * hMax / 100);
      g.fillRoundRect(cx - bw / 2, yB - h + 1, bw, h, min(2, bw / 2), (ok && p) ? rainCol(p) : C_TRACK);
      if (k == 0) g.fillRect(cx - bw / 2, yB + 3, bw, 2, C_ACCENT);                // simdi
      if (k % 3 == 0) {
        char t[4]; snprintf(t, sizeof(t), "%02d", (Net::localHour() + k) % 24);
        drawText(t, cx, yB + 10, MC_DATUM, 1, k == 0 ? C_ACCENT : C_SUBTEXT, C_BG);
      }
    }
  }
  void pageWeather() {
    TFT_eSPI& g = G();
    char b[32], t[8];
    bool wx = Net::hasWeather();
    uint8_t pkH = 0, pk = Wx::rainPeak(12, &pkH);
    vbg = C_BG;
    if (!wx) {                                                      // veri yok: arac sensoru + nasil alinir
      int cy = portrait ? 70 : 42;
      drawWxIconS(SW / 2, cy, 3, true, portrait ? 2.4f : 2.0f);
      if (ambOk()) {
        snprintf(t, sizeof(t), "%d", (int)val[V_AMBIENT]);
        float r = vNumber(t, SW / 2 - 4, cy + (portrait ? 58 : 44), 30, 0.13f, 90, C_ACCENT);
        g.drawCircle((int)r + 5, cy + (portrait ? 45 : 31), 3, C_ACCENT);
        drawText("Araç dış sıcaklığı", SW / 2, cy + (portrait ? 86 : 68), MC_DATUM, 1, C_SUBTEXT, C_BG);
      }
      drawText("Hava verisi yok", SW / 2, portrait ? 190 : 104, MC_DATUM, 2, C_TEXT2, C_BG, SW - 8);
      drawText(portrait ? "iPhone Kestirmesi" : "iPhone Kestirmesi ile gelir", SW / 2, portrait ? 208 : 121, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 8);
      if (portrait) drawText("ile gelir", SW / 2, 220, MC_DATUM, 1, C_SUBTEXT, C_BG);
      return;
    }
    snprintf(t, sizeof(t), "%d", Wx::tempNow());
    // guncelleme saati
    char up[8] = "";
    { time_t a = rtcWx.wxAt; struct tm tmv; localtime_r(&a, &tmv); snprintf(up, sizeof(up), "%02d:%02d", tmv.tm_hour, tmv.tm_min); }
    if (!portrait) {
      drawText("Hava", 6, 9, ML_DATUM, 2, C_TEXT2, C_BG);
      drawText(up, 44, 10, ML_DATUM, 1, C_SUBTEXT, C_BG);
      if (ambOk()) {                                                  // aracin kendi dis sensoru
        snprintf(b, sizeof(b), "%d", (int)val[V_AMBIENT]);
        int w = drawDeg(b, SW - 6, 9, MR_DATUM, 2, C_ACCENT);
        drawText("Araç", SW - 10 - w, 9, MR_DATUM, 1, C_SUBTEXT, C_BG);
      }
      drawWxIconS(32, 46, Net::wcode, Net::isDay, 2.2f);
      float r = vNumber(t, 92, 45, 34, 0.13f, 70, C_TEXT);
      g.drawCircle((int)r + 5, 31, 3, C_TEXT); g.drawCircle((int)r + 5, 31, 2, C_TEXT);
      int xr = 146;
      drawText(Net::wText(), xr, 30, ML_DATUM, 2, C_TEXT, C_BG, SW - xr - 4);
      snprintf(b, sizeof(b), "%d° / %d°", rtcWx.tmin, rtcWx.tmax); drawText(b, xr, 47, ML_DATUM, 2, C_TEXT2, C_BG);
      snprintf(b, sizeof(b), "Rüzgâr %d km/sa", rtcWx.wind); drawText(b, xr, 61, ML_DATUM, 1, C_SUBTEXT, C_BG, SW - xr - 2);
      g.drawFastHLine(6, 72, SW - 12, C_TRACK);
      drawDrop(10, 82, 3, rainCol(max<int>(pk, 40)));
      drawText("Yağmur", 17, 82, ML_DATUM, 1, C_TEXT2, C_BG);
      if (!rtcWx.nH) drawText("saatlik veri yok", SW - 6, 82, MR_DATUM, 1, C_SUBTEXT, C_BG);
      else if (pk >= 20) { snprintf(b, sizeof(b), "en çok %%%d  %02d:00", pk, pkH); drawText(b, SW - 6, 82, MR_DATUM, 1, rainCol(pk), C_BG); }
      else drawText("12 saat kuru", SW - 6, 82, MR_DATUM, 1, C_OK, C_BG);
      wxChart(8, 113, SW - 16, 22);
    } else {
      drawText("Hava", SW / 2, 10, MC_DATUM, 2, C_TEXT2, C_BG);
      drawWxIconS(SW / 2, 44, Net::wcode, Net::isDay, 2.4f);
      float r = vNumber(t, SW / 2 - 4, 94, 40, 0.13f, 90, C_TEXT);
      g.drawCircle((int)r + 6, 77, 4, C_TEXT); g.drawCircle((int)r + 6, 77, 3, C_TEXT);
      drawText(Net::wText(), SW / 2, 126, MC_DATUM, 2, C_TEXT, C_BG, SW - 8);
      snprintf(b, sizeof(b), "%d° / %d°  %d km/sa", rtcWx.tmin, rtcWx.tmax, rtcWx.wind); drawText(b, SW / 2, 143, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 8);
      if (ambOk()) {
        snprintf(b, sizeof(b), "Araç  %d°C", (int)val[V_AMBIENT]);
        int w = textW(b, 2) + 16; g.fillRoundRect(SW / 2 - w / 2, 152, w, 17, 8, C_PANEL);
        drawText(b, SW / 2, 161, MC_DATUM, 2, C_ACCENT, C_PANEL);
      } else drawText(up, SW / 2, 161, MC_DATUM, 1, C_SUBTEXT, C_BG);
      drawDrop(9, 181, 3, rainCol(max<int>(pk, 40)));
      drawText("Yağmur", 16, 181, ML_DATUM, 1, C_TEXT2, C_BG);
      if (pk >= 20) { snprintf(b, sizeof(b), "%%%d %02d:00", pk, pkH); drawText(b, SW - 5, 181, MR_DATUM, 1, rainCol(pk), C_BG); }
      wxChart(5, 219, SW - 10, 28);
    }
  }

  void pageDtc() {
    TFT_eSPI& g = G();
    topBar("Arıza kodları");
    int y = 26;
    if (!dtcRead) {
      drawText("M5 uzun: kodları oku", SW / 2, SH / 2 - 8, MC_DATUM, 2, C_TEXT, C_BG);
      drawText("Motor arıza lambası ve kayıtlı kodlar", SW / 2, SH / 2 + 12, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 8);
      return;
    }
    // MIL
    g.fillCircle(16, y + 6, 6, mil ? C_WARN : C_TRACK);
    char b[40];
    if (portrait) snprintf(b, sizeof(b), mil ? "Lamba YANIK (%d)" : "Lamba sönük (%d)", max(0, (int)milCount));
    else snprintf(b, sizeof(b), mil ? "Arıza lambası YANIK  (%d kod)" : "Arıza lambası sönük  (%d kod)", max(0, (int)milCount));
    drawText(b, 28, y + 6, ML_DATUM, 2, mil ? C_WARN : C_OK, C_BG, SW - 32);
    y += 20;
    if (dtcN == 0) drawText("Kayıtlı arıza kodu yok", SW / 2, y + 22, MC_DATUM, 2, C_OK, C_BG);
    int cols = portrait ? 2 : 4;
    for (int i = 0; i < dtcN; i++) {
      int x = 6 + (i % cols) * ((SW - 12) / cols), yy = y + (i / cols) * 22;
      g.fillRoundRect(x, yy, (SW - 12) / cols - 4, 18, 5, C_PANEL);
      drawText(dtc[i], x + ((SW - 12) / cols - 4) / 2, yy + 9, MC_DATUM, 2, C_WARN, C_PANEL);
    }
    bool armed = clearArmAt && millis() - clearArmAt < 4000;
    const char* help = portrait ? "M5 uzun: tekrar oku" : "M5 uzun: tekrar oku   Silmek: Ayarlar > OBD2";
    drawText(help, SW / 2, SH - (portrait ? 12 : 7), MC_DATUM, 1, armed ? C_BAD : C_SUBTEXT, C_BG, SW - 4);
  }

  // Tarama / baglanti ekranlari
  void pageScan() {
    TFT_eSPI& g = G();
    drawText("OBD2 adaptör seç", 8, 9, ML_DATUM, 2, C_ACCENT, C_BG);
    if (scanning) { float a = (millis() % 1200) / 1200.0f * 360; g.drawSmoothArc(SW - 12, 9, 6, 4, (int)a, ((int)a + 90) % 360, C_ACCENT, C_BG, true); }
    int top = 22, rowH = 20, vis = (SH - top - 14) / rowH;
    int first = max(0, dsel - vis + 1);
    for (int r = 0; r < vis && first + r < nDev; r++) {
      int i = first + r, y = top + r * rowH;
      bool s = i == dsel;
      if (s) g.fillRoundRect(3, y, SW - 6, rowH - 2, 5, C_PANEL);
      uint16_t bg = s ? C_PANEL : C_BG;
      drawText(devs[i].name, 10, y + rowH / 2 - 1, ML_DATUM, 2, devs[i].elm ? C_TEXT : C_SUBTEXT, bg, SW - 40);
      if (devs[i].elm) drawText("OBD", SW - 8, y + rowH / 2 - 1, MR_DATUM, 1, C_OK, bg);
    }
    if (!nDev) drawText(scanning ? "Aranıyor... kontak açık olsun" : "Bulunamadı (A uzun: tekrar ara)", SW / 2, SH / 2, MC_DATUM, portrait ? 1 : 2, C_SUBTEXT, C_BG, SW - 6);
    drawText(portrait ? "B: sonraki  A: bağlan" : "B: sonraki  A: bağlan  A uzun: ara  GÜÇ: geri", SW / 2, SH - 6, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 4);
  }
  void pageStatus() {
    TFT_eSPI& g = G();
    float a = (millis() % 1200) / 1200.0f * 360;
    uint16_t c = st == S_ERROR ? C_BAD : st == S_NOECU ? C_WARN : C_ACCENT;
    int cy = portrait ? 90 : 42;
    if (st == S_ERROR) { g.drawWideLine(SW / 2 - 12, cy - 12, SW / 2 + 12, cy + 12, 4, c, C_BG); g.drawWideLine(SW / 2 + 12, cy - 12, SW / 2 - 12, cy + 12, 4, c, C_BG); }
    else g.drawSmoothArc(SW / 2, cy, 18, 14, (int)a, ((int)a + 100) % 360, c, C_BG, true);
    drawText(oc.name[0] ? oc.name : "OBD2", SW / 2, cy + 30, MC_DATUM, 2, C_TEXT, C_BG, SW - 8);
    drawText(msg, SW / 2, cy + 50, MC_DATUM, portrait ? 1 : 2, c, C_BG, SW - 6);
    if (has[V_BATT]) { char b[16]; snprintf(b, sizeof(b), "Akü %.1f V", val[V_BATT]); drawText(b, SW / 2, cy + 67, MC_DATUM, 1, C_SUBTEXT, C_BG); }
    if (!oc.haveMac) drawText("Ayarlar > OBD2 > Adaptör seç", SW / 2, SH - 20, MC_DATUM, 1, C_WARN, C_BG, SW - 4);
    drawText(st == S_ERROR && oc.haveMac ? "A uzun: tekrar bağlan   M5: sonraki" : "M5: sonraki ekran", SW / 2, SH - 6, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 4);
  }

  // ---- OBD ayar menusu (GUC tusu) -------------------------------------------------
  bool menu = false; int msel = 0;
  bool smenu = false;                                // adaptor arama listesi
  // ---- ARAC SAGLIGI (v5.6): degerlerin zaman icindeki seyrine bakan erken uyarilar ------------
  //  Teshis degil, "kontrol ettir" uyarisi. Ogrenilen degerler NVS "obd" icinde (hcr, hwu, hnc...).
  //  Akü      : her marsta dinlenme ve mars ani voltaji (mars oncesi ATRV 4 Hz), son 8 calistirma
  //  Termostat: soguk calistirmadan 80°C'ye isinma suresi; 15 dk surup 75°C altinda kalirsa uyari
  //  Hararet  : motorun ogrenilen normal sicakliginin 8°C+ ustunde calisma (alarm esiginden once)
  //  Yakit ay.: uzun donem yakit duzeltme (LTFT) ±%10 disinda kalirsa (sicak motor)
  //  Rolanti  : duruyorken devir dalgalanmasi (standart sapma) yuksekse
  //  Bakim    : M5'in saydigi km ile bakim araligi
  namespace Health {
    enum : uint8_t { H_NONE, H_OK, H_WARN, H_BAD };
    enum Kind : uint8_t { K_BATT, K_THERM, K_HOT, K_TRIM, K_IDLE, K_SVC, K_N };
    static const uint16_t SVC_KM[] = {0, 5000, 7500, 10000, 15000, 20000};
    // kalici
    uint8_t  crank[8] = {0}; uint8_t nCrank = 0;      // mars voltaji kodlu: (V-6)*20
    uint8_t  warm[5] = {0};  uint8_t nWarm = 0;       // isinma suresi (dk)
    float    normC = 0;      uint16_t nNorm = 0;      // ogrenilen normal motor sicakligi
    float    km = 0, svcKm = 0; uint8_t svcIdx = 0;   // M5'in saydigi toplam km, son bakim km'si, aralik
    float    lastRest = 0;                            // son dinlenme voltaji
    // anlik
    float    ltft = 0; uint8_t nLtft = 0; uint32_t ltftAt = 0;
    float    hotEma = 0; uint32_t hotSince = 0;
    uint16_t idleBuf[30]; uint8_t idleN = 0; uint8_t idleBad = 0; float idleSd = 0; uint32_t idleAt = 0;
    bool     engWasOff = false, running = false; uint32_t startAt = 0; float startC = -99, startKm = 0;
    uint8_t  warmMinNow = 0; bool warmDone = false, thermWarn = false;
    struct VS { uint32_t t; float v; } vr[24]; uint8_t vrI = 0;
    uint8_t  hs[K_N] = {0};                           // durum (H_*)
    bool     shown[K_N] = {false};                    // bu acilista uyari gosterildi
    bool     dirty = false;

    static float cdec(uint8_t c) { return 6.0f + c / 20.0f; }
    static uint8_t cenc(float v) { return (uint8_t)constrain((int)lroundf((v - 6.0f) * 20.0f), 1, 255); }
    float crankLast() { return nCrank ? cdec(crank[0]) : 0; }

    void load() {
      op.begin("obd", true);
      nCrank = op.getBytes("hcr", crank, 8); nCrank = min<uint8_t>(nCrank, 8); while (nCrank && !crank[nCrank - 1]) nCrank--;
      nWarm = op.getBytes("hwu", warm, 5); nWarm = min<uint8_t>(nWarm, 5); while (nWarm && !warm[nWarm - 1]) nWarm--;
      normC = op.getFloat("hnc", 0); nNorm = op.getUShort("hnn", 0);
      km = op.getFloat("hkm", 0); svcKm = op.getFloat("hsk", 0); svcIdx = op.getUChar("hsv", 0) % COUNT_OF(SVC_KM);
      lastRest = op.getFloat("hrv", 0);
      op.end();
    }
    void save() {
      op.begin("obd", false);
      op.putBytes("hcr", crank, 8); op.putBytes("hwu", warm, 5);
      op.putFloat("hnc", normC); op.putUShort("hnn", nNorm);
      op.putFloat("hkm", km); op.putFloat("hsk", svcKm); op.putUChar("hsv", svcIdx); op.putFloat("hrv", lastRest);
      op.end();
      dirty = false;
    }
    void onKm(float d) { km += d; }
    bool rpmFresh() { return has[V_RPM] && millis() - lastAt[V_RPM] < 3000; }
    // Mars oncesi / marsta voltaji sik oku
    bool fastVolt() { return st == S_RUN && rpmFresh() && val[V_RPM] < 450 && !running; }
    void onVolt(float v) { vr[vrI] = {millis(), v}; vrI = (vrI + 1) % 24; }

    void evalBatt() {
      if (!nCrank) { hs[K_BATT] = H_NONE; return; }
      float c = cdec(crank[0]);
      float a = 0, b = 0; int na = 0, nb = 0;
      for (int i = 0; i < nCrank; i++) { if (i < 3) { a += cdec(crank[i]); na++; } else { b += cdec(crank[i]); nb++; } }
      a /= max(1, na); if (nb) b /= nb;
      if (c < 9.6f) hs[K_BATT] = H_BAD;
      else if ((nb >= 2 && a < b - 0.35f && c < 10.3f) || (lastRest > 0 && lastRest < 12.2f)) hs[K_BATT] = H_WARN;
      else hs[K_BATT] = H_OK;
    }
    void recordStart() {                              // motor calisti: mars penceresini degerlendir
      uint32_t now = millis(); float lo = 99, hi = 0;
      for (int i = 0; i < 24; i++) {
        if (!vr[i].t || now - vr[i].t > 6000) continue;
        lo = min(lo, vr[i].v);
        if (now - vr[i].t > 1200) hi = max(hi, vr[i].v); // mars oncesi en yuksek = dinlenme
      }
      if (lo < 99 && hi > 0 && hi - lo >= 0.4f && lo > 6.5f) {
        memmove(crank + 1, crank, 7); crank[0] = cenc(lo); nCrank = min<uint8_t>(nCrank + 1, 8);
        lastRest = hi; dirty = true;
        Serial.printf("[Saglik] mars: dinlenme %.2f V, en dusuk %.2f V\n", hi, lo);
        evalBatt();
      }
    }

    void service() {
      static uint32_t lastEval = 0, seenRpmAt = 0, seenCoolAt = 0, seenLtftAt = 0, seenSpdAt = 0;
      uint32_t now = millis();
      if (st == S_RUN && rpmFresh()) {
        float r = val[V_RPM];
        if (r < 100) { if (running) { running = false; } engWasOff = true; }
        else if (r > 500 && !running) {
          running = true; startAt = now; startKm = km; warmDone = false; thermWarn = false; warmMinNow = 0;
          startC = has[V_COOLANT] ? val[V_COOLANT] : -99;
          idleN = 0; idleBad = 0; hotSince = 0; hotEma = 0;
          if (engWasOff) recordStart();
          engWasOff = false;
        }
      } else if (st != S_RUN) { running = false; }
      if (!running) {
        if (dirty && now - lastEval > 5000) save();
        lastEval = now;
        return;
      }
      float runMin = (now - startAt) / 60000.0f;
      bool coolOk = has[V_COOLANT] && now - lastAt[V_COOLANT] < 6000;
      float c = coolOk ? val[V_COOLANT] : -99;
      // Termostat / isinma
      if (startC < -90 && coolOk) startC = c;
      if (coolOk && startC > -90 && startC < 50 && !warmDone && c >= 80) {
        warmDone = true; warmMinNow = (uint8_t)constrain((int)lroundf(runMin), 1, 99);
        memmove(warm + 1, warm, 4); warm[0] = warmMinNow; nWarm = min<uint8_t>(nWarm + 1, 5); dirty = true;
      }
      bool ambOkC = !(has[V_AMBIENT] && val[V_AMBIENT] < -5);
      if (coolOk && startC > -90 && startC < 50 && !warmDone && runMin > 15 && km - startKm > 5 && c < 75 && ambOkC) thermWarn = true;
      hs[K_THERM] = thermWarn ? H_WARN : (nWarm ? (warm[0] > 20 ? H_WARN : H_OK) : H_NONE);
      // Normal sicaklik ogren + hararet egilimi
      if (coolOk && lastAt[V_COOLANT] != seenCoolAt) {
        seenCoolAt = lastAt[V_COOLANT];
        bool moving = has[V_SPEED] && val[V_SPEED] > 30;
        if (c >= 80 && runMin > 10 && moving) {
          nNorm = min<uint16_t>(nNorm + 1, 60000);
          normC += (c - normC) / min<uint16_t>(nNorm, 500);
          if (nNorm % 150 == 0) dirty = true;
        }
        hotEma = hotEma ? hotEma * 0.9f + c * 0.1f : c;
        uint8_t thr = HOT_C[oc.hot] ? HOT_C[oc.hot] : 110;
        bool hot = nNorm >= 300 && c >= 80 && hotEma > normC + 8 && hotEma < thr;
        if (hot) { if (!hotSince) hotSince = now; } else hotSince = 0;
      }
      hs[K_HOT] = nNorm < 300 ? H_NONE : (hotSince && now - hotSince > 60000) ? H_WARN : H_OK;
      // Uzun donem yakit duzeltme
      if (sup[V_LTFT] && has[V_LTFT] && lastAt[V_LTFT] != seenLtftAt) {
        seenLtftAt = lastAt[V_LTFT];
        if (c >= 80 && val[V_RPM] > 600) { ltft = nLtft ? ltft * 0.8f + val[V_LTFT] * 0.2f : val[V_LTFT]; if (nLtft < 255) nLtft++; ltftAt = now; }
      }
      hs[K_TRIM] = (!sup[V_LTFT] || nLtft < 6) ? H_NONE : fabsf(ltft) > 18 ? H_BAD : fabsf(ltft) > 10 ? H_WARN : H_OK;
      // Rolanti dalgalanmasi: duruyor, sicak motor
      bool stopped = has[V_SPEED] && now - lastAt[V_SPEED] < 3000 && val[V_SPEED] < 1;
      if (lastAt[V_RPM] != seenRpmAt) {
        seenRpmAt = lastAt[V_RPM];
        float r = val[V_RPM];
        if (stopped && c >= 75 && r > 400 && r < 1300) {
          idleBuf[idleN++] = (uint16_t)r;
          if (idleN >= 30) {
            float m = 0, s = 0; for (int i = 0; i < 30; i++) m += idleBuf[i]; m /= 30;
            for (int i = 0; i < 30; i++) s += (idleBuf[i] - m) * (idleBuf[i] - m);
            idleSd = sqrtf(s / 30); idleAt = now; idleN = 0;
            idleBad = idleSd > 60 ? min<uint8_t>(idleBad + 1, 9) : (idleBad ? idleBad - 1 : 0);
          }
        } else idleN = 0;
      }
      (void)seenSpdAt;
      hs[K_IDLE] = !idleAt ? H_NONE : idleBad >= 3 ? H_WARN : H_OK;
      // Bakim
      if (SVC_KM[svcIdx]) { float left = SVC_KM[svcIdx] - (km - svcKm); hs[K_SVC] = left <= 0 ? H_BAD : left <= 500 ? H_WARN : H_OK; }
      else hs[K_SVC] = H_NONE;
      evalBatt();
      if (dirty && now - lastEval > 300000) { save(); lastEval = now; }   // 5 dk'da bir
    }

    // Bilgi karti icin: gosterilmemis ilk uyari
    bool next(uint8_t& k, char* val8, char* l1, char* l2) {
      for (int i = 0; i < K_N; i++) {
        if (shown[i] || hs[i] < H_WARN) continue;
        shown[i] = true; k = i; val8[0] = 0;
        switch (i) {
          case K_BATT:
            snprintf(val8, 8, "%.1f", crankLast());
            if (hs[i] == H_BAD) { strcpy(l1, portrait ? "Marşta voltaj" : "Marş anında voltaj çok düştü"); strcpy(l2, portrait ? "Aküyü test ettirin" : "Aküyü test ettirin"); }
            else if (lastRest > 0 && lastRest < 12.2f) { snprintf(val8, 8, "%.1f", lastRest); strcpy(l1, portrait ? "Dinlenme voltajı" : "Kontak açıkken akü voltajı"); strcpy(l2, portrait ? "Akü şarjı düşük" : "Akü şarjı düşük, şarj ettirin"); }
            else { strcpy(l1, portrait ? "Marş voltajı düşüyor" : "Marş voltajı son çalıştırmalarda düştü"); strcpy(l2, portrait ? "Aküyü test ettirin" : "Kışa girmeden aküyü test ettirin"); }
            break;
          case K_THERM:
            if (thermWarn) { snprintf(val8, 8, "%d", has[V_COOLANT] ? (int)val[V_COOLANT] : 0); strcpy(l1, portrait ? "15 dk'da ısınmadı" : "Motor 15 dakikada ısınmadı"); }
            else { snprintf(val8, 8, "%d", warm[0]); strcpy(l1, portrait ? "dk ısınma süresi" : "dakikada ısınabildi (uzun)"); }
            strcpy(l2, portrait ? "Termostat?" : "Termostat açık kalıyor olabilir"); break;
          case K_HOT:
            snprintf(val8, 8, "%d", (int)lroundf(hotEma));
            snprintf(l1, 40, portrait ? "Normali %d°C" : "Bu motorun normali %d°C", (int)lroundf(normC));
            strcpy(l2, portrait ? "Soğutmayı kontrol" : "Soğutma suyu / fan kontrol ettirin"); break;
          case K_TRIM:
            snprintf(val8, 8, "%+d", (int)lroundf(ltft));
            strcpy(l1, portrait ? "Uzun yakıt ayarı %" : "Uzun dönem yakıt ayarı (%)");
            strcpy(l2, ltft > 0 ? (portrait ? "Hava kaçağı?" : "Hava kaçağı / MAF / yakıt basıncı?") : (portrait ? "Zengin karışım?" : "Zengin karışım: enjektör / sensör?")); break;
          case K_IDLE:
            snprintf(val8, 8, "%d", (int)lroundf(idleSd));
            strcpy(l1, portrait ? "Rölanti ± devir" : "Rölantide devir dalgalanıyor (± rpm)");
            strcpy(l2, portrait ? "Buji/bobin kontrol" : "Buji, bobin, gaz kelebeği kontrol"); break;
          default: {
            float left = SVC_KM[svcIdx] - (km - svcKm);
            snprintf(val8, 8, "%d", (int)fabsf(left));
            strcpy(l1, left <= 0 ? (portrait ? "km geçti" : "km bakım zamanı geçti") : (portrait ? "km kaldı" : "km sonra bakım zamanı"));
            strcpy(l2, portrait ? "Bakım yaklaştı" : "Yağ ve filtre bakımı planlayın"); break;
          }
        }
        return true;
      }
      return false;
    }
  }

  // ---- Arac sagligi sayfasi -----------------------------------------------------------
  void pageHealth() {
    using namespace Health;
    TFT_eSPI& g = G();
    topBar("Araç sağlığı");
    static const char* NAME[] = {"Akü", "Isınma", "Motor ısısı", "Yakıt ayarı", "Rölanti", "Bakım"};
    int top = 20, rowH = portrait ? 35 : 18;
    for (int i = 0; i < K_N; i++) {
      char v[24] = "";
      switch (i) {
        case K_BATT:  if (nCrank) snprintf(v, sizeof(v), "marş %.1f V", crankLast()); else strcpy(v, "marş bekleniyor"); break;
        case K_THERM: if (thermWarn) strcpy(v, "ısınmadı!"); else if (nWarm) snprintf(v, sizeof(v), "%d dk", warm[0]); else strcpy(v, "soğuk çalıştırma"); break;
        case K_HOT:   if (nNorm >= 300) snprintf(v, sizeof(v), "normal %d°C", (int)lroundf(normC)); else snprintf(v, sizeof(v), "öğreniyor %d%%", nNorm / 3); break;
        case K_TRIM:  if (!sup[V_LTFT] && st == S_RUN) strcpy(v, "araç vermiyor"); else if (nLtft >= 6) snprintf(v, sizeof(v), "%+.0f %%", ltft); else strcpy(v, "sıcak motor"); break;
        case K_IDLE:  if (idleAt) snprintf(v, sizeof(v), "± %d rpm", (int)lroundf(idleSd)); else strcpy(v, "duruşta ölçülür"); break;
        default:      if (SVC_KM[svcIdx]) snprintf(v, sizeof(v), "%d km kaldı", (int)(SVC_KM[svcIdx] - (km - svcKm))); else strcpy(v, "ayarlanmadı"); break;
      }
      uint8_t s = Health::hs[i];
      uint16_t c = s == H_BAD ? C_BAD : s == H_WARN ? C_WARN : s == H_OK ? C_OK : C_TRACK;
      int y = top + i * rowH;
      if (portrait) {
        g.fillCircle(9, y + 9, 4, c);
        drawText(NAME[i], 18, y + 9, ML_DATUM, 2, C_TEXT, C_BG, SW - 22);
        drawText(v, 18, y + 25, ML_DATUM, 1, s >= H_WARN ? c : C_SUBTEXT, C_BG, SW - 22);
      } else {
        g.fillCircle(9, y + 8, 4, c);
        drawText(NAME[i], 18, y + 8, ML_DATUM, 2, C_TEXT, C_BG, 100);
        drawText(v, SW - 6, y + 8, MR_DATUM, 2, s >= H_WARN ? c : C_TEXT2, C_BG, SW - 124);
      }
    }
  }

  enum MI : uint8_t { M_ENABLE, M_ADAPT, M_STYLE, M_GAUGES, M_FUEL, M_DISP, M_TANK, M_HOT, M_SHIFT, M_SPD, M_SLEEP, M_PRICE,
                      M_SVC, M_SVCDONE, M_TRIPRST, M_CLEAR, M_BACK, M_N };
  uint32_t svcArm = 0;
  uint32_t clearArmMenu = 0;
  bool gmenu = false; int gsel = 0;                  // gosterge secim alt menusu
  uint8_t nextSup(uint8_t v) {
    for (int k = 1; k <= V_COUNT; k++) { uint8_t c = (v + k) % V_COUNT; if (st != S_RUN || sup[c]) return c; }
    return v;
  }
  void menuLabel(int i, char* l, char* r) {
    switch (i) {
      case M_ENABLE: strcpy(l, "Sürüş asistanı"); strcpy(r, oc.enabled ? "Açık" : "Kapalı"); break;
      case M_STYLE:  strcpy(l, "Görünüm"); strcpy(r, OSTYLE_TXT[oc.style]); break;
      case M_GAUGES: { strcpy(l, "Göstergeler"); int n = 0; for (int k = 0; k < V_COUNT; k++) n += (oc.mask >> k) & 1; sprintf(r, "%d seçili", n); break; }
      case M_FUEL:  strcpy(l, "Yakıt tipi"); strcpy(r, FUEL_TXT[oc.fuel]); break;
      case M_DISP:  strcpy(l, "Motor hacmi"); sprintf(r, "%.1f L", DISP10[oc.disp] / 10.0f); break;
      case M_TANK:  strcpy(l, "Depo hacmi"); sprintf(r, "%d L", TANK_L[oc.tank]); break;
      case M_HOT:   strcpy(l, "Hararet alarmı"); if (HOT_C[oc.hot]) sprintf(r, "%d°C", HOT_C[oc.hot]); else strcpy(r, "Kapalı"); break;
      case M_SHIFT: strcpy(l, "Vites uyarısı"); if (SHIFT[oc.shift]) sprintf(r, "%d rpm", SHIFT[oc.shift]); else strcpy(r, "Kapalı"); break;
      case M_SPD:   strcpy(l, "Hız uyarısı"); if (SPD[oc.spd]) sprintf(r, "%d km/sa", SPD[oc.spd]); else strcpy(r, "Kapalı"); break;
      case M_SLEEP: strcpy(l, "Otomatik uyku"); if (SLEEP_OBD[oc.sleep]) sprintf(r, "%d dk", SLEEP_OBD[oc.sleep]); else strcpy(r, "Kapalı"); break;
      case M_PRICE: strcpy(l, "Yakıt fiyatı"); sprintf(r, "%d TL/L", oc.price); break;
      case M_ADAPT: strcpy(l, "Adaptör seç"); strcpy(r, oc.haveMac ? oc.name : "seçilmedi"); break;
      case M_SVC:   strcpy(l, "Bakım aralığı"); if (Health::SVC_KM[Health::svcIdx]) sprintf(r, "%u km", Health::SVC_KM[Health::svcIdx]); else strcpy(r, "Kapalı"); break;
      case M_SVCDONE: strcpy(l, "Bakım yapıldı"); if (svcArm && millis() - svcArm < 4000) strcpy(r, "emin misiniz?"); else snprintf(r, 28, "%d km önce", (int)(Health::km - Health::svcKm)); break;
      case M_TRIPRST: strcpy(l, "Yolculuğu sıfırla"); snprintf(r, 28, "%.1f km", tripKm); break;
      case M_CLEAR: strcpy(l, "Arıza kodlarını sil"); strcpy(r, clearArmMenu && millis() - clearArmMenu < 4000 ? "emin misiniz?" : (st == S_RUN ? "" : "bağlı değil")); break;
      case M_BACK:  strcpy(l, "< Geri"); r[0] = 0; break;
    }
  }
  void pageGaugeMenu() {
    TFT_eSPI& g = G();
    drawText("Göstergeler", 8, 9, ML_DATUM, 2, C_ACCENT, C_BG);
    int top = 22, rowH = portrait ? 22 : 18, vis = (SH - top - 12) / rowH, n = V_COUNT + 1;
    int first = max(0, gsel - vis + 1);
    for (int r = 0; r < vis && first + r < n; r++) {
      int i = first + r, y = top + r * rowH;
      bool s = i == gsel; uint16_t bg = s ? C_PANEL : C_BG;
      if (s) g.fillRoundRect(3, y, SW - 6, rowH - 2, 5, C_PANEL);
      if (i == V_COUNT) { drawText("< Geri", 10, y + rowH / 2 - 1, ML_DATUM, 2, C_SUBTEXT, bg); continue; }
      uint8_t v = ORDER[i];
      bool on = (oc.mask >> v) & 1, ok = st != S_RUN || sup[v];
      g.drawRoundRect(9, y + rowH / 2 - 6, 12, 12, 3, on ? C_OK : C_SUBTEXT);
      if (on) g.fillRoundRect(12, y + rowH / 2 - 3, 6, 6, 1, C_OK);
      drawText(DEFS[v].name, 28, y + rowH / 2 - 1, ML_DATUM, 2, ok ? C_TEXT : C_SUBTEXT, bg, SW - 70);
      if (!ok) drawText("yok", SW - 8, y + rowH / 2 - 1, MR_DATUM, 1, C_SUBTEXT, bg);
    }
    drawText("B: sonraki  A: seç/kaldır  GÜÇ: geri", SW / 2, SH - 5, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 4);
  }
  void pageMenu() {
    TFT_eSPI& g = G();
    drawText("OBD2 sürüş asistanı", 8, 9, ML_DATUM, 2, C_ACCENT, C_BG, SW - 30);
    g.fillCircle(SW - 10, 9, 4, st == S_RUN ? C_OK : st == S_CONNECT || st == S_INIT ? C_ACCENT : oc.enabled ? C_WARN : C_TRACK);
    int top = 22, rowH = portrait ? 26 : 19, vis = (SH - top - 12) / rowH;
    int first = max(0, msel - vis + 1);
    for (int r = 0; r < vis && first + r < M_N; r++) {
      int i = first + r, y = top + r * rowH;
      bool s = i == msel;
      uint16_t bg = s ? C_PANEL : C_BG;
      if (s) g.fillRoundRect(3, y, SW - 6, rowH - 2, 5, C_PANEL);
      char l[28], v[28]; menuLabel(i, l, v);
      if (portrait) {
        drawText(l, 9, y + 7, ML_DATUM, 1, i == M_CLEAR ? C_WARN : C_TEXT, bg, SW - 14);
        drawText(v, 9, y + 18, ML_DATUM, 1, C_ACCENT, bg, SW - 14);
      } else {
        drawText(l, 9, y + rowH / 2 - 1, ML_DATUM, 2, i == M_CLEAR ? C_WARN : C_TEXT, bg, 130);
        drawText(v, SW - 9, y + rowH / 2 - 1, MR_DATUM, 2, C_ACCENT, bg, SW - 140);
      }
    }
    drawText("B: sonraki  A: değiştir  GÜÇ: geri", SW / 2, SH - 5, MC_DATUM, 1, C_SUBTEXT, C_BG, SW - 4);
  }
  void nextStyle() {                                               // Sportif > Konfor > K1 > K2 > K3
    oc.style = (oc.style + 1) % COUNT_OF(OSTYLE_TXT); shownFor = 255;
    if (oc.style >= 2) page = 0;                                   // kokpit sayfasini goster
    buildPages();
  }
  void setEnabled(bool on);
  void menuSelect() {
    switch (msel) {
      case M_STYLE:  nextStyle(); break;
      case M_GAUGES: gmenu = true; gsel = 0; return;
      case M_FUEL:  oc.fuel = (oc.fuel + 1) % 3; if (st == S_RUN) computeSupport(); break;
      case M_DISP:  oc.disp = (oc.disp + 1) % COUNT_OF(DISP10); break;
      case M_TANK:  oc.tank = (oc.tank + 1) % COUNT_OF(TANK_L); break;
      case M_HOT:   oc.hot = (oc.hot + 1) % COUNT_OF(HOT_C); break;
      case M_SHIFT: oc.shift = (oc.shift + 1) % COUNT_OF(SHIFT); break;
      case M_SPD:   oc.spd = (oc.spd + 1) % COUNT_OF(SPD); break;
      case M_SLEEP: oc.sleep = (oc.sleep + 1) % COUNT_OF(SLEEP_OBD); break;
      case M_PRICE: oc.price = oc.price >= 150 ? 20 : oc.price + 1; break;          // A uzun: +10
      case M_ENABLE: setEnabled(!oc.enabled); return;
      case M_ADAPT:
        if (!oc.enabled) setEnabled(true);
        Link::close(); smenu = true; startScan(); return;
      case M_SVC: Health::svcIdx = (Health::svcIdx + 1) % COUNT_OF(Health::SVC_KM); Health::save(); return;
      case M_SVCDONE:
        if (svcArm && millis() - svcArm < 4000) { svcArm = 0; Health::svcKm = Health::km; Health::shown[Health::K_SVC] = false; Health::save(); showToast("Bakım kaydedildi", C_OK, 1300); }
        else { svcArm = millis(); showToast("Onay için tekrar seçin", C_WARN, 1500); }
        return;
      case M_TRIPRST: { bool k = closeTrip(); showToast(k ? "Kaydedildi ve sıfırlandı" : "Yolculuk sıfırlandı", C_OK, 1300); return; }
      case M_CLEAR:
        if (st != S_RUN) { showToast("Araç bağlı değil", C_WARN, 1000); return; }
        if (clearArmMenu && millis() - clearArmMenu < 4000) { clearArmMenu = 0; clearBusy = true; showToast("Siliniyor...", C_WARN, 1500); }
        else { clearArmMenu = millis(); showToast("Silmek için A'ya tekrar basın", C_WARN, 1800); }
        return;
      case M_BACK: menu = false; saveCfg(); if (::menu.open) buildMenu(); return;
    }
    saveCfg();
  }

  // ---- Motor durdu: yolculuk kaydi + (istege bagli) otomatik uyku -------------------
  uint32_t offSince = 0, sleepAt = 0; bool hadRun = false;
  bool engineOff() {
    if (!hadRun) return false;                                    // bu acilista hic arac verisi gelmedi
    if (st == S_RUN) return has[V_RPM] && val[V_RPM] < 100 && millis() - lastAt[V_RPM] < 5000;
    return st != S_SCAN;                                          // baglaniyor / yanit yok / hata
  }
  void goSleep() {
    closeTrip(); saveCfg();
    Link::close();
    goDeepSleep();
  }
  void powerService() {
    uint32_t now = millis();
    if (st == S_RUN) hadRun = true;
    if (st == S_ERROR && oc.haveMac && !smenu && now - stAt > 30000 && !Pair::active && !directMode) connectSaved();   // adaptoru yeniden dene
    if (!engineOff()) { offSince = 0; sleepAt = 0; return; }
    if (!offSince) offSince = now;
    if (now - offSince > 60000 && tripKm >= 0.3f) closeTrip();    // motor durdu: yolculugu kaydet
    uint32_t T = SLEEP_OBD[oc.sleep] * 60000UL;
    if (!T || now - offSince < T || now - app.lastInput < T || menu) { sleepAt = 0; return; }
    if (!sleepAt) { sleepAt = now + 10000; Snd::limit(); }
    if ((int32_t)(now - sleepAt) >= 0) goSleep();
  }

  // ---- Surus uyarilari: her ekranin (gozler, maskot, menu...) ustunde -------------
  enum Al : uint8_t { AL_NONE, AL_MIL, AL_HOT, AL_VOLT, AL_SPEED, AL_SHIFT };
  uint8_t  alShow = AL_NONE;
  uint32_t alAt = 0, alAck[6] = {0}, voltBadSince = 0, lastMilPoll = 0, dtcSig = 0, dtcAckSig = 0;
  // Ariza kodunun kisa aciklamasi (SAE J2012 gruplari)
  const char* dtcText(const char* c) {
    switch (c[0]) {
      case 'B': return "Gövde / kaporta";
      case 'C': return "Şasi: fren, direksiyon";
      case 'U': return "Ağ / modül iletişimi";
      default: break;
    }
    if (c[1] == '1' || c[1] == '3') return "Üreticiye özel kod";
    switch (c[2]) {
      case '0': return "Yakıt-hava / emisyon";
      case '1': case '2': return "Yakıt-hava ölçümü";
      case '3': return "Ateşleme / tekleme";
      case '4': return "Emisyon kontrolü";
      case '5': return "Hız / rölanti kontrolü";
      case '6': return "Beyin / çıkış devresi";
      case '7': case '8': case '9': return "Şanzıman";
      case 'A': return "Hibrit sistem";
      default:  return "Motor / aktarma";
    }
  }
  bool alertBlocking() { return alShow == AL_MIL || alShow == AL_HOT || alShow == AL_VOLT; }
  bool alertActive() { return oc.enabled && alShow != AL_NONE; }
  void ackAlert() {
    if (alShow == AL_MIL) dtcAckSig = dtcSig;
    alAck[alShow] = millis();
    alShow = AL_NONE;
  }
  void alertService() {
    uint32_t now = millis();
    if (st == S_RUN && now - lastMilPoll > 60000 && !dtcBusy && !pendingDtc && !clearBusy) { lastMilPoll = now; dtcBusy = true; }   // dakikada bir MIL + kodlar
    uint32_t sig = (mil ? 1 : 0) + milCount * 7u;
    for (int i = 0; i < dtcN; i++) for (const char* p = dtc[i]; *p; p++) sig = sig * 31u + (uint8_t)*p;
    dtcSig = sig;
    bool running = st == S_RUN && has[V_RPM] && val[V_RPM] > 500;
    bool cMil  = st == S_RUN && dtcRead && (mil || dtcN > 0) && dtcSig != dtcAckSig;
    bool cHot  = st == S_RUN && oc.hot && has[V_COOLANT] && val[V_COOLANT] >= HOT_C[oc.hot];
    bool vb    = running && has[V_BATT] && (val[V_BATT] < 12.8f || val[V_BATT] > 15.0f);
    if (!vb) voltBadSince = 0; else if (!voltBadSince) voltBadSince = now;
    bool cVolt = vb && now - voltBadSince > 20000;                // 20 sn surerse
    if (alertBlocking()) {
      bool still = alShow == AL_MIL ? cMil : alShow == AL_HOT ? cHot : cVolt;
      if (!still) alShow = AL_NONE;
      else if (now - alAt > 15000) ackAlert();                    // 15 sn sonra kapanir (3 dk sonra yine)
      else return;
    }
    uint8_t n = AL_NONE;
    if (cMil) n = AL_MIL;
    else if (cHot && now - alAck[AL_HOT] > 180000) n = AL_HOT;
    else if (cVolt && now - alAck[AL_VOLT] > 180000) n = AL_VOLT;
    else if (alarm == 2) n = AL_SPEED;
    else if (alarm == 3) n = AL_SHIFT;
    if (n != alShow) {
      alShow = n; alAt = now;
      if (alertBlocking()) { Snd::error(); app.lastInput = now; }  // ekran kararmissa ac
    }
  }
  void drawAlert() {
    TFT_eSPI& g = G();
    uint32_t now = millis();
    if (!alertBlocking()) {                                       // hiz / vites: ustte yanip sonen serit
      bool on = (now / 300) % 2;
      uint16_t c = alShow == AL_SHIFT ? C_BAD : C_WARN;
      g.fillRoundRect(2, 1, SW - 4, 20, 8, on ? c : C_PANEL);
      char b[28];
      if (alShow == AL_SPEED) snprintf(b, sizeof(b), "Hız %d km/sa", (int)val[V_SPEED]);
      else snprintf(b, sizeof(b), portrait ? "Vites yükselt" : "Vites yükselt  %d", (int)val[V_RPM]);
      drawText(b, SW / 2, 11, MC_DATUM, 2, on ? C_BG : c, on ? c : C_PANEL, SW - 10);
      return;
    }
    uint16_t c = alShow == AL_MIL ? C_WARN : C_BAD;
    int w = SW - 10, h = portrait ? 160 : 118, x = 5, y = SH / 2 - h / 2;
    g.fillRoundRect(x, y, w, h, 10, C_BG);
    g.drawRoundRect(x, y, w, h, 10, c); g.drawRoundRect(x + 1, y + 1, w - 2, h - 2, 9, c);
    g.fillRoundRect(x + 2, y + 2, w - 4, 22, 8, c);
    const char* title = alShow == AL_MIL ? "Arıza lambası" : alShow == AL_HOT ? "Hararet!" : "Şarj sistemi";
    drawText(title, SW / 2, y + 13, MC_DATUM, 2, C_BG, c, w - 8);
    char b[40];
    if (alShow == AL_MIL) {
      if (!dtcN) { snprintf(b, sizeof(b), "Kod okunamadı (%d)", (int)milCount); drawText(b, SW / 2, y + h / 2, MC_DATUM, 2, C_TEXT, C_BG, w - 8); }
      int rows = min((int)dtcN, portrait ? 3 : 2), step = portrait ? 36 : 30, y0 = y + 32;
      for (int i = 0; i < rows; i++) {
        int yy = y0 + i * step;
        drawText(dtc[i], x + 10, yy + 8, ML_DATUM, 2, C_WARN, C_BG);
        drawText(dtcText(dtc[i]), portrait ? x + 10 : x + 72, portrait ? yy + 24 : yy + 8, ML_DATUM, portrait ? 1 : 2, C_TEXT2, C_BG, portrait ? w - 16 : w - 80);
      }
      if (dtcN > rows) { snprintf(b, sizeof(b), "+%d kod daha", dtcN - rows); drawText(b, SW / 2, y + h - 24, MC_DATUM, 1, C_SUBTEXT, C_BG); }
    } else {
      if (alShow == AL_HOT) snprintf(b, sizeof(b), "%d", (int)val[V_COOLANT]);
      else snprintf(b, sizeof(b), "%.1f", val[V_BATT]);
      vbg = C_BG;
      int cyv = y + h / 2 + (portrait ? -2 : 2);
      float r = vNumber(b, SW / 2 - 10, cyv, portrait ? 40 : 34, 0.14f, w - 50, c);
      drawText(alShow == AL_HOT ? "°C" : "V", (int)r + 4, cyv + (portrait ? 20 : 17), BL_DATUM, 2, c, C_BG);
      const char* sub = alShow == AL_HOT ? (portrait ? "Yavaşlayın" : "Motor suyu sıcak, yavaşlayın")
                      : val[V_BATT] < 12.8f ? (portrait ? "Düşük voltaj" : "Düşük voltaj: alternatör?")
                                            : (portrait ? "Yüksek voltaj" : "Yüksek voltaj: regülatör?");
      drawText(sub, SW / 2, y + h - 26, MC_DATUM, portrait ? 2 : 1, C_TEXT2, C_BG, w - 8);
    }
    drawText(portrait ? "Tuşa bas: tamam" : "Bir tuşa basın: tamam", SW / 2, y + h - 10, MC_DATUM, 1, C_SUBTEXT, C_BG, w - 8);
  }

  // ---- Koruyucu: OBD2 arayuzleri (M5 tusu ile degisir) ----------------------------
  const char* pageName(uint8_t p) {
    switch (p) {
      case P_COCKPIT: return OSTYLE_TXT[oc.style];
      case P_GRAPH: return "Grafik";
      case P_ACCEL: return "Hızlanma";
      case P_TRIP:  return "Yolculuk";
      case P_HIST:  return "Geçmiş";
      case P_WX:    return "Hava";
      case P_HEALTH: return "Araç sağlığı";
      case P_DTC:   return "Arıza kodları";
      case P_CONN:  return "Bağlantı";
      default:      return p < V_COUNT ? DEFS[p].name : "";
    }
  }
  const char* curPageName() { buildPages(); return pageName(plist[page]); }   // enkoder kutusu OLED'i
  void drawSaver() {
    TFT_eSPI& g = G();
    g.fillRect(0, 0, SW, SH, C_BG);
    uint8_t cur = curParam();
    if (cur == P_CONN) pageStatus();
    else if (cur == P_COCKPIT) pageCockpit();
    else if (cur == P_TRIP) pageTrip();
    else if (cur == P_HIST) pageHist();
    else if (cur == P_ACCEL) pageAccel();
    else if (cur == P_GRAPH) pageGraph();
    else if (cur == P_DTC) pageDtc();
    else if (cur == P_HEALTH) pageHealth();
    else if (cur == P_WX) { pageWeather(); buildPages(); for (int i = 0; i < pn; i++) g.fillCircle(SW / 2 + (i - pn / 2) * 6, SH - 3, i == page ? 2 : 1, i == page ? C_TEXT : C_TRACK); }
    else pageGauge(cur);
    if (cur < V_COUNT) offBanner();
    if (sleepAt) {                                                 // otomatik uyku geri sayimi
      int sec = max(0, (int)((int32_t)(sleepAt - millis()) + 999) / 1000);
      int w = min(SW - 10, 200), h = 58, x = SW / 2 - w / 2, y = SH / 2 - h / 2;
      g.fillRoundRect(x, y, w, h, 8, C_PANEL); g.drawRoundRect(x, y, w, h, 8, C_WARN);
      drawText("Motor kapalı", SW / 2, y + 12, MC_DATUM, 2, C_WARN, C_PANEL);
      char b[24]; snprintf(b, sizeof(b), "%d sn sonra uyku", sec);
      drawText(b, SW / 2, y + 31, MC_DATUM, 2, C_TEXT, C_PANEL, w - 8);
      drawText("Tuşa basın: iptal", SW / 2, y + 48, MC_DATUM, 1, C_SUBTEXT, C_PANEL, w - 8);
    }
    drawToast();
  }
  // M5 (A) tusu: kisa = sonraki arayuz, uzun = sayfaya ozel islem
  // Adaptor bagli (baslatiliyor / veri / motor kapali): ana ekran OBD2 arayuzleri
  bool dashActive() { return on(); }                               // OBD2 aciksa ana ekran hep OBD2
  bool linked() { return on() && (st == S_INIT || st == S_RUN || st == S_NOECU); }
  uint32_t driveSec() { return linked() ? tripMs / 1000 : 0; }   // bu yolculukta motor calisma suresi
  void prevPage() {
    buildPages();
    page = (page + pn - 1) % pn; buildPages();
    showToast(pageName(plist[page]), C_ACCENT, 700);
    Snd::click();
  }
  void saverKey(bool lng) {
    buildPages();
    uint8_t cur = plist[page];
    if (!lng) {
      page = (page + 1) % pn; buildPages();
      showToast(pageName(plist[page]), C_ACCENT, 700);
      Snd::click();
      return;
    }
    Snd::sel();
    if (cur < V_COUNT || cur == P_COCKPIT) { nextStyle(); buildPages(); showToast(OSTYLE_TXT[oc.style], C_ACCENT, 900); saveCfg(); }
    else if (cur == P_GRAPH) { oc.focus = nextGraph(oc.focus); saveCfg(); showToast(DEFS[oc.focus].name, C_ACCENT, 900); }
    else if (cur == P_ACCEL) { accelReset(); showToast("Yeni ölçüm", C_ACCENT, 900); }
    else if (cur == P_HIST)  { histOff += portrait ? 5 : 4; }
    else if (cur == P_DTC)   { if (st == S_RUN) { dtcBusy = true; showToast("Okunuyor...", C_ACCENT, 1200); } else showToast("Araç bağlı değil", C_WARN, 1000); }
    else if (cur == P_CONN)  { if (oc.haveMac) { Link::close(); connectSaved(); } else showToast("Ayarlar > OBD2 > Adaptör seç", C_WARN, 1600); }
    else if (cur == P_TRIP)  { showToast("Sıfırlama: Ayarlar > OBD2", C_SUBTEXT, 1400); }
    else if (cur == P_HEALTH) { showToast("Ayarlar > OBD2 > Bakım", C_SUBTEXT, 1400); }
    else if (cur == P_WX)    { showToast(Net::hasWeather() ? "iPhone Kestirmesi günceller" : "Hava: iPhone Kestirmesi", C_SUBTEXT, 1400); }
  }

  // ---- OBD2 ayarlari (normal menuden acilir) ---------------------------------------
  void drawSettings() {
    if (smenu) pageScan();
    else if (gmenu) pageGaugeMenu();
    else pageMenu();
  }
  void openSettings() { menu = true; msel = 0; gmenu = smenu = false; }
  void leaveScan() {
    smenu = false; stopScan();
    if (st == S_SCAN) { if (oc.haveMac) connectSaved(); else setSt(S_ERROR, "Adaptör seçilmedi"); }
  }
  // Ayar penceresi ortak islemleri (tuslar + enkoder)
  void sSelect() {
    if (smenu) { if (nDev) { connectTo(devs[dsel].mac, devs[dsel].name, devs[dsel].atype); smenu = false; } }
    else if (gmenu) {
      if (gsel == V_COUNT) { gmenu = false; saveCfg(); }
      else {
        uint32_t bit = 1UL << ORDER[gsel];
        if ((oc.mask & bit) && (oc.mask & ~bit) == 0) showToast("En az bir gösterge", C_WARN, 1000);
        else oc.mask ^= bit;
        buildPages();
      }
    }
    else menuSelect();
  }
  void sBack() {
    if (smenu) leaveScan();
    else if (gmenu) { gmenu = false; saveCfg(); }
    else { menu = false; saveCfg(); if (::menu.open) buildMenu(); }
  }
  void sStep(int dir) {
    if (smenu) { if (nDev) dsel = (dsel + (dir > 0 ? 1 : nDev - 1)) % nDev; }
    else if (gmenu) gsel = (gsel + (dir > 0 ? 1 : V_COUNT)) % (V_COUNT + 1);
    else msel = (msel + (dir > 0 ? 1 : M_N - 1)) % M_N;
  }
  // Enkoder: 1 sonraki, -1 onceki, 2 sec, 3 geri
  void encKey(int8_t a) {
    userActivity();
    if (a == 3) { Snd::back(); sBack(); return; }
    Snd::click();
    if (a == 2) sSelect(); else sStep(a);
  }
  void settingsButtons() {
    static bool pu = false, pd = false, longU = false, longD = false;
    uint32_t now = millis();
    bool u = btnUp.down, d = btnDn.down;
    if (u || d) userActivity();
    if (HW::powerPressed()) {
      userActivity(); Snd::back();
      sBack();
      pu = u; pd = d; longU = longD = true;
      return;
    }
    if (u && !longU && now - btnUp.downAt > 700) {
      longU = true;
      if (smenu) { startScan(); Snd::sel(); }
      else if (!gmenu && msel == M_PRICE) { oc.price = oc.price + 10 > 150 ? 20 : oc.price + 10; saveCfg(); Snd::sel(); }
    }
    if (d && !longD && now - btnDn.downAt > 700) {
      longD = true;
      if (smenu) { if (nDev) dsel = (dsel + nDev - 1) % nDev; }
      else if (gmenu) gsel = (gsel + V_COUNT) % (V_COUNT + 1);
      else msel = (msel + M_N - 1) % M_N;
      Snd::click();
    }
    bool relU = pu && !u, relD = pd && !d;
    pu = u; pd = d;
    if (relU) { if (!longU) {
      Snd::click();
      sSelect();
    } longU = false; }
    if (relD) { if (!longD) {
      Snd::click();
      if (smenu) { if (nDev) dsel = (dsel + 1) % nDev; }
      else if (gmenu) gsel = (gsel + 1) % (V_COUNT + 1);
      else msel = (msel + 1) % M_N;
    } longD = false; }
  }

  // ---- Baslatma ve ana dongu --------------------------------------------------------
  bool started = false;
  void start() {
    if (!started) {
      hist = (float*)calloc(120, sizeof(float));
      devs = (Dev*)calloc(10, sizeof(Dev));
      rx = (char*)calloc(256, 1);
      val = (float*)calloc(V_COUNT, sizeof(float)); vMin = (float*)calloc(V_COUNT, sizeof(float));
      vMax = (float*)calloc(V_COUNT, sizeof(float)); lastAt = (uint32_t*)calloc(V_COUNT, sizeof(uint32_t));
      if (!hist || !devs || !rx || !val || !vMin || !vMax || !lastAt) { showToast("OBD2: RAM yetersiz", C_BAD, 2000); return; }
      started = true;
    }
    bool ok = Link::begin();
    Serial.printf("[OBD] BLE istemci %s, bos RAM %u, en buyuk blok %u\n", ok ? "hazir" : "HATA",
                  (unsigned)ESP.getFreeHeap(), (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
    if (!ok) setSt(S_ERROR, "Bluetooth hatası");
    else if (oc.haveMac) connectSaved();
    else setSt(S_ERROR, "Adaptör seçilmedi");
  }
  void setEnabled(bool on) {
    oc.enabled = on; saveCfg();
    if (on) { start(); showToast("Sürüş asistanı açık", C_OK, 1000); }
    else { stopScan(); Link::close(); setSt(S_ERROR, "Kapalı"); alShow = AL_NONE; showToast("Sürüş asistanı kapalı", C_SUBTEXT, 1000); }
  }
  bool on() { return oc.enabled && started; }
  bool keepAwake() { return on() && st == S_RUN && !engineOff(); }
  void begin() {
    loadCfg();
    Health::load();
    memset(has, 0, sizeof(has)); memset(sup, 1, sizeof(sup)); memset(supBits, 0, sizeof(supBits));
    if (oc.enabled) start();
  }
  void loop() {
    if (!started) return;
    if (menu && millis() - app.lastInput > 90000) { if (smenu) leaveScan(); menu = gmenu = false; saveCfg(); }
    if (!oc.enabled) return;
    if (!directMode) service();                                    // eslesme sirasinda bekle
    tick();
    alarms();
    alertService();
    Health::service();
    powerService();
  }
}

// ============================================================================
//  BILGI UYARILARI (hava): buzlanma, yagmur yaklasiyor, sis, kuvvetli ruzgar, asiri sicak
// ============================================================================
//  Engellemeyen kart: 10 sn sonra kendiliginden kapanir, herhangi bir tus = tamam.
//  Her uyari 3 saatte bir en fazla (ruzgar/sicak 6 saat). Ayarlar > Hava uyarilari.
//  Kaynak: iPhone Kestirmeler tahmini + aracin dis sicaklik sensoru (OBD PID 46).
namespace Info {
  enum K : uint8_t { K_NONE, K_ICE, K_RAIN, K_FOG, K_WIND, K_HEAT, K_HEALTH, K_N };
  uint8_t  show = K_NONE;
  uint8_t  hk = 0; char hv[8] = "", hl1[40] = "", hl2[40] = "";   // arac sagligi karti
  uint32_t at = 0, last[K_N] = {0}, checkAt = 0;
  int      v = 0; uint8_t vh = 0;             // gosterilen deger (+ saat)
  bool     fromCar = false;
  constexpr uint32_t SHOW_MS = 10000;

  bool active() { return show != K_NONE; }
  void ack() { show = K_NONE; }
  bool rearm(uint8_t k, uint32_t now) { uint32_t gap = (k == K_WIND || k == K_HEAT) ? 6UL * 3600000UL : 3UL * 3600000UL; return !last[k] || now - last[k] > gap; }

  void service() {
    uint32_t now = millis();
    if (show != K_NONE) { if (now - at > SHOW_MS) ack(); return; }
    if (now < 20000 || now - checkAt < 2000) return;
    if (at && now - at < 60000) return;                          // uyarilar arasinda en az 1 dk
    checkAt = now;
    if (Obd::alertBlocking() || Pair::modal != Pair::M_NONE || menu.open || app.camera || AutoRot::calStep) return;
    bool amb = Obd::ambOk(); int a = amb ? (int)Obd::val[Obd::V_AMBIENT] : 99;
    bool wx = Net::hasWeather() && cfg.wxAlerts;
    if (!cfg.wxAlerts) amb = false;
    uint8_t k = K_NONE;
    if (Obd::Health::next(hk, hv, hl1, hl2)) k = K_HEALTH;          // arac sagligi once
    else if (((amb && a <= 3) || (wx && Wx::minTemp(3) <= 2)) && rearm(K_ICE, now)) {
      k = K_ICE; fromCar = amb && a <= 3; v = fromCar ? a : Wx::minTemp(3);
    } else if (wx && rearm(K_RAIN, now)) {
      int i = Wx::idxNow(); uint8_t h; uint8_t pk = Wx::rainPeak(3, &h);
      if (i >= 0 && pk >= 60 && rtcWx.hP[i] < 40) { k = K_RAIN; v = pk; vh = h; }
    }
    if (k == K_NONE && wx && (Net::wcode == 45 || Net::wcode == 48) && rearm(K_FOG, now)) k = K_FOG;
    if (k == K_NONE && wx && rtcWx.wind >= 50 && rearm(K_WIND, now)) { k = K_WIND; v = rtcWx.wind; }
    if (k == K_NONE && ((amb && a >= 36) || (wx && Wx::tempNow() >= 36)) && rearm(K_HEAT, now)) { k = K_HEAT; fromCar = amb && a >= 36; v = fromCar ? a : Wx::tempNow(); }
    if (k == K_NONE) return;
    show = k; at = now; last[k] = now;
    userActivity();                                               // ekran kararmissa ac
    Snd::sel();
    if (k == K_HEALTH) Serial.printf("[Uyari] arac sagligi %d: %s %s\n", hk, hv, hl1);
    else Serial.printf("[Uyari] hava uyarisi %d (deger %d)\n", k, v);
  }

  static void icSnow(int cx, int cy, int r, uint16_t c) {
    TFT_eSPI& g = G(); float w = max(2.0f, r / 6.0f);
    for (int i = 0; i < 3; i++) {
      float an = (90 + i * 60) * DEG_TO_RAD, dx = cosf(an) * r, dy = sinf(an) * r;
      g.drawWideLine(cx - dx, cy - dy, cx + dx, cy + dy, w, c, C_BG);
      for (int sg = -1; sg <= 1; sg += 2) {
        float px = cx + sg * dx * 0.6f, py = cy + sg * dy * 0.6f, b1 = an + 0.8f, b2 = an - 0.8f, L = r * 0.32f;
        g.drawWideLine(px, py, px + sg * cosf(b1) * L, py + sg * sinf(b1) * L, w * 0.8f, c, C_BG);
        g.drawWideLine(px, py, px + sg * cosf(b2) * L, py + sg * sinf(b2) * L, w * 0.8f, c, C_BG);
      }
    }
  }
  static void icWind(int cx, int cy, int r, uint16_t c) {
    TFT_eSPI& g = G(); float w = max(2.0f, r / 7.0f);
    for (int i = 0; i < 3; i++) {
      int y = cy - r / 2 + i * r / 2, x0 = cx - r, x1 = cx + r / 3 - (i == 1 ? 0 : r / 4);
      g.drawWideLine(x0 + (i == 1 ? 0 : r / 5), y, x1, y, w, c, C_BG);
      int rr = max(3, r / 5);                                        // ucta yukari kivrim (sag yarim daire)
      g.drawSmoothArc(x1, y - rr, rr + (int)w / 2, max(1, rr - (int)w / 2), 180, 360, c, C_BG, true);
    }
  }
  // Arac sagligi ikonlari
  static void icBatt(int cx, int cy, int s, uint16_t c) {
    TFT_eSPI& g = G(); int w = s * 2, h = s * 14 / 10, x = cx - w / 2, y = cy - h / 2 + 2;
    g.fillRoundRect(x + w / 5 - 3, y - 5, 7, 6, 1, c); g.fillRoundRect(x + w - w / 5 - 4, y - 5, 7, 6, 1, c);
    g.fillRoundRect(x, y, w, h, 5, c); g.fillRoundRect(x + 3, y + 3, w - 6, h - 6, 3, C_BG);
    g.fillRect(x + w / 5 - 4, y + h / 2 - 1, 9, 3, c);
    g.fillRect(x + w - w / 5 - 5, y + h / 2 - 1, 9, 3, c); g.fillRect(x + w - w / 5 - 2, y + h / 2 - 4, 3, 9, c);
  }
  static void icTherm(int cx, int cy, int s, uint16_t c) {
    TFT_eSPI& g = G();
    g.fillRoundRect(cx - s / 4, cy - s, s / 2, s * 3 / 2, s / 4, c);
    g.fillRoundRect(cx - s / 4 + 2, cy - s + 2, s / 2 - 4, s * 3 / 2 - 2, s / 4, C_BG);
    g.fillCircle(cx, cy + s * 2 / 3, s / 2, c);
    g.fillRect(cx - 1, cy - s / 3, 3, s, c);
    for (int i = 0; i < 3; i++) g.fillRect(cx + s / 4 + 2, cy - s + 4 + i * s / 3, s / 4, 2, c);
  }
  static void icWrench(int cx, int cy, int s, uint16_t c) {
    TFT_eSPI& g = G();
    g.drawWideLine(cx - s * 0.6f, cy + s * 0.6f, cx + s * 0.3f, cy - s * 0.3f, max(3.0f, s / 3.5f), c, C_BG);
    g.fillCircle(cx + s * 0.45f, cy - s * 0.45f, s * 0.45f, c);
    g.fillCircle(cx + s * 0.62f, cy - s * 0.62f, s * 0.22f, C_BG);
  }
  static void icEngine(int cx, int cy, int s, uint16_t c) {        // motor blogu + "!"
    TFT_eSPI& g = G();
    g.fillRoundRect(cx - s, cy - s / 2, s * 2, s + 2, 4, c);
    g.fillRect(cx - s / 2, cy - s / 2 - 5, s, 5, c);
    g.fillRect(cx - s - 5, cy - 2, 5, 6, c); g.fillRect(cx + s, cy - s / 4, 4, s / 2, c);
    drawText("!", cx, cy + 1, MC_DATUM, 4, C_BG, c);
  }
  void drawHealth() {
    TFT_eSPI& g = G();
    using namespace Obd::Health;
    uint16_t c = Obd::Health::hs[hk] == H_BAD ? C_BAD : C_WARN;
    static const char* T[] = {"Akü zayıflıyor", "Isınma sorunu", "Motor normalden sıcak", "Yakıt ayarı sapmış", "Rölanti dengesiz", "Bakım zamanı"};
    const char* title = T[hk % 6];
    if (hk == K_BATT && Obd::Health::hs[hk] == H_BAD) title = "Akü zayıf";
    int w = SW - 10, h = portrait ? 196 : 118, x = 5, y = SH / 2 - h / 2;
    g.fillRoundRect(x, y, w, h, 10, C_BG);
    g.drawRoundRect(x, y, w, h, 10, c); g.drawRoundRect(x + 1, y + 1, w - 2, h - 2, 9, c);
    g.fillRoundRect(x + 2, y + 2, w - 4, 22, 8, c);
    drawText(title, SW / 2, y + 13, MC_DATUM, 2, C_BG, c, w - 8);
    int icx = portrait ? SW / 2 : x + 38, icy = portrait ? y + 54 : y + 54;
    if (hk == K_BATT) icBatt(icx, icy, portrait ? 19 : 21, c);
    else if (hk == K_THERM || hk == K_HOT) icTherm(icx, icy, portrait ? 18 : 20, c);
    else if (hk == K_SVC) icWrench(icx, icy, portrait ? 18 : 20, c);
    else icEngine(icx, icy, portrait ? 16 : 18, c);
    Obd::vbg = C_BG;
    float vx = portrait ? SW / 2 - 4 : x + 118, vy = portrait ? y + 106 : y + 52;
    float r = Obd::vNumber(hv, vx, vy, portrait ? 34 : 36, 0.14f, portrait ? 100 : 110, C_TEXT);
    if (hk == K_BATT) drawText("V", (int)r + 4, (int)vy + (portrait ? 17 : 18), BL_DATUM, 2, C_TEXT, C_BG);
    else if (hk == K_THERM || hk == K_HOT) { if (!(hk == K_THERM && !Obd::Health::thermWarn)) { g.drawCircle((int)r + 6, (int)vy - 14, 4, C_TEXT); g.drawCircle((int)r + 6, (int)vy - 14, 3, C_TEXT); } }
    if (portrait) {
      drawText(hl1, SW / 2, y + 138, MC_DATUM, 2, C_TEXT2, C_BG, w - 8);
      drawText(hl2, SW / 2, y + 156, MC_DATUM, 2, c, C_BG, w - 8);
      drawText("Tuşa bas: tamam", SW / 2, y + h - 20, MC_DATUM, 1, C_SUBTEXT, C_BG);
    } else {
      drawText(hl1, SW / 2, y + 84, MC_DATUM, 1, C_TEXT2, C_BG, w - 8);
      drawText(hl2, SW / 2, y + 97, MC_DATUM, 2, c, C_BG, w - 8);
    }
    float left = 1.0f - constrain((millis() - at) / (float)SHOW_MS, 0.0f, 1.0f);
    g.fillRoundRect(x + 12, y + h - 8, w - 24, 2, 1, C_TRACK);
    g.fillRoundRect(x + 12, y + h - 8, (int)((w - 24) * left), 2, 1, c);
  }
  void draw() {
    if (show == K_NONE) return;
    if (show == K_HEALTH) { drawHealth(); return; }
    TFT_eSPI& g = G();
    uint16_t c = show == K_ICE ? tft.color565(120, 210, 255) : show == K_RAIN ? tft.color565(70, 160, 255)
               : show == K_FOG ? C_TEXT2 : show == K_WIND ? tft.color565(150, 230, 200) : tft.color565(255, 120, 60);
    const char* title = show == K_ICE ? "Buzlanma riski" : show == K_RAIN ? "Yağmur yaklaşıyor"
                      : show == K_FOG ? "Sis: görüş düşük" : show == K_WIND ? "Kuvvetli rüzgâr" : "Aşırı sıcak";
    char val[8] = "", l1[40] = "", l2[40] = "";
    bool deg = false;
    switch (show) {
      case K_ICE:  snprintf(val, sizeof(val), "%d", v); deg = true;
                   strcpy(l1, fromCar ? (portrait ? "Dış sıcaklık" : "Araç dış sıcaklığı") : (portrait ? "3 saat içinde" : "Önümüzdeki 3 saatte en düşük"));
                   strcpy(l2, portrait ? "Köprüler kayabilir" : "Köprü ve gölgeler kayabilir"); break;
      case K_RAIN: snprintf(val, sizeof(val), "%%%d", v);
                   snprintf(l1, sizeof(l1), portrait ? "%02d:00 civarı" : "%02d:00 civarı yağış bekleniyor", vh);
                   strcpy(l2, portrait ? "Mesafeyi artırın" : "Takip mesafesini artırın"); break;
      case K_FOG:  strcpy(l1, portrait ? "Sisli hava" : "Görüş mesafesi düşük");
                   strcpy(l2, portrait ? "Sis farını açın" : "Sis farını açın, yavaşlayın"); break;
      case K_WIND: snprintf(val, sizeof(val), "%d", v);
                   strcpy(l1, "km/sa rüzgâr");
                   strcpy(l2, portrait ? "Yavaş ve dikkatli" : "Köprü ve açık yolda dikkat"); break;
      default:     snprintf(val, sizeof(val), "%d", v); deg = true;
                   strcpy(l1, fromCar ? (portrait ? "Dış sıcaklık" : "Araç dış sıcaklığı") : "Hava sıcaklığı");
                   strcpy(l2, portrait ? "Hararete dikkat" : "Hararet göstergesine dikkat"); break;
    }
    int w = SW - 10, h = portrait ? 196 : 118, x = 5, y = SH / 2 - h / 2;
    g.fillRoundRect(x, y, w, h, 10, C_BG);
    g.drawRoundRect(x, y, w, h, 10, c); g.drawRoundRect(x + 1, y + 1, w - 2, h - 2, 9, c);
    g.fillRoundRect(x + 2, y + 2, w - 4, 22, 8, c);
    drawText(title, SW / 2, y + 13, MC_DATUM, 2, C_BG, c, w - 8);
    bool noVal = !val[0];
    int icx = (portrait || noVal) ? SW / 2 : x + 38, icy = portrait ? y + 54 : (show == K_RAIN ? y + 48 : y + (noVal ? 52 : 54));
    if (portrait && noVal) icy = y + 80;
    float s = portrait ? 2.3f : 2.4f;
    if (show == K_ICE) icSnow(icx, icy, portrait ? 20 : 22, c);
    else if (show == K_RAIN) drawWxIconS(icx, icy - 2, 63, true, s);
    else if (show == K_FOG) drawWxIconS(icx, icy, 45, true, s);
    else if (show == K_WIND) icWind(icx, icy, portrait ? 20 : 22, c);
    else drawSunS(icx, icy, portrait ? 12 : 13, c);
    if (!noVal) {
      Obd::vbg = C_BG;
      float vx = portrait ? SW / 2 - 4 : x + 118, vy = portrait ? y + 106 : y + 52;
      float r = Obd::vNumber(val, vx, vy, portrait ? 34 : 36, 0.14f, portrait ? 90 : 100, C_TEXT);
      if (deg) { g.drawCircle((int)r + 6, (int)vy - 14, 4, C_TEXT); g.drawCircle((int)r + 6, (int)vy - 14, 3, C_TEXT); }
    }
    if (portrait) {
      drawText(l1, SW / 2, y + 138, MC_DATUM, 2, C_TEXT2, C_BG, w - 8);
      drawText(l2, SW / 2, y + 156, MC_DATUM, 2, c, C_BG, w - 8);
      drawText("Tuşa bas: tamam", SW / 2, y + h - 20, MC_DATUM, 1, C_SUBTEXT, C_BG);
    } else {
      drawText(l1, SW / 2, y + 84, MC_DATUM, 1, C_TEXT2, C_BG, w - 8);
      drawText(l2, SW / 2, y + 97, MC_DATUM, 2, c, C_BG, w - 8);
    }
    float left = 1.0f - constrain((millis() - at) / (float)SHOW_MS, 0.0f, 1.0f);   // kendiliginden kapanma
    g.fillRoundRect(x + 12, y + h - 8, w - 24, 2, 1, C_TRACK);
    g.fillRoundRect(x + 12, y + h - 8, (int)((w - 24) * left), 2, 1, c);
  }
}

void setup() {
  HW::begin();                          // Plus2: gucu hemen tut / Plus: AXP192
  Serial.begin(115200);

  prefs.begin("btvol", false);
  prefsNames.begin("btnames", false);
  loadAll();
  Pref::load();
  LostLog::load();
  Net::load();
  Net::rtcInit();

  tft.init();
  backlightBegin();
  applyOrientation();
  blTarget = blFull();

  btnUp.begin(PIN_BTN_UP);
  btnDn.begin(PIN_BTN_DOWN);
  Snd::begin();
  IR::begin();
  IRL::load();
  Imu::begin();
#if USE_ENCODER
  encoderBegin();
#endif
#if !USE_ENCODER
  EncUnit::begin();                      // Grove I2C enkoder unitesi (takiliysa)
#endif

  uint8_t req = rtcWx.bootReq;
  rtcWx.bootReq = BOOT_NORMAL;
  if (req == BOOT_OBD || req == BOOT_SYNC || req == BOOT_PORTAL) req = BOOT_NORMAL;   // eski modlar                    // eski ayri OBD modu artik yok
  splash(req != BOOT_NORMAL);           // tam animasyon yalnizca normal acilista
  if (req == BOOT_KARAOKE) Kara::run();  // karaoke: ayri mod, geri donmez (GUC ile normal moda)

  // Temiz yeniden baslatma: sistem saati yazilim resetinde sifirlanir -> geri yukle
  if (req == BOOT_CLEAN && !Net::timeValid() && rtcWx.clockEpoch > 1700000000UL) {
    struct timeval tv = { (time_t)(rtcWx.clockEpoch + 1 + millis() / 1000), 0 };
    settimeofday(&tv, nullptr);
  }
  HwRtc::restore();                     // pilli donanim saati (iPhone baglaninca duzeltilir)
  {
    esp_reset_reason_t rr = esp_reset_reason();
    const char* rs = rr == ESP_RST_PANIC ? "COKME (panic)" : rr == ESP_RST_INT_WDT || rr == ESP_RST_TASK_WDT || rr == ESP_RST_WDT ? "COKME (watchdog)"
                   : rr == ESP_RST_SW ? "yazilim" : rr == ESP_RST_POWERON ? "guc" : rr == ESP_RST_DEEPSLEEP ? "uyku" : rr == ESP_RST_BROWNOUT ? "voltaj dusuk" : "diger";
    Serial.printf("[Boot] yeniden baslama nedeni: %s\n", rs);
    bool crash = rr == ESP_RST_PANIC || rr == ESP_RST_INT_WDT || rr == ESP_RST_TASK_WDT || rr == ESP_RST_WDT;
    if (crash && ancsArm == 0xA5C5) { ancsBlocked = true; Serial.println("[Boot] onceki cokme bildirim servisinden sonra: bu oturumda bildirimler kapali"); }
    ancsArm = 0;
  }
  Serial.printf("[RAM] Bluetooth oncesi: %u\n", (unsigned)ESP.getFreeHeap());

  bleBegin();
  Obd::begin();                          // OBD2 surus asistani (aciksa BLE adaptore baglanir)
  Bonds::refresh();
  batteryService();
  app.lastInput = millis();
  Serial.printf("[BT Volume v%s] hazir - \"%s\" (kayitli: %d, bos RAM: %u, eslesme: %s)\n",
                FW_VERSION, DEVICE_NAME, Bonds::count, (unsigned)ESP.getFreeHeap(),
                cfg.pairConfirm ? "kod onayli" : "onaysiz");
  BondGuard::logMem("acilis");
  if (req == BOOT_NORMAL) Snd::boot();
}

// ---- Telefon: baglanti takibi + saat/hava/muzik verisini uygula (ana dongu) ---------
void Phone::service() {
  static uint32_t connSeen = 0, failAt = 0;
  uint32_t now = millis();
  if (!bleConnected || bleActive < 0) {
    if (st != IDLE) st = IDLE;
    tries = 0; connSeen = 0;
    if (Music::has || Music::vol >= 0) Music::clear();
  } else {
    if (connSeen != bleConnAt) { connSeen = bleConnAt; st = IDLE; tries = 0; reTry = 0; opened = false; Music::clear(); }   // yeni baglanti
    // v5.7.1: iPhone'da servis sonuclari geliyor ama "arama bitti" olayi gelmeyebiliyor -> 1,5 sn sessizlikte devam
    if (st == SEARCHING && nSvc > 0 && now - lastResAt > 1500 && !setupPending) {
      Serial.printf("[Telefon] arama bitis olayi gelmedi, bulunan %u servisle devam\n", nSvc);
      setupPending = true;
    }
    if ((st == OPENING || st == SEARCHING) && now - t > 12000 && !setupPending) {
      Serial.printf("[Telefon] zaman asimi: %s (%u servis)\n", st == OPENING ? "baglanti acilmadi" : "servis arama bitmedi", nSvc);
      st = FAILED;
    }
    if (st == REFRESH && now - t > 6000) {                       // kesif olayi gelmedi: dogrudan ara
      Serial.println("[Telefon] kesif olayi gelmedi, dogrudan arama");
      st = esp_ble_gattc_search_service(gif, connId, nullptr) == ESP_OK ? SEARCHING : FAILED; t = now;
    }
    if (st == READY && !ancsOn && reTry < 1 && now - readyAt > 10000 && !BondGuard::suspended) {   // ANCS sonradan gorunur
      reTry++; qClear();
      if (esp_ble_gattc_cache_refresh(addr) == ESP_OK) { st = REFRESH; t = now; Serial.println("[Telefon] bildirim servisi yok, yeniden kesif"); }
    }
    if (st == FAILED) {
      if (!failAt) failAt = now;
      // Sanal baglantiyi kapatmak telefonu tamamen dusurebiliyor (v5.7 logu): acik kaldiysa yeniden ara
      if (now - failAt > (tries < 3 ? 20000UL : 60000UL)) {
        failAt = 0;
        if (opened) {
          tries++; nSvc = 0; lastResAt = 0; t = now;
          Serial.printf("[Telefon] servis aramasi yineleniyor (deneme %u)\n", tries);
          st = esp_ble_gattc_search_service(gif, connId, nullptr) == ESP_OK ? SEARCHING : FAILED;
        } else st = IDLE;
      }
    }
    static uint32_t whyAt = 0;
    if (st == IDLE && now - bleConnAt > 15000 && now - whyAt > 15000) {
      whyAt = now;
      Serial.printf("[Telefon] bekliyor: sifreleme %s, tampon %s, eslesme %s, deneme %u\n", peers[bleActive].authed ? "tamam" : "YOK",
                    BondGuard::suspended ? "BOS" : "tamam", Pair::modal == Pair::M_NONE ? "yok" : "VAR", tries);
    }
    if (st == IDLE && peers[bleActive].authed && now - bleConnAt > 3000 && !NameReader::busy() &&
        !BondGuard::suspended && Pair::modal == Pair::M_NONE) {
      tries++;
      Serial.printf("[Telefon] servis kesfi basliyor (deneme %u)\n", tries);
      if (!request(bleActive)) { st = FAILED; Serial.println("[Telefon] baglanti acilamadi"); }
    }
    if (busy && now - busyAt > 3000) {                              // yanitsiz islem: siradakine gec
      Serial.printf("[GATT] islem %u yanitsiz (tur %u, h%u), atlandi\n", nStart, curType, curH);
      busy = false; kick();
    }

    if (ancsArm == 0xA5C5 && st == READY && now - readyAt > 30000) { ancsArm = 0; Serial.println("[Telefon] bildirim servisi kararli"); }
    if (setupPending) { setupPending = false; Serial.printf("[Telefon] %u servis bulundu\n", nSvc); setupServices(); }
  }
  // saat
  if (gotZone) {
    gotZone = false;
    Net::setTz((long)zone15 * 900L);
    Serial.printf("[Saat] iPhone saat dilimi: UTC%+.2f\n", zone15 / 4.0f);
  }
  if (gotTime) {
    gotTime = false;
    int y = tbuf[0] | (tbuf[1] << 8);
    if (y >= 2024 && y < 2100 && tbuf[2] >= 1 && tbuf[2] <= 12 && tbuf[3] >= 1 && tbuf[3] <= 31) {
      time_t utc = civilToEpoch(y, tbuf[2], tbuf[3], tbuf[4], tbuf[5], tbuf[6]) - Net::tzOff;
      struct timeval tv = { utc, 0 };
      settimeofday(&tv, nullptr);
      bool w = HwRtc::write(utc);
      Net::timeSrc = 2;
      if (rtcWx.wxAt == 1 && rtcWx.wcode >= 0) {                 // saatten once gelen hava verisi
        rtcWx.wxAt = (uint32_t)utc;
        if (!rtcWx.wxHourSet) rtcWx.wxHour = (uint8_t)Net::localHour();
      }
      Serial.printf("[Saat] iPhone: %04d-%02d-%02d %02d:%02d:%02d (donanim saati %s)\n", y, tbuf[2], tbuf[3], tbuf[4], tbuf[5], tbuf[6], w ? "yazildi" : "YOK");
    }
  }
  // tani: bildirim trafigi (icerik yazilmaz)
  static uint16_t nsSeen = 0, dsSeen = 0;
  if (nsCount != nsSeen || dsCount != dsSeen) {
    Serial.printf("[Bildirim] yeni: %u, okunan: %u\n", nsCount, dsCount);
    nsSeen = nsCount; dsSeen = dsCount;
  }
  // hava: Kestirmeler bildirimi
  if (wxReady) {
    static char line[sizeof(wxLine)];
    memcpy(line, wxLine, sizeof(line)); uint32_t uid = wxUid;
    wxReady = false;
    bool ok = Wx::parse(line);
    Serial.printf("[Hava] iPhone bildirimi %s (%u bayt): %.120s\n", ok ? "alindi" : "COZULEMEDI", (unsigned)strlen(line), line);
    if (ok) { showToast("Hava güncellendi", C_OK, 1200); ancsDismiss(uid); Eyes::onWeather(); }
  }
  // muzik: yeni sarki -> kart
  if (Music::newTrack) {
    Music::newTrack = false;
    if (Music::has) { Music::cardAt = now; Eyes::onTrack(); }   // ekran kapaliysa uyandirmaz
  }
  // telefonun ses seviyesi -> halka (kendi tus basisimizdan hemen sonra degil)
  if (Music::volNew && now - app.lastVolAt > 900 && !KeyQueue::busy()) {
    Music::volNew = false;
    int lv = constrain((int)lroundf(Music::vol * volSteps()), 0, volSteps());
    if (lv != app.level || (app.muted && lv > 0)) {
      app.level = lv; app.muted = false; markChanged();
      char b[20]; snprintf(b, sizeof(b), "Ses %%%d", (int)lroundf(Music::vol * 100));
      app.volShowAt = now;
      if (app.saver || menu.open) showToast(b, C_ACCENT, 900);
    }
  }
}

// Seri port test satirlari: "MGMWX;T=18;C=Yagmurlu;..." (hava), "MGMTIME=<epoch UTC>" (saat)
void serialService() {
  static char line[320]; static uint16_t n = 0;
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      line[n] = 0;
      if (n && strstr(line, "MGMWX")) { if (Wx::parse(line)) { showToast("Hava güncellendi", C_OK, 1200); Eyes::onWeather(); } }
      else if (n && !strncmp(line, "MGMTIME=", 8)) {
        struct timeval tv = { (time_t)atol(line + 8), 0 };
        if (tv.tv_sec > 1700000000) { settimeofday(&tv, nullptr); HwRtc::write(tv.tv_sec); Net::timeSrc = 2; showToast("Saat ayarlandı", C_OK, 1200); }
      }
      n = 0;
    } else if (n < sizeof(line) - 1) line[n++] = c;
  }
}

void loop() {
  // Baglanti kopunca (veya eslesme modunda) yeniden reklam
  static uint32_t discAt = 0;
  if (bleNeedAdvert) {
    if (!discAt) discAt = millis();
    if (millis() - discAt > 400) {
      bleNeedAdvert = false; discAt = 0;
      if (!bleHidden && (!bleConnected || Pair::active)) bleAdvertise(true);
      bool quiet = millis() - app.lastRejectAt < 4000;     // tercih disi cihaz atildi
      if (!bleConnected) Serial.println("[BT] telefon baglantisi koptu");
      if (!bleConnected && !bleHidden && !quiet) { showToast("Bağlantı koptu", C_WARN, 1400); Snd::disconnect(); }
      if (!bleConnected && !quiet) Eyes::onDisconnect();
      refreshPeerName();
      if (menu.open) buildMenu();
    }
  }
  if (bleNewConn) {
    bleNewConn = false;
    // Kayitli telefon: sifreleme olayi baglanti olayindan once gelebiliyor -> simdi isle
    if (authPendAt && millis() - authPendAt < 8000) {
      for (int i = 0; i < MAX_PEERS; i++)
        if (peers[i].used && !peers[i].authed) { peers[i].authed = true; peers[i].authAt = millis(); memcpy(peers[i].idAddr, authPendAddr, 6); Serial.printf("[BT] onceki sifreleme cihaz %d icin uygulandi\n", i); break; }
      authPendAt = 0;
    }
    if (bleActive >= 0) { const uint8_t* a = peers[bleActive].bda; Serial.printf("[BT] telefon baglandi %02X:%02X:%02X:%02X:%02X:%02X\n", a[0], a[1], a[2], a[3], a[4], a[5]); }
    Bonds::refresh();
    BondSnap::take();
    // Kayitli olmayan cihaz baglandi: birazdan eslesme gelecek -> kayit icin RAM ac
    for (int i = 0; i < MAX_PEERS; i++) {
      if (!peers[i].used || peers[i].checked || peers[i].authed) continue;
      peers[i].knownAtConn = Bonds::indexOf(peers[i].bda) >= 0;
      if (!peers[i].knownAtConn && !BondGuard::suspended) {
        BondGuard::resumeAt = millis() + 15000;
        BondGuard::suspend();
      }
    }
    // Tercihli cihaz secildiyse: tercih disi kayitli cihaz sifreleme bittikten sonra
    // birakilir (peerService). Burada sadece bildirimi susturuyoruz.
    bool rejected = false;
    for (int i = 0; i < MAX_PEERS; i++)
      if (peers[i].used && !peers[i].checked && Pref::set && !Pair::active &&
          !Pref::is(peers[i].bda) && Bonds::indexOf(peers[i].bda) >= 0) rejected = true;
    if (Pair::active && !bleHidden) bleAdvertise(true);
    refreshPeerName();
    if (rejected && !(bleActive >= 0 && !peers[bleActive].rejected)) {
      if (menu.open) buildMenu();
    } else {
    if (!Pair::active) {
      char t[40];
      if (app.peerName[0]) snprintf(t, sizeof(t), "Bağlandı: %s", app.peerName);
      else strcpy(t, "Bağlandı");
      showToast(t, C_OK, 1500);
      Snd::connect();
    }
    if (app.saver) Eyes::react(Eyes::LOVE, 2600);                // koruyucuda: mutlu
    else userActivity();
    if (menu.open) buildMenu();
    }
  }

  Input::service();
#if USE_ENCODER
  encoderService();
#endif
#if !USE_ENCODER
  EncUnit::service();
#endif
  KeyQueue::service();
  Snd::service();
  IR::service();
  IRL::service();
  AutoRot::service();
  Mic::service();
  if (IR::changed || AutoRot::changed || IRL::changed) {
    IR::changed = AutoRot::changed = IRL::changed = false;
    if (menu.open) buildMenu();
  }
  Cam::service();
  SleepT::service();
  wifiService();
  NameReader::service();
  Phone::service();
  serialService();
  {                                                             // OBD2 adaptoru baglandi: ana ekran OBD2
    static bool dashWas = false;
    bool dash = Obd::linked();
    if (dash && !dashWas) {
      if (app.saver) app.saver = false;
      userActivity();
      if (!menu.open) showToast("OBD2 bağlandı", C_OK, 1200);
    }
    dashWas = dash;
  }
  Info::service();
  Scan::service();
  peerService();
  pairingService();
  BondGuard::service();
  batteryService();
  Obd::loop();
  powerService();
  backlightService();
  render();
  delay(1);
}
