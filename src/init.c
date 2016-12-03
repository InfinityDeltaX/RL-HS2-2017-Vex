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



void yaw(int direction){
  driveSet(10, 10, 10, 10);
}

void move(int forwardsbackwards, int sideways){
   int a = -1 * forwardsbackwards + -1 * sideways;
   int b = forwardsbackwards + -1 * sideways;
   int c = forwardsbackwards + sideways;
   int d = -1 * forwardsbackwards + sideways;

   driveSet(a/2, b/2, c/2, d/2);
}

void movementControl(){
  int forwardsbackwards = -1 * joystickGetAnalog(1, 2);
  int sideways = joystickGetAnalog(1, 1);

  move(forwardsbackwards, sideways);
}

void driveSet(int a, int b, int c, int d){
  motorSet(drivePort1, a*driveMultiplier1);
  motorSet(drivePort2, b*driveMultiplier2);
  motorSet(drivePort3, c*driveMultiplier3);
  motorSet(drivePort4, d*driveMultiplier4);
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
}
