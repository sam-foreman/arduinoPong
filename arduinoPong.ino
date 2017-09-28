
// CS410 Assignment 3 - PONG!
// written by Sam Foreman, Ned Geeslin, and Xavier Bohorquez
// note: make sure the name of the USB port matches up with the name in webserv.c
// note: requires connection to web server for paddle movement

#include "Adafruit_GFX.h"
#include "Adafruit_HT1632.h"

#define HT_DATA 2
#define HT_WR 3
#define HT_CS 4
#define HT_CS2 5

Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS, HT_CS2);

//setup game
void setup() {
  Serial.begin(9600);
  
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  start("LETS");
  start("PLAY");
  start("PONG");
  delay(1000);
  
}

//position variables for the ball
int x = 8;
int y = 8;
int oldX = 0;
int oldY = 0;
int xDirection = 1;
int yDirection = 0;
int xMoveVal = 5;
int yMoveVal = 5;
int xMove = xMoveVal;
int yMove = yMoveVal;


int del = 10;
int val;

//position variables for the paddle
int paddleL = 6;
int paddleR = 6;
int paddleLDirection = 0;
int paddleRDirection = 0;

//initialize score
int lScore = 0;
int rScore = 0;

int keyPressed = 0;

void loop() {
  //check for input from server
  if (Serial.available() > 0)
    serial();

  //draw paddles and ball on screen
  drawLPaddle();
  drawRPaddle();
  matrix.drawPixel(x, y, 1);
  matrix.writeScreen();
  coordCheck();
  oldX = x;
  oldY = y;

  //move the ball forward if the xMove hits 0 - this prevents the ball from moving every refresh which is waay too fast
  if (xMove == 0)
  {
    x += xDirection;
    xMove = xMoveVal;
  }
  else
    xMove -= 1;
  if (yMove == 0)
  {
    y += yDirection;
    yMove = yMoveVal;
  }
  else
    yMove -= 1;

   matrix.clrPixel(oldX, oldY);

   if (paddleLDirection < 0)
    paddleLDirection += 1;
   else if (paddleLDirection > 0)
    paddleLDirection -= 1;

   if (paddleRDirection < 0)
    paddleRDirection += 1;
   else if (paddleRDirection > 0)
    paddleRDirection -= 1;
}

//checks if the ball hits the end of the matrix
void coordCheck()
{

//if ball hits a wall, switch directions
  if (y > 14)
  {
    matrix.clrPixel(oldX, oldY);
    matrix.clrPixel(x, y);
    y = 14;
    yDirection = -yDirection;

  }
  else if (y < 0)
  {
    y = 0;
    yDirection = -yDirection;
  }

  //if ball hits wall on the x side, check if it collides with the paddle
  //if it hits the paddle, change the speed (based on the paddle's movement) and reverse direction
  //if not, then it's a goal
  if (x > 22)
    {

     if (yDirection == 0)
        {
          if (paddleRDirection > 0)
            yDirection = -1;
          else if (paddleRDirection < 0)
            yDirection = 1;
        }

        //if the ball and paddle are moving in the same direction, speed the ball up.
        //if they're in opposite directions, slow down, else keep the speed the same.
     if ((paddleRDirection > 0 && yDirection == 1) || (paddleRDirection < 0 && yDirection == -1))
    {
      yMoveVal += 1;
      if (yMoveVal > 8)
      {
        yDirection = 0;
        yMoveVal = 8;
      }
      xMoveVal += 1;
      if (xMoveVal > 8)
      {
        xMoveVal = 8;
      }
    }
    else if ((paddleRDirection > 0 && yDirection == -1) || (paddleRDirection < 0 && yDirection == 1))
    {
      yMoveVal -= 1;
      if (yDirection == 0)
        {
          yDirection = 1;
        /*  if (paddleRDirection < 0)
            yDirection = -1;
           else
            yDirection = 1;*/
        }
      if (yMoveVal < 1)
        yMoveVal = 1;
      xMoveVal -= 1;
      if (xMoveVal < 1)
        xMoveVal = 1;
    }
      if (checkGoal() == false)
      {
        x = 22;
        xDirection = -xDirection;
      }
      else
        goal();
    }
  else if (x < 1)
  {
     if (yDirection == 0)
        {
          if (paddleLDirection > 0)
            yDirection = -1;
          else if (paddleLDirection < 0)
            yDirection = 1;
        }
    if ((paddleLDirection > 0 && yDirection == 1) || (paddleLDirection < 0 && yDirection == -1))
    {
      yMoveVal += 1;
      if (yMoveVal > 8)
      {
        yDirection = 0;
        yMoveVal = 8;
      }

      xMoveVal += 1;
      if (xMoveVal > 8)
        xMoveVal = 8;
    }
    else if ((paddleLDirection > 0 && yDirection == -1) || (paddleLDirection < 0 && yDirection == 1))
    {
      if (yDirection == 0)
        {
          yDirection = 1;
         /* if (paddleRDirection < 0)
            yDirection = -1;
           else
            yDirection = 1;*/
        }
      yMoveVal -= 1;
      if (yMoveVal < 1)
        yMoveVal = 1;

      xMoveVal -= 1;
      if (xMoveVal < 1)
        xMoveVal = 1;
    }
    if (checkGoal() == false)
    {
      x = 1;
     xDirection = -xDirection;
    }
    else
      goal();
  }

}


boolean checkGoal()
{
  //if the y position of the ball collides with the paddle, it's not a goal
  if (xDirection == -1)
  {
    if (y == paddleL || y == paddleL+1 || y == paddleL+2 || y == paddleL+3)
      return false;
    else
    {
      rScore += 1;
      xDirection = 1;
      x = 1;
      y = 8;
      return true;
    }
  }
  else
  {
    if (y == paddleR || y == paddleR+1 || y == paddleR+2 || y == paddleR+3)
      return false;
    else
    {
      xDirection = -1;
      x = 22;
      y = 8;
      lScore += 1;
      return true;
    }
  }
    
}

void goal()
{
  
  //in the even of a goal:
      // - reset paddle and ball positions/speed
      // - flash the score and GOAL
      // - if any score reaches 5, flash the game over screen
  xMoveVal = 5;
  yMoveVal = 5;
  xMove = xMoveVal;
  yMove = yMoveVal;
  paddleL = 6;
  paddleR = 6;
  paddleLDirection = 0;
  paddleRDirection = 0;
  yDirection = 0;
  matrix.setRotation(3);
  matrix.clearScreen();
  matrix.setCursor(0, 0);
  matrix.print("GOAL");
  matrix.setCursor(4, 8);
  matrix.print(lScore);
  matrix.setCursor(10, 8);
  matrix.print("-");
  matrix.setCursor(16, 8);
  matrix.print(rScore);
  matrix.writeScreen();
  delay(1500);
  matrix.clearScreen();
  matrix.setRotation(0);

  if (lScore == 5)
  {
    matrix.setCursor(0, 0);
    matrix.print("P L");
    matrix.print(lScore);
    matrix.setCursor(10, 8);
     matrix.print("-");
     matrix.setCursor(16, 8);
     matrix.print(rScore);
     matrix.writeScreen();
    delay(1500);
    matrix.clearScreen();
        matrix.setCursor(0, 0);
    matrix.print("WIN");
    matrix.print(lScore);
    matrix.setCursor(10, 8);
     matrix.print("-");
     matrix.setCursor(16, 8);
     matrix.print(rScore);
     matrix.writeScreen();
    delay(1500);
    matrix.clearScreen();
    matrix.setCursor(0, 0);
    matrix.print("P L");
    matrix.print(lScore);
    matrix.setCursor(10, 8);
     matrix.print("-");
     matrix.setCursor(16, 8);
     matrix.print(rScore);
     matrix.writeScreen();
    delay(1500);
    matrix.clearScreen();
    matrix.setCursor(0, 0);
    matrix.print("WIN");
    matrix.print(lScore);
    matrix.setCursor(10, 8);
     matrix.print("-");
     matrix.setCursor(16, 8);
     matrix.print(rScore);
     matrix.writeScreen();
    delay(1500);

    //infinite loop (the game is over)
    while (1)
      delay(5000);
  
  }
  else if (rScore == 5)
  {

        matrix.setCursor(0, 0);
    matrix.print("P R");
    matrix.print(lScore);
    matrix.setCursor(10, 8);
     matrix.print("-");
     matrix.setCursor(16, 8);
     matrix.print(rScore);
     matrix.writeScreen();
    delay(1500);
    matrix.clearScreen();
     matrix.setCursor(0, 0);
    matrix.print("WIN");
    matrix.print(lScore);
    matrix.setCursor(10, 8);
     matrix.print("-");
     matrix.setCursor(16, 8);
     matrix.print(rScore);
     matrix.writeScreen();
    delay(1500);
    matrix.clearScreen();
     matrix.setCursor(0, 0);
    matrix.print("P R");
    matrix.print(lScore);
    matrix.setCursor(10, 8);
     matrix.print("-");
     matrix.setCursor(16, 8);
     matrix.print(rScore);
     matrix.writeScreen();
    delay(1500);
    matrix.clearScreen();
        matrix.setCursor(0, 0);
    matrix.print("WIN");
    matrix.print(lScore);
    matrix.setCursor(10, 8);
     matrix.print("-");
     matrix.setCursor(16, 8);
     matrix.print(rScore);
     matrix.writeScreen();
    delay(1500);

    //infinite loop (the game is over)
    while (1)
    delay(5000);
    
  }
  
}

//draw paddle positions on the screen
void drawLPaddle()
{
  matrix.drawPixel(0, paddleL, 1);
   matrix.drawPixel(0, paddleL+1, 1);
   matrix.drawPixel(0, paddleL+2, 1);
   matrix.drawPixel(0, paddleL+3, 1);
}
void drawRPaddle()
{
  matrix.drawPixel(23, paddleR, 1);
  matrix.drawPixel(23, paddleR+1, 1);
  matrix.drawPixel(23, paddleR+2, 1);
  matrix.drawPixel(23, paddleR+3, 1);
}

//displays text
void start(String val)
{  
  matrix.setCursor(0, 4);
  matrix.print(val);
  matrix.writeScreen();
  delay(500);
  matrix.clearScreen();
  delay(500);
   
}

//checks the inputted serial val
// 49 = w = move paddleL up
// 50 = s = move paddleL down
// 51 = o = move paddleR up
// 52 = o = move paddleR down
// note that the paddles are moved by 2, one pixel across is too slow
void serial()
{
  int val;
  val = Serial.read();
  if (val == 49)
   {
      paddleL -= 2;
      matrix.clrPixel(0, paddleL+4);
      matrix.clrPixel(0, paddleL+5);
      if (paddleL < 0)
        paddleL = 0;

      paddleLDirection = 10;
    }
    else if (val == 50)
    {
      paddleL += 2;
      matrix.clrPixel(0, paddleL-1);
      matrix.clrPixel(0, paddleL-2);
      if (paddleL > 12)
        paddleL = 12;

      paddleLDirection = -10;
    }
    else if (val == 51)
      {
        paddleR -= 2;
        matrix.clrPixel(23, paddleR+4);
        matrix.clrPixel(23, paddleR+5);
        if (paddleR < 0)
          paddleR = 0;

        paddleRDirection = 10;
      }
   else if (val == 52)
   {
      paddleR += 2;
      matrix.clrPixel(23, paddleR-1);
      matrix.clrPixel(23, paddleR-2);
      if (paddleR > 12)
        paddleR = 12;

      paddleRDirection = -10;
   }
}
