# embedded-common
This repository contains the Sensirion i2c abstraction layer and commonly shared
code. It is a dependency of the Sensirion embedded drivers and it should not be
necessary to clone it independently.

We recommend to download the repository recursively within the
[SGP](https://github.com/Sensirion/embedded-sgp) and
[SHT](https://github.com/Sensirion/embedded-sht) projects.

## Repistory content
* `*.c` files (CRC calculation function)
* `*.h` files (type definitions, prototype declarations for i2c
  communication,...)
* `hw_i2c` folder (stub for implementation of the hardware i2c communication,
  including sample code for some platforms)
* `sw_i2c` folder (stub for implementation of the software i2c communication
  (bit-banging GPIOs), including sample code for some platforms)

### sensirion_arch_config.h
You need to specify the integer sizes on your system.
We assume you have the `<stdint.h>` file available, so we have an
include directive for it. If this is not the case on your platform you need to
specify the following types yourself:

* s64 = signed 64bit integer
* u64 = unsigned 64bit integer
* s32 = signed 32bit integer
* u32 = unsigned 32bit integer
* s16 = signed 16bit integer
* u16 = unsigned 16bit integer
* s8 = signed 8bit integer
* u8 = unsigned 8bit integer

Make sure you specify the whole range (64bit sized integers are used on a 32bit
system for some sensor data).

### hw_i2c_implementation.c
In this file you implement the hardware i2c communication on your system
provided there is one available. You will find four function stubs that
you have to implement out. There is an init, a read, a write and a delay
function. For further information please check out some of the sample
implementations in the `hw_i2c/sample-implementations` folder in this
repository.

### sw_i2c_implementation.c
If you don't have any hardware i2c communication
capability you can emulate that in software, meaning bit-banging through GPIOs.
In addition to the hardware i2c implementation, all you need to do additionally
is configuring SDA (data line) and SCL (clock line) by adjusting the
`sw_i2c_implementation.c` file. What's more, you also need to implement the
delay function. For further information please check out some of the sample
implementations in the `sw_i2c/sample-implementations` folder in this
repository.
