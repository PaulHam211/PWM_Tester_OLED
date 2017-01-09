*/***********************************************************
**  _____      ____  __   _____ ___ ___ _____ ___ ___    ___  _    ___ ___  
*  | _ \ \    / /  \/  | |_   _| __/ __|_   _| __| _ \  / _ \| |  | __|   \ 
*  |  _/\ \/\/ /| |\/| |   | | | _|\__ \ | | | _||   / | (_) | |__| _|| |) |
*  |_|   \_/\_/ |_|  |_|   |_| |___|___/ |_| |___|_|_\  \___/|____|___|___/ 
*                                                                                                                                                     
* PWM Servo Cable Connections:
* Red wire attached to +5V
* GND wire attached to ground
* Yellow/White wire attached to digital pin 3
*
* OLED Connections:
* VCC +5v
* GND GND
* SDA Analog pin 4
* SCL Analog pin 5
*
* Adapted from the Arduino Tape Measure with OLED Display code By Chris Rouse Nov 2015
*
*********************************************************/

#include "U8glib.h"
#include <SPI.h>
#include <Wire.h>
// setup u8g object
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); // I2C

String longString="";

#define CH1_PIN  3        // Digital IO pin connected to the PWM signal.
float RAWPWM_CH1;
int RAWPWMint;
int PWM_CH1;
int PWM;
int PWM2;

int MinPWM = 1000;
int MaxPWM = 2000;

void setup() 
{
//Serial.begin(9600);
pinMode(CH1_PIN, INPUT);
Wire.begin();
}

void loop() 
{
PWMRead();

u8g.firstPage();
do {
draw();
} while( u8g.nextPage() );
delay(50);
}

void PWMRead()
{
  RAWPWM_CH1 = pulseIn(CH1_PIN, HIGH);

  if(RAWPWM_CH1 < MinPWM || MinPWM < 600)
  {
  MinPWM = min(RAWPWM_CH1, 1000);
  }
  
  if(RAWPWM_CH1 > MaxPWM || MaxPWM > 3000)
  {
  MaxPWM = max(RAWPWM_CH1, 2000);
  }
  
  PWM_CH1 = map(RAWPWM_CH1,MinPWM, MaxPWM, 1000, 2000);

  PWM = map(PWM_CH1,1000, 2000, 0, 100);
  PWM2 = map(PWM_CH1,1000, 2000, -50, 50);

  RAWPWMint = RAWPWM_CH1;
  
 // For debugging PWM Values
  Serial.print("MinPWM - ");
  Serial.print(MinPWM);
  Serial.print(" MaxPWM - ");
  Serial.print(MaxPWM);

  Serial.print(" RAWPWMint - ");
  Serial.print(RAWPWMint);
  
  Serial.print(" RAWPWM_CH1 - ");
  Serial.print(RAWPWM_CH1);
  Serial.print(" PWM - ");
  Serial.print(PWM_CH1);
  Serial.print(" PWM2 - ");
  Serial.println(PWM);
}

void draw(void){
u8g.setFont(u8g_font_profont15);

u8g.drawStr(30,15, "PWM Tester");

if(RAWPWMint > 2500 || RAWPWMint < 600){
longString = "---";
}
else{
longString=String(RAWPWMint); // large letters
}

const char* newData = (const char*) longString.c_str();

// Draw Values
u8g.setFont(u8g_font_profont22);
u8g.drawStr(40,40, newData);

u8g.drawHLine(15,55,100);

// Scale (10 lines)
for(int f = 15;f <120; f = f+10){
u8g.drawLine(f,50,f,59);
}
// Slider -50-0-50%
for(int f = 64; f< PWM2+66; f++){
u8g.drawBox(f,51,1,8);
}
for(int f = 64; f> PWM2+64; f--){
u8g.drawBox(f,51,1,8);
}
}
