//UI details
#define BUTTON_X 34
#define BUTTON_Y 90
#define BUTTON_W 50
#define BUTTON_H 46
#define BUTTON_SPACING_X 13
#define BUTTON_SPACING_Y 13
#define BUTTON_TEXTSIZE 2

// text box where numbers go
#define TEXT_X 90
#define TEXT_Y 24
#define TEXT_W 140
#define TEXT_H 36
#define TEXT_TSIZE 3
#define TEXT_TCOLOR TTGREEN
#define TEXT_LEN 7

char buttonlabels[10][5] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};

void tft_init()
{
  // Initialize LCD Display
  tft.reset();
  uint16_t identifier = tft.readID();
  //Serial.println(identifier);
  if (identifier == 0x0101)     // Bugfix for mcu_friend displays
    identifier = 0x9341;
  tft.begin(identifier);
  Serial.println("Display initialized.");
  tft.setRotation(1);
  screenStatus = 0;
  startScreen();
}

void printText(int curX, int curY, int textSize, int textColor, String text)
{
  // helper to print colored text
  tft.setCursor(curX, curY);
  tft.setTextColor(textColor);
  tft.setTextSize(textSize);
  tft.println(text);
}

void startScreen()
{
  // Draw Start Screen
  tft.fillScreen(BLACK);
  printText( 25, 35, 5, WHITE, "IC TESTER");
  printText(25, 90, 1, RED, "Idea by Akshay Baweja & Frank Hellmann");
  printText(25,110, 1, WHITE, "Mod by Miochael Gorbachev GVM-Soft'23");
  printText( 20,220, 2, GREEN,  "Touch anywhere to START");
}

void drawTop(String title)
{
  // Draws Top Text Line and Bar
  title.trim();
  tft.fillRect(0,0,319,18,BLACK);
  tft.drawFastHLine(0,19,319,GRAY);
  //tft.setCursor(160-(title.length()*6), 1);
  tft.setCursor(5, 1);
  tft.setTextColor(CYAN,BLACK);
  tft.setTextSize(1);
  tft.setTextWrap(false);
  tft.println(title);
   tft.setTextSize(2);
}

void autopos(String title)
{
  // Draws Top Text Line and Bar
  title.trim();
  
  tft.setCursor(0, 200);
  tft.setTextColor(YELLOW,BLACK);
  tft.setTextSize(1);
  tft.setTextWrap(false);
  tft.fillRect(0,180,150,100,BLACK);
  if (title!="NO") tft.println(title);
   tft.setTextSize(2);
}

void drawStatus(String text, int col)
{
  // Draws Status Bar below IC
  text.trim();
  int len = text.length();
  if (len > 17) len=17;
  tft.fillRect(120,218,319,239,BLACK);
  tft.setCursor(188-(len*5), 218);
  tft.setTextColor(col,BLACK);
  tft.setTextSize(2);
  tft.setTextWrap(false);
  tft.println(text);
}

void drawIC(int numPins, String number)
{
  // Draws IC outline and pinout description
  const int sx = 166;
  const int sy = 26;
  String num;
  boolean errors = false;
  // Clear
  tft.fillRect(sx-80,sy-6,252,192,BLACK);

  // Draw IC box
  tft.fillCircle(sx+30,sy+1,10,GRAY);
  tft.fillRect(sx+20,sy-10,21,10,BLACK);
  tft.drawRect(sx,sy,60,180,TTGREEN);
  tft.drawRect(sx+1,sy+1,58,90+((numPins-13)*8),WHITE);
  
  for (int i = 0; i < (numPins/2); i++) {
    // draw Pin boxes
    tft.drawRect(sx-15,sy+4+(i*14),14,14,WHITE);
    tft.drawRect(sx+60+1,sy+4+(i*14),14,14,WHITE);

    // Print pin numbers
    if (i<9) {            // Fill up to two chars
      num = " ";
    } else {
      num = "";
    }
    num = num + String(1+i);
    printText(sx-40,sy+4+(i*14),1,GRAY,num);
    num = String(numPins-i);
    printText(sx+60+17,sy+4+(i*14),1,GRAY,num);

    // Print IC pin function names
    if (number!=""){
      // LEFT side
      String pinn = pinname[i];
      int len = pinn.length();
      int col = BLUE;   // Default is blue labels
      if (pinn.indexOf("VCC") > -1 || pinn.indexOf("VEE") > -1 || pinn.indexOf("VDD") > -1 || pinn.indexOf("GND") > -1 ) {
        col = WHITE;    // Powerlines get white labels
      }
      if (pinn.indexOf("NC") > -1 || pinn.indexOf("TST") > -1) {
        col = DRKBLUE;  // Not connected or Test pins get darkblue labels
      }
      printText(sx-52-(len*5),sy+4+(i*14),1,col,pinn);
      
      // RIGHT side
      pinn = pinname[numPins-i-1];
      len = pinn.length();
      col = BLUE;       // Default is blue labels
      if (pinn.indexOf("VCC") > -1 || pinn.indexOf("VEE") > -1 || pinn.indexOf("VDD") > -1 || pinn.indexOf("GND") > -1 ) {
        col = WHITE;    // Powerlines get white labels
      }
      if (pinn.indexOf("NC") > -1 || pinn.indexOf("TST") > -1) {
        col = DRKBLUE;  // Not connected or Test pins get darkblue labels
      }
      printText(sx+128-(len*5),sy+4+(i*14),1,col,pinn);

      // Paint Error Indicator Boxes
      if (errpin[i]) {
        tft.fillRect(sx-14,sy+5+(i*14),14,11,RED);
        errors=true;
      }
      if (errpin[numPins-i-1]) {
        tft.fillRect(sx+62,sy+5+(i*14),14,11,RED);
        errors=true;
      }
    }
  }

  // Print IC Number
  if (number!=""){
    tft.setRotation(2);
    if (errors) {  // BAD IC -> RED Number
      printText(sy+20+((numPins-13)*5),sx-52,2,RED,number);
    } else {
      printText(sy+20+((numPins-13)*5),sx-52,2,WHITE,number);
    }
    tft.setRotation(1);
  }
}

void modeScreen()
{
  // Draw Main Screen
  tft.fillScreen(BLACK);
  drawIC(pinCount,currentIC);
  drawStatus(String(pinCount)+" Pin",TTGREEN);

  buttons[0].initButton(&tft, 45, 105, 80, 50, GRAY, TTGREEN, WHITE, (char *)"Auto", 2);
  buttons[0].drawButton();

  buttons[1].initButton(&tft, 45, 160, 80, 50, GRAY, TTGREEN, WHITE, (char *)"Manual", 2);
  buttons[1].drawButton();

  buttons[2].initButton(&tft, 45,  50, 80, 50, GRAY, TTGREEN, WHITE, (char *)"Pins", 2);
  buttons[2].drawButton();

  // If IC was selected before show Retest button
  if (currentIC!=""){
    buttons[3].initButton(&tft, 45, 215, 80, 50, GRAY, TTGREEN, WHITE, (char *)"Retest", 2);
    buttons[3].drawButton();
  }
}


String getIC()
{
  // Draws Numpad and gets IC Number
  String number;
  int i = 0;
  boolean status = false;
  tft.fillScreen(BLACK);
  drawTop("Enter IC Number");
  // create buttons
  for (uint8_t row = 0; row < 2; row++) {
    for (uint8_t col = 0; col < 5; col++) {
      buttons[col + row * 5].initButton(&tft, BUTTON_X + col * (BUTTON_W + BUTTON_SPACING_X),
                                        BUTTON_Y + row * (BUTTON_H + BUTTON_SPACING_Y), // x, y, w, h, outline, fill, text
                                        BUTTON_W, BUTTON_H, WHITE, TTGREEN, WHITE,
                                        buttonlabels[col + row * 5], BUTTON_TEXTSIZE);
      buttons[col + row * 5].drawButton();
    }
  }
  buttons[10].initButton(&tft, 90, BUTTON_Y + 2 * (BUTTON_H + BUTTON_SPACING_Y), 140, BUTTON_H, WHITE, GREEN, WHITE, (char *)"OK", BUTTON_TEXTSIZE);
  buttons[10].drawButton();

  buttons[11].initButton(&tft, 238, BUTTON_Y + 2 * (BUTTON_H + BUTTON_SPACING_Y), 140, BUTTON_H, WHITE, RED, WHITE, (char *)"Clear", BUTTON_TEXTSIZE);
  buttons[11].drawButton();
  // create 'text field'
  tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, WHITE);

  while (!status)
  {
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
    }

    // go thru all the buttons, checking if they were pressed
    for (uint8_t b = 0; b < 12; b++)
    {
      if (buttons[b].contains(p.x - 8, p.y - 8))
      {
        //Serial.print("Pressing: "); Serial.println(b);
        buttons[b].press(true);  // tell the button it is pressed
      }
      else
      {
        buttons[b].press(false);  // tell the button it is NOT pressed
      }
    }

    // now we can ask the buttons if their state has changed
    for (uint8_t b = 0; b < 12; b++)
    {
      if (buttons[b].justReleased())
      {
        //Serial.print("Released: "); Serial.println(b);
        buttons[b].drawButton();  // draw normal
      }

      if (buttons[b].justPressed())
      {
        buttons[b].drawButton(true);  // draw invert!

        // if a numberpad button, append the relevant # to the textfield
        if (b < 10) {
          if (i < TEXT_LEN)
          {
            number += buttonlabels[b][0];
            i++;
          }
        }

        // clr button! delete char
        if (b == 11) {
          tft.setCursor(TEXT_X + 8, TEXT_Y + 8);
          tft.setTextColor(BLACK);
          tft.setTextSize(TEXT_TSIZE);
          tft.print(number);
          number = "";
          i = 0;
        }

        // update the current text field
        //Serial.println(textfield);
        tft.setCursor(TEXT_X + 8, TEXT_Y + 8);
        tft.setTextColor(TEXT_TCOLOR, BLACK);
        tft.setTextSize(TEXT_TSIZE);
        tft.print(number);

        // we dont really check that the text field makes sense
        // just try to call
        if (b == 10) {
          Serial.print("IC no: "); Serial.println(number);
          status = true;
        }
        delay(100); // UI debouncing
      }
    }
  }
  return number;
}

void getTouch()
{
  // Wait for touchscreen press
  boolean status = false;
  while (1)
  {
    TSPoint q = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if (q.z > MINPRESSURE && q.z < MAXPRESSURE)
    {
      status = true;
      break;
    }
    delay(10);
  }

  pinMode(XM, OUTPUT);
  digitalWrite(XM, LOW);
  pinMode(YP, OUTPUT);
  digitalWrite(YP, HIGH);
}

int getTouchLR()
{
  // Wait for Touchscreen press and return xposition
  TSPoint p;
  boolean status = false;

  while (1)
  {
    p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
      // scale from 0->1023 to tft.width
      p.x = map(p.x, TS_MINX, TS_MAXX, tft.height(), 0);
      p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());

      //Swapping for Set Rotation 3
      p.x = p.x + p.y; p.y = p.x - p.y; p.x = p.x - p.y;
      status = true;
      break;
    }
    delay(10);
  }

  pinMode(XM, OUTPUT);
  digitalWrite(XM, LOW);
  pinMode(YP, OUTPUT);
  digitalWrite(YP, HIGH);

  Serial.println("Position X: "+String(p.x));
  return p.x;  
}
