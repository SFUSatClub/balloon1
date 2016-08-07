# Balloon Launch 1

## Hardware

- Arduino Due

- GPS Module

	- [Adafruit Ultimate GPS Breakout](https://www.adafruit.com/product/746)

## Software

### Prerequisites

1. Install git

### PlatformIO

#### Installation

1. Since we plan on using the Arduino Due and there are some [issues](https://github.com/platformio/platformio/issues/710) with the current version of PlatformIO, we will be using the [development version](https://github.com/platformio/platformio/issues/710) for now. **Atom is recommended**  but the command line is great as well.

	If you're using Atom, here's how to use the dev version:
    
    ```
    Menu PlatformIO: Settings > PlatformIO IDE > Use development version of PlatformIO
    ```

	If you're using the command-line, here's how to use the dev version:
    
    ```sh
    # install the latest development version of PlatformIO
	pip install -U https://github.com/platformio/platformio/archive/develop.zip
    ```

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