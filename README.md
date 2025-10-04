# Commodore 64 Keyboard Adapter

This project allows you to use a real Commodore 64 keyboard as an extended ASCII keyboard via an Arduino Pro Micro (Leonardo compatible). It scans the C64 keyboard matrix and sends keypresses as ASCII characters over USB serial or enhanced ASCI code beyond the 0x80 value.

## Goal

The goal is to have a cheap solution with a very good and easy-to-use keyboard for any SBC project. Since the C64 is ultimately a simple matrix, we can easily do what we want.

## Features

- Supports standard, shifted, control and shifted+control key mappings
- Debounced key scanning
- Debug output via serial (configurable)
- Easy pin mapping for C64 keyboard rows and columns

## Hardware

- **Arduino Pro Micro** (Leonardo compatible)
- Commodore 64 keyboard. A real one.
- Wiring according to the pin mapping in [`C64Keyboard.ino`](C64Keyboard.ino)

## Usage

1. Connect the C64 keyboard to the Arduino Pro Micro as described in the comments of [`C64Keyboard.ino`](C64Keyboard.ino).
2. Select the "Leonardo" board in the Arduino IDE.
3. Upload [`C64Keyboard.ino`](C64Keyboard.ino) to your Arduino.
4. Open the serial monitor at 115200 baud to see debug output or receive ASCII keypresses.

## Pin Mapping

See the comments at the top of [`C64Keyboard.ino`](C64Keyboard.ino) for the full row/column mapping between the C64 keyboard and Arduino pins.

## License

MIT License

## Credits

- Laurent FAVARD
- Based on work by DJ Sures (Synthiam.com) and Chris Garrett
