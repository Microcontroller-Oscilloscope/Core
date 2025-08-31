# Unsupported Boards

This project allows for users to implement boards that are unofficially supported using PlatformIO. Boards that aren't supported by PlatformIO and/or aren't able to use the `Arduino` framework aren't supported. Check [here](https://docs.platformio.org/en/latest/boards/index.html) to verify that your board is supported and can implement the `Arduino` framework before attempting any custom solutions.

## Unsupported Board Instructions

- Open PlatformIO Home
![](/docs/Unsupported_Images/Home.png)
- Create a new PlatformIO project
![](/docs/Unsupported_Images/New.png)
- Create a project name
![](/docs/Unsupported_Images/Name.png)
- Select your chosen microcontroller from the boards list
![](/docs/Unsupported_Images/Board.png)
- Select the `Arduino` framework
![](/docs/Unsupported_Images/Framework.png)
- Click `Finish`
	- Optionally chose a location for the project
- Configure `{Project Workspace}/platformio.ini` to be the same this [ini file](/docs/Unsupported_Examples/platformio.ini.txt), removing `.txt` from the filename
	- The section `Configuration Flags` in `platformio.ini` will need to be changed based on your board selected. This requires going through the relevant NVM library for your selected board, likely `EEPROM.h`
- Add the file `{Project Workspace}/src/board_consts.h`, using this [header file](/docs/Unsupported_Examples/board_consts.h.txt), removing `.txt` from the filename and adjusting the parameters
	- Configure the file to add relevant macros from [compile_flags.h](/src/compile_flags/compile_flags.h) for building the libraries
- Add the file `{Project Workspace}/src/main.h`, using this [header file](/docs/Unsupported_Examples/main.h.txt), removing `.txt` from the filename
- Configure `{Project Workspace}/src/main.cpp`, using this [cpp file](/docs/Unsupported_Examples/main.cpp.txt), removing `.txt` from the filename
- Create any needed custom methods using [this](/docs/Custom_Implementation.md) as a guide
- `Build` the project to ensure that no errors occur
- Configure `{Project Workspace}/platformio.ini` to use `test_env` for `default_envs`
- `Test` the project to ensure libraries operate as expected