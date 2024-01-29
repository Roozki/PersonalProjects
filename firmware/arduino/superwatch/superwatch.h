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
int math = 0;
int cpen = 1;
int igen201 = 2;
int civl = 3;
int igen230 = 4;
int chbe = 5;

String classes[6] =   { "MATH 101", "CPEN 312", "IGEN 201", "CIVL 215", "IGEN 230", "CHBE 244" };
String classdesc[6] = { "Tues, 12:00 - 14:00", "Wed/Fri, 8:30 - 10:00", "Tue/Thur, 10:30 - 12:00", "MWF, 10:00 - 11:00", "Thur, 9:00 - 10:00", "MWF, 11:00 - 12:00" };
String classday[6] = { "xTu", "xWF", "xTuTh", "xMWF", "xTh", "xMWF"};
int classhr[6] = {12, 8, 10, 10, 9, 11};
int classmin[6] = {00, 30, 30, 00, 00, 00};

//temp variables, am i bad for making these?
String tString;int classtime[12] = { 9, 00, 1, 00}; //h, m, h, m, h, m...


//hardware objects
TFT_eSPI tft = TFT_eSPI();
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ca.pool.ntp.org", -(3600 * 8));

const char* ssid = "ubcvisitor";  // "StiggleSWERF V3";
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
#define NUM_CLASSES 6


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

int nextclassindex;

