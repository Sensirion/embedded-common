# Changelog

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

 * [`added`]   incremental frame construction to SHLDC. This includes a set of
               functions to create a frame step by step. For all used data types
               in a frame there is a method to add it on top of the already built
               frame. This saves memory by using a smaller number of buffers and
               is a useful way to build the frame with generated drivers.
 * [`added`]   incremental frame construction to I2C. This includes a set of
               functions to create a frame step by step. For all used data types
               in a frame there is a method to add it on top of the already built
               frame. This saves memory by using a smaller number of buffers and
               is a useful way to build the frame with generated drivers.
 * [`renamed`] functions to match file name. All functions of embedded-common
               now have their file name as a prefix to the function name. Mostly
               this was already the case but now it's consistent.
 * [`renamed`] `sensirion_i2c_release()` to `sensirion_i2c_hal_free()`.
 * [`renamed`] `sensirion_uart_open()` to `sensirion_uart_hal_init()`.
 * [`renamed`] `sensirion_uart_close()` to `sensirion_uart_hal_free()`.
 * [`renamed`] `sensirion_sleep_usec()` to in Sensirion I2C/UART HAL to
               `sensirion_i2c/uart_hal_sleep_usec()` respectively. This is a
               necessary evil to make use of both the I2C and UART part at the
               same time possible and keep the files to edit minimal. It's true
               that those two functions are exactly the same and will be probably
               implemented the same if the I2C and UART part would be used together,
               but we also would have it implemented twice. This would result in a
               compiler error. Since we don't want to add another file which needs to
               be edited by the user we name them differently.
 * [`added`]   function `sensirion_common_copy_bytes()` to `sensirion_common.[ch]`
               to copy bytes from one buffer to another one.

## [0.2.0] - 2020-11-11

 * [`changed`] `base_config.inc` to work with I2C and SHDLC at the same time.
               Changed HAL implemantation locations and moved the UART one from
               the Makefile of the test in the sensor repo to here. Also changed
               the way how mux sources are included so they are only needed by I2C
               tests.
 * [`added`]   SHDLC implementation. This implementation was imported from the
               `embedded-uart-sps` [repository][embedded-uart-common]. Since it's
               general code which could be used by multiple sensors it makes more
               sense to interate it here. The SHDLC part also uses the files in
               `common/`. In addition to that it adds two `.[ch]` file pairs.
               First `sensirion_shdlc.[ch]` which contains the SHDLC implementation
               for Sensirion sensors. Sencondly `sensirion_uart_hal.h` and
               `sensirion_uart_implementation.c` which describe the hardware
               abstraction layer used by `sensirion_shdlc.c`.
               This hal can be implemented in `sensirion_uart_implentation.c` for
               the platform the sensor will be used on.
 * [`renamed`] `sensirion_arch_config.h` to `sensirion_config.h`. The reason for
               this is that the arch part is not really needed since it's the only
               config in embedded-common.
 * [`changed`] Structure of embedded-common. File names should be more logical
               and the spliting of the different parts of the code as well.
               Embedded-common now splits the code into three `.[ch]` file pairs
               and a configuration `.h` file.
               First there are the three files in `common/`. `sensirion_config.h`
               is similar to `sensirion_arch_config.h` from before. It contains
               configurations for the code to be edited by the user.
               Then there is the `sensirion_common.[ch]` pair which contains helper
               functions which are not protocol spesific, at the moment it's mostly
               type conversions.
               Secondly there are the two other file pairs in `i2c/`. The pair
               `sensirion_i2c.[ch]` contains all functions for the i2c
               implementation of Sensirion. `sensirion_i2c_hal.h` describes the
               hardware abstraction layer used by `sensirion_i2c.c`. This hal can
               be implemented in `sensirion_hw_i2c_implementation.c` which is
               located in `i2c/hw`. If no HW controller for I2C is present one can
               also implement the GPIO SW-I2C implentation in
               `i2c/sw/sensirion_sw_i2c_implementation.c`.


## [0.1.0] - 2020-11-04

 * First release with support for I2C using a HW controller or SW bit-banging, 
   with sample implementations for common platforms and helper functions.

[Unreleased]: https://github.com/Sensirion/embedded-common/compare/0.2.0...master
[0.2.0]: https://github.com/Sensirion/embedded-common/compare/0.1.0...0.2.0
[0.1.0]: https://github.com/Sensirion/embedded-common/releases/tag/0.1.0
[embedded-uart-common]: https://github.com/Sensirion/embedded-uart-sps/tree/f31d181/embedded-uart-common

