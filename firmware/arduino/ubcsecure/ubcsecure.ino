#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <esp_wpa2.h>
#include <esp_wifi.h>

const char* ssid = "ubcvisitor";
#define id         "rojoz"
#define password  "Athena12!!!!"


//static const char incommon_ca[] PROGMEM = 




void setup() {
  Serial.begin(115200);
Serial.println(WiFi.macAddress());

  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;

    // Make a HTTP request to the ChatGPT API
   
    http.begin("https://api.openai.com/v1/completions");
    http.addHeader("Authorization", "Bearer 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");
    http.addHeader("Content-Type", "application/json");
    //
    //http.addHeader("param", "model=text-davinci-002&prompt:How far away is the sun right now?");
    //http.addHeader("engine", "text-davinci-002");

    //http.addHeader("prompt", "This is a test that");

   int httpResponseCode = http.POST("{\"model\":\"text-davinci-003\",\"prompt\":\"Hello, Bill Nye is the\"}");
    if (httpResponseCode > 0) { //Check for the returning code
      String response = http.getString();
      Serial.println(response);
    }

    http.end(); //Close connection
  }
  else {
    Serial.println("Error in WiFi connection");
  }

  delay(30000); //Send a request every 30 seconds
}