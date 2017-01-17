# Created by Walker Powell

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int CALIBRATE_TIME = 3000;
const int PHOTO_RES_PIN = A0;
const int SCORE_FREQ = 1000;
int score = 0;
unsigned long lastScore;
unsigned long startTime;
int lightThreshold;
const int TOLERANCE = 50;
int timeRemaining = 30;
int prevTime = 30;
int highScore = 0;

void setup() {
  lcd.begin(16, 2);
  
  callibrateLight();
  
  lcd.clear();
  lcd.print("Score: ");
  lcd.setCursor(7, 0);
  lcd.print(score);
  
  lcd.setCursor(14, 0);
  lcd.print(timeRemaining);
  
  lcd.setCursor(0, 1);
  lcd.print("High score: ");
  lcd.setCursor(12, 1);
  lcd.print(highScore);
}

void loop() {
  if (analogRead(PHOTO_RES_PIN) < (lightThreshold - TOLERANCE)) {
    playGame();
  }
  
  delay(40);
}

void playGame() {
  score = 0;
  
  startTime = millis();
  while (millis() - startTime <= 31000) {
    timeRemaining = 30 - (((millis() - startTime) % 30000) / 1000);
    if (prevTime != timeRemaining) {
      prevTime = timeRemaining;
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Score: ");
      lcd.setCursor(7, 0);
      lcd.print(score);
      
      lcd.setCursor(14, 0);
      lcd.print(timeRemaining);
      
      lcd.setCursor(0, 1);
      lcd.print("High score: ");
      lcd.setCursor(12, 1);
      lcd.print(highScore);
    }
    
    if (analogRead(PHOTO_RES_PIN) < (lightThreshold - TOLERANCE)
        && (millis() - lastScore) > SCORE_FREQ) {
      score++;
      lastScore = millis();
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Score: ");
      lcd.setCursor(7, 0);
      lcd.print(score);
      
      lcd.setCursor(14, 0);
      lcd.print(timeRemaining);
      
      lcd.setCursor(0, 1);
      lcd.print("High score: ");
      lcd.setCursor(12, 1);
      lcd.print(highScore);
    }

    delay(40);
  }
  
  if (score > highScore) {
    highScore = score;
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("Score: ");
    lcd.setCursor(7, 0);
    lcd.print(score);
    
    lcd.setCursor(14, 0);
    lcd.print(timeRemaining);
    
    lcd.setCursor(0, 1);
    lcd.print("High score: ");
    lcd.setCursor(12, 1);
    lcd.print(highScore);
  }
  
  timeRemaining = 30;
}

void callibrateLight() {
  int sum = 0;
  int count = 0;
  lcd.print("Callibrating...");
  while (millis() < CALIBRATE_TIME) {
    sum += analogRead(PHOTO_RES_PIN);
    count++;
    delay(100);
  }
  lightThreshold = sum / count;
  lcd.setCursor(0, 1);
  lcd.print("Threshold: ");
  lcd.setCursor(11, 1);
  lcd.print(lightThreshold);
  delay(2000);
}
