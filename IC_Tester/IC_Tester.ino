#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#define MINPRESSURE 150
#define MAXPRESSURE 1000

const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_MINX = 117, TS_MAXX = 897, TS_MINY = 89, TS_MAXY = 898;
const int TS_LR = 250;

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x8410
#define TTGREEN 0x03ED
#define DRKBLUE 0x0008

//TFT initialization
MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_GFX_Button buttons[12];

#define MEGA_SOFT_SPI 1
#define chipSelect 10
#define SOFTWARE_SPI 1
#include <SPI.h>
#include <SD_patched.h>
#define fname "database.txt"

//Pin Definitions
const int pin14[14] = {26, 28, 30, 32, 34, 36, 38, 39, 37, 35, 33, 31, 29, 27};
const int pin16[16] = {26, 28, 30, 32, 34, 36, 38, 40, 41, 39, 37, 35, 33, 31, 29, 27};
const int pin18[18] = {26, 28, 30, 32, 34, 36, 38, 40, 42, 43, 41, 39, 37, 35, 33, 31, 29, 27};
const int pin20[20] = {26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27};
const int pin24[24] = {26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27};
String pinname[24] = {"    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    ","    "};
boolean errpin[24] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
int pinCount = 14;  
int *pin;

//Structure Definiton for IC
typedef struct {
  String num;
  String name;
  String rus;
} IC;

String currentIC="";
int screenStatus = 0, lastStatus = 0;

// Reset Function, resets Arduino
void(* resetFunc) (void) = 0;

// SD Card init
void SD_init()
{
  pinMode(chipSelect, OUTPUT);

  // If we do not find the SD-Card, notify and reset
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present.");
    getTouch();
    resetFunc();
  }

  Serial.println("SD card initialized.");
}


void setup() {
  Serial.begin(115200);
  digitalWrite(22,LOW);
  tft_init();
  SD_init();
}

void loop() {
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.height(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());

    //Swapping for TFT Rotation 3
    p.x = p.x + p.y; p.y = p.x - p.y; p.x = p.x - p.y;

    delay(10);

    // Check if buttons are pressed

    if (screenStatus == 0)            // Startup Screen
      screenStatus = 1;

    else if (screenStatus == 1)       // Main Screen Buttons
    {
      if (buttons[0].contains(p.x, p.y))
      {
        Serial.println("AUTO MODE");
        screenStatus = 2;
      }
      else if (buttons[1].contains(p.x, p.y))
      {
        Serial.println("MANUAL MODE");
        screenStatus = 3;
      }
      else if (buttons[2].contains(p.x, p.y))
      {
        Serial.println("CHANGE PIN COUNT");
        pinCount += 2;
        if (pinCount>24) pinCount=14;
        currentIC="";
        tft.fillRect(0,185,110,60,BLACK);
        drawIC(pinCount,currentIC);
        drawStatus(String(pinCount)+" Pin",TTGREEN);
        screenStatus = 1;
      }
      else if (currentIC!="")
      {
        if (buttons[3].contains(p.x, p.y))
        {
          Serial.println("RETEST IC");
          screenStatus = 4;
        }
      }
    }
  }
  
  pinMode(XM, OUTPUT);
  digitalWrite(XM, LOW);
  pinMode(YP, OUTPUT);
  digitalWrite(YP, HIGH);

  // Simple state machine to display the different screens
  // 
  if (screenStatus != lastStatus)     // Check if the button presses need screen updates
  {
    switch (screenStatus)
    {
      case 0: startScreen();          // Show Startup Screen
        break;
      case 1: modeScreen();           // Main Screen  
        break;
      case 2: autoSearch(pinCount);   // Auto Screen
        if (currentIC!=""){           // If we selected an IC before show the Retest button 
          buttons[3].initButton(&tft, 45, 215, 80, 50, GRAY, TTGREEN, WHITE, (char *)"Retest", 2);
         tft.fillRect(0,200,110,100,BLACK);
          buttons[3].drawButton();
        }
        screenStatus=1;               // Go back to Main Screen
        break;
      case 3: currentIC = getIC();    // Manual Screen, first display the IC Number Input Screen 
        for (int i=0;i<24;i++) pinname[i] = "    ";  // Clear pin descriptions
        modeScreen();                 // display the empty Main Screen again
        if (currentIC != "") {        // if we entered a number do a manual test
          manualSearch(currentIC);    
        }
        screenStatus=1;               // Go back to Main Screen
        break;
      case 4:                         // Retest IC
        if (currentIC != "") {        // if we have a valid IC retest it
          manualSearch(currentIC);  
        }
        screenStatus=1;               // Go back to Main Screen
        break;
    }
    lastStatus = screenStatus;
  }
  delay(5);
}
