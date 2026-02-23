//************* HEADER **************
/* 
  Arduino based FM radio using TEA5767 FM module 
  From instructables and simplified 
*/

//********** LIBRARIES **************
#include <Wire.h> // I2C comms standard
#include <TEA5767Radio.h>  // FM module
#include <Button.h> // push buttons
#include <Adafruit_GFX.h> // display
#include <Adafruit_SSD1306.h> // display

//********** DEFINES ************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED  display height, in pixels

//********** GLOBALS ************
TEA5767Radio radio = TEA5767Radio(); 
float frequency = 0;  
char* label = "";
int i = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
float s[] = { 87.7, 92.3, 93.1, 94.5, 95, 97.3, 102.1, 103.3, 104.4, 105.3, 105.7 };
char* l[] = { "F.Culture", "  FIP", "F.Inter", "Top Music", "F.Musique", "F.Inter", "  RFM", "Europe 1", "F.Info ", "Nostalgie", "  RTL" };

int DOWN = 12; // pin 12, button down
int UP = 11; // pin 11, button up
int downState = 0;
int upState = 0;

//************** SETUP ********************
void setup() {
  Serial.begin(9600);
  Serial.println("==== INIT RADIO ====");
  delay(1000);
  Wire.begin();
  frequency = s[i]; // frequency
  label = l[i]; // label
  radio.setFrequency(frequency);

  Serial.println("==== INIT BUTTONS ====");
  delay(1000);
  pinMode(UP, INPUT); 
  pinMode(DOWN, INPUT); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  displayUpdate();

}

//*************** LOOP ********************
void loop() {
  downState = digitalRead(DOWN);
  if ( downState == LOW ) { // go up to next station
    Serial.println("==== Button DOWN pressed ====");
    i --;
    if (i < 0) { 
      i = 10;
    }
    delay(200);
    frequencyUpdate();
    
  }

  //upState = digitalRead(UP);
  //if ( upState == HIGH ) { // go up to next station
  //  Serial.println("==== Button UP pressed ====");
  //  i ++;
  //  if (i > 10) { 
  //    i = 0;
  //  }
  //  delay(200);
  //  frequencyUpdate();

  //}
}

void frequencyUpdate() { // UP or DOWN frequency
  frequency = s[i];
  label = l[i];
  radio.setFrequency(frequency);
  displayUpdate();

}

void displayUpdate() { // LCD display update
  // set 
  delay(10);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(10, 25);

  // and display
  display.println(label);
  display.display(); 
  delay(1000);
  
}
