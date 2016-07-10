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

