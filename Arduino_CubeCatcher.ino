#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

struct player{
  int x;
  int y;
};

player Player;

struct cube{
  int x;
  int y;
};

cube Cube;


#define I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define LEFT_PIN 7
#define RIGHT_PIN 6

#define DELAY 20
// DELAY = 1000 / FPS

int leftState = 0;
int rightState = 0;

unsigned long previousStamp = 5000; // This is the point where the game should start
const long AllowedClickInterval = 500;


Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*
Remember to check your I2C address and screen resolution if you have a different screen.
Mine is a GME12864-77 which uses the SH110X driver, but some would need to use SSD1306 instead
*/

void setup(){
  delay(100);
  Serial.begin(9600);

  pinMode(LEFT_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT); 

  display.begin(I2C_ADDRESS, true);
  display.display();
  display.clearDisplay();

  gameDataLoader();
  menu();

}

void menu(){
  delay(100);

  // display.clearDisplay();
  display.setTextColor(SH110X_WHITE);

  display.drawFastVLine(56, 12, 16, SH110X_WHITE);
  display.drawFastVLine(64, 20, 16, SH110X_WHITE);
  display.drawFastVLine(72, 12, 16, SH110X_WHITE);

  display.drawLine(56,12, 64, 4, SH110X_WHITE );
  display.drawLine(72,12, 64, 4, SH110X_WHITE );

  display.drawLine(56, 28, 64, 36, SH110X_WHITE);
  display.drawLine(72, 28, 64, 36, SH110X_WHITE);

  display.drawLine(56, 12,64, 20, SH110X_WHITE);
  display.drawLine(72, 12, 64, 20, SH110X_WHITE);
  display.setTextSize(1);
  
  display.setCursor(8, 48);
  display.print("Arduino CubeCatcher");

  display.display();

  delay(5000);
  display.clearDisplay();
}


bool collisionDectection(){
  return false;
}

void gameDataLoader(){
  Player.x = 63;
  Player.y = 127;
}

void loop(){

  unsigned long currentStamp = millis();



  leftState = digitalRead(LEFT_PIN);
  rightState = digitalRead(RIGHT_PIN);

  
  if (leftState == HIGH && (currentStamp - previousStamp >= AllowedClickInterval)) {
    previousStamp =  millis();
    if(Player.x <= 1){
      Player.x+=1;
    }
    else{
      Player.x-=1;
    }
    Serial.println( String(Player.x) );
  }

  if (rightState == HIGH && (currentStamp - previousStamp >= AllowedClickInterval) ){
    previousStamp = millis();
    if(Player.x >= 127){
      Player.x-=1;
    }else{
    Player.x += 1;
    }
    Serial.println( String(Player.x) );
  }

  /*
  Player.x = 63;
  Player.y = 127;
  display.drawRect(Player.x, Player.y, 2, 2, SH110X_WHITE);
  display.display();

  Cube.x = static_cast<int>(random(1,128));
  Cube.y = 1;

  while(Cube.y != 127){
    display.drawRect(Cube.x, Cube.y, 2, 2, SH110X_WHITE);
    Cube.y++;
    display.display();
    delay(100);
    display.clearDisplay();
  }

  if (Cube.y == 127){
    Cube.x = static_cast<int>(random(1,128));
    Cube.y = 1;
  }


  */
}