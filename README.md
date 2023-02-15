IC Tester for the classic CMOS and TLL ICs with Touch Display

based code on Frank Hellmann, 2019 http://www.gotohellmann.com/ & original idea Akshay Bawejas http://www.akshaybaweja.com/ https://github.com/akshaybaweja/Smart-IC-Tester

Added tests and pinouts for 18,20 & 24 Pin ICs. The base contains 422 ICs. Russian analogues have been added to the database.

This project needs the Arduino IDE 1.8.10+
a Arduino Mega 2560 Module
an ILI9341 240x320 Pixel RGB TFT LCD Touch Display Shield
minimus 1GB Micro-SD Card that fits the Display Shield
a 24pin ZIF Socket
Setting Up and Programming
Format the SDCard as FAT/FAT32 Filesystem
Copy the database (database.txt) into the root folder of the SDCard
Plug the SDCard into the TFT shield and connect the Arduino Mega to the computer
Start the Arduino IDE
Check you have the following librarys installed (Tools -> Library Manager):
MCUFRIEND_kbv (try the Adafruit_ILI9341 if the display doesn't work)
Adafruit GFX Library (included the Bus Library as well)
Adafruit Touchscreen
SD_patched (download from above, includes Arduino Mega Software SPI fix
Select the proper Arduino Mega 2560 under Tools -> Board -> Arduino AVR Boards
Select the correct COM port for upload
Compile and Upload the sketch
Enjoy
Sorry !!! Some elements may be incorrectly defined. I didn't have a certain number of items available to check. I took them from someone else's database. Write me what needs to be corrected or added and I will do it![IMG_20230215_110331](https://user-images.githubusercontent.com/125442802/218992424-ceb1f64f-07d8-49f0-8b6a-7ff79fe6c02c.jpg)
![IMG_20230215_110453](https://user-images.githubusercontent.com/125442802/218992539-fc5e0c3b-e121-41ba-be4e-5aa2780b78f2.jpg)
![IMG_20230215_110504](https://user-images.githubusercontent.com/125442802/218992560-fb521b8e-3ba6-456c-8207-80a8bde3cc70.jpg)
![IMG_20230214_171314](https://user-images.githubusercontent.com/125442802/218992592-ac4c552b-92b6-413c-94f2-3022ff81f729.jpg)
![IMG_20230215_110015](https://user-images.githubusercontent.com/125442802/218992615-31f2f07c-8dc0-4256-814f-a39f6b36293a.jpg)
