
# arcticOS

An open-source operating system for feature phones with E-ink displays.

## The Vision

Every modern phone company either sells your data or your attention, and they make you pay huge amounts to let them do it. I aim to change that, by building a phone that is:
- Low-cost
- Fully open-source, in both hardware and software
- As modular as possible, with no stupid software locks to prevent you from extending, repairing, or upgrading your phone
- Completely ad-free
- Easy for anyone with basic knowledge in SMD soldering and 3D printing to replicate

## Features

- User interface written in Python
- Display server for interfacing with the display from languages other than Python
- Python APIs for interfacing with the display, keyboard, and apps
- Emulator for testing on desktop computers (Using Pygame for the display server)
- Calculator
- Notes
  
## Planned Features
- Calls and SMS with the SIM7600* series of modems.
- IRC and Matrix messages
- Music Player
- Calendar
- Clock with alarms
- E-mail client
- OTA updates?
- User Data Encryption
- Support for GUI apps written in languages other than Python

## Planned Features (Emulator)
- SIM7600 emulator
- Better keyboard input
- Flashlight & frontlight support
- Audio support

## Roadmap

- Develop usable demo of OS
- Design and fabricate prototype hardware capable of running the OS
- Acquire funding via Kickstarter
- Fix issues with prototype
- Begin selling development units
- Improve OS until it is in a semi-mature state
- Fix issues with development units
- Begin shipping beta-testing units to testers in USA
- Fix issues with OS until it is ready for release
- Fix issues with beta units
- Stress test and improve hardware until it is ready for release
- Begin shipping final units

## Run Locally

Clone the project

```bash
  git clone https://github.com/jhonnystene/arcticOS
```

Go to the project directory

```bash
  cd arcticOS
```

Install dependencies

```bash
  pip3 install pygame
```

Fix permissions
```bash
  chmod +x start.sh
  chmod +x start-displayserver.sh
```

Start the emulator

```bash
  ./start.sh
```

  
## FAQ

#### Will it support WhatsApp?

No.

#### Will it support group messaging?

Group messaging will be supported through the IRC/Matrix clients. SMS group messaging is done differently by different phone brands and it is impossible to support all of them.

#### What will the final hardware look like?

The final hardware configuration hasn't been fully decided yet, but it should look something like this:
- 2.7" E-ink display
- Full QWERTY keyboard
- USB-C for charging and data transfer, 3.5mm headphone jack
- Swappable Li-ion battery
- MicroSD card for OS and user data storage
- Modular SIM7600-series cellular modem

#### When will it be done?

I can't say for sure. Development is much more expensive and takes much more time then I could have guessed. It will likely speed up after the Kickstarter, though.
## License

[GNU General Public License Version 3.0](https://choosealicense.com/licenses/gpl-3.0/)

  