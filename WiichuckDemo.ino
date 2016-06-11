/*
 * WiiChuckDemo -- 
 *
 * 2008 Tod E. Kurt, http://thingm.com/
 *
 */

#include <Wire.h>
#include <math.h>
#include "nunchuck_funcs.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

int loop_cnt=0;

byte accx,accy,zbut,cbut;
#define LPIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, LPIN, NEO_GRB + NEO_KHZ800);


void setup()
{
    Serial.begin(19200);
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
    
    Serial.print("WiiChuckDemo ready\n");
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void setAllPix(uint32_t c){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint32_t joyPositionToColor(){
  //x and y sit at 128 and have min ~30, max ~225
  int x = nunchuck_joyx();
  int y = nunchuck_joyy();

  double adjustedx = x-128;
  double adjustedy = y-128;
  //these range from approx -100 to +100

  //convert to polar, don't care about radius, just theta
  double thetaRadians = atan2(adjustedy, adjustedx) + 3.14159; //tan^-1(y/x)
  double thetaDegrees = thetaRadians * (180/3.1415);
  double scaledDegrees = (255.0/360.0) * thetaDegrees;
  byte wheelpos = (byte) scaledDegrees;

  Serial.print("scaledDegrees: ");
  Serial.print(scaledDegrees);
  Serial.print("   wheelpos: ");
  Serial.print(wheelpos);
  Serial.print("\n");
  return Wheel(wheelpos);

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

        accx  = nunchuck_accelx(); // ranges from approx 70 - 182
        accy  = nunchuck_accely(); // ranges from approx 65 - 173
        zbut = nunchuck_zbutton();
        cbut = nunchuck_cbutton(); 
            
        /*Serial.print("accx: "); Serial.print((byte)accx,DEC);
        Serial.print("\taccy: "); Serial.print((byte)accy,DEC);
        Serial.print("\tzbut: "); Serial.print((byte)zbut,DEC);
        Serial.print("\tcbut: "); Serial.println((byte)cbut,DEC);*/
        //nunchuck_print_data();
        uint32_t color = joyPositionToColor();
        setAllPix(color);
    
    }
    loop_cnt++;
    delay(1);
}

