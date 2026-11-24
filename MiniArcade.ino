
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// XIAO ESP32-C3 pins
const int BUTTON_A = 3;  // D1
const int BUTTON_B = 4;  // D2

// Game settings
int selectedGame = 0;
const char* gameNames[] = {
  "TAMAGOTCHI",
  "PONG",
  "DINO RUN"
};

enum Game {
  MENU,
  PET,
  PONG,
  DINO
};

Game currentGame = MENU;

bool previousA = false;
bool previousB = false;

unsigned long lastFrame = 0;
unsigned long bothPressedAt = 0;
bool returningToMenu = false;

// Pong variables
int paddleY = 24;
int ballX = 64;
int ballY = 32;
int ballDX = 2;
int ballDY = 1;
int pongScore = 0;

// Dino variables
int dinoY = 48;
int dinoVelocity = 0;
bool jumping = false;
int obstacleX = 128;
int dinoScore = 0;

// --------------------------------------------------
// SETUP
// --------------------------------------------------

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);

  Wire.begin(6, 7);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED not found!");
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(15, 25);
  display.println("MINI ARCADE");
  display.display();

  delay(1200);
}

// --------------------------------------------------
// MAIN LOOP
// --------------------------------------------------

void loop() {
  bool aDown = digitalRead(BUTTON_A) == LOW;
  bool bDown = digitalRead(BUTTON_B) == LOW;

  bool aPressed = aDown && !previousA;
  bool bPressed = bDown && !previousB;

  previousA = aDown;
  previousB = bDown;

  // Hold both buttons for one second to return to menu
  if (aDown && bDown) {
    if (!returningToMenu) {
      bothPressedAt = millis();
      returningToMenu = true;
    }

    if (millis() - bothPressedAt >= 1000) {
      currentGame = MENU;
      returningToMenu = false;
      previousA = true;
      previousB = true;
      drawMenu();
      delay(200);
      return;
    }
  } else {
    returningToMenu = false;
  }

  // Limit game updates
  if (millis() - lastFrame < 35) {
    return;
  }
  lastFrame = millis();

  switch (currentGame) {
    case MENU:
      if (bPressed) {
        selectedGame = (selectedGame + 1) % 3;
      }

      if (aPressed) {
        if (selectedGame == 0) currentGame = PET;
        if (selectedGame == 1) {
          resetPong();
          currentGame = PONG;
        }
        if (selectedGame == 2) {
          resetDino();
          currentGame = DINO;
        }
      }

      drawMenu();
      break;

    case PET:
      drawPet();
      break;

    case PONG:
      updatePong(aDown, bDown);
      drawPong();
      break;

    case DINO:
      updateDino(aPressed);
      drawDino();
      break;
  }
}

// --------------------------------------------------
// MENU
// --------------------------------------------------

void drawMenu() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(17, 2);
  display.println("MINI");

  display.setCursor(17, 19);
  display.println("ARCADE");

  display.setTextSize(1);
  display.setCursor(10, 43);
  display.print("> ");
  display.println(gameNames[selectedGame]);

  display.display();
}

// --------------------------------------------------
// TAMAGOTCHI
// --------------------------------------------------

void drawPet() {
  display.clearDisplay();

  // Starry background
  display.drawPixel(8, 8, SSD1306_WHITE);
  display.drawPixel(110, 10, SSD1306_WHITE);
  display.drawPixel(22, 35, SSD1306_WHITE);
  display.drawPixel(100, 42, SSD1306_WHITE);
  display.drawPixel(48, 7, SSD1306_WHITE);
  display.drawPixel(118, 55, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(29, 2);
  display.println("MY LITTLE PET");

  // Pet ears
  display.fillTriangle(39, 27, 45, 12, 54, 28, SSD1306_WHITE);
  display.fillTriangle(74, 28, 83, 12, 89, 27, SSD1306_WHITE);

  // Pet body
  display.fillRoundRect(38, 23, 52, 34, 12, SSD1306_WHITE);

  // Eyes
  display.fillCircle(53, 37, 2, SSD1306_BLACK);
  display.fillCircle(75, 37, 2, SSD1306_BLACK);

  // Mouth
  display.drawPixel(63, 44, SSD1306_BLACK);
  display.drawPixel(64, 45, SSD1306_BLACK);
  display.drawPixel(65, 44, SSD1306_BLACK);

  // Feet
  display.fillRoundRect(44, 52, 13, 7, 3, SSD1306_WHITE);
  display.fillRoundRect(71, 52, 13, 7, 3, SSD1306_WHITE);

  display.display();
}

// --------------------------------------------------
// PONG
// --------------------------------------------------

void resetPong() {
  paddleY = 24;
  ballX = 64;
  ballY = 32;
  ballDX = 2;
  ballDY = 1;
  pongScore = 0;
}

void updatePong(bool aDown, bool bDown) {
  if (aDown && !bDown) paddleY -= 3;
  if (bDown && !aDown) paddleY += 3;

  paddleY = constrain(paddleY, 0, 48);

  ballX += ballDX;
  ballY += ballDY;

  if (ballY <= 1 || ballY >= 62) {
    ballDY = -ballDY;
  }

  if (ballX >= 122) {
    ballDX = -abs(ballDX);
  }

  // Paddle collision
  if (ballX <= 12 && ballX >= 7 &&
      ballY >= paddleY && ballY <= paddleY + 16) {
    ballDX = abs(ballDX);
    pongScore++;
  }

  // Ball returns to the center after missing
  if (ballX < 0) {
    ballX = 64;
    ballY = 32;
    ballDX = 2;
    pongScore = 0;
  }
}

void drawPong() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("PONG  Score: ");
  display.println(pongScore);

  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  // Player paddle
  display.fillRect(5, paddleY, 4, 16, SSD1306_WHITE);

  // Opponent paddle
  display.fillRect(119, ballY - 8, 4, 16, SSD1306_WHITE);

  // Ball
  display.fillCircle(ballX, ballY, 2, SSD1306_WHITE);

  display.display();
}

// --------------------------------------------------
// DINO RUN
// --------------------------------------------------

void resetDino() {
  dinoY = 48;
  dinoVelocity = 0;
  jumping = false;
  obstacleX = 128;
  dinoScore = 0;
}

void updateDino(bool aPressed) {
  if (aPressed && !jumping) {
    jumping = true;
    dinoVelocity = -8;
  }

  if (jumping) {
    dinoY += dinoVelocity;
    dinoVelocity++;

    if (dinoY >= 48) {
      dinoY = 48;
      jumping = false;
      dinoVelocity = 0;
    }
  }

  obstacleX -= 3;

  if (obstacleX < -5) {
    obstacleX = 128;
    dinoScore++;
  }

  // Obstacle collision
  if (obstacleX < 22 && obstacleX > 5 &&
      dinoY > 35) {
    resetDino();
  }
}

void drawDino() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("DINO RUN  ");
  display.println(dinoScore);

  display.drawLine(0, 57, 127, 57, SSD1306_WHITE);

  // Dino body
  display.fillRect(10, dinoY, 12, 9, SSD1306_WHITE);
  display.fillRect(18, dinoY - 5, 7, 8, SSD1306_WHITE);

  // Dino eye
  display.drawPixel(22, dinoY - 3, SSD1306_BLACK);

  // Dino legs
  display.drawLine(13, dinoY + 9, 11, dinoY + 12, SSD1306_WHITE);
  display.drawLine(19, dinoY + 9, 21, dinoY + 12, SSD1306_WHITE);

  // Obstacle
  display.fillRect(obstacleX, 45, 5, 12, SSD1306_WHITE);

  display.display();
}
