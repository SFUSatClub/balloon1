# Balloon Launch 1

## Hardware

- Arduino Due

- GPS Module

	- [Adafruit Ultimate GPS Breakout](https://www.adafruit.com/product/746)

## Software

### Prerequisites

1. Install git

1. Install [PlatformIO](http://docs.platformio.org/en/stable/installation.html#installation)

1. Integrate your [IDE with PlatformIO](http://docs.platformio.org/en/stable/ide.html#ide)

	- [Atom](http://docs.platformio.org/en/stable/ide/atom.html) is recommended but using the PlatformIO CLI directly with your favourite editor is even nicer

#### Usage

- The Atom IDE is pretty straight forward to use, but here are some useful tips:

      <richard, elaborate on this, what buttons/keyboard shortcuts to compile/upload etc>

- If you're using the command-line, here are some common PlatformIO commands (make sure you're in project root):

	 ``` sh
     # build the project
     platformio run

     # upload to board
     platformio run --target upload

     # serial monitor
     platformio serialports monitor

     # library stuff
     platformio lib
     ```

## Overall System
- Modules are created once on setup and globally stored in an array
- No dynamic memory allocations except in initialization
- Modules may persist data/send data over uno->radio link by overriding
the appropriate methods
- Watchdog added
- Globals.h added
  - Allow writing to Serial using c++ style cout and cin streams
  - WATCHDOG_RESET() method to appease the watchdog
- Added a project editorconfig, makefile, and improved ycm completion

## Modules
- Scheduler and Tasks
  - IMPLEMENTED, WORKING: Successful multitasking of modules
- Radio
  - IMPLEMENTED, WORKING: Successful relaying of GPS telemetry
  (lat,long,time,alt) over due->uno->radio link
  - More testing needed to see if Modules can reliably relay information
  over the APRS data field
- GPS
  - IMPLEMENTED, WORKING: GPS gets satellite fixes, logs data to SD
  card, and provides RTC+telemetry to system
  - Only source of RTC for now
- SDCard
  - IMPLEMENTED, WORKING: SDCard reliably persists all data from all
  modules
- StateHandler
  - IMPLEMENTED, UNTESTED
  - Analyze GPS and Barometer values and relay state changes to system
- IMU
  - IMPLEMENTED, UNTESTED
  i2c address crashing the due on runtime)
  - Might change implementation and hardware IMU to the mpu6050
- Buzzer
  - IMPLEMENTED, UNTESTED
  - Toggles a digital pin to turn on a buzzer
- Barometer
  - IMPLEMENTED, UNTESTED
- Photocells
  - IMPLEMENTED, UNTESTED
- Temperature sensor
  - IMPLEMENTED, TESTED
  TODO: Add support for multiple temperature sensors
  TODO: Decide on the R2 value
