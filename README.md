# ECU-ESP8266

Experiments with [@patience4711](https://github.com/patience4711)s [read-APSystems-YC600-QS1-DS3](https://github.com/patience4711/read-APSystems-YC600-QS1-DS3/issues/109#issuecomment-1518397994) project and [platformIO](platformio.org) to read sun harvest data out of an APSystems inverter (solar power).

Targetted microcontroller is the ESP8266, build system an Ubuntu 18.04

## Lessons learnt and thoughts not finished

- My C and C++ skills are rusty (at best).
- Project layout/ app architecture for an embedded system puts different
  spotlights then when building a web application with Ruby on Rails.
- Still not sure about the trade-off between global state and e.g. buffers and
  local variables.
- Also, I tend to think of headers as the interface to a module (you find in
  zigbee.h what you want to call if you are NOT in zigbee.cpp).
- On a similar line, going object-oriented C++ or module-based C or something else is yet up for grabs.
- I am ditching vim for platformio here, but lets see how long it'll last.
- Will need to look into general and especially embedded conventions, e.g. https://barrgroup.com/embedded-systems/books/embedded-c-coding-standard . Or GNU/Linux or GTK...
- `SoftwareSerial` might help in developing and debugging (currently we throw away our possibility to debug to Serial, because that is remapped to the zigbee module). Or use `Serial1`?

### Feature ideas
/*
 * Feature Ideas:
 *   -> Real Captive Portal for easier access.
 *   -> MultiMode to feed into another network
 *   -> Button to switch on / off Wifi
 *   -> LCD Display
 */

## License and Copyright

[Unclear to me, but we are allowed to
hack](https://github.com/patience4711/read-APSystems-YC600-QS1-DS3/issues/105).

Copyright of all zigbee related code 2021-2023
[@patience4711](https://github.com/patience4711), other code 2023 Felix
Wolfsteller.

Code by Felix Wolfsteller is released under the [AGPLv3+](https://www.gnu.org/licenses/agpl-3.0.en.html).
