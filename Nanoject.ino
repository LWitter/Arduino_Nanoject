// Trial to make a directory structure and interface with keys
#include <LiquidCrystal.h>
#include <Wire.h>
#include <EEPROM.h>
/*************************************************************
The nanoject needs some form of menu stucture:

*************************************************************/
// Select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// Define our special characters for the rotating circle
byte p20[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
};
byte p40[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
};
byte p60[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
};
byte p80[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
};
byte p100[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
byte downArrow[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};

byte upArrow[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};

byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};
// Set up the menu:
String menuItems[] = {"Set Volume", "Set Time", "Inject!", "Manual Move", "Calibrate","Position"};//, "Move UP", "Move DOWN"};
// Define some variables that we need:
// Navigation button variables
int readKey;
int savedDistance = 0;
// Menu control variables
int menuPage = 0;
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + .5);
int cursorPosition = 0;
// Characters
int wheel = 0;

// For injections:
int Volume;
int Time;
int VolumeIncr;
int TimeIncr;
int MotorPosition = 0;      // 0 is fully retracted, we have a limited number of steps in total.
int MaxMotorPosition = 3000;  // We don't want to jam the nanoject, so a maximal position is wise.
// For calibrations:
float Diameter = 0;
int CalibrationSteps[] = {1, 5, 10, 25, 50, 100, 250, 500};
float CalibrationDiameters[] = {0, 0, 0, 0, 0, 0, 0, 0};
float CalibrationVolumes[] = {0, 0, 0, 0, 0, 0, 0, 0};
int NrCalibrationSteps = 6;
float sum_xy = 0;
float sum_xx = 0;
float avgVolumes = 0;
float Rres = 0;
float Rtot = 0;
float Rsquare = 0;
float CalibrationSlope;
int BinaryCharacterArray[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int BinaryDecimalArray[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int BinaryArray[8];


int choice = 0;    // Should be zero, because default zero
// For the motor:
int pwm_a = 3;  //PWM control for motor outputs 1 and 2 
int pwm_b = 11;  //PWM control for motor outputs 3 and 4 
int dir_a = 2;  //direction control for motor outputs 1 and 2 
int dir_b = 12;  //direction control for motor outputs 3 and 4
int pwm_a_val;
int pwm_b_val;
int dir_a_val;
int dir_b_val;
int revs = 0;
int wait = 50;

void setup()
{
Serial.begin(9600);

 // First we set some pins and parameters for the stepper motor:
 pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
 pinMode(pwm_b, OUTPUT);
 pinMode(dir_a, OUTPUT);
 pinMode(dir_b, OUTPUT);
 pwm_a_val = digitalRead(pwm_a);
 pwm_b_val = digitalRead(pwm_b);
 dir_a_val = digitalRead(dir_a);
 dir_b_val = digitalRead(dir_b);
 // Starting the LCD:
 lcd.begin(16, 2);              // start the lcd library
 // Special Characters:
 lcd.createChar(0, menuCursor);
 lcd.createChar(1, upArrow);
 lcd.createChar(2, downArrow);
 lcd.createChar(3, p20);
 lcd.createChar(4, p40);
 lcd.createChar(5, p60);
 lcd.createChar(6, p80);
 lcd.createChar(7, p100);
 
 // Following values are for the injection, they are saved in EEPRMOM!
// EERPROM can hold values up to 255, so we need a trick:
int EEPROMVolume = EEPROM.read(0);
int EEPROMTime = EEPROM.read(1);
// Get the correct volume and time:
 if (EEPROMVolume<=10) Volume = EEPROMVolume*5;
 else if (EEPROMVolume<=30) Volume = EEPROMVolume*10-50;
 else Volume = EEPROMVolume*50-1250;
 if (Volume>1000) Volume = 1000;
  
 if (EEPROMTime<=10) Time = EEPROMTime*5;
 else if (EEPROMTime<=30) Time = EEPROMTime*10-50;
 else Time = EEPROMTime*50-1250;
 if (Time>1200) Time = 1200;
 
 // Get the calibration from memory:
  CalibrationSlope = Load();
  Serial.println(CalibrationSlope);

  // Welcome message
 lcd.setCursor(3,0);
 lcd.print("WELCOME TO");
 lcd.setCursor(0,1);
 lcd.print("ARDUINO NANOJECT");
 delay(1000);
 lcd.clear();   
};

void loop()
{
  mainMenuDraw();
  drawCursor();
  operateMainMenu();
};

