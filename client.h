#ifndef _CLIENT_H_
#define _CLIENT_H_

class Client{
  public:
    // Waits for the other client to respond when data must be sent simultaneously
    void wait(Adafruit_ILI9341 tft);

    // Prints a "ready" character over serial3
    void send_ready_message();

};


#endif
