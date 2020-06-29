# WiFi GBP Emulator

A GameBoy printer emulator which provides the received data over a WiFi connection.
This code has been created for a "LOLIN(WEMOS) D1 R2 & mini" [ESP8266 based board](https://github.com/esp8266/arduino).   
For uploading the Filesystem the LittleFS tool [Arduino IDE Plugin](https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases) is required. Follow the installation instructions [here](https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/)

## Setup
Before compiling the project you need to create a `config.h` inside the project folder.  
([`config.h.sample.txt`](/wifi-gbp-emulator/config.h.sample.txt) should be used as a reference)  
If you run the default setup (similar to the WiFi-Printer) that file can be left empty.  
If your (hardware-)setup is different you can adjust the parameters there.  

## Bootmode
The code is designed to check pin `D0` for high to boot in printer mode.  
Through this it is possible to use the +5v provided by the GameBoy to switch the mode.  

The default will alternate the bootmode automatically between printer and server on each boot, so you can use the reset-button to switch modes.    
If you have the possibility to sense the 5V signal, from the GameBoy, you can uncomment `#define SENSE_BOOT_MODE` in your `config.h`.  

## Configuration
If the device is not configured the default settings (AP only) will be used
### Default config
If not present the ESP will set up an accesspoint with ssid/password `gameboyprinter`.     
It will output it's IP adress over the serial monitor and on an optionally connected display.  
(usually the device should be accessible via `http://192.168.4.1`)    
It will also be discoverable via Bonjour/mDNS via `http://gameboyprinter.local`.
### Manual Setup via `conf.json`
Wifi setup can be done via a file `conf.json` in the `/data/` folder.  
The basic format are as follows:
```` json
{
  "mdns": "gameboyprinter",
  "ap": {
    "ssid":"gameboyprinter",
    "psk":"gameboyprinter"
  },
  "networks": [
    {
      "ssid":"Your SSID",
      "psk":"Your wifi password"
    }
  ]
}
````

All config options are optional
#### `"mdns"`
Change the name by which the printer will be accessible via Bonjour/mDNS
#### `"ap"`
AccessPoint config
Enter ssid/psk to open an accesspoint different to the default one.
#### `"networks"`
Array of objects with keys ssid/psk.  
The first reachable network in that list will be used to conect to.  
⚠ Do not use more than 10 elements. Otherwise you might see an "Out of Memory" error in the serial monitor.
### Manual Setup via web interface
ToDo / not yet implemented

## Automatic update of the webserver content
* You can run [`update_w.ps1`](./update_w.ps1) (windows-powershell) or [`update_w.sh`](./update_w.sh) (bash) to download the latest release of the [gb-printer-web](https://github.com/HerrZatacke/gb-printer-web/releases/) and automatically unzip it to the correct folder.  
* After doing so upload the content of that folder to the ESP via the [arduino-esp8266fs-plugin](https://github.com/esp8266/arduino-esp8266fs-plugin/releases/) in the Arduino IDE.

## Hardware Setup

For a basic setup only the connection with a Gameboy Link Cable is required. 

Gameboy Original/Color Link Cable Pinout
```
 __________
|  6  4  2 |
 \_5__3__1_/ (at cable)

| Link Cable | ESP8266 |
|------------|---------|
| Pin 1      | D8      |
| Pin 2      | D7      |
| Pin 3      | D6      |
| Pin 5      | D5      |
| Pin 6      | G       |

```

Additionally an [OLED Display](https://github.com/HerrZatacke/wifi-gbp-emulator/#oled-display) can be added via D1 -> Display SCL / D2 -> Display SDA 

## Links / Research
* Original GPB-Emulator by [mofosyne: Arduino Gameboy Printer Emulator](https://github.com/mofosyne/arduino-gameboy-printer-emulator)  
* Tutorial on how to work with an ESP8266 by [ttapa: ESP8266 Beginner's Guide](https://tttapa.github.io/ESP8266/Chap01%20-%20ESP8266.html)  
* Potentially faster library for data transfer [applefreak/esp8266-gameboy-printer](https://github.com/applefreak/esp8266-gameboy-printer)
* ESP8266 D1 Mini [Pinout](https://escapequotes.net/esp8266-wemos-d1-mini-pins-and-diagram/)
* SPIFFS/LittleFS [Filesystem Docs](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html)
* [Link cable breakout board](https://github.com/Palmr/gb-link-cable)

## OLED Display
You can add a [tiny oled display like this](https://www.amazon.de/gp/product/B07BDFXFRK)   
To use it, you need to uncomment `#define USE_OLED` and the following lines   
It will even display a tiny printer icon when in print mode !["Tiny printer-icon"](docs/printer-icon.png "Tiny printer-icon")

## Demo Video
This video shows an older version of the printer-emulator code.    
### ⚠ Take care ⚠
You should not power the ESP from the GameBoy, as this might damage the GameBoy itself. What you see in the video was meant as a proof of concept which is basically not implemented anymore.  

[![Video of the first prototype](docs/video.jpg "Video of the first prototype")](https://www.youtube.com/watch?v=HHPHkeio85U)
