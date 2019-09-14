// ---------- МАТРИЦА ---------
#define BRIGHTNESS 40      // стандартная маскимальная яркость (0-255)
#define CURRENT_LIMIT 2000 // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH 16 // ширина матрицы
#define HEIGHT 8 // высота матрицы
// #define MATRIX_WIDTH 16 // ширина матрицы
// #define MATRIX_HEIGHT 8 // высота матрицы

#define COLOR_ORDER RGB // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE 0      // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE 0 // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION 0  // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
// при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/

// ============= ДЛЯ РАЗРАБОТЧИКОВ =============
#define LED_PIN 7 // пин ленты
//#define BTN_PIN 4
#define MODE_AMOUNT 18

#define NUM_LEDS WIDTH*HEIGHT
#define SEGMENTS 1 // диодов в одном "пикселе" (для создания матрицы из кусков ленты)
// ---------------- БИБЛИОТЕКИ -----------------
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
#define NTP_INTERVAL 60 * 1000 // обновление (1 минута)

#include <FastLED.h>
CRGB leds[NUM_LEDS];

#include "GyverButton.h"
#define BTN_PIN 8
GButton button(BTN_PIN);

unsigned char matrixValue[8][16];
boolean dawnFlag = false;

#define MODE 0        // режим при запуске
int8_t currentMode = 0;
boolean loadingFlag = true;
boolean ONflag = true;
uint32_t eepromTimer;
boolean settChanged = false;

struct
{
  byte brightness = 50;
  byte speed = 30;
  byte scale = 40;
} modes[MODE_AMOUNT];

void setup()
{
    // ЛЕНТА
    FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS) /*.setCorrection( TypicalLEDStrip )*/;
    FastLED.setBrightness(BRIGHTNESS);
    if (CURRENT_LIMIT > 0)
        FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.clear();
    FastLED.show();

    Serial.begin(115200);
    Serial.println();
}

void loop()
{
    effectsTick();
    buttonTick();
}
