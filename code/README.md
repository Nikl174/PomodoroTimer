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
