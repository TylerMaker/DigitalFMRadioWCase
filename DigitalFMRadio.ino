/*         Arduino Digital FM Radio with build in speakers
  libraries & schematic: www.ardumotive.com
       dev: Michalis Vasilakis // Date 24/2/2015 // Version 1.0        
              Many thanks to nicuflorica.blogspot.ro                       */

//Libraries:
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <TEA5767.h>
#include <Wire.h>
#include <Button.h>

//Constants:
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3 ); //Pinout:(SCLK, DIN, DC, CS, RST)
TEA5767 Radio; //Pinout SLC and SDA - Arduino pins A5 and A4
Button btn_forward(11, HIGH); //Search station up button
Button btn_backward(12, HIGH);//Search station down button

//Variables:
double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;
unsigned long last_pressed;
unsigned char buf[5];
int stereo;
int signal_level;
double current_freq;
unsigned long current_millis = millis();

void setup () {
  //Init
  Wire.begin();
  Radio.init();
  Radio.set_frequency(95.2); //On power on go to station 95.2

  Serial.begin(250000);

}

void loop () {
  Radio.set_frequency(95.2); //On power on go to station 95.2

  LogNow("loop : 0");

  if (Radio.read_status(buf) == 1) {
    LogNow("loop : 1");

    current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
    stereo = Radio.stereo(buf);
    signal_level = Radio.signal_level(buf);
      }

  //When button pressed, search for new station
  if (search_mode == 1) {
        LogNow("loop : 2");

      if (Radio.process_search (buf, search_direction) == 1) {
        LogNow("loop : 3");

          search_mode = 0;
      }
  }
  //If forward button is pressed, go up to next station
  if (btn_forward.isPressed()) {
    LogNow("loop : 4");

    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
    delay(1000);
  }
  //If backward button is pressed, go down to next station
  if (btn_backward.isPressed()) {
    LogNow("loop : 5");
    
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_DOWN;
    Radio.search_down(buf);
    delay(1000);
  } 
   delay(100);

} 

void LogNow (String msg) {
    // Inform the user
    Serial.println(msg);
    Serial.print("read_status : ");
    Serial.println(Radio.read_status(buf));

    Serial.print("stereo : ");
    Serial.println(Radio.stereo(buf));

    Serial.print("signal_level : ");
    Serial.println(Radio.signal_level(buf));

    Serial.print("frequency_available : ");
    Serial.println(Radio.frequency_available(buf));

    Serial.print("current_freq : ");
    current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
    Serial.println(current_freq);

    delay(2000);

} 