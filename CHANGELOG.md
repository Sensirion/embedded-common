# Changelog

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

 * [`changed`] Structure of embedded-common. File names should be more logical
               and the spliting of the different parts of the code as well.
               Embedded-common now splites the code into three `.h/.c` file pairs
               and a configuration `.h` file.
               First there are the three files in `common/`. `sensirion_config.h`
               is similar to `sensirion_arch_config.h` from before. It contains
               configurations for the code to be edited by the user.
               Then there is the `sensirion_common.h/c` pair which contains helper
               functions which are not protocol spesific, at the moment it's mostly
               type convertions.
               Secondly there are the two other file pairs in `i2c/`. The pair
               `sensirion_i2c.h/c` contains all functions for the i2c
               implementation of Sensirion. `sensirion_i2c_hal.h` describes the
               hardware abstraction layer used by `sensirion_i2c.c`. This hal can
               be implemented in `sensirion_hw_i2c_implementation.c` which is
               located in `i2c/hw`. If no HW controller for I2C is present one can
               also implement the GPIO SW-I2C implentation in
               `i2c/sw/sensirion_sw_i2c_implementation.c`.


## [0.1.0] - 2020-11-04

 * First release with support for I2C using a HW controller or SW bit-banging, 
   with sample implementations for common platforms and helper functions.

[Unreleased]: https://github.com/Sensirion/embedded-common/compare/0.1.0...master
[0.1.0]: https://github.com/Sensirion/embedded-common/releases/tag/0.1.0

