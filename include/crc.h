#ifndef CRC_H
#define CRC_H

/*
    Implementation of common crc code 8/16/32/64

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

/*
    All funtions has the same prototype
    uintX_t crcX(const void *data, size_t size);

    Where X = {8, 16, 32, 64}

    PARAMS
    @IN data - pointer to data
    @IN size - sizeof(data)

    RETURN
    crc(data)
*/

#include <stddef.h>
#include <stdint.h>

uint8_t  crc8(const void *data, size_t size);
uint16_t crc16(const void *data, size_t size);
uint32_t crc32(const void *data, size_t size);
uint64_t crc64(const void *data, size_t size);

#endif