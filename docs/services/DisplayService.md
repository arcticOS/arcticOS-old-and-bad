# DisplayService Documentation
DisplayService is the main service providing display functionality to arcticOS. It has support for an emulator (via Pygame) as well as a Raspberry Pi driver for the 2.7" monochrome E-ink from Waveshare.

## Commands
### `C` - Clear the screen
This command erases the current buffer.
This command takes no arguments.
### `R` - Refresh the screen
This command displays the current buffer on the display.
This command takes no arguments.
### `B` - Get text boundry
This command returns the size of a string at a given font size.
This command takes the following arguments:
```
  <Text String> <Font Size>
```
### `T` - Draw text
This command draws a string at a given font size.
This command takes the following arguments:
```
  <Text String> <Font Size> <X Position> <Y Position>
```
### `TC` - Draw centered text
This command draws a string at a given font size centered on a certain position.
This command takes the following arguments:
```
  <Text String> <Font Size> <X Position> <Y Position>
```
### `R` - Draw rectangle
This command draws a rectangle.
This command takes the following arguments:
```
  <X Position> <Y Position> <Width> <Height>
```
### `RF` - Draw filled rectangle
This command draws a filled rectangle.
This command takes the following arguments:
```
  <X Position> <Y Position> <Width> <Height>
```
### `L` - Draw line
This command draws a line.
This command takes the following arguments:
```
  <Starting X Position> <Starting Y Position> <Ending X Position> <Ending Y position>
```