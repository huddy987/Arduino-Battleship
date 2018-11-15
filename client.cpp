#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>             // Library for SPI mode
#include <Adafruit_ILI9341.h> // Controller chip library
#include "TouchScreen.h"    //Library for TouchScreen
#include "client.h"
#include "draw_handler.h"

// Waits for the other client to respond when data must be sent simultaneously
void Client::wait(Adafruit_ILI9341 tft){
  // If we are waiting, draw a waiting screen
  if (!Serial3.available()){
    // Draw a waiting screen while waiting for the other arduino to respond
    draw_waiting(tft);
  }

  // If we are waiting, loop until the other arduino responds
  while(!Serial3.available()){
    continue;
  }
  // Read the buffer to clear it if we recieve something
  Serial3.read();
}

// Prints a "ready" character over serial3
void Client::send_ready_message(){
  // Write a single A to our opponent to signal that we are ready
  Serial3.write("A");
}
