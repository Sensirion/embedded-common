/*
 * Copyright (c) 2018, Sensirion AG
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

#include "sensirion_shdlc.h"
#include "sensirion_common.h"
#include "sensirion_config.h"
#include "sensirion_uart_hal.h"

#define SHDLC_START 0x7e
#define SHDLC_STOP 0x7e

#define SHDLC_MIN_TX_FRAME_SIZE 6
/** start/stop + (4 header + 255 data) * 2 because of byte stuffing */
#define SHDLC_FRAME_MAX_TX_FRAME_SIZE (2 + (4 + 255) * 2)

/** start/stop + (5 header + 255 data) * 2 because of byte stuffing */
#define SHDLC_FRAME_MAX_RX_FRAME_SIZE (2 + (5 + 255) * 2)

#define RX_DELAY_US 20000

void sensirion_shdlc_initialize_frame(struct sensirion_shdlc_frame* frame,
                                      uint8_t* buffer, uint16_t buffer_size) {
    frame->buffer = buffer;
    frame->buffer_size = buffer_size;
    frame->offset = 0;
    frame->checksum = 0;
    frame->address = 0;
    frame->state = 0;
    frame->data_length = 0;
}

int16_t sensirion_shdlc_xcv(struct sensirion_shdlc_frame* tx_frame,
                            struct sensirion_shdlc_frame* rx_frame,
                            uint16_t expected_rx_data, uint32_t delay_usec) {
    int16_t error = sensirion_shdlc_tx(tx_frame);
    if (error) {
        return error;
    }

    sensirion_uart_hal_sleep_usec(delay_usec);

    error = sensirion_shdlc_rx(rx_frame, expected_rx_data);
    if (error) {
        return error;
    }
    if (rx_frame->command != tx_frame->command) {
        return SENSIRION_SHDLC_ERR_RX_COMMAND;
    }
    if (rx_frame->address != tx_frame->address) {
        return SENSIRION_SHDLC_ERR_RX_ADDRESS;
    }
    return NO_ERROR;
}

static void sensirion_shdlc_stuff_byte(struct sensirion_shdlc_frame* tx_frame,
                                       uint8_t data) {
    switch (data) {
        case 0x11:
        case 0x13:
        case 0x7d:
        case 0x7e:
            // byte stuffing is done by inserting 0x7d and inverting bit 5
            tx_frame->buffer[tx_frame->offset++] = 0x7d;
            tx_frame->buffer[tx_frame->offset++] = data ^ (1 << 5);
            return;
        default:
            tx_frame->buffer[tx_frame->offset++] = data;
            return;
    }
}

void sensirion_shdlc_add_uint8_t_to_frame(
    struct sensirion_shdlc_frame* tx_frame, uint8_t data) {

    sensirion_shdlc_stuff_byte(tx_frame, data);
    tx_frame->checksum += data;
}

void sensirion_shdlc_begin_frame(struct sensirion_shdlc_frame* tx_frame,
                                 uint8_t command, uint8_t address,
                                 uint8_t data_length) {
    tx_frame->checksum = 0;
    tx_frame->offset = 0;
    tx_frame->buffer[tx_frame->offset++] = SHDLC_START;
    tx_frame->address = address;
    tx_frame->command = command;
    tx_frame->data_length = data_length;
    sensirion_shdlc_add_uint8_t_to_frame(tx_frame, address);
    sensirion_shdlc_add_uint8_t_to_frame(tx_frame, command);
    sensirion_shdlc_add_uint8_t_to_frame(tx_frame, data_length);
}

void sensirion_shdlc_add_uint32_t_to_frame(
    struct sensirion_shdlc_frame* tx_frame, uint32_t data) {

    sensirion_shdlc_add_uint8_t_to_frame(tx_frame,
                                         (uint8_t)((data & 0xFF000000) >> 24));
    sensirion_shdlc_add_uint8_t_to_frame(tx_frame,
                                         (uint8_t)((data & 0x00FF0000) >> 16));
    sensirion_shdlc_add_uint8_t_to_frame(tx_frame,
                                         (uint8_t)((data & 0x0000FF00) >> 8));
    sensirion_shdlc_add_uint8_t_to_frame(tx_frame,
                                         (uint8_t)((data & 0x000000FF) >> 0));
}

void sensirion_shdlc_add_int32_t_to_frame(
    struct sensirion_shdlc_frame* tx_frame, int32_t data) {

    sensirion_shdlc_add_uint32_t_to_frame(tx_frame, (uint32_t)data);
}

void sensirion_shdlc_add_uint16_t_to_frame(
    struct sensirion_shdlc_frame* tx_frame, uint16_t data) {

    sensirion_shdlc_add_uint8_t_to_frame(tx_frame,
                                         (uint8_t)((data & 0xFF00) >> 8));
    sensirion_shdlc_add_uint8_t_to_frame(tx_frame,
                                         (uint8_t)((data & 0x00FF) >> 0));
}

void sensirion_shdlc_add_int16_t_to_frame(
    struct sensirion_shdlc_frame* tx_frame, int16_t data) {

    sensirion_shdlc_add_uint16_t_to_frame(tx_frame, (uint16_t)data);
}

void sensirion_shdlc_add_float_to_frame(struct sensirion_shdlc_frame* tx_frame,
                                        float data) {
    union {
        uint32_t uint32_data;
        float float_data;
    } convert;

    convert.float_data = data;
    sensirion_shdlc_add_uint32_t_to_frame(tx_frame, convert.uint32_data);
}

void sensirion_shdlc_add_bytes_to_frame(struct sensirion_shdlc_frame* tx_frame,
                                        uint8_t* data, uint16_t data_length) {
    uint16_t i;

    for (i = 0; i < data_length; i++) {
        sensirion_shdlc_add_uint8_t_to_frame(tx_frame, data[i]);
    }
}

void sensirion_shdlc_finish_frame(struct sensirion_shdlc_frame* tx_frame) {
    sensirion_shdlc_add_uint8_t_to_frame(tx_frame, ~(tx_frame->checksum));
    tx_frame->buffer[tx_frame->offset++] = SHDLC_STOP;
}

int16_t sensirion_shdlc_tx(struct sensirion_shdlc_frame* tx_frame) {

    int16_t tx_length;

    tx_length = sensirion_uart_hal_tx(tx_frame->offset, tx_frame->buffer);
    if (tx_length < 0) {
        return tx_length;
    }
    if (tx_length != tx_frame->offset) {
        return SENSIRION_SHDLC_ERR_TX_INCOMPLETE;
    }
    return NO_ERROR;
}

static uint8_t
sensirion_shdlc_unstuff_next_byte(struct sensirion_shdlc_frame* rx_frame) {

    uint8_t data = rx_frame->buffer[rx_frame->offset++];

    if (data == 0x7d) {
        data = rx_frame->buffer[rx_frame->offset++];
        data = data ^ (1 << 5);
    }
    rx_frame->checksum += data;
    return data;
}

int16_t sensirion_shdlc_rx(struct sensirion_shdlc_frame* rx_frame,
                           uint8_t expected_data_length) {
    int16_t rx_length;
    uint16_t i;
    rx_frame->offset = 0;
    rx_frame->checksum = 0;

    rx_length = sensirion_uart_hal_rx(
        2 + (5 + (uint16_t)expected_data_length) * 2, rx_frame->buffer);
    if (rx_length < 1 || rx_frame->buffer[rx_frame->offset++] != SHDLC_START) {
        return SENSIRION_SHDLC_ERR_MISSING_START;
    }

    rx_frame->address = sensirion_shdlc_unstuff_next_byte(rx_frame);
    rx_frame->command = sensirion_shdlc_unstuff_next_byte(rx_frame);
    rx_frame->state = sensirion_shdlc_unstuff_next_byte(rx_frame);
    rx_frame->data_length = sensirion_shdlc_unstuff_next_byte(rx_frame);

    if (expected_data_length < rx_frame->data_length) {
        return SENSIRION_SHDLC_ERR_FRAME_TOO_LONG; /* more data than
                                                      expected */
    }

    for (i = 0; i < rx_frame->data_length && rx_frame->offset < rx_length - 2;
         i++) {
        rx_frame->buffer[i] = sensirion_shdlc_unstuff_next_byte(rx_frame);
    }

    if (i < rx_frame->data_length) {
        return SENSIRION_SHDLC_ERR_ENCODING_ERROR;
    }

    sensirion_shdlc_unstuff_next_byte(rx_frame);

    // (CHECKSUM + ~CHECKSUM) = 0xFF
    if (rx_frame->checksum != 0xFF) {
        return SENSIRION_SHDLC_ERR_CRC_MISMATCH;
    }

    if (rx_frame->offset >= rx_length ||
        rx_frame->buffer[rx_frame->offset] != SHDLC_STOP) {
        return SENSIRION_SHDLC_ERR_MISSING_STOP;
    }

    if (0x7F & rx_frame->state) {
        return SENSIRION_SHDLC_ERR_EXECUTION_FAILURE;
    }

    return NO_ERROR;
}
