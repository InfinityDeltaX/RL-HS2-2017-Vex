/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include <math.h>



void yawSet(int speed){
  driveSet(speed, speed, speed, speed);
}

void stopAll(){
  driveSet(0, 0, 0, 0);
}

void yawRelative(float direction, int speed){
  speed = abs(speed);
  if(direction < 0) speed = speed * -1;

  yawSet(speed);
  int rotationSoFar = currentPositionR;

  while(){
    trackPosition();
    delay(5);
    imeGet(0, &rotationSoFar);
  }
  stopAll();
}

void movementControl(){
  int rotationPower = joystickGetAnalog(1, 4);
  int forwardsbackwardsPower = -1 * joystickGetAnalog(1, 2);
  int sidewaysPower = joystickGetAnalog(1, 1);

  moveRelative(forwardsbackwardsPower, sidewaysPower, rotationPower);
}

void moveRelative(int forwardsbackwardsPower, int sidewaysPower, int rotationPower){
   int a = -1 * forwardsbackwardsPower + -1 * sidewaysPower + rotationPower;
   int b = forwardsbackwardsPower + -1 * sidewaysPower + rotationPower;
   int c = forwardsbackwardsPower + sidewaysPower + rotationPower;
   int d = -1 * forwardsbackwardsPower + sidewaysPower + rotationPower;

   driveSet(a/2, b/2, c/2, d/2);
}

void moveAbsolute(int fieldX, int fieldY, float targetfieldR, int speed){
  int xdelta = fieldX - currentPositionX;
  int ydelta = fieldY - currentPositionY;
  int movementAngle = atan(ydelta/ (float) xdelta);
  //int rdelta = fieldR - currentPositionR;

  int distance = sqrt(xdelta*xdelta + ydelta*ydelta);
  int steps = 10;

  float startRAngle = computeAngleFromR(currentPositionR);
  float endRAngle = targetfieldRAngle-startRAngle;
  float currentRAngle = 0;

  float turnByAngle = endRAngle/steps;

  int delayTime = distance/speed/steps;

  for(int i = 0; i < steps; i++){
    movementAngle
    movePolarRelative(movementAngle-currentRAngle, speed);
    delay(delayTime);
    yawRelative(turnByAngle, 64);
    currentRAngle += turnByAngle;
  }
  //int //in the same amount of time we need to complete rdelta turns.
}

void convertMotorPowerToTicksPerSecond(int power){

}

void glideRelative(int x, int y, int r, int speed){ //move in the x, y direction while rotating towards r.

}

void movePolarRelative(float angle, int speed){
  moveRelative(sin(angle) * speed, cos(angle) * speed, 0);
}

void driveSet(int aPower, int bPower, int cPower, int dPower){
  motorSet(drivePort1, aPower*driveMultiplier1);
  motorSet(drivePort2, bPower*driveMultiplier2);
  motorSet(drivePort3, cPower*driveMultiplier3);
  motorSet(drivePort4, dPower*driveMultiplier4);

  trackPosition();
}

  void trackPosition(){ //add all the movement since the last time this function was run. Therefore, we should try to run it every time the robot turns.
    int a, b, c, d;
    imeGet(0, &a);
    imeGet(1, &b);
    imeGet(2, &c);
    imeGet(3, &d);

    resetAllIMEs();

    int rDist = (a + b + c + d)/4;
    int fwbDist = ( (b-rDist) + (c-rDist) ) /2 + -1 * ( (a-rDist) + (d-rDist) ) /2;
    int sideDist = ( (c-rDist) + (d-rDist) ) /2 + -1 * ( (a-rDist) + (b-rDist) ) /2;
    float angle = computeAngleFromR(rDist);
    int xDist = sin(angle) * sideDist + cos(angle) * fwbDist;
    int yDist = cos(angle) * fwbDist + sin(angle) * sideDist;

    currentPositionX += xDist;
    currentPositionY += yDist;
    currentPositionR += rDist;
  }

  void resetAllIMEs(){
    imeReset(0); imeReset(1); imeReset(2); imeReset(3);
  }

  double computeAngleFromR(int r){
    float angle = r/ (encoderTicksPerTurn * wheelTurnsPerRobotTurn);
  }

  void computePosition(){


  }

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO() {
}

/*
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An autonomous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */

void initialize() {
  drivePort1 = 2;
  drivePort2 = 3;
  drivePort3 = 4;
  drivePort4 = 5;

  driveMultiplier1 = 1;
  driveMultiplier2 = 1;
  driveMultiplier3 = 1;
  driveMultiplier4 = 1;

  DIRECTION_LEFT = -1;
  DIRECTION_RIGHT = 1;

  encoderTicksPerTurn = 39.2;

  currentPositionR = 0;
  currentPositionX = 0;
  currentPositionY = 0;

  imeInitializeAll();
}
