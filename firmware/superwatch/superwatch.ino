//my superwatch

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <TFT_eSPI.h>
#include <Button2.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define CUSTOM_SETTINGS
#define INCLUDE_SENSOR_MODULE
#include <DabbleESP32.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


//for reference, more info and modifyabiliyt in TFT_eSPI.h
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


//    ++++   flags
bool gptReady = true;
//bool gnssCool = true;
bool btCoolDown = true;
String classes[] = {"CIVL 215"};  


//temp variables, am i bad for making these?
String tString;

//hardware objects
TFT_eSPI tft = TFT_eSPI();
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ca.pool.ntp.org", -(3600 * 8));


const char* ssid = "ubcvisitor"; // "StiggleSWERF V3"; 
//const char* password =   //"----";


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


bool bypass = false;
bool powerlow = false;
Button2 btn1;
Button2 btn2;
int menu = 0;
int menuMax = 1;
int menuMin = -2;
int brightness = 200;
int maxBright = 255;
int minBright = 1;
bool brightflag = 0;


String gpt3Comm(String prompt, int type) {
  String output = "null failure";

  if (WiFi.status() == WL_CONNECTED) {  //Check WiFi connection status
    HTTPClient http;
    // Make a HTTP request to the ChatGPT API
    http.begin("https://api.openai.com/v1/completions");
    //yield();

    http.addHeader("Authorization", "Bearer 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");
    http.addHeader("Content-Type", "application/json");
    String request = "{\"model\":\"text-davinci-003\",\"max_tokens\":60,\"prompt\":\"" + prompt +"\"}";

    int httpResponseCode = http.POST(request);
    if (httpResponseCode > 0) {  //Check for the returning code
      String response = http.getString();
      output = response;
    }

    http.end();  //Close connection
  } else {
    output = "Internet Connection Failure";
  }
  gptReady = false;  //needs a cooldown

  //parsing
  output = output.substring(output.lastIndexOf("text") + 2, output.indexOf("index") - 1);

  return output;
}

void lowpower() {
  tft.fillScreen(TFT_DARKGREY);
  tft.setCursor(20, 50);
  tft.setTextFont(2);
  tft.setTextSize(3);
  tft.setTextColor(TFT_LIGHTGREY);
  delay(200);
  tft.print("S");
  ledcWrite(0, 100);  // 0-15, 0-255 (with 8 bit resolution); 0=totally dark;255=totally shiny

  delay(200);
  tft.print(" L");
  ledcWrite(0, 70);

  delay(200);
  tft.print(" E");
  ledcWrite(0, 50);

  delay(200);
  tft.print(" E");
  ledcWrite(0, 10);

  delay(200);
  tft.print(" P");
  ledcWrite(0, 5);

  delay(500);
  tft.fillScreen(TFT_BLACK);
  ledcWrite(0, 0);
}
void disReminder(float timeout, String reminder) {
  tft.fillScreen(TFT_NAVY);
  tft.setTextColor(TFT_CYAN);
  tft.setTextFont(1);
  tft.setTextSize(3);
  tft.setCursor(10, 10);
  tft.print("reminders");
  tft.setCursor(10, 40);
  tft.setTextColor(TFT_LIGHTGREY);
  tft.setTextSize(1);
  tft.setTextFont(2);
  //tft.setTextPadding(10);
  tft.print(reminder);
}

void btn1Callback(Button2& btn) {
  switch (btn.getType()) {

    case single_click:
      if (!brightflag) {
        if (!powerlow) {
          menu++;
          if (menu > menuMax) {
            menu = menuMax;
          }
        }
      } else {
        brightness -= (6 + random(0, 6));
        if (brightness < minBright) {
          brightness = minBright;
        }
      }


      break;
    case double_click:
      if (!powerlow) {
        if (brightflag) {
          brightness -= 60;
          if (brightness < minBright) {
            brightness = minBright;
          }
        }
      }
      break;
    case triple_click:
      if (!powerlow) {
      }
      break;
    case long_click:
      powerlow = !powerlow;
      break;
  }
  bypass = true;
}
void btn2Callback(Button2& btn) {
  switch (btn.getType()) {
    case single_click:
      if (!brightflag) {
        menu--;
        if (menu < menuMin) {
          menu = menuMin;
        }
      } else {

        brightness += (6 + random(0, 10));
        if (brightness > maxBright) {
          brightness = maxBright;
        }
      }
      break;
    case double_click:
      if (!powerlow && !brightflag) {

      } else {
        brightness += 60;
        if (brightness > maxBright) {
          brightness = maxBright;
        }
      }
      break;
    case triple_click:
      if (!powerlow) {
      }
      break;
    case long_click:
      brightflag = !brightflag;
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
      tft.println(F("Monday"));
      break;
    case (2):
      tft.println(F("Tuesday"));
      break;
    case (3):
      tft.println(F("Wednesday"));
      break;
    case (4):
      tft.println(F("Thursday"));
      break;
    case (5):
      tft.println(F("Friday"));
      break;
    case (6):
      tft.println(F("Saturday"));
      break;
    case (0):
      tft.println(F("Sunday"));
      break;
  }
  fontqset(10, tft.getCursorY() + 5, TFT_CYAN, 2, 1);
  tft.print("Longitude: ");
  tft.println(Sensor.getGPSlongitude());
  tft.setCursor(10, tft.getCursorY());
  tft.print("Latitude: ");
  tft.println(Sensor.getGPSLatitude());

}

void disGNSS(int lat, int long) {
  // if (gnssCool){
  //   //TODO
  // }
}



void disMisc(int cmd) {
  switch (cmd) {
    case (1):
      {
        tft.setCursor(10, (height / 3) * 2);
        tft.setTextSize(2);
        tft.setTextColor(TFT_BLUE);
        tft.setTextFont(2);
        tft.print("ROWAN ZAWADZKI");
        break;
      }
    case (2):
      {
        tft.setCursor(160, 100);
        tft.setTextSize(2);
        tft.setTextColor(TFT_GREEN);
        tft.setTextFont(1);
        int v = analogRead(ADC_PIN);
        float voltage = ((float)v / 4095.0) * 2.0 * 3.3 * 1.1;
        tft.print(String(voltage) + "V");
        break;
      }
    case (10):
      {
        tft.setCursor(4 * width / 5, 0);
        tft.setTextSize(1);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.print(menu);
        break;
      }
    case (40):  //bluetooth
      {
         tft.fillScreen(TFT_DARKCYAN);
         fontqset(10, 5, TFT_LIGHTGREY, 4, 1);
         tft.print("BT COMMS");
         fontqset(10, 30, TFT_PINK, 1, 2);
         tft.print("x accel:");
         tft.println(Sensor.getAccelerometerXaxis());
         tft.print(" y accel:");
         tft.println(Sensor.getAccelerometerYaxis());
         tft.print(" z accel:");
         tft.println(Sensor.getAccelerometerZaxis());

        break;
      }
    case (50):  //STANDBY
      {
        tft.fillScreen(TFT_LIGHTGREY);
        fontqset(10, 20, TFT_OLIVE, 4, 2);
        tft.print("STANDBY");
        break;
      }
    case (80):  //brightness
      {
        tft.fillScreen(TFT_OLIVE);
        fontqset(10, 5, TFT_WHITE, 2, 3);
        tft.print("brightness");
        tft.drawRect(10, 70, brightness, 30, TFT_LIGHTGREY);
        fontqset(10, 100, TFT_WHITE, 2, 2);
        tft.print(brightness);
        break;
      }
    case (90):  //GPT test
      {
        tft.fillScreen(TFT_OLIVE);
        fontqset(10, 5, TFT_WHITE, 1, 2);
        tft.print("GPT3 COMMS");
        fontqset(10, 20, TFT_WHITE, 2, 1);
        tft.print(tString);
        break;
      }
  }
}

void fontqset(int x, int y, int colour, int font, int size) {
  tft.setCursor(x, y);
  tft.setTextColor(colour);
  tft.setTextFont(font);
  tft.setTextSize(size);
}




void setup() {
  //  Serial.begin(9600); serial lame. tft ftw!
  //oo but bluetooth
  tft.init();
  tft.setRotation(3);  // Landscape orientation, USBC <-- left
  tft.fillScreen(TFT_MAROON);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(10, height / 2);
  tft.setTextSize(2);
  tft.setTextFont(4);

  tft.print("INIT....");
  delay(1000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);

  WiFi.begin(ssid);
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
Dabble.begin("SUPERWATCH!!!");

tft.println(F("bluetooth enabled"));
delay(100);
  tft.println(F("Attempting Connection to specified network using preset credentials"));

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
  // delay(50);

  tft.println(" therefore, I am");
  tft.setTextColor(TFT_MAGENTA);
  delay(100);



  timeClient.begin();

  pinMode(TFT_BL, OUTPUT);
  ledcSetup(0, 5000, 8);     // 0-15, 5000, 8
  ledcAttachPin(TFT_BL, 0);  // TFT_BL, 0 - 15
  ledcWrite(0, 200);         // 0-15, 0-255 (with 8 bit resolution); 0=totally dark;255=totally shiny




}

String nextClass(){


}

void loop() {
  //hardware refresh
  btn1.loop();
  btn2.loop();
  Dabble.processInput();

  if (micros() - disPrevTime > 900000 || bypass) {
    if (!powerlow && !brightflag) {
      switch (menu) {
        case (0):
          disTime();
          disMisc(2);
          break;
        case (1):
          disReminder(96.0, "TestniDDDDDD");
          break;
        case (-2):  //GPT testsing
          if (gptReady) {
            disMisc(50);

            tString = gpt3Comm("Hello, could you share some good job interview tips?", 1);
          }
          disMisc(90);
          break;
        case (-1):  //bluetooth comms
            disMisc(40);
        default:
          disMisc(0);
          break;
      }
      disMisc(10);  //power+menu


    } else if (brightflag) {

      disMisc(80);
      ledcWrite(0, brightness);

    } else {
      lowpower();
      while (powerlow) {
        btn1.loop();
      }
      ledcWrite(0, brightness);
    }
    disPrevTime = micros();
    bypass = false;
  }
}