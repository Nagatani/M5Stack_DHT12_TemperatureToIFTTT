#include <M5Stack.h>
#include <time.h>

#include "DHT12.h"
DHT12 dht12;

#include "ClockController.h"
ClockController cc;

#include "IFTTTWebhooks.h"
IFTTTWebhooks ift;

int lastRunHour = 0;
int lastRunMin = 0;

void sendWebhooks(double temp, double humi) {
  tm t = cc.getDateTime();
  if (t.tm_hour != lastRunHour) {
    // 1時間おきに送信
    lastRunHour = t.tm_hour;
    lastRunMin = t.tm_min;
    // URLエンコードは各自でやって
    ift.sendToIFTTT("%E3%83%AA%E3%83%93%E3%83%B3%E3%82%B0%E6%B8%A9%E5%BA%A6%E7%AE%A1%E7%90%86", String(temp), String(humi));
  }
}

void clockTask(void * pvParameters) {
  for (;;) {
    tm timeinfo = cc.getDateTime();
    M5.Lcd.fillRect(0, 0, 319, 110, BLACK);
    M5.Lcd.setTextColor(WHITE);

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.println(&timeinfo, "%Y-%m-%d");
    M5.Lcd.setCursor(10, 50);
    M5.Lcd.println(&timeinfo, "     %H:%M");
    M5.Lcd.println();
    delay(30000);
  }
}

void updateTempAndHumi(void * pvParameters) {

  for (;;) {
    double t = dht12.readTemperature();
    double h = dht12.readHumidity();

    // Webhooksへ送信
    sendWebhooks(t, h);

    // LCD表示設定
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.fillRect(0, 110, 319, 209, BLACK);
    M5.Lcd.setTextSize(10);
    if (t >= 35) {
      M5.Lcd.setTextColor(RED);
    } else if (t >= 32) {
      M5.Lcd.setTextColor(YELLOW);
    } else if (t >= 28) {
      M5.Lcd.setTextColor(GREEN);
    } else {
      M5.Lcd.setTextColor(WHITE);
    }
    M5.Lcd.setCursor(10, 120);
    M5.Lcd.print("T:");
    M5.Lcd.println(t);

    if (h >= 90) {
      M5.Lcd.setTextColor(RED);
    } else if (h >= 70) {
      M5.Lcd.setTextColor(YELLOW);
    } else if (h >= 50) {
      M5.Lcd.setTextColor(GREEN);
    } else {
      M5.Lcd.setTextColor(WHITE);
    }
    M5.Lcd.setCursor(10, 180);
    M5.Lcd.print("H:");
    M5.Lcd.println(h);

    delay(60000);
  }
}

void setup() {
  M5.begin();
  Wire.begin();
  cc = ClockController();
  cc.setupTimeZone();
  ift = IFTTTWebhooks();

  xTaskCreatePinnedToCore(clockTask, "clockTask", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(updateTempAndHumi, "updateTempAndHumi", 4096, NULL, 2, NULL, 0);
}

void loop() {
  M5.update();
}
