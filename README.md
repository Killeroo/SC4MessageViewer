# SC4MessageViewer
A simple and configurable DLL mod for Simcity 4 that prints out ingame message events to a console.

![image](https://github.com/Killeroo/SC4MessageViewer/assets/9999745/63241067-aecd-4822-b4a9-48efcc3dd44c)

# Disclaimer
This mod is intended to be used as a tool for experimentation and research, this is in no way meant to be used in normal every day gameplay. Tracking frequently occuring messages WILL slow down your game or cause other issues. In addition to that, having a seperate console window attached to the game will cause unexpected behaviour and have unintended consequences. Use at your own risk.

# How to use
Grab the latest version from the [releases page](https://github.com/Killeroo/SC4MessageViewer/releases), extract the contents of the zip file to your Simcity 4 plugins folder and run the game!

You can change what message ids are displayed by modifying the contents of `message_viewer_config.ini`. The basic structure of the config file is as follows:

```
kSC4MessageBodyFontStyleID = 0x4A809915
kSC4MessagePreLoad = 0x26C63340
kSC4MessageLoad = 0x26C63341
kSC4MessagePostLoad = 0x26C63342
```
Each line contains a display name for the message id (this is actually displayed in the console when the event occurs) and the message id of the event that is registered with the game. Included in the default config file are a list of of preconfigured message id's from [nsgomez](https://github.com/nsgomez). If no config file is found, a default one will be created at startup time.

# License
```
MIT License

Copyright (c) 2023 Matthew Carney

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
