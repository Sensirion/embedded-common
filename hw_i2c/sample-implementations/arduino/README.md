# Arduino Specific README
This guide contains some hints on getting the generic reference drivers working
on Arduino platforms. Note that this is a generic driver meant to be adaptable
to any platform with minimal effort but it is not, by design, a ready-to-use
Arduino driver.

Kindly keep in mind that Sensirion cannot assist with general Arduino questions
not related to the driver.

## Project Setup
Arduino compiles all files in a Library (See
[Build-Process](https://github.com/arduino/Arduino/wiki/Build-Process)). Thus,
for Arduino to pick up the driver as a library all files must be in a directory
with the name of that library and it must be in the folder where your Arduino
IDE installs libraries.

For the sake of this tutorial we'll call this the `sxx30` folder, where _sxx_ is
substituted for the actual project name (name of your _.ino_ file). We suggest
using the driver's name as project name e.g. _sgp30_, _sps30_, _shtc1_ etc.

### Files to copy to your Library folder
Use as base the *release* version of your driver from `make release` or, if
you're not sure what this means, download the release Zip from the driver's
releases page on GitHub, e.g. for the embedded-sgp driver download
_sgp30-x.y.z.zip_ from
[https://github.com/Sensirion/embedded-sgp/releases](https://github.com/Sensirion/embedded-sgp/releases)

* Copy all content except the example usage file (`example_usage.c` or
  `sxx30_example_usage.c`) from the release folder into the `sxx30` library
  folder.
* Copy the Arduino sample implementation
  `sensirion_hw_i2c_implementation.cpp` from this folder into the `sxx30`
  library folder.

### A Typical Example Project

```cpp
#include <Arduino.h>

#include "sxx30.h" /* Adapt for your sensor */


void setup() {
    /*
    // Initialize Serial for debugging
    Serial.begin(115200);

    while (!Serial) {
      delay(100);
    }
    */
}

void loop() {
    // adapt the code from the main() method content from sxx30_example_usage.c
    // make sure to replace any sleep(x) calls with delay(x * 1000)
}
```

### sensirion\_arch\_config.h
Copy the `sensirion_arch_config.h` from this folder to your `sxx30` folder.
Note that it may need additional tweaks according to your specific arduino
platform. If needed, you should ask for help on an Arduino forum for your
specific platform as Sensirion is not in a position to help you with such
questions.

### Board-Specific Setup
Let us know if your Arduino needs a tweak and we'll be happy to document your
changes here.

## Common Compile Errors
This section lists some of the most common compile errors encountered along with
possible solutions. If your error does not appear in this list, your error is
likely not related to the driver and you should ask for help on an Arduino forum
for your specific platform as Sensirion is not in a position to help you with
such questions.

### Conflicting type declaration
If you encounter errors similar to the one below:

    arduino_sps30/sensirion_arch_config.h:46:14: error: conflicting declaration
    'typedef long int s32'
     typedef long s32;
                  ^
    In file included from
    C:\Users\foo\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.4.2\cores\esp8266/esp8266_peri.h:24:0,

    error: 's32' has a previous declaration as 'typedef int s32'
     typedef signed int          s32;

Typically, you should be able to adapt the Sensirion typedef to whatever the
conflicting declaration is. Try to redefine it to the conflicting definitions:
`typedef signed int s32` - the compiler should then let you off with a warnings.

If this works for your platform, please let us know by opening a GitHub issue
stating your Arduino model along with the typedef changes needed, such that we
can add them to the list above.

Otherwise, change every conflicting type in the sensirion driver to avoid the
conflict by using another type name, e.g. rename all instances of `s32` to
`sensirion_s32` etc. and update the typedef accordingly in
`sensirion_arch_config.h`:

Redefine conflicting types (pick as needed)

```c
typedef unsigned long long int sensirion_u64;
typedef long long int sensirion_s64;
typedef long sensirion_s32;
typedef unsigned long sensirion_u32;
typedef short sensirion_s16;
typedef unsigned short sensirion_u16;
typedef char sensirion_s8;
typedef unsigned char sensirion_u8;
typedef float sensirion_f32;
```

### Sleep was not declared in this scope
Delete the example usage file or replace `sleep(x)` with `delay(x * 1000)`
Multiply by 1000 because delay uses milliseconds but sleep uses seconds.

    error: 'sleep' was not declared in this scope

## Hardware

### Limited Receive Buffer
Arduino's Wire library has a limited receive buffer of 30bytes, which is not
enough for the SPS30. A possible workaround is using a software-i2c
implementation. See [embedded-sps #7](https://github.com/Sensirion/embedded-sps/issues/7)
for more details.
