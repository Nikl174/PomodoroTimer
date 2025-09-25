// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <tinyNeoPixel_Static.h>
#include "conf.h"


#ifdef DEBUG
#define LOG_PRINTF(format, ...) printf(format, __VA_ARGS__);
#define LOG(format) printf(format);
#else
#define LOG_PRINTF(format, ...)
#define LOG(format)
#endif

/*! \struct Buttons
 *  \brief Counting struct for the number of button presses
 *
 *  TODO
 */
struct Buttons {
  unsigned int start_stop;
  unsigned int skip;
  unsigned int restart;
};

/*! \enum PomodoroState
 *
 *  Encodes the current Interval State That is running
 */
enum PomodoroState {
  P_STATE_POMODORO,
  P_STATE_SHORT_BREAK,
  P_STATE_LONG_BREAK,
  P_STATE_NONE
};

/*! \struct Pomodoro
 *  \brief State information for the pomodoro cycle
 *
 *  TODO
 */
struct Pomodoro {
  unsigned int intervals_done;
  unsigned long start_ms;
  unsigned int interval_sec_passed;
  unsigned int pomodoro_sec;
  unsigned int short_break_sec;
  unsigned int long_break_sec;
  unsigned int long_break_intervals;
  PomodoroState interval_state;
};

struct Visualize {
  unsigned int none_r;
  unsigned int none_g;
  unsigned int none_b;
  int brightness;
  int factor;
};

Visualize vis;
byte pixelarray[NUMPIXELS * 3];
Buttons buts;
Pomodoro pom;
tinyNeoPixel pixels = tinyNeoPixel(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800, pixelarray);

// \brief update passed seconds in the pomodoro state of the current interval
// TODO handle for integer overflow of millis
void pomodoro_update_passed_sec(Pomodoro *pom) {

  if (pom->start_ms) {
    // difference between start of timer and now in s
    unsigned long new_sec = (millis() - pom->start_ms) / 1000;

    pom->interval_sec_passed += new_sec;
    // if more then one second, set new point (because of the above +=)
    if (new_sec) pom->start_ms = millis();

    LOG_PRINTF("Updated Interval by '%i' to '%i'", new_sec, pom->interval_sec_passed);
  } else {
    LOG("Could not update passed seconds, interval not started!");
  }
}

// \brief toggles between start and stopping of the timer and initialy starts the pomodoro cycle
// done by setting a starting-point (in ms)
void pomodoro_start_stop(Pomodoro *pom) {
  switch (pom->interval_state) {
    case P_STATE_NONE:
      // start the cycle by saving the current milliseconds
      pom->start_ms = millis();
      pom->interval_state = P_STATE_POMODORO;
      LOG("Starting Pomodoro Timer");
      break;
    default:
      // currently in an interval
      if (pom->start_ms) {
        // pomodoro is running, stop by erasing start_ms
        pomodoro_update_passed_sec(pom);
        pom->start_ms = 0;
        LOG("Stopping current interval");
      } else {
        // interval is stopped, save the start point
        pom->start_ms = millis();
        LOG("Continue current interval");
      }
  };
}

// \brief Advances the Pomodoro State Machine
// returns 1 if the state changed
// pomodoro_update_passed_sec(pom) should be called before
int pomodoro_update_state(Pomodoro *pom, bool skip) {
  switch (pom->interval_state) {
    case P_STATE_POMODORO:
      if (pom->interval_sec_passed >= pom->pomodoro_sec || skip) {
        // end if interval

        // pomodoro done, increase counter
        if (!skip) pom->intervals_done++;

        // check long break
        if (pom->intervals_done && pom->intervals_done % pom->long_break_intervals == 0) {
          // long break qualified
          pom->interval_state = P_STATE_LONG_BREAK;
        } else {
          pom->interval_state = P_STATE_SHORT_BREAK;
        }
        // reset time and passed seconds for new state
        pom->interval_sec_passed = 0;
        pom->start_ms = millis();
        return 1;
      }
      break;
    case P_STATE_SHORT_BREAK:
      if (pom->interval_sec_passed >= pom->short_break_sec || skip) {
        // end if interval
        pom->interval_state = P_STATE_POMODORO;
        // reset time and passed seconds for new state
        pom->interval_sec_passed = 0;
        pom->start_ms = millis();
        return 1;
      }
      break;
    case P_STATE_LONG_BREAK:
      if (pom->interval_sec_passed >= pom->long_break_sec || skip) {
        // end if interval

        pom->interval_state = P_STATE_POMODORO;
        // reset time and passed seconds for new state
        pom->interval_sec_passed = 0;
        pom->start_ms = millis();
        return 1;
      }
      break;
    default:
      // state not changed
      return 0;
  }
}

void visualise_pomodoro_time(Pomodoro *pom, tinyNeoPixel *pixel, Visualize *visual) {
  float scaled_time;

  // visualisation depends on pomodoro state
  switch (pom->interval_state) {
    case P_STATE_POMODORO:

      // scale time to the number of LEDS
      //            ______________________________ Scale Factor
      scaled_time = ((float)NUMPIXELS / pom->pomodoro_sec) * pom->interval_sec_passed;
      // set all pixel that are in the time interval with right color
      for (int i = 0; i < NUMPIXELS; i++) {

        if (scaled_time >= i)
          pixel->setPixelColor(i, POMODORO_COLOR((*pixel)));
      }

      break;
    case P_STATE_LONG_BREAK:

      // scale time to the number of LEDS
      //            ______________________________ Scale Factor
      scaled_time = ((float)NUMPIXELS / pom->long_break_sec) * pom->interval_sec_passed;
      // set all pixel that are in the time interval with right color
      for (int i = 0; i < NUMPIXELS; i++) {

        if (scaled_time >= i)
          pixel->setPixelColor(i, LONG_BREAK_COLOR((*pixel)));
      }

      break;
    case P_STATE_SHORT_BREAK:

      // scale time to the number of LEDS
      //            ______________________________ Scale Factor
      scaled_time = ((float)NUMPIXELS / pom->short_break_sec) * pom->interval_sec_passed;
      // set all pixel that are in the time interval with right color
      for (int i = 0; i < NUMPIXELS; i++) {

        if (scaled_time >= i)
          pixel->setPixelColor(i, SHORT_BREAK_COLOR((*pixel)));
      }

      break;
  }
  // adjust brightness afterwards and show (send to LEDs)
  pixel->setBrightness(visual->brightness);
  pixel->show();

  // reset color values for pixels afterwards, but do not show!
  for (int i = 0; i < NUMPIXELS; i++) pixel->setPixelColor(i, 0, 0, 0);
}


void setup() {


  buts = {
    .start_stop = 0,
    .skip = 0,
    .restart = 0,
  };
  pom = {
    .intervals_done = 0,
    .start_ms = 0,
    .interval_sec_passed = 0,
    .pomodoro_sec = POMODORO_SEC,
    .short_break_sec = SHORT_BREAK_SEC,
    .long_break_sec = LONG_BREAK_SEC,
    .long_break_intervals = LONG_BREAK_INTERVALS,
    .interval_state = P_STATE_NONE,
  };
  vis = {
    .none_r = 255,
    .none_g = 0,
    .none_b = 0,
    .brightness = BRIGHTNESS,
    .factor = -COLOR_SCALE,
  };


  // activate internal pull-up for buttons
  pinMode(SKIP_PIN, INPUT_PULLUP);
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(RESTART_PIN, INPUT_PULLUP);
  pinMode(NEO_PIN, OUTPUT);
}

void loop() {

  // read each pin, ! because of pull up
  buts.start_stop = !digitalRead(START_PIN);
  buts.skip = !digitalRead(SKIP_PIN);
  buts.restart = !digitalRead(RESTART_PIN);


  // update passed seconds and pomodoro state, giving the skip button to maybe skip the current state
  pomodoro_update_passed_sec(&pom);
  pomodoro_update_state(&pom, buts.skip);

  // toggle start/stop when button was pressed
  if (buts.start_stop) {
    pomodoro_start_stop(&pom);
  }

  // completely reset the pomodoro timer
  if (buts.restart) {
    // TODO show Cycles while pressed
    pom.interval_sec_passed = 0;
    pom.intervals_done = 0;
    pom.start_ms = 0;
    pom.interval_state = P_STATE_NONE;
  }

  // 12 as an idle color cycle
  pixels.setPixelColor(NONE_LED, pixels.Color(vis.none_r, vis.none_g, vis.none_b));

  // cycle through all colors
  if (vis.none_b == 0 && vis.none_r > 0) {
    vis.none_g += COLOR_SCALE;
    vis.none_r -= COLOR_SCALE;
  } else if (vis.none_r == 0 && vis.none_g > 0) {
    vis.none_b += COLOR_SCALE;
    vis.none_g -= COLOR_SCALE;
  } else if (vis.none_g == 0 && vis.none_b > 0) {
    vis.none_r += COLOR_SCALE;
    vis.none_b -= COLOR_SCALE;
  }

  if (!pom.start_ms) {
    // timer is paused, pulse brightness
    if (vis.brightness >= BRIGHTNESS) {
      vis.factor = -COLOR_SCALE;
    } else if (vis.brightness <= COLOR_SCALE) {
      vis.factor = COLOR_SCALE;
    }
    vis.brightness += vis.factor;
  } else {
    vis.brightness = BRIGHTNESS;
  }


  // show a loading circle for the current passed time in the interval
  visualise_pomodoro_time(&pom, &pixels, &vis);

  delay(DELAY_MS);
}
