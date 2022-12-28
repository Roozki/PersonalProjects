#include <NTPClient.h>
// change next line to use with another board/shield
//#include <ESP8266WiFi.h>
#include <WiFi.h>  // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
#include <TFT_eSPI.h>
#include <Button2.h>
//for reference, more info and modifyabiliyt in TFT_eSPI
// #define TFT_BLACK       0x0000      /*   0,   0,   0 */
// #define TFT_NAVY        0x000F      /*   0,   0, 128 */
// #define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
// #define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
// #define TFT_MAROON      0x7800      /* 128,   0,   0 */
// #define TFT_PURPLE      0x780F      /* 128,   0, 128 */
// #define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
// #define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
// #define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
// #define TFT_BLUE        0x001F      /*   0,   0, 255 */
// #define TFT_GREEN       0x07E0      /*   0, 255,   0 */
// #define TFT_CYAN        0x07FF      /*   0, 255, 255 */
// #define TFT_RED         0xF800      /* 255,   0,   0 */
// #define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
// #define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
// #define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
// #define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
// #define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
// #define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F      
// #define TFT_BROWN       0x9A60      /* 150,  75,   0 */
// #define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
// #define TFT_SILVER      0xC618      /* 192, 192, 192 */
// #define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
// #define TFT_VIOLET      0x915C      /* 180,  46, 226 */



TFT_eSPI tft = TFT_eSPI();

int width = 240;
int height = 135;

int disPrevTime = 0;

#define RED 0xF800
#define WHITE 0xFFFF
#define ORANGE 0xF57C

#define BUTTON_1 35
#define BUTTON_2 0
#define ADC_PIN 34
#define ADC_EN 14

const char *ssid = "StiggleSWERF V3";
const char *password = "mountainbike";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ca.pool.ntp.org", -(3600 * 8));

bool bypass = false;
Button2 btn1;
Button2 btn2;
int menu = 1;




void disReminder(float timeout, String reminder){
  tft.fillScreen(TFT_BLACK){
    

  }


}

void btn1Callback(Button2& btn) {
    switch (btn.getType()) {
        case single_click:
            menu = 2;
            tft.fillScreen(TFT_WHITE);
            delay(1000);
            break;
        case double_click:
            
            break;
        case triple_click:
            
            break;
        case long_click:
            
            break;
    }
    bypass = true;
}
void btn2Callback(Button2& btn) {
    switch (btn.getType()) {
        case single_click:
            menu = 1;
            break;
        case double_click:
            
            break;
        case triple_click:
            
            break;
        case long_click:
            
            break;
    }
        bypass = true;

}
void disTime() {
  timeClient.update();
  tft.setTextSize(3);
  tft.setTextFont(1);
  tft.setTextColor(TFT_MAGENTA);

  tft.setCursor(10, 10);
  tft.fillScreen(TFT_BLACK);

  tft.println(timeClient.getFormattedTime());
  int day = timeClient.getDay();
  tft.setTextColor(TFT_PINK);
  tft.setCursor(10, tft.getCursorY() + 3);
  tft.setTextFont(2);
  switch (day) {
    case (1):
      tft.println("Monday");
      break;
    case (2):
      tft.println("Tuesday");
      break;
    case (3):
      tft.println("Wednesday");
      break;
    case (4):
      tft.println("Thursday");
      break;
    case (5):
      tft.println("Friday");
      break;
    case (6):
      tft.println("Saturday");
      break;
    case (7):
      tft.println("Sunday");
      break;
  }
}

void disMisc(int cmd) {
  switch (cmd) {
    case (1):
      tft.setCursor(10, (height / 3) * 2);
      tft.setTextSize(2);
      tft.setTextColor(TFT_BLUE);
      tft.setTextFont(2);
      tft.print("ROWAN ZAWADZKI");
      break;
    case (2):
      tft.setCursor(160, 100);
      tft.setTextSize(2);
      tft.setTextColor(TFT_GREEN);
      tft.setTextFont(1);
      int v = analogRead(ADC_PIN);
      float voltage = ((float)v / 4095.0) * 2.0 * 3.3 * 1.1;
      tft.print(String(voltage) + "V");
      break;
  }
}





void setup() {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(3);  // Landscape orientation, USB at bottom right
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, height / 2);
  tft.setTextSize(2);
  tft.setTextFont(4);

  tft.print("INIT....");
  delay(1000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);

  WiFi.begin(ssid, password);
  tft.setCursor(10, 10);
  tft.setTextFont(1);
  tft.setTextColor(TFT_MAGENTA);
  pinMode(ADC_EN, OUTPUT);
  digitalWrite(ADC_EN, HIGH);
  tft.println("ADC enabled");
  btn1.begin(BUTTON_1);
  btn2.begin(BUTTON_2);
  delay(100);

btn1.setClickHandler(btn1Callback);
btn1.setLongClickDetectedHandler(btn1Callback);  // this will only be called upon detection
btn1.setDoubleClickHandler(btn1Callback);
btn1.setTripleClickHandler(btn1Callback);

btn2.setClickHandler(btn2Callback);
btn2.setLongClickDetectedHandler(btn2Callback);  // this will only be called upon detection
btn2.setDoubleClickHandler(btn2Callback);
btn2.setTripleClickHandler(btn2Callback);


  tft.println("buttons enabled");
  delay(100);

  tft.println("Attempting Connection to specified network using preset credentials");

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    tft.print(".");
  }
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_BLACK);
  tft.print("Connection Appears Successfull");
  tft.setCursor(0, 20);

  tft.println("i.");
  delay(50);

  tft.println(".rsntienrsetnrstn");
  delay(50);

  tft.println("ariestoierasntnb.");
  delay(50);

  tft.println("tr/iesktrksotiersktoierst");
  delay(50);

  tft.println("hr;ysatnorst'nrs");
  delay(50);
  tft.println("ink..");
  delay(50);

  tft.println(".rsntienrsetnrstn");
  delay(50);

  tft.println("ariestoierasntnb.");
  delay(50);

  tft.println("tr/iesktrksotiersktoierst");
  delay(50);

  tft.println("hr;ysatnorst'nrs");
  delay(50);
  tft.println("ink..");

  tft.println(" therefore, I am");
  tft.setTextColor(TFT_MAGENTA);
  delay(100);



  timeClient.begin();
}

void loop() {
btn1.loop();
btn2.loop();


if(micros() - disPrevTime > 1000000 || bypass){

  switch (menu) {
    case (1):
      disTime();
      disMisc(2);
      break;
    case (2):
      disReminder(96.0, "your balls");
      break;
  }
  disPrevTime = micros();
  bypass = false;
}
}