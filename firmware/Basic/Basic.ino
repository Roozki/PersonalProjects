#include <NTPClient.h>
// change next line to use with another board/shield
//#include <ESP8266WiFi.h>
#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
#include <TFT_eSPI.h>



TFT_eSPI tft = TFT_eSPI(); 

int width = 240;
int height = 135;

#define RED        0xF800
#define WHITE      0xFFFF
#define ORANGE     0xF57C00
const char *ssid     = "StiggleSWERF V3";
const char *password = "mountainbike";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ca.pool.ntp.org", -(3600*8));


void disTime(){
 timeClient.update();
 tft.setTextSize(3);
 tft.setTextFont(1);
    tft.setTextColor(TFT_MAGENTA);

tft.setCursor(10, 10);
tft.fillScreen(TFT_BLACK);

tft.print(timeClient.getFormattedTime());
}

void disMisc(int cmd){
switch(cmd){
  case(1):
    tft.setCursor(10, (height/3)*2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLUE);
    tft.setTextFont(2);
    tft.print("ROWAN ZAWADZKI");
}
}

void setup(){
  Serial.begin(9600);
    tft.init();
  tft.setRotation(3); // Landscape orientation, USB at bottom right
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
  tft.setCursor(0, height / 2);
    tft.setTextFont(1);
tft.setTextColor(TFT_MAGENTA);

    tft.println("Attempting Connection to specified network using preset credentials");

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 50 );
    tft.print ( "." );
  }
  tft.setTextColor(TFT_GREEN);
tft.setCursor(0,0);
  tft.fillScreen(TFT_BLACK);
delay(1000);
  tft.print("Connection Appears Successfull");
tft.setCursor(0,20);
delay(2000);

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

 disTime();
 disMisc(1);

  delay(1000);
}
