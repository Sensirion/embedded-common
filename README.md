# embedded-common
This repository contains the implementation of the Sensirion I2C and SHDLC
protocol and some commonly used functions for sensor implementations.
It is a dependency of the Sensirion embedded sensor drivers. Sensirion
sensors all use a protocol on top of the I2C/UART communications layer.
There are two of those kinds of protocols used by Sensirion sensors, one
for UART communication, called SHDLC, and one for I2C communication.
To keep this implementation in one place only, this repository exists and is
used by all embedded drivers.

**It should not be necessary to clone this repository independently**, and we
recommend to clone the repository recursively from the respective projects.

If you're looking for sensor drivers, please refer to the release bundles (zip
files) from the respective projects. An overview is listed at
https://github.com/Sensirion/info#repositories

## Repository content

The source code in this repository is spilt into three parts: Common code,
I2C protocol and SHDLC protocol.

### Common

In the `common/` folder are three files. `senirion_config.h` is the general
configuration file. Depending on your hardware you will need to edit this.
For more information, see the section named `sensirion_config.h` further down
in this document. There are also the two files `sensirion_common.[ch]`. These
contain common helper functions used by both UART and I2C sensor drivers.

### I2C

In the `i2c/` folder is the implementation of the protocol used by Sensirion
sensors for I2C communication. This implementation is spilt into two parts.
The implementation of the high level protocol in the files `sensirion_i2c.[ch]`
and a hardware abstraction layer in `senirion_i2c_hal.[ch]`.
In `sensirion_i2c.[ch]` all the implementation details (CRC calculation, byte order
swapping, ...) of Sensirion's I2C communication are implemented. The finished frame
is then sent to the attached hardware using the Hardware Abstraction Layer (HAL) in
the other two files. Since the way how to send out this frame depends on underlying
hardware and operating system, we provide different implementations for that part
of the code. You can find these in the folder `i2c/sample-implementations/` for the
most common platforms. We even provide a implementation for software I2C using
bit-banging on GPIOs in `i2c/sample-implementations/GPIO_bit_banging/`. If we
don't provide code for your platform yet you can use the provided template structure
in the `i2c/` folder to implement it yourself. We're very happy to review and include
more architectures in the form of a pull request on GitHub.

### SHDLC

The `shdlc` folder contains the implementation of the protocol used by Sensirion
sensors for UART communication. These sensors use Sensirion's own SHDLC protocol.
The high level implementation of this protocol can be found in
`sensirion_shdlc.[ch]` and the hardware abstraction layer in `sensirion_uart_hal.[ch]`.
In `sensirion_shdlc.[ch]` all the implementation details (checksum calculation,
byte order swapping, frame construction, ...) of Sensirion's SHDLC communication are
implemented. The finished frame is then sent to your hardware using the HAL
in the other two files. Since the way how to send
out this frame depends on underlying hardware and operating system we provide
different implementations for that part of the code. You can find these in the
folder `shdlc/sample-implementaions/` for the most common platforms. If we
we don't provide code for your platform yet, you can use the provided template
structure in the `shdlc/` folder to implement it yourself. We're very happy to
review and include more architectures in the form of a pull request on GitHub.

## Files to be adjusted by the User

The following files are the only ones you should need to change if you want to
use the functionality of `embedded-common`. Here we explain which changes need
to be made according to your hardware platform.

### sensirion\_config.h

In this file we keep all the included libraries for our drivers. It is located
in the `common/` folder. The two libraries our drivers use are `<stdint.h>` and
`<stdlib.h>`. The features we use out of those libraries is standard integer sizes
from `<stdint.h>` and `NULL` from `<stdlib.h>`. We assume that those libraries
are provided by your toolchain, compiler or are available on your system, so we have
include directives for them. If this is not the case on your platform you need to
specify the following integer types yourself:

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
on Sensirion's I2C HAL. It is defined in `sensirion_i2c_hal.h` and implemented in
the corresponding `.c` file, both located in the `i2c/` folder. To use the I2C part
of `embedded-common` you either need to implement the functions in sensirion_i2c_hal.c or
replace it with a sample implementation from i2c/sample-implementation/.
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
