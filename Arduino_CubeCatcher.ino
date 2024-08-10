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
int score = 0;

unsigned long previousStamp = 5000; // This is the point where the game should start
const long AllowedClickInterval = 40;
const long AllowedCubeMovementInterval = 50;


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

  // Draw a cube 
  display.drawFastVLine(56, 12, 16, SH110X_WHITE);
  display.drawFastVLine(64, 20, 16, SH110X_WHITE);
  display.drawFastVLine(72, 12, 16, SH110X_WHITE);

  display.drawLine(56,12, 64, 4, SH110X_WHITE );
  display.drawLine(72,12, 64, 4, SH110X_WHITE );

  display.drawLine(56, 28, 64, 36, SH110X_WHITE);
  display.drawLine(72, 28, 64, 36, SH110X_WHITE);

  display.drawLine(56, 12,64, 20, SH110X_WHITE);
  display.drawLine(72, 12, 64, 20, SH110X_WHITE);

  // Title

  display.setTextColor(SH110X_WHITE);
  
  display.setTextSize(1);
  
  display.setCursor(8, 48);
  display.print("Arduino CubeCatcher");

  display.display();

  delay(5000);
  display.clearDisplay();
}

bool collisionDectection(int player_x, int player_y, int cube_x, int cube_y, int size){
  if( (abs(player_x - cube_x) < size) && (abs(player_y - cube_y) < size) ){  
    return true;
  }
  else {
    return false;
  }
}

// The initial position of the game objects

void gameDataLoader(){
  Player.x = 63;
  Player.y = 60;

  Cube.x = 62;
  Cube.y = 1;
}

void loop(){
  unsigned long currentStamp = millis();

  display.drawRect(Player.x, Player.y, 4, 4, SH110X_WHITE);

  display.drawRect(Cube.x, Cube.y, 4, 4, SH110X_WHITE);
  
  leftState = digitalRead(LEFT_PIN);
  rightState = digitalRead(RIGHT_PIN);
/*
  I need to limit of the speed of how the player character moves along with the falling speed of the cube, but using delay() will stop the code from running
  So I set up two time intervals declared at the beginning of the program, and only allow movement when the span between the previous movement and the current movement
  is greater than my allowed time interval
  For reference, see: https://docs.arduino.cc/built-in-examples/digital/BlinkWithoutDelay/
*/
  if (leftState == HIGH && (currentStamp - previousStamp >= AllowedClickInterval)) {
    previousStamp =  millis();
    if(Player.x <= 1){
      Player.x++;
    }
    else{
      Player.x--;
    }
  }

  if (rightState == HIGH && (currentStamp - previousStamp >= AllowedClickInterval) ){
    previousStamp = millis();
    if(Player.x >= 123){
      Player.x--;
    }else{
      Player.x++;
    }
  }

  if(currentStamp - previousStamp >= AllowedCubeMovementInterval){
    Cube.y++;
  }

  if(Cube.y == 64){
    Cube.y = 1;
    Cube.x = static_cast<int>(random(1,123));
  }

  // No need for collision detection when the cube is too far away
  if (Cube.y >= 56){
    if(collisionDectection(Player.x, Player.y, Cube.x, Cube.y, 4)){
      Cube.y = 1;
      Cube.x = static_cast<int>(random(1,123));
    }
  }

  display.display();

  display.clearDisplay();
}