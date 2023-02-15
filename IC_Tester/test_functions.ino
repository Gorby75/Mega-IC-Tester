boolean testCase(String test, int pins)
{
  // Helper function to perform IC checks
  boolean result = true;
  int clockPins[24];
  int clocks = 0;

  // Load the hardware pin definition into pin
  switch (pins)
  {
    case 14 : pin = pin14;
      break;
    case 16 : pin = pin16;
      break;
    case 18 : pin = pin18;
      break;
    case 20 : pin = pin20;
      break;
    case 24 : pin = pin24;
      break;
  }
  
  Serial.println("SignalIn : " + test);
  Serial.print("Response : ");

  // Setting up Vcc, GND and input pins
  for (int i = 0; i < pins; i++)
  {
    switch (test[i])
    {
      case 'V' : pinMode(pin[i], OUTPUT); digitalWrite(pin[i], HIGH);
        break;
      case 'G' : pinMode(pin[i], OUTPUT); digitalWrite(pin[i], LOW);
        break;
      case 'L' : digitalWrite(pin[i], LOW); pinMode(pin[i], INPUT_PULLUP);
        break;
      case 'H' : digitalWrite(pin[i], LOW); pinMode(pin[i], INPUT_PULLUP);
        break;
      case 'Z' : digitalWrite(pin[i], LOW); pinMode(pin[i], INPUT_PULLUP);
        break;
    }
  }

  delay(5);

  // Setting output signals
  for (int i = 0; i < pins; i++)
  {
    switch (test[i])
    {
      case 'X' :
      case '0' : pinMode(pin[i], OUTPUT); digitalWrite(pin[i], LOW);
        break;
      case '1' : pinMode(pin[i], OUTPUT); digitalWrite(pin[i], HIGH);
        break;
      case 'C' : clockPins[clocks++] = pin[i]; pinMode(pin[i], OUTPUT); digitalWrite(pin[i], LOW);
        break;
    }
  }

  // Setting up clock pins
  if (clocks)
  {
    for (int i = 0; i < clocks; i++) {
      //Clock Trigger
      pinMode(clockPins[i], INPUT_PULLUP);
      delay(10);
      pinMode(clockPins[i], OUTPUT);
      digitalWrite(clockPins[i], LOW);
    }
  }

  delay(5);

  // Reading and testing outputs
  for (int i = 0; i < pins; i++)
  {
    switch (test[i])
    {
      case 'H' : if (!digitalRead(pin[i])) {
          // Not good! Error! 
          result = false;
          Serial.print('L');
          errpin[i]=true;
        }
        else Serial.print(' ');
        break;
      case 'Z' : if (!digitalRead(pin[i])) {
          // Not good! Error!
          // TODO: Pulldown Pullup checks of High-Z outputs 
          result = false;
          Serial.print('L');
          errpin[i]=true;
        }
        else Serial.print(' ');
        break;
      case 'L' : if (digitalRead(pin[i])) {
          // Not good! Error! 
          result = false;
          Serial.print('H');
          errpin[i]=true;
        }
        else Serial.print(' ');
        break;
      default : Serial.print(' ');
    }
  }
  Serial.println(";");
  if (result == false) {
    Serial.println("Case Result : Error detected!");
  } else {
    Serial.println("Case Result : Test OK!");
  }
  return result;
}


void autoSearch(int pins)
{
  // Auto Search Functions. Checks for matching 
  // IC definitions with the right pin count.
  File dataFile = SD.open(fname);

  String newCase;
  String rus;
  IC newIC;
  int ICpins;
  boolean result;
  IC ans[24];
  unsigned long filepos;
  unsigned long anspos[24];

  if (dataFile)
  {
    int count = 0;
    int i;
    for (i=0;i<24;i++) errpin[i] = false;
    i=0;

    drawStatus("Testing",YELLOW);
    
    while (dataFile.available())
    {
      dataFile.readStringUntil('$');
      // Read IC name and description
      newIC.num = dataFile.readStringUntil('\n');
      newIC.name = dataFile.readStringUntil('\n');
      newIC.rus = dataFile.readStringUntil('\n');
      ICpins = dataFile.readStringUntil('\n').toInt();
      filepos=dataFile.position();

      // If the pin count matches do tests
      if (ICpins == pins)
      {
        result = true;
        for (i=0;i<24;i++) errpin[i] = false;
        drawStatus("Testing",TTGREEN);
        Serial.println("Testing Now");

        // read pin function description in
        i=0;
        while (dataFile.peek() != '#' && i<ICpins)
        {
          pinname[i]=dataFile.readStringUntil('\n');
          i++;
        }
        newCase=dataFile.readStringUntil('\n'); // Dummy Read for #

        // read test cases in and test them
        while (dataFile.peek() != '$')
        {
          newCase = dataFile.readStringUntil('\n');
          newCase.trim();
          if (newCase == "NOVECTORS" || testCase(newCase, ICpins) == false)
          {
              result = false;
              break;
          }
        }

        // check for matches and store answers
        if (result == true)
        {
          Serial.println("\nMatch Found !!\n");
          ans[count] = newIC;
          anspos[count++] = filepos;
          Serial.println(newIC.num+"::"+newIC.name+"::"+filepos);
        }

        drawStatus("Testing",YELLOW);
        Serial.println("Test Completed");
      }
    }
    
    // Show Results
    if (count == 0)
    {
      Serial.println("No Match Found");
      for (i=0;i<24;i++) errpin[i] = false;
      drawTop("IC Tester");
      drawStatus("Testing",BLACK);
      drawStatus("No Match Found",MAGENTA);
    }
    else
    {
      Serial.println("\nCount : " + String(count));
      for (i=0;i<24;i++) errpin[i] = false;
      if (count == 1) {
        drawStatus("1 Match Found",GREEN);
      } else {
        drawStatus(String(count) + " Matches Found",GREEN);
      }
      delay(500);
      int maxcount=count;
      // Display all matches
      while (count--)
      {
        Serial.println(String(count) + ". " + ans[count].num + " : " + ans[count].name);
        // read pin function description in
        dataFile.seek(anspos[count]);
        i=0;
        while (dataFile.peek() != '#' && i<pinCount)
        {
          pinname[i]=dataFile.readStringUntil('\n');
          i++;
        }
        currentIC=ans[count].num;
        drawIC(pinCount,ans[count].num);
        drawTop(ans[count].name);
        autopos(ans[count].rus);
        if (count>0) {
          drawStatus("<--  "+String(maxcount-count)+"/"+String(maxcount)+"  -->",GREEN);
        } else {
          drawStatus("<--  "+String(maxcount-count)+"/"+String(maxcount)+"  END",GREEN);
        }
        if ( getTouchLR() < TS_LR && count < maxcount ){ 
          if (count < maxcount -1) { 
            count=count+2;
          } else {
            count++;
          }
        }
      }
      currentIC=ans[0].num;
      drawTop(ans[0].name);
      drawStatus("",GREEN);
    }
    dataFile.close();
  }
  else
  {
    // Error while reading IC data
    Serial.print("error opening "); Serial.println(fname);
    dataFile.close();

    tft.fillScreen(BLACK);
    tft.setCursor(5, 140);
    tft.setTextColor(RED);  tft.setTextSize(2);
    tft.print("ERROR:");
    tft.setTextColor(MAGENTA);
    tft.println("SD CARD");

    tft.setCursor(40, 220);
    tft.setTextColor(RED);  tft.setTextSize(2);
    tft.println("Touch for MAIN MENU");

    getTouch();
    resetFunc();
  }
}

void manualSearch(String number)
{
  // Manual Search function
  // takes given IC number and checks for match
  File dataFile = SD.open(fname);

  String buffer;
  String newCase;
  String rus;
  IC newIC;
  int pins, send_status = -1;
  boolean result = true;
  int i;
  for (i=0;i<24;i++) errpin[i] = false;
  
  // if the file is available, read from it:
  if (dataFile)
  {
    drawStatus("Testing",YELLOW);
    while (dataFile.available())
    {
      result = true;
      dataFile.readStringUntil('$');

      // Read IC name/number
      buffer = dataFile.readStringUntil('\n');
      buffer.trim();
      number.trim();

      // Check if we have a match
      if (number == buffer)
      {
        newIC.num = buffer;
        newIC.name = dataFile.readStringUntil('\n');
        rus = dataFile.readStringUntil('\n');
        pins = dataFile.readStringUntil('\n').toInt();
        pinCount=pins;
        
        // read pin function description in
        i=0;
        while (dataFile.peek() != '#' && i<pins)
        {
          pinname[i]=dataFile.readStringUntil('\n');
          i++;
        }
        newCase=dataFile.readStringUntil('\n'); // Dummy Read for # line

        // read test cases in and test them
        while (dataFile.peek() != '$')
        {
          newCase = dataFile.readStringUntil('\n');
          newCase.trim();
          if (newCase == "NOVECTORS") {
            send_status = 2;
            break;
          }
          if (testCase(newCase, pins) == false)
          {
            result = false;
          }
          delay(5);
        }

        if (send_status == -1 ) {
          if (result == true)
          {
            //Serial.println("IC is GOOD");
            send_status = 1;
          }
          else
          {
            //Serial.println("IC is BAD");
            send_status = 0;
          }
          break;
        }
      }
    }
    dataFile.close();
    
    // Did we find a match?
    if (send_status == -1)
    {
      // No file entry matches given IC number
     
      currentIC="";
      drawIC(pinCount,"");
      drawTop("IC Tester");
      drawStatus(number+" not found",MAGENTA);

    } else if (send_status == 2) { 
      // We found a match but no test vectors
     
      currentIC=newIC.num;
      drawIC(pinCount,newIC.num);
      drawTop(newIC.name);
      drawStatus(newIC.num+" has no tests",MAGENTA);
      
    } else if (send_status == 0) {
      // We found one match, but IC is bad      
      currentIC=newIC.num;
      drawIC(pinCount,newIC.num);
      drawTop(newIC.name);
      drawStatus(newIC.num+" is BAD",RED);
      
    } else { 
      // We found a match and IC is good!
      currentIC=newIC.num;
      drawIC(pinCount,newIC.num);
      drawTop(newIC.name);
      if (rus!="NO") drawStatus("RUS "+rus,GREEN);
      else
      {
      drawStatus(newIC.num+" is GOOD",GREEN);
      }
    } 
  }
  else
  {
    // Error reading in IC data from SD
    Serial.print("error opening "); Serial.println(fname);
    dataFile.close();
    
    tft.fillScreen(BLACK);
    tft.setCursor(5, 140);
    tft.setTextColor(RED);  tft.setTextSize(2);
    tft.print("ERROR:");
    tft.setTextColor(MAGENTA);
    tft.println("SD CARD");

    tft.setCursor(40, 220);
    tft.setTextColor(RED);  tft.setTextSize(2);
    tft.println("Touch for MAIN MENU");

    getTouch();
    resetFunc();
  }
}
