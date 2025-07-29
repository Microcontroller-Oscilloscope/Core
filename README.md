# Microcontroller Oscilloscope Core

> [!NOTE]
> This repository is still being developed and is currently not in a stable state

This repository is designed to allow makers without access to an oscilloscope but with access to a microcontroller (Arduino, ESP, etc.) to monitor signals in a similar manner. This repository holds the central codebase needed to implement the [Microcontroller Oscilloscope](https://github.com/Microcontroller-Oscilloscope) on a microcontroller.

## Requirements:
- A laptop/computer
- A monitoring device (laptop, computer, phone, etc.)
	- The same laptop/computer can be used as the monitoring device
	- Check [Applications](/docs/Applications.md) for the software specific to your monitoring device
- [Visual Studio Code](https://code.visualstudio.com/download) **`Visual Studio Code` and `Visual Studio` are NOT the same application!** Please ensure you have the proper one installed
- [PlatformIO](https://platformio.org/)
- Access to a [supported microcontroller](/docs/MC_Support.md)
	- Unsupported microcontrollers can be used but require additional work to get operational
- A cable to connect and upload code to. This will vary between microcontrollers

## Setup:
- Install Visual Studio Code (VSCode) and then install the PlatformIO extension
- Open the repository that corresponds to your microcontroller located [here](/docs/MC_Support.md)
	- Modify `src/board_consts.h` as desired. The base configuration will work.
- Rename `platformio.ini.txt` to `platformio.ini`
- Connect your selected board to your computer
- Upload the program to your microcontroller
	- [Insert driver error]
	- [Insert port communication error]
- [Insert connection to application]