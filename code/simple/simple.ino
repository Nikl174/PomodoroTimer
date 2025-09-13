// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <tinyNeoPixel.h>


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            1
#define START_STOP_PIN 2
#define SKIP_PIN       3
#define RESTART_PIN    4

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
tinyNeoPixel pixels = tinyNeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
unsigned long previous_time = 0;        // Previous time LED changed state
unsigned long interval = 1000;

void setup() {

  pixels.begin(); // This initializes the NeoPixel library.


    // set pin to input
  pinMode(START_STOP_PIN, INPUT);             
  pinMode(SKIP_PIN, INPUT);
  pinMode(RESTART_PIN, INPUT);

  // turn on pullup resistors
  digitalWrite(START_STOP_PIN, HIGH);
  digitalWrite(SKIP_PIN, HIGH);
  digitalWrite(RESTART_PIN, HIGH);     
}

void loop() {
  int start_stop, restart, skip;
  unsigned long now = millis();   // Get current time

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  for (int i = 0; i < NUMPIXELS; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).
  }

  start_stop = digitalRead(START_STOP_PIN);
  restart = digitalRead(RESTART_PIN);
  skip = digitalRead(SKIP_PIN);

  

  previous = now;
  
}
