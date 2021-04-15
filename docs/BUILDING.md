# Building arcticOS

## Step 1. Install the requirements
On Debian & Debian-based distros (like Ubuntu) the `scripts/install-toolchain.sh` script can automatically install this for you. If you are not on a Debian-based distro, you will need to install the AVR version of GCC, AVR Libc, and avrdude.

## Step 2. Run the makefile
This should automatically work. If not, you may need to set where your `avr-gcc` binary is located.

## Step 3. Install with avrdude
This depends on what revision of the device you're using.