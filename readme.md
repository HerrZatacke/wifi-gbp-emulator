# Websocket GBP Emulator

A GameBoy printer emulator which provides the received data over a websocket.
This code has been created for a "LOLIN(WEMOS) D1 R2 & mini" [ESP2866 based board](https://github.com/esp8266/arduino).   
For uploading the Filesystem the LittleFS tool [Arduino IDE Plugin](https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases) is required

## Bootmode
The code is designed to check pin `D0` for high to boot in printer mode.  
Through this it is possible to use the +5v provided by the GameBoy to switch the mode.  

If you dont want to solder, you can uncomment `#define ALTERNATE_BOOT_MODE` in `websocket-gbp-emulator.ino`.  
This will alternate the bootmode automatically between printer and server on each boot, so you can use the reset-button to switch modes.  

## Configuration
If the device is not configured the default settings (AP only) will be used
### Default config
If not present the ESP will set up an accesspoint with ssid/password `gameboyprinter`.     
It will output it's IP adress over the serial monitor and on an optionally connected display.  
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

## Links / Research
* Original GPB-Emulator by [mofosyne: Arduino Gameboy Printer Emulator](https://github.com/mofosyne/arduino-gameboy-printer-emulator)  
* Tutorial on how to implement Websockets with an ESP2866 by [ttapa: WebSocket communication](https://tttapa.github.io/ESP8266/Chap14%20-%20WebSocket.html)  
* Potentially faster library for data transfer [applefreak/esp8266-gameboy-printer](https://github.com/applefreak/esp8266-gameboy-printer)
* ESP2866 D1 Mini [Pinout](https://escapequotes.net/esp8266-wemos-d1-mini-pins-and-diagram/)
* SPIFFS/LittleFS [Filesystem Docs](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html)
* [Link cable breakout board](https://github.com/Palmr/gb-link-cable)

## ToDo's
* ~~[mDNS](https://tttapa.github.io/ESP8266/Chap08%20-%20mDNS.html)~~
* ~~Webserver serving a static Web-Application (will be in a [different Repository](https://github.com/HerrZatacke/gb-printer-web))~~


## Done
* ~~Websockets serving received printer packages~~
* Websocket implementation has been removed due to performance issues, instead the ESP should now detect the presence of the GameBoy over it's 5V signal. and Either Boot in "server" or "printer" mode  

## OLED Display
You can add a [tiny oled display like this](https://www.amazon.de/gp/product/B07BDFXFRK)   
To use it, you need to uncomment `#define USE_OLED` and the following lines   
It will even display a tiny printer icon when in print mode !["Tiny printer-icon"](docs/printer-icon.png "Tiny printer-icon")

## Demo Video
⚠ Obviusly you should not power the ESP from the GameBoy, as this might damage the GameBoy itself.

[![Video of the first prototype](docs/video.jpg "Video of the first prototype")](https://www.youtube.com/watch?v=HHPHkeio85U)

