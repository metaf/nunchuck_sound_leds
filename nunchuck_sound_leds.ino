#include <FastLED.h>
#include <Wire.h>
#include <math.h>
#include "nunchuck_funcs.h"
#define LED_DATA_PIN 6
#define NUM_LEDS 8

CRGB strip[NUM_LEDS]; //some memory for LED state.
int loop_cnt=0;


void setup()
{
    Serial.begin(19200);

    //Setup the nunchuck
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake

    //Setup the LEDs
    FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(strip, NUM_LEDS);
    
}

void setAllPix(CRGB c){
  for(uint16_t i=0; i<NUM_LEDS; i++) {
    strip[i] = c;
  }//would something like memset be better?  a CRGB isn't just a byte though.
  FastLED.show();
  
}

//// Input a value 0 to 255 to get a color value.
//// The colours are a transition r - g - b - back to r.
//uint32_t Wheel(byte WheelPos) {
//  WheelPos = 255 - WheelPos;
//  if(WheelPos < 85) {
//    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//  }
//  if(WheelPos < 170) {
//    WheelPos -= 85;
//    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//  }
//  WheelPos -= 170;
//  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//}

CRGB nunchuckAsColorPicker(){
  //x and y sit at 128 and have min ~30, max ~225
  int x = nunchuck_joyx();
  int y = nunchuck_joyy();

  double adjustedx = x-128;
  double adjustedy = y-128;
  //these range from approx -100 to +100

  //convert to polar, get theta
  double thetaRadians = atan2(adjustedy, adjustedx) + 3.14159; //tan^-1(y/x)
  double thetaDegrees = thetaRadians * (180/3.1415);
  double scaledDegrees = (255.0/360.0) * thetaDegrees;
  //working in such a memory constrained environment, it could make sense to declare fewer
  //doubles here and work inplace?  I'm not pressed for memory yet though.
  byte wheelpos = (byte) scaledDegrees;

  //convert to polar, get radius
  double r = sqrt( pow(adjustedx,2) + pow(adjustedy,2));
  byte rad = map((long)r, 0, 100, 0, 255);
  
  
  //accely center 128, min 65, max 180
  //byte accely = map(nunchuck_accely(), 65, 180, 0, 255);
  //Serial.println(accely);

  
  byte h = wheelpos;
  byte s;
  byte v;

  //this if statement makes the middle of the joystick white instead of black.
  if (r < 30){
    s=0;
    v=(r*2) + 50;
  }
  else {
    s=255;
    v=rad;
  }
  if (nunchuck_zbutton() != 1){
    v = 0;
  }
  return CHSV(h,s,v);
  

  
  return CHSV(wheelpos,255,r);

  /***
   * 
   * Going forward lets' use HSL to RGB where theta is H and r is L (and S is max)
   * https://github.com/ratkins/RGBConverter/blob/master/RGBConverter.h
   */
  
}

void loop()
{
    if( loop_cnt > 100 ) { // every 100 msecs get new data
        loop_cnt = 0;
        nunchuck_get_data();
        CRGB color = nunchuckAsColorPicker();
        setAllPix(color);
    
    }
    loop_cnt++;
    delay(1);
}

