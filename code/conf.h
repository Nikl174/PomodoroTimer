#ifndef CONF_H

#define CONF_H

// #define LONG_BREAK_SEC 1200 // 20 min
// #define SHORT_BREAK_SEC 300 // 5 min
// #define POMODORO_SEC 1500   // 25 min
#define LONG_BREAK_SEC 24 // 20 min
#define SHORT_BREAK_SEC 6 // 5 min
#define POMODORO_SEC 24   // 25 min
#define LONG_BREAK_INTERVALS 4

// Which pin on the Arduino is connected to the NeoPixels?
#define NEO_PIN 1
#define SKIP_PIN 0
#define START_PIN 4
#define RESTART_PIN 5

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12

// update delay, currently determines the responsiveness
#define DELAY_MS 100

// color of the pomodoro visualisation, 0 - 255, (R, G, B)
#define POMODORO_COLOR(pixels) pixels.Color(155, 155, 155)
#define SHORT_BREAK_COLOR(pixels) pixels.Color(155, 155, 0)
#define LONG_BREAK_COLOR(pixels) pixels.Color(0, 155, 155)

#define NONE_LED 11

#define COLOR_SCALE 3

#define BRIGHTNESS 80

// comment out to remove debug messages
// #define DEBUG

#endif /* end of include guard: CONF_H */
