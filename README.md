# Arduino Battleship

## By Hudson https://github.com/huddy987 and Richmond https://github.com/naviza

## Description:
Arduino rendition of the popular Hasbro boardgame "Battlesgip". Written in C++ for
2 Arduinos using the Adafruit 2.4" touchscreen display.
Players begin the game by picking a boat of length 5, a boat of length 4,
and a boat of length 3. Once both players have selected their boats, they take turns by
simultaneously guessing a square to "shoot" their opponent's boats. The game
ends when one or both players have all their battleships sunk.

## CODE INSTRUCTIONS
1. Simply upload to an Arduino Mega.

## HARDWARE REQUIREMENTS:
2 x Arduino MEGA 2560
2 x Adafruit 2.4" TFT LCD (ILI9341)
2 x breadboard
25 x wires


## KNOWN ISSUES:
If both players send boats at the same time, null data is sent over.


## RESOURCES:
https://learn.adafruit.com/adafruit-2-4-color-tft-touchscreen-breakout/spi-wiring-test
https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives


## ACKNOWLEDGEMENTS:
The set up of the touchscreen display was inspired by code from the adafruit tutorials that showed how to set up and
calibrate the screen. Code was written using adafruit libraries alongside the arduino library. Other than that,
all code was written from scratch by us.



## WIRING:

GND <--> DISPLAY GND

5V <--> DISPLAY VIN

PIN 52 <--> DISPLAY CLK

PIN 50 <--> DISPLAY MISO

PIN 51 <--> DISPLAY MOSI

PIN 10 <--> DISPLAY CS

PIN 9 <--> DISPLAY D/C

PIN 7<--> DISPLAY X+

PIN A11<--> DISPLAY X-

PIN A10<--> DISPLAY Y+

PIN 6<--> DISPLAY Y-

TX3 <--> RX3

RX3 <--> TX3

ARDUINO 1 GND <--> ARDUINO 2 GND



## SCREEN DIAGRAM FOR REFERENCE (VARIABLES FOLLOW THIS GRID):

Ex: A5 is the top left corner of the grid.


        (ARDUINO BREADBOARD IS UP HERE)

              A   B    C     D    E   F    G
      -----------------------------------------
      |  M  | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 5
      |     |----------------------------------|
      |  E  | 28 | 29 | 30 | 32 | 32 | 33 | 34 | 4
      |     |----------------------------------|
      |  N  | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 3
      |     |----------------------------------|
      |  U  | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 2
      |     |----------------------------------|
      |  "  | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 1
      |     |----------------------------------|
      |  "  | 00 | 01 | 02 | 03 | 04 | 05 | 06 | 0
      -----------------------------------------
