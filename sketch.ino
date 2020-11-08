#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// macros from DateTime.h
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)


/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  


#define CTRL_PIN 53
#define TOGGLE_PIN 5



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


bool inSetMode = true;

unsigned long timer = 0;
unsigned long timerMillis = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(CTRL_PIN, OUTPUT);
  digitalWrite(CTRL_PIN, HIGH);
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
}

void loop() {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextColor(SSD1306_WHITE);  
    
    inSetMode = digitalRead(TOGGLE_PIN) == HIGH;
    if (inSetMode) {
      //time((millis() - timer)/ 1000);
      int pot1 = analogRead(A1);
      int pot2 = analogRead(A0);

      int hourSet = round(map(pot1, 0, 1024, 0, 12));
      int minSet = round(map(pot2, 0, 1024, 0, 60));
      displayTimer(hourSet, minSet);
      timer = hourSet*60*60 + minSet*60;
      timer = timer*1000;
      Serial.print("hourSet ");
      Serial.println(hourSet);
      Serial.print("minSet");
      Serial.println(minSet);
      Serial.print("timer");
      Serial.println(timer);
      timerMillis = millis();
     
    }

    else {
      Serial.print("millis ");
      Serial.println(millis());
      Serial.print("timerMillis ");
      Serial.println(timerMillis);
      unsigned long elapsedTime = millis() - timerMillis;
      Serial.print("elapsedTime ");
      Serial.println(elapsedTime);
      displayTime((timer - elapsedTime)/1000);
      Serial.print("timer ");
      Serial.println(timer);


      
      if (elapsedTime > timer) {
        digitalWrite(CTRL_PIN, LOW);
      }
    }
    
    display.display();
    

}


void time(long val){  
  int days = elapsedDays(val);
  int hours = numberOfHours(val);
  int minutes = numberOfMinutes(val);
  int seconds = numberOfSeconds(val);

  // digital clock display of current time
  Serial.print(days,DEC);  
  //printDigits(hours);  
  //printDigits(minutes);
  //printDigits(seconds);
 Serial.println();  
 
}

void displayTime(long val) {
  int days = elapsedDays(val);
  int hours = numberOfHours(val);
  int minutes = numberOfMinutes(val);
  int seconds = numberOfSeconds(val);
  
  display.setTextSize(2);            
  display.println(val);
  
}

void displayTimer(int hours, int minutes) {
  display.setTextSize(1); 
  display.println("SET hour:minute");
  display.setTextSize(2); 
  display.print(hours);
  display.print(":");
  display.print(minutes);
}

