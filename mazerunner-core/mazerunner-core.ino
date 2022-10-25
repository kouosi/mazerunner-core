/*
 * File: mazerunner.ino
 * Project: mazerunner
 * File Created: Monday, 5th April 2021 8:38:15 am
 * Author: Peter Harrison
 * -----
 * Last Modified: Thursday, 8th April 2021 8:38:41 am
 * Modified By: Peter Harrison
 * -----
 * MIT License
 *
 * Copyright (c) 2021 Peter Harrison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "cli.h"
#include "config.h"
#include "maze.h"
#include "mouse.h"
#include "reports.h"
#include "src/adc.h"
#include "src/encoders.h"
#include "src/motion.h"
#include "src/motors.h"
#include "src/sensors.h"
#include "src/switches.h"
#include "src/systick.h"
#include <Arduino.h>

// Global objects
Systick systick;
AnalogueConverter adc(EMITTER_A, EMITTER_B);
Switches switches(SWITCHES_CHANNEL);
Encoders encoders;
Sensors sensors;
Motion motion;
Motors motors;
Profile forward;
Profile rotation;
Maze maze PERSISTENT;
Mouse mouse;
CommandLineInterface cli;
Reporter reporter;

void setup() {
  Serial.begin(BAUDRATE);
  adc.init();
  systick.begin();
  pinMode(USER_IO_6, OUTPUT);
  pinMode(EMITTER_A, OUTPUT);
  pinMode(EMITTER_B, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  motors.setup();
  encoders.setup();
  Serial.print('-');
  if (switches.button_pressed()) {
    maze.initialise_maze();
    for (int i = 0; i < 4; i++) {
      digitalWrite(LED_BUILTIN, 1);
      delay(50);
      digitalWrite(LED_BUILTIN, 0);
      delay(50);
    }
    Serial.println(F("Maze cleared"));
    switches.wait_for_button_release();
  }

  sensors.disable();
  Serial.println(F("RDY"));
}

void loop() {
  if (cli.read_serial() > 0) {
    cli.interpret_line();
  }
  if (switches.button_pressed()) {
    switches.wait_for_button_release();
    mouse.execute_cmd(switches.read());
  }
}
