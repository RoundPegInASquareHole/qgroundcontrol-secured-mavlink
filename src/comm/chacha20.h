#ifndef __CHACHA20_H
#define __CHACHA20_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Converts a 32-bit unsigned integer to a little-endian byte array.
 *
 * This function converts the given 32-bit unsigned integer `v` into a little-endian
 * byte array `p`. The byte array must have at least 4 bytes allocated. Each byte
 * of the integer `v` is stored in the array `p` in little-endian order.
 *
 * @param v The 32-bit unsigned integer to be converted.
 * @param p Pointer to the byte array where the result will be stored.
 *           This array must have at least 4 bytes allocated.
 */
static inline void u32t8le(uint32_t v, uint8_t p[4]) 
{
    p[0] = v & 0xff;
    p[1] = (v >> 8) & 0xff;
    p[2] = (v >> 16) & 0xff;
    p[3] = (v >> 24) & 0xff;
}

/**
 * @brief Converts a little-endian byte array to a 32-bit unsigned integer.
 *
 * This function converts a little-endian byte array `p` into a 32-bit unsigned integer.
 * The byte array must have at least 4 bytes allocated. The bytes are interpreted
 * as little-endian, with the least significant byte stored first.
 *
 * @param p Pointer to the byte array containing the little-endian data.
 *           This array must have at least 4 bytes allocated.
 * @return The 32-bit unsigned integer value reconstructed from the byte array `p`.
 */
static inline uint32_t u8t32le(uint8_t p[4]) 
{
    uint32_t value = p[3];

    value = (value << 8) | p[2];
    value = (value << 8) | p[1];
    value = (value << 8) | p[0];

    return value;
}

/**
 * @brief Rotates a 32-bit unsigned integer left by a specified number of bits.
 *
 * This function rotates the 32-bit unsigned integer `x` left by `n` bits.
 * The rotation is done using a bitwise left shift operation (`<<`) and a bitwise
 * OR operation (`|`). The rotation amount `n` is taken modulo 32, so any value
 * of `n` is valid.
 *
 * @param x The 32-bit unsigned integer to be rotated.
 * @param n The number of bits by which to rotate `x` left.
 * @return The result of rotating `x` left by `n` bits.
 */
static inline uint32_t rotl32(uint32_t x, int n) 
{
    return x << n | (x >> (-n & 31));
}

/**
 * @brief Performs a ChaCha20 quarter round operation.
 *
 * This function applies the ChaCha20 quarter round operation to the provided
 * ChaCha20 state array `x`. The quarter round operation involves a series of
 * addition, XOR, and rotation operations performed on specific elements of the
 * state array.
 *
 * @param x Pointer to the ChaCha20 state array.
 * @param a Index of the first element in the quarter round operation.
 * @param b Index of the second element in the quarter round operation.
 * @param c Index of the third element in the quarter round operation.
 * @param d Index of the fourth element in the quarter round operation.
 * 
 * @see https://tools.ietf.org/html/rfc7539#section-2.1
 */
static void chacha20_quarterround(uint32_t *x, int a, int b, int c, int d) 
{
    x[a] += x[b]; x[d] = rotl32(x[d] ^ x[a], 16);
    x[c] += x[d]; x[b] = rotl32(x[b] ^ x[c], 12);
    x[a] += x[b]; x[d] = rotl32(x[d] ^ x[a],  8);
    x[c] += x[d]; x[b] = rotl32(x[b] ^ x[c],  7);
}

/**
 * @brief Serializes a ChaCha20 state array into a byte array.
 *
 * This function serializes the provided ChaCha20 state array `in` into a byte array `output`.
 * Each 32-bit integer in the state array is converted to a little-endian byte sequence and
 * stored in the output array. The output array must have at least 64 bytes allocated.
 *
 * @param in Pointer to the ChaCha20 state array to be serialized.
 * @param output Pointer to the byte array where the serialized state will be stored.
 *               This array must have at least 64 bytes allocated.
 */
static void chacha20_serialize(uint32_t in[16], uint8_t output[64]) 
{
    int i;
    for (i = 0; i < 16; i++) {
        u32t8le(in[i], output + (i << 2));
    }
}

/**
 * @brief Performs a ChaCha20 block operation.
 *
 * This function performs a ChaCha20 block operation using the provided ChaCha20 state array `in`.
 * The block operation consists of a specified number of ChaCha20 quarter rounds applied iteratively
 * to the state array, followed by addition of the original state to the resulting state, and finally,
 * serialization of the resulting state into a byte array `out`.
 *
 * @param in Pointer to the ChaCha20 state array representing the input to the block operation.
 * @param out Pointer to the byte array where the resulting block will be stored.
 *            This array must have at least 64 bytes allocated.
 * @param num_rounds The number of rounds to perform the ChaCha20 block operation.
 *                   Typically, this value is a multiple of 2, such as 20 or 12, as specified by the ChaCha20 algorithm.
 */
static void chacha20_block(uint32_t in[16], uint8_t out[64], int num_rounds) 
{
    int i;
    uint32_t x[16];

    memcpy(x, in, sizeof(uint32_t) * 16);

    for (i = num_rounds; i > 0; i -= 2) {
        chacha20_quarterround(x, 0, 4,  8, 12);
        chacha20_quarterround(x, 1, 5,  9, 13);
        chacha20_quarterround(x, 2, 6, 10, 14);
        chacha20_quarterround(x, 3, 7, 11, 15);
        chacha20_quarterround(x, 0, 5, 10, 15);
        chacha20_quarterround(x, 1, 6, 11, 12);
        chacha20_quarterround(x, 2, 7,  8, 13);
        chacha20_quarterround(x, 3, 4,  9, 14);
    }

    for (i = 0; i < 16; i++) {
        x[i] += in[i];
    }

    chacha20_serialize(x, out);
}

/**
 * @brief Initializes the ChaCha20 state array.
 *
 * This function initializes the ChaCha20 state array `s` using the provided key,
 * counter, and nonce. The state array represents the initial state of the ChaCha20
 * cipher before encryption or decryption begins.
 *
 * @param s Pointer to the ChaCha20 state array to be initialized.
 * @param key Pointer to the 256-bit (32-byte) key used for encryption or decryption.
 *            The key must be provided as a byte array.
 * @param counter Initial value of the 32-bit counter used in the ChaCha20 block function.
 * @param nonce Pointer to the 96-bit (12-byte) nonce value. Nonce is used to generate a
 *              unique stream for each encryption/decryption operation.
 * 
 * @see https://tools.ietf.org/html/rfc7539#section-2.3
 */
static void chacha20_init_state(uint32_t s[16], uint8_t key[32], uint32_t counter, uint8_t nonce[12]) 
{
    int i;

    // Constant used in ChaCha state arrayhttps://dxr.mozilla.org/mozilla-beta/source/security/nss/lib/freebl/chacha20.c
    s[0] = 0x61707865;
    s[1] = 0x3320646e;
    s[2] = 0x79622d32;
    s[3] = 0x6b206574;

    for (i = 0; i < 8; i++) {
        s[4 + i] = u8t32le(key + i * 4);
    }

    s[12] = counter;

    for (i = 0; i < 3; i++) {
        s[13 + i] = u8t32le(nonce + i * 4);
    }
}

/**
 * @brief Performs ChaCha20 encryption or decryption with XOR operation.
 *
 * This function encrypts or decrypts the input data `in` using the ChaCha20 cipher
 * algorithm in counter mode with a XOR operation. The resulting ciphertext or
 * plaintext is stored in the output array `out`.
 *
 * @param key Pointer to the 256-bit (32-byte) key used for encryption or decryption.
 *            The key must be provided as a byte array.
 * @param counter Initial value of the 32-bit counter used in the ChaCha20 block function.
 * @param nonce Pointer to the 96-bit (12-byte) nonce value. Nonce is used to generate a
 *              unique stream for each encryption/decryption operation.
 * @param in Pointer to the input data to be encrypted or decrypted.
 * @param out Pointer to the output buffer where the resulting ciphertext or plaintext
 *            will be stored. The output buffer must be preallocated and have sufficient
 *            space to accommodate the output data.
 * @param inlen Length of the input data in bytes.
 */
static void ChaCha20XOR(uint8_t key[32], uint32_t counter, uint8_t nonce[12], uint8_t *in, uint8_t *out, int inlen)
{
    int i, j;

    uint32_t s[16];
    uint8_t block[64];

    chacha20_init_state(s, key, counter, nonce);

    for (i = 0; i < inlen; i += 64) {
        chacha20_block(s, block, 20);
        s[12]++;

        for (j = i; j < i + 64; j++) {
            if (j >= inlen) {
                break;
            }
            out[j] = in[j] ^ block[j - i];
        }
    }
}

[[maybe_unused]] static void hex_print(uint8_t* pv,uint16_t s,uint16_t len)
{
    uint8_t * p = pv;

    if (NULL == pv)
        printf("NULL");
    else
    {
        unsigned int i ;
        for (i=s; i<len;++i)
        printf("%02x ", p[i]);
    }

    printf("\n\n");
}

#endif
