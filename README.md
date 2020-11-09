# embedded-common
This repository contains the Sensirion I2C hardware abstraction layer,
the Sensirion SHDLC/UART hardware abstraction layer and commonly shared code
for both parts. It is a dependency of the Sensirion embedded drivers.
**It should not be necessary to clone this repository independently**, and we
recommend to clone the repository recursively from the respective projects.

If you're looking for sensor drivers, please refer to the release bundles (zip
files) from the respective projects. An overview is listed at
https://github.com/Sensirion/info#repositories

## Repository content
* `common` folder (shared functions and global defines)
* `shdlc` folder (implementation of the SHDLC communication and UART HAL
  including sample code for some platforms)
* `i2c` folder (implementation of the I2C communication)
* `i2c/hw` folder (stub for implementation of the hardware I2C HAL,
  including sample code for some platforms)
* `i2c/sw` folder (stub for implementation of the software I2C HAL
  (bit-banging GPIOs), including sample code for some platforms)
* `tests-common-config` folder (configuration files for tests, used by driver repositories)

### sensirion\_config.h
You need to specify the integer sizes on your system.
We assume you have the `<stdint.h>` file available, so we have an
include directive for it. If this is not the case on your platform you need to
specify the following types yourself:

* `int64_t` = signed 64bit integer
* `uint64_t` = unsigned 64bit integer
* `int32_t` = signed 32bit integer
* `uint32_t` = unsigned 32bit integer
* `int16_t` = signed 16bit integer
* `uint16_t` = unsigned 16bit integer
* `int8_t` = signed 8bit integer
* `uint8_t` = unsigned 8bit integer

Make sure you specify the whole range (64bit sized integers are used on a 32bit
system for some sensor data).

### sensirion\_hw\_i2c\_implementation.c
Use this file when using a dedicated I2C controller (use software I2C from the
next section for bit banging on GPIOs.)

Use this file to implement the I2C HAL via the I2C controller of your
system. Implement the documented function stubs: an init, a read, a write and a
delay function. For further information please check out some of the sample
implementations in the `i2c/hw/sample-implementations` folder in this
repository.

### sensirion\_sw\_i2c\_implementation.c
Use this file if you don't have a dedicated hardware I2C controller (otherwise
see the previous section) to emulate an I2C controller in software by
bit-banging GPIOs. You will thus need to implement the stubs necessary to drive
the GPIO pins used for SDA (data line) and SCL (clock line) along with the delay
function, that must be precise enough to drive the pins at the desired
frequency.  For further information please check out some of the sample
implementations in the `i2c/sw/sample-implementations` folder in this
repository.
