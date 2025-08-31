# Microcontroller Support

Listed here are the documented capabilites for supported microcontrollers. Microcontrollers that aren't officially supported are due to their pin interfaces, which aren't reliable for Oscilloscope use. Custom implementations can be created for boards without official support, which can be found [here](/docs/Unsupported_Board.md). Boards that are supported but require different solutions for common functions can be modified, which can be found [here](/docs/Custom_Implementation.md).

Mark Descriptions:<br>
&check;: Fully supported<br>
&cross;: Not and won't be supported<br>
-: Not yet developed<br>
*: Developed but not tested<br>
!: Currently in development

| Board Brands |
| -- |
| [Arduino](#arduino) |
| [Espressif](#espressif) |
| [Raspberry Pi](#raspberry-pi) |
| [STMicroelectronics](#stmicroelectronics) |
| [Teensy](#teensy) |

> ## Arduino

| Device | Oscilloscope
| -- | -- |
| Due | &cross; |
| Leonardo | &cross; |
| Mega | &cross; |
| Micro | - |
| Nano | - |
| [Uno r3 (only non SMD)](https://github.com/Microcontroller-Oscilloscope/Uno) | ! |
| Uno r4 | &cross; |
| Zero | &cross; |

> ## Espressif

| Device | Oscilloscope |
| -- | -- |
| [ESP32 Devkit C v4](https://github.com/Microcontroller-Oscilloscope/ESP32) | ! |
| ESP 8266 | - |

> ## Raspberry Pi

| Device | Oscilloscope |
| -- | -- |
| Pico<br>[Pico W](https://github.com/Microcontroller-Oscilloscope/Pico) | -<br>! |
| Pico 2<br>Pico 2 W | -<br>- |

> ## STMicroelectronics

| Device | Oscilloscope |
| -- | -- |
| STM32 Black Pill | - |
| STM32 Nucleo | &cross; |

> ## Teensy

| Device | Oscilloscope |
| -- | -- |
| Teensy 4.1 | - |
| Teensy 4.0 | - |
| Teensy 3.6 | - |
| Teensy 3.5 | - |
| Teensy 3.2 | - |
| Teensy++ 2.0 | - |
| Teensy 2.0 | - |
| Teensy LC | - |