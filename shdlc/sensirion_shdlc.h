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

#ifndef SENSIRION_SHDLC_H
#define SENSIRION_SHDLC_H

#include "sensirion_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SENSIRION_SHDLC_ERR_NO_DATA -1
#define SENSIRION_SHDLC_ERR_MISSING_START -2
#define SENSIRION_SHDLC_ERR_MISSING_STOP -3
#define SENSIRION_SHDLC_ERR_CRC_MISMATCH -4
#define SENSIRION_SHDLC_ERR_ENCODING_ERROR -5
#define SENSIRION_SHDLC_ERR_TX_INCOMPLETE -6
#define SENSIRION_SHDLC_ERR_FRAME_TOO_LONG -7
#define SENSIRION_SHDLC_ERR_EXECUTION_FAILURE -8
#define SENSIRION_SHDLC_ERR_RX_ADDRESS -9
#define SENSIRION_SHDLC_ERR_RX_COMMAND -10

struct sensirion_shdlc_frame {
    uint8_t* buffer;
    uint16_t buffer_size;
    uint16_t offset;
    uint8_t checksum;
    uint8_t address;
    uint8_t command;
    uint8_t state;
    uint8_t data_length;
};

/**
 * sensirion_shdlc_initialize_frame() - initialize sensirion_shdlc_frame struct
 *
 * @param frame  frame to initialize
 * @param buffer buffer to store data of the frame in
 * @param buffer_size size of the buffer to store data in
 */
void sensirion_shdlc_initialize_frame(struct sensirion_shdlc_frame* frame,
                                      uint8_t* buffer, uint16_t buffer_size);

/**
 * sensirion_shdlc_xcv() - transceive (transmit then receive) an SHDLC frame
 *
 * @param tx_frame frame struct containing tx frame to send
 * @param rx_frame frame struct to store rx frame in
 * @param expected_rx_data number of bytes expected to receive
 *
 * @return            NO_ERROR on success, an error code otherwise.
 */
int16_t sensirion_shdlc_xcv(struct sensirion_shdlc_frame* tx_frame,
                            struct sensirion_shdlc_frame* rx_frame,
                            uint16_t expected_rx_data, uint32_t delay_usec);

/**
 * sensirion_shdlc_add_uint8_t_to_frame() - Add a uint8_t to the frame at
 *                                          offset.
 *
 * @param tx_frame Pointer to buffer in which the write frame will be prepared.
 *                 Caller needs to make sure that there is enough space left in
 *                 the buffer pointed to by the data member after offset in
 *                 tx_frame, to write the data.
 * @param data     uint8_t to be written into the frame.
 */
void sensirion_shdlc_add_uint8_t_to_frame(
    struct sensirion_shdlc_frame* tx_frame, uint8_t data);

/**
 * sensirion_shdlc_begin_frame() - Initialize buffer and add the first three
 *                                 fixed-use data bytes to it.
 *
 * @param tx_frame    Pointer to struct sensirion_shdlc_frame, all members will
 *                    be initialized in this function.
 * @param buffer      The pointer to a big enough buffer to write the whole
 *                    tx_frame. Caller needs to initialize this buffer.
 * @param command     Command to be written into the frame.
 * @param address     Address to be written into the frame.
 * @param data_length Number of bytes of data to be written into the frame after
 *                    this method.
 */
void sensirion_shdlc_begin_frame(struct sensirion_shdlc_frame* tx_frame,
                                 uint8_t command, uint8_t address,
                                 uint8_t data_length);

/**
 * sensirion_shdlc_add_uint32_t_to_frame() - Add a uint32_t to the frame at
 *                                           offset.
 *
 * @param tx_frame Pointer to buffer in which the write frame will be prepared.
 *                 Caller needs to make sure that there is enough space left in
 *                 the buffer pointed to by the data member after offset in
 *                 tx_frame, to write the data.
 * @param data     uint32_t to be written into the frame.
 */
void sensirion_shdlc_add_uint32_t_to_frame(
    struct sensirion_shdlc_frame* tx_frame, uint32_t data);

/**
 * sensirion_shdlc_add_int32_t_to_frame() - Add a int32_t to the frame at
 *                                          offset.
 *
 * @param tx_frame Pointer to buffer in which the write frame will be prepared.
 *                 Caller needs to make sure that there is enough space left in
 *                 the buffer pointed to by the data member after offset in
 *                 tx_frame, to write the data.
 * @param data     int32_t to be written into the frame.
 */
void sensirion_shdlc_add_int32_t_to_frame(
    struct sensirion_shdlc_frame* tx_frame, int32_t data);

/**
 * sensirion_shdlc_add_uint16_t_to_frame() - Add a uint16_t to the frame at
 *                                          offset.
 *
 * @param tx_frame Pointer to buffer in which the write frame will be prepared.
 *                 Caller needs to make sure that there is enough space left in
 *                 the buffer pointed to by the data member after offset in
 *                 tx_frame, to write the data.
 * @param data     uint16_t to be written into the frame.
 */
void sensirion_shdlc_add_uint16_t_to_frame(
    struct sensirion_shdlc_frame* tx_frame, uint16_t data);

/**
 * sensirion_shdlc_add_int16_t_to_frame() - Add a int16_t to the frame at
 *                                         offset.
 *
 * @param tx_frame Pointer to buffer in which the write frame will be prepared.
 *                 Caller needs to make sure that there is enough space left in
 *                 the buffer pointed to by the data member after offset in
 *                 tx_frame, to write the data.
 * @param data     int16_t to be written into the frame.
 */
void sensirion_shdlc_add_int16_t_to_frame(
    struct sensirion_shdlc_frame* tx_frame, int16_t data);

/**
 * sensirion_shdlc_add_float_to_frame() - Add a float to the frame at offset.
 *
 * @param tx_frame Pointer to buffer in which the write frame will be prepared.
 *                 Caller needs to make sure that there is enough space left in
 *                 the buffer pointed to by the data member after offset in
 *                 tx_frame, to write the data.
 * @param data     float to be written into the frame.
 */
void sensirion_shdlc_add_float_to_frame(struct sensirion_shdlc_frame* tx_frame,
                                        float data);

/**
 * sensirion_shdlc_add_bytes_to_frame() - Add a byte array to the frame at
 *                                        offset.
 *
 * @param tx_frame    Pointer to buffer in which the write frame will be
 *                    prepared. Caller needs to make sure that there is enough
 *                    space left in the buffer pointed to by the data member
 *                    after offset in tx_frame, to write the data.
 * @param data        Pointer to data to be written into the frame.
 * @param data_length Number of bytes to be written into the frame.
 */
void sensirion_shdlc_add_bytes_to_frame(struct sensirion_shdlc_frame* tx_frame,
                                        uint8_t* data, uint16_t data_length);

/**
 * sensirion_shdlc_finish_frame() - Write closing part of the frame.
 *
 * @param tx_frame Pointer to buffer in which the write frame will be prepared.
 *                 Caller needs to make sure that there is enough space left
 *                 in the buffer pointed to by the data member after offset
 *                 in tx_frame, to write 3 bytes.
 */
void sensirion_shdlc_finish_frame(struct sensirion_shdlc_frame* tx_frame);

/**
 * sensirion_shdlc_tx() - Transmit the SHDLC frame.
 *
 * @param tx_frame    Pointer to buffer in which the write frame was prepared.
 *
 * @return            NO_ERROR on success, an error code otherwise.
 */
int16_t sensirion_shdlc_tx(struct sensirion_shdlc_frame* tx_frame);

/**
 * sensirion_shdlc_rx_inplace() - Receive an SHDLC frame in a prepared buffer.
 *
 * @note The header and data must be discarded on failure
 *
 * @param rx_frame             Pointer to buffer in which the RX frame will be
 *                             received in and the return data will be stored
 *                             in. Offset and checksum member will be
 *                             initialized by this function, but
 * rx_frame->buffer needs to point to a buffer big enough to store the whole
 * unprocessed RX frame in.
 * @param expected_data_length Expected data amount to receive.
 * @param header               Memory where the SHDLC header containing the
 *                             sender address, command, state and data_length
 *                             is stored.
 *
 * @return            NO_ERROR on success, an error code otherwise
 */
int16_t sensirion_shdlc_rx(struct sensirion_shdlc_frame* rx_frame,
                           uint8_t expected_data_length);

#ifdef __cplusplus
}
#endif

#endif /* SENSIRION_SHDLC_H */
