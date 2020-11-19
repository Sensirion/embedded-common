# embedded-common
This repository contains the implementation of the Sensirion I2C and SHDLC
protocol and some commonly used functions for sensor implementations.
It is a dependency of the Sensirion embedded sensor drivers. Sensirion
senors all use a protocol on top of the bit level communications like I2C or
UART. There are two of those kinds protocols used by Sensirion sensors, one
for UART communication and one for I2C communication. These two work the same
for all sensor types and therefore need to be implemented only once. To keep
this implementation in one place this repository exists and is used by all
embedded drivers.

**It should not be necessary to clone this repository independently**, and we
recommend to clone the repository recursively from the respective projects.

If you're looking for sensor drivers, please refer to the release bundles (zip
files) from the respective projects. An overview is listed at
https://github.com/Sensirion/info#repositories

## Repository content

The source code in this repository can be spilt into three parts.

### Common

In the `common/` folder are three files. `senirion_config.h` is a general
configuration file. Depending on your hardware you will need to edit this.
You can find more about this further down. There are also
`sensirion_common.[ch]`. These contain common helper functions used by both
UART and I2C sensor drivers.

### I2C

In the `i2c/` folder is the implementation of the protocol used by Sensirion
sensors for I2C communication. This implementation is spilt into two parts.
The implementation of the high level protocol in the files `sensirion_i2c.[ch]`
and a hardware abstraction layer in `senirion_i2c_hal.[ch]`.
In `sensirion_i2c.[ch]` all the implementation details (CRC calculation,byte order,
...) of Sensirion's I2C communication are implemented. The finished frame is then
send to your hardware using the HAL (Hardware Abstraction Layer) in the other
two files. Since the way how to send out this frame depends on underlying
hardware and operating system we provide different implementations for that part
of the code. You can find these in the folder `i2c/sample-implementations/` for the
most common platforms. We even provide a implementation for software I2C using
bit-banging on GPIOs in `i2c/sample-implementations/GPIO_bit_banging/`. If your
platform isn't there you can use the empty structure in the `i2c/` folder to
implement it yourself.

### SHDLC

In the `shdlc` folder is the implementation of the protocol used by Sensirion
sensors for UART communication. These sensors use Sensirions own SHDLC protocol.
The high level implementation of this protocol can be found in
`sensirion_shdlc.[ch]` and the hardware abstraction layer in `sensirion_uart_hal.[ch]`.
In `sensirion_shdlc.[ch]` all the implementation details (checksum calculation,
byte order, frame constructon, ...) of Sensirion's SHDLC communication are
implemented. The finished frame is then send to your hardware using the HAL
(Hardware Abstraction Layer) in the other two files. Since the way how to send
out this frame depends on underlying hardware and operating system we provide
different implementations for that part of the code. You can find these in the
folder `shdlc/sample-implementaions/` for the most common platforms. If your
platform isn't there you can use the empty structure in the `shdlc/` folder to
implement it yourself.

## Files to be adjusted by the User

The following files are the only ones you should need to change if you want to
use the functionality of `embedded-common`. Here we explain which changes need
to be made according to your hardware platform.

### sensirion\_config.h

In this file we keep all the included libraries for our drivers. It is located
in the `common/` folder.  The two libraries our drivers use are `<stdint.h>` and
`<stdlib.h>`. The features we use out of those libraries is standard integer sizes
from `<stdint.h>` and NULL from `<stdlib.h>`. We assume you have both those libraries
available on your system, so we have include directives for them. If this is not the
case on your platform you need to specify the following integer types yourself:

* `int64_t` = signed 64bit integer
* `uint64_t` = unsigned 64bit integer
* `int32_t` = signed 32bit integer
* `uint32_t` = unsigned 32bit integer
* `int16_t` = signed 16bit integer
* `uint16_t` = unsigned 16bit integer
* `int8_t` = signed 8bit integer
* `uint8_t` = unsigned 8bit integer

In addition to that you will need to specify `NULL`. For both we have a commented
template where just need to fill in your system specific values.

Make sure you specify the whole range (64bit sized integers are used on a 32bit
system for some sensor data).

### sensirion\_i2c\_hal.c

As already explained earlier is all the I2C related code in this repository based
on Sensirion's I2c HAL. It is defined in `sensirion_i2c_hal.h` and implemented in
the corresponding `.c` file, both located in the `i2c/` folder. To use the I2c part
of `embedded-common` you either need to implement the `sensirion_i2c_hal.c` file or
replace the unimplemented version in `i2c/` with the correct sample implementation
from `i2c/sample-implementation/`.
If you want to use software I2C on GPIOs using bit banging, then there is a sample
implementation for `sensirion_i2c_hal.c` in the `GPIO_bit_banging` folder but then
you will have to implement the `sensirion_sw_i2c.c` file also located there. More
about this in the `README.md` located in the `GPIO_bit_banging` folder.


### sensirion\_shdlc\_hal.c

As already explained earlier is all the SHDLC and UART related code in this repository
based on Sensirion's UART HAL. It is defined in `sensirion_uart_hal.h` and implemented
in the corresponding `.c` file, both located in the `shdlc/` folder. To use the SHDLC
part of `embedded-common` you either need to implement the `sensirion_uart_hal.c`
file or replace the unimplemented version in `shdlc/` with the correct sample
implementation from `shdlc/sample-implementation/`.
