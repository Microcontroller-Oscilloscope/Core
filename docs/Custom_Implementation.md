# Custom Implementation

The [Microcontroller Oscilloscope](/README.md) project supports custom implementations of methods used for core functionality. This can be done for microcontrollers that [aren't supported](/docs/MC_Support.md) or ones where a custom implementation is more desired.

Method Override Support:
- NVM functionality
	- [header file](/src/nvm/generic_nvm.h)
	- [test file `void testNVM()`](https://github.com/Microcontroller-Oscilloscope/UnityTests/blob/main/src/nvm_tests/nvm_tests.h)

## Override Instructions

An example confiugration is shown [here](#example-configuration-for-custom-nvm).

- Click the PlatformIO command `clean` in the project workspace to remove all existing build files
- Configure `src/board_consts.h` in the project workspace for your selected microcontroller to enable the desired custom overrides
- Implement the required methods from the header file specified above
- Make sure code compiles
- Configure `platformio.ini` to run in test mode [example here](#platformioini)
- Run tests
	- If error occurs where it can't find `unity.h`, ensure you run the PlatformIO command `Test` to install Unity library
	- While not strictly required for the new functionality to work, this confirms it works with the project and that edge cases are handled properly
- Restore `platformio.ini` after tests are complete

## Example Configuration for Custom NVM:
This is an example configuration for creating a custom NVM method. These principles can be applied to other method overrides.

Folder Layout:<br>
- &#x1f4c1; {Project Workspace}
	- &#x1f4c1; lib
		- *&#x1f4c1; custom_nvm
			- *&#x1f4c4; [custom_nvm.h](#custom_nvmh)
			- *&#x1f4c4; [custom_nvm.cpp](#custom_nvmcpp)
	- &#x1f4c1; src
		- !&#x1f4c4; [board_consts.h](#board_constsh)
	- !&#x1f4c4; [platformio.ini](#platformioini)

*: New file/directory<br>
!: Modified file

### custom_nvm.h

```c++
#ifndef CUSTOM_NVM_H
#define CUSTOM_NVM_H // prevents multiple declarations

#if !DEFAULT_NVM // ensures code isn't using default nvm

#include <nvm/generic_nvm.h> // header file for nvm
// ... any other needed header files

#endif
#endif
```

### custom_nvm.cpp

```c++
#include "custom_nvm.h"

#if !DEFAULT_NVM // ensures code isn't using default nvm

// implementation of nvmInit(uint16_t setNVMSize) from generic_nvm.h
enum NVMStartCode nvmInit(uint16_t setNVMSize) {
	return NVM_FAILED;
}

// ... rest of functions required from generic_nvm.h (NVM functionality header file)

#endif
```

### board_consts.h

```c++
// ... start of file

// whether to use the default nvm methods over custom nvm methods
#define DEFAULT_NVM false // set to false to not use default nvm methods

// ... rest of file
```

### platformio.ini

```ini
[platformio]
;default_envs = {board} ; comment out existing 'default_envs'
default_envs = test_env ; replace with 'test_env'

; ... rest of file
```