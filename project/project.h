/*
    Header file containing software configurations, constants, and
    clarifying definitions for the Arduino.
*/
#pragma once

#include <LinkedList.h>
#include <Regexp.h>
#include <SPI.h>
#include <StaticThreadController.h>
#include <Thread.h>
#include "HWDefs.h"

// Platform parameters and bounds
#define NUM_MOTORS 6
#define MIN_POS 0  // maximum and minimum position values for all actuators defined by analogRead()
#define MAX_POS 1023
int ZERO_POS[NUM_MOTORS] = { 199, 186, 194, 194, 192, 199 };  // measured position bounds by actuator
int END_POS[NUM_MOTORS] = { 833, 827, 826, 829, 831, 833 };

// Pin group arrays; each value corresponding to the actuator (see HWDefs for values)
const uint8_t SS_PINS[NUM_MOTORS] = { SS_PIN_1, SS_PIN_2, SS_PIN_3, SS_PIN_4, SS_PIN_5, SS_PIN_6 };
const uint8_t DIR_PINS[NUM_MOTORS] = { DIR_PIN_1, DIR_PIN_2, DIR_PIN_3, DIR_PIN_4, DIR_PIN_5, DIR_PIN_6 };
const uint8_t PWM_PINS[NUM_MOTORS] = { PWM_PIN_1, PWM_PIN_2, PWM_PIN_3, PWM_PIN_4, PWM_PIN_5, PWM_PIN_6 };
const uint8_t POT_PINS[NUM_MOTORS] = { POT_PIN_1, POT_PIN_2, POT_PIN_3, POT_PIN_4, POT_PIN_5, POT_PIN_6 };

// Movement parameters
#define RESET_DELAY 4000  // time (ms) for actuators to reset position during initialization
#define POSITION_TOLERANCE 5  // uncertainty for which offset from desired position is acceptable
#define PWM_NEAR 20  // value for PWM when position is in tolerance (but not in position)
#define PWM_FAR 100  // default PWM value; scaling is disabled in early development
typedef enum MotorDirection  // to clarify the direction in which actuators move
{
    RETRACT = 0,
    EXTEND = 1
};

// Serial/SPI configuration parameters (https://www.arduino.cc/en/Reference/SPI)
#define BAUD_RATE 115200
#define BIT_ORDER LSBFIRST
#define DATA_MODE SPI_MODE1  // clock polarity = low, phase = high 
#define PRINT_INTERVAL 2000  // minimum time (ms) between printing serial info

// Serial input parameters
#define MAX_BUFFER_SIZE 31  // 4 bytes per position (6 digits), 7 for limiter characters
#define INPUT_TRIGGER 10  // number of characters in the input queue before activating parsing
#define NUM_READINGS 100 // number of analog readings to normalize to acquire position
#define INPUT_INTERVAL 100  // interval (ms) for input thread
#define PARSER_INTERVAL 500  // interval (ms) for parser thread
#define TRANSLATOR_INTERVAL 500 // interval (ms) for translation thread 
const char START_CHAR = '<';
const char SENTINEL_CHAR = '>';
const char DELIMITER_CHAR = ',';
const char TARGET_PATTERN[] = "<(%d?%d?%d?%d),"  // pattern to match proper input from the input buffer
                               "(%d?%d?%d?%d),"  // matches the last fully received proper string
                               "(%d?%d?%d?%d),"  // buffer overflow issues should be handled by the hardware/interface
                               "(%d?%d?%d?%d),"
                               "(%d?%d?%d?%d),"
                               "(%d?%d?%d?%d)>\n*\r*$";