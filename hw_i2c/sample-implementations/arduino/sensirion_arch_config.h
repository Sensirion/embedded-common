/*
 * Copyright (c) 2019, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SENSIRION_ARCH_CONFIG_H
#define SENSIRION_ARCH_CONFIG_H

// #include <Arduino.h> // uncomment if needed

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long long int uint64_t;
typedef long long int int64_t;
typedef long int32_t;
typedef unsigned long uint32_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef char int8_t;
typedef unsigned char uint8_t;
typedef float float32_t;

/*
 * If you encounter errors similar to this
 *     error: 'int32_t' has a previous declaration as 'typedef int int32_t'
 *
 * Change every conflicting type in the sensirion driver to avoid the conflict
 by
 * using another name, e.g. rename all instances of `int32_t` to `sensirion_s32`
 etc.
 * and change the typedef accordingly in the typedef list above
 *
 * Redefine conflicting types (pick as needed)

 * typedef unsigned long long int sensirion_u64;
 * typedef long long int sensirion_s64;
 * typedef long sensirion_s32;
 * typedef unsigned long sensirion_u32;
 * typedef short sensirion_s16;
 * typedef unsigned short sensirion_u16;
 * typedef char sensirion_s8;
 * typedef unsigned char sensirion_u8;
 * typedef float sensirion_f32;
 */

/**
 * Define the endianness of your architecture:
 * 0: little endian, 1: big endian
 * Use the following code to determine if unsure:
 * ```c
 * #include <stdio.h>
 *
 * int is_big_endian(void) {
 *     union {
 *         unsigned int u;
 *         char c[sizeof(unsigned int)];
 *     } e = { 0 };
 *     e.c[0] = 1;
 *
 *     return (e.i != 1);
 * }
 *
 * int main(void) {
 *     printf("Use #define SENSIRION_BIG_ENDIAN %d\n", is_big_endian());
 *
 *     return 0;
 * }
 * ```
 */
#define SENSIRION_BIG_ENDIAN 0

#ifdef __cplusplus
}
#endif

#endif /* SENSIRION_ARCH_CONFIG_H */
