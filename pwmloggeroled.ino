#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include "font.h";
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int ledPin =  LED_BUILTIN;// the number of the LED pin
int rawcurrentin = 0;
float iin = 0;
int offset = 2490;
int mvperamp = 66;
float watthour =0;
float wattsec=0;

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated


void setup()
{ if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  
  
  
  
  
  
  TCCR0A = TCCR0A | 0x30;
 TCCR0B = TCCR0B & 0xF8 | 0x01;
TCCR2B = TCCR2B & 0xF8 | 0x01;
 pinMode(ledPin, OUTPUT);
 analogWrite(5, 117);
analogWrite(6, 137);
     Serial.begin(9600);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Vi:"));
  display.setCursor(0,10);             // Start at top-left corner
  display.println(F("Ai:"));
  display.setCursor(0,20);             // Start at top-left corner
  display.println(F("pw:"));
display.setCursor(0,30);             // Start at top-left corner
  display.println(F("Wh:"));
  display.display();
 // delay(2000); 
  
  
  
  
}
char string[8];
char strin[8];
int del;
const int setPoint = 1450;
 int measurement = 0;
 int pulseWidth = 0;
 int difference = 0;
 int stepSize = 0;
 long interval = 0;   
void loop()
{
  float voltagein = ((analogRead (A0)*5.0*120400.0)/(21700.0*1024 ))-0.5;
  float ampsin = 0;
  for (int i = 0; i < 1000; i++) {
    ampsin = ampsin + analogRead(A5);
  }
  rawcurrentin = ampsin / 1000;
  //   rawvoltagein=analogRead(voltagein);
  //   rawcurrentout=analogRead(currentout);
  //  rawvoltageout=analogRead(voltageout);
  iin = ((rawcurrentin / 1024.0 * 5.0 * 1000) - offset) / mvperamp;
  //measurement = analogRead (A1);
/*pulseWidth = map(measurement, 0, 1023, 0, 253);
 analogWrite(3, pulseWidth);*/

  
measurement = voltagein*100;
 difference = abs(setPoint - measurement);
 stepSize = difference;
 if (measurement < setPoint)
 {
 pulseWidth += stepSize;
 if (pulseWidth > 255) pulseWidth = 255;
 }
 if (measurement > setPoint)
 {
 pulseWidth -= stepSize;
 if (pulseWidth < 0) pulseWidth = 0;
 }
  //Serial.println(differenceM);
analogWrite(3, pulseWidth);
// constants won't change:
        // interval at which to blink (milliseconds)
//interval =50000/((pulseWidth/25)*2);
interval =64000;
unsigned long currentMillis = millis();
//Serial.println(currentMillis);
  //Serial.println(previousMillis);

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    float power=voltagein*iin;
    
    wattsec=wattsec+power;
    watthour=wattsec/3600.00;
    Serial.println(wattsec);
  }
 // Serial.println(interval);

 

//LcdXY(16,0);
float pulswi=pulseWidth*1.0;
//char vdis = dtostrf(voltagein,5,2,string);



display.setCursor(20,0);             // Start at top-left corner
  display.println(voltagein);
  display.setCursor(20,10);             // Start at top-left corner
  display.println(iin);
  display.setCursor(20,20);             // Start at top-left corner
  display.println(pulswi);
display.setCursor(20,30);             // Start at top-left corner
  display.println(watthour);
  display.display();
/*  LcdWriteString(dtostrf(voltagein,5,2,string));
  LcdXY(16,1);
  LcdWriteString(dtostrf(iin,5,2,string));
  LcdXY(16,2);
LcdWriteString(dtostrf(pulswi,5,2,string));
LcdXY(16,3);
LcdWriteString(dtostrf(watthour,5,2,string));*/
}
