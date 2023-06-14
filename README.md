# espBoy

The espBoy is a handheld game console that can play several simple games

## Setup

1. Clone this repo
```
git clone https://github.com/Trevader24135/espBoy
```

2. Install Visual Studio Code (If you do not already have it) (available here: [https://code.visualstudio.com/download](https://code.visualstudio.com/download))

2. Open the directory containing the repo's files in Visual Studio Code

3. Install the PlatformIO extension by opening the `extensions` tab on the left menu bar, and searching for `platformio`. Click install.

![PlatformIO Extention installation](images/platformio_installation.png)

4. You can now build and upload the project using PlatformIO's tools available in the left menu bar. Ensure that your esp32 device is plugged in, then click "Upload". The project should build and upload to your device.

## Hardware support

Currently hardware support is under development. The board currently being used for development is the NodeMCU-32s, but the code should work for all esp32 variants, though modifications to the Hardware.h file will likely need made.