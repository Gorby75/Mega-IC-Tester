IC Tester for the classic CMOS and TLL ICs with Touch Display

based code on Frank Hellmann, 2019 http://www.gotohellmann.com/ & original idea Akshay Bawejas http://www.akshaybaweja.com/ https://github.com/akshaybaweja/Smart-IC-Tester

Added tests and pinouts for 18,20 & 24 Pin ICs. The base contains 422 ICs. Russian analogues have been added to the database.

This project needs the Arduino IDE 1.8.10+

a Arduino Mega 2560 Module

an ILI9341 240x320 Pixel RGB TFT LCD Touch Display Shield

minimus 1GB Micro-SD Card that fits the Display Shield

a 24pin ZIF Socket



Setting Up and Programming

Format the SDCard as FAT/FAT32 Filesystem. Copy the database (database.txt) into the root folder of the SDCard. Plug the SDCard into the TFT shield and connect the Arduino Mega to the computer. Start the Arduino IDE. Check you have the following librarys installed (Tools -> Library Manager): MCUFRIEND_kbv (try the Adafruit_ILI9341 if the display doesn't work). Adafruit GFX Library (included the Bus Library as well). Adafruit Touchscreen. SD_patched (download from above, includes Arduino Mega Software SPI fix. Select the proper Arduino Mega 2560 under Tools -> Board -> Arduino AVR Boards. Select the correct COM port for upload. Compile and Upload the sketch. Enjoy

Sorry !!! Some elements may be incorrectly defined. I didn't have a certain number of items available to check. I took them from someone else's database. Write me what needs to be corrected or added and I will do it!

![T_chema](https://user-images.githubusercontent.com/125442802/219011874-526468a1-553a-444b-a92e-665b739dd98b.jpg)
![T_sh1](https://user-images.githubusercontent.com/125442802/219011880-74a5c129-1eb8-43b3-9f7b-212fb89f0321.JPG)
![T_sh2](https://user-images.githubusercontent.com/125442802/219011882-c092854d-4b8a-4402-bf63-411809dc5b17.JPG)
![T_sh3](https://user-images.githubusercontent.com/125442802/219011884-bd9033ed-fc09-4ab6-953f-bd8f57de2d39.JPG)
![T_sh4](https://user-images.githubusercontent.com/125442802/219011887-5ca2a110-48df-4b09-9f03-b7df83441ad0.JPG)
![T_sh5](https://user-images.githubusercontent.com/125442802/219011889-1a32831e-5ada-418a-a883-0b24558597f3.JPG)
