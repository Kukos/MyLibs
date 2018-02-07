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

/* Polynomial: x^{8}+x^{7}+x^{6}+x^{4}+x^{2}+1  = 0xD5 */
uint8_t  crc8(const void *data, size_t size);

/* Polynomial x^{16} + x^{12} + x^5 + 1  = 0x1021 */
uint16_t crc16(const void *data, size_t size);

/* Polynomial x^{32}+x^{26}+x^{23}+x^{22}+x^{16}+x^{12}+x^{11}+x^{10}+x^{8}+x^{7}+x^{5}+x^{4}+x^{2}+x+1 = 0x04C11DB7 */
uint32_t crc32(const void *data, size_t size);

/* Polynomial x^{64}+x^{4}+x^{3}+x+1 = 0x000000000000001B */
uint64_t crc64(const void *data, size_t size);

#endif