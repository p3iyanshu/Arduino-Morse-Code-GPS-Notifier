#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

// ✅ Wi-Fi credentials — fill in your own
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ✅ Pushover credentials — get from https://pushover.net
const char* pushoverUserKey  = "YOUR_PUSHOVER_USER_KEY";
const char* pushoverAPIToken = "YOUR_PUSHOVER_API_TOKEN";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    Serial.print(" Status: ");
    Serial.println(WiFi.status());
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Connected to Wi-Fi!");
    Serial.print("🏠 IP Address: ");
    Serial.println(WiFi.localIP());

    sendPushoverNotification("✅ Hello from NodeMCU ESP8266!");
  } else {
    Serial.println("\n❌ Failed to connect to Wi-Fi.");
  }
}

void loop() {
  // Nothing in loop
}

void sendPushoverNotification(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // ⚠️ Accept all certificates (not for production use)

    HTTPClient http;
    http.begin(client, "https://api.pushover.net/1/messages.json");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData =
      "token=" + String(pushoverAPIToken) +
      "&user=" + String(pushoverUserKey) +
      "&message=" + message;

    int httpCode = http.POST(postData);

    if (httpCode > 0) {
      Serial.printf("✅ Pushover Response Code: %d\n", httpCode);
      String response = http.getString();
      Serial.println("📨 Response: " + response);
    } else {
      Serial.printf("❌ HTTP Request failed. Error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("❌ WiFi not connected during notification attempt.");
  }
}
