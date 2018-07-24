#include "IFTTTWebhooks.h"

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "config.h"

IFTTTWebhooks::IFTTTWebhooks() {  
}

// Maker WebHook
void IFTTTWebhooks::sendToIFTTT(String value1, String value2, String value3) {
  WiFiClient client;
  const char* server = "maker.ifttt.com";

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("conecting to wifi.");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("wifi connected!");

  Serial.println("connecting to IFTTT Webhooks server...");
  if (!client.connect(server, 80)) {
    Serial.println("Connection failed!");
    return;
  }
  
  Serial.println("Connected!");
  String url = "/trigger/" + WEBHOOKS_EVENT + "/with/key/" + WEBHOOKS_KEY + "?value1=" + value1 + "&value2=" + value2 + "&value3=" + value3;
  client.println("GET " + url + " HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();

  Serial.print("Waiting for response ");

  int count = 0;
  while (!client.available()) {
    delay(50);
    Serial.print(".");
    count++;
    if (count > 20 * 20) {
      Serial.println("(send) failed!");
      return;
    }
  }
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  if (!client.connected()) {
    Serial.println("disconnecting from server.");
    client.stop();
  }
  WiFi.disconnect(true);
}

