# Simple implementation of the Pomodoro timer in Arduion

- Initial '12' blinks
## Buttons:

| Button | Function | 
| ------ | -------- |
| Restart| Cancels the Current Routine and Starts again |
| Start/Stop| Toggles running of the current round (pause or productivity time) |
| Skip | Skips the currently running cycle|

## Structure

- state informations:
    - which current cycle (Break, Productivity)
    - number of finished cycles
    - button pressed (interrupt)

## Compile and Upload

``` bash
arduino-cli compile --fqbn megaTinyCore:megaavr:atxy2 code.ino

arduino-cli upload -p /dev/ttyACM0 --fqbn megaTinyCore:megaavr:atxy2 -P serialupdi57k code.ino
```
