//see LICENSE for license
// The following is a RISC-V program to test the functionality of the
// aes RoCC accelerator.
// Compile with riscv-unknown-elf-gcc aes-rocc.c
// Run with spike --extension=aes pk a.out

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include "aes.h"

// prints string as hex
static void phex(uint8_t *str);

int main() {
    //DO NOT MODIFY
    unsigned char enc_buf[128];
    unsigned char plaintext[1][32] = {
        {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
         0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51}
    };
    unsigned char ciphertext[1][32] = {
        {0x60,0x1e,0xc3,0x13,0x77,0x57,0x89,0xa5,0xb7,0xa7,0xf5,0x04,0xbb,0xf3,0xd2,0x28,
         0xf4,0x43,0xe3,0xca,0x4d,0x62,0xb5,0x9a,0xca,0x84,0xe9,0x90,0xca,0xca,0xf5,0xc5}
    };
    unsigned char iv[1][16] = {
	{0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff},
    };
    unsigned char key[1][32] = {
	{0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,
         0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4}
    };
    unsigned char decrypted_text[32];
    //END DO NOT MODIFY

    // Convert char to int
    // This cast works when no ROCC, but has bug when pass address to ROCC
    //uint8_t *key_uint8 = (uint8_t *) key;
    //uint8_t *iv_uint8 = (uint8_t *) iv;
    //uint8_t *plaintext_uint8 = (uint8_t *) plaintext;

    int dummy_result;
    unsigned int keylen = 32;
    unsigned int ivlen = 16;
    uint32_t msglen = 32;

    uint8_t key_uint8[keylen];
    uint8_t iv_uint8[ivlen];
    uint8_t plaintext_uint8[msglen];
    uint8_t ciphertext_uint8[msglen];
    for (uint8_t i = 0; i < keylen; i++) {
        key_uint8[i] = key[0][i];
        plaintext_uint8[i] = plaintext[0][i];
        ciphertext_uint8[i] = ciphertext[0][i];
    }
    for (uint8_t i = 0; i < ivlen; i++)
        iv_uint8[i] = iv[0][i];

    unsigned long long initCycle, duration;
    initCycle = rdcycle();
    //NOTE: fences are only needed if the accelerator is accessing memory
    asm volatile ("fence");
    //YOUR CODE HERE: Invoke your AES acclerator, write the encrypted
    //output of plaintext to enc_buf
    ROCC_INSTRUCTION(0, dummy_result, &plaintext_uint8, &enc_buf, 0);
    ROCC_INSTRUCTION(0, dummy_result, &key_uint8, &iv_uint8, 1);
    ROCC_INSTRUCTION(0, dummy_result, msglen, 0, 2);
    asm volatile ("fence");

    //printf("encrypt 0x60 == %.2x \n", enc_buf[0]);
    //phex(enc_buf);

    //DO NOT MODIFY
    duration = rdcycle() - initCycle;
    printf("AES Encryption took %llu cycles!\n", duration);
    initCycle = rdcycle();
    //END DO NOT MODIFY

    //YOUR CODE HERE: Invoke your AES acclerator, write the decrypted
    //output of enc_buf to decrypted_text
    asm volatile ("fence");
    ROCC_INSTRUCTION(0, dummy_result, &enc_buf, &decrypted_text, 0);
    ROCC_INSTRUCTION(0, dummy_result, &key_uint8, &iv_uint8, 1);
    ROCC_INSTRUCTION(0, dummy_result, msglen, 0, 2);
    asm volatile ("fence");
    
    //printf("decrypt 0x6b == %.2x \n", decrypted_text[0]);
    //phex(decrypted_text);

    //DO NOT MODIFY
    duration = rdcycle() - initCycle;
    printf("AES Decryption took %llu cycles!\n", duration);

    // Check result
    assert(memcmp(enc_buf, ciphertext[0], msglen) == 0);
    assert(memcmp(decrypted_text, plaintext[0], msglen) == 0);
    //END DO NOT MODIFY

    if (memcmp(enc_buf, ciphertext[0], msglen) == 0) {
        printf("ENCRYPT SUCCESS!\n");
    } else {
        printf("ENCRYPT FAILURE!\n");
    }
    if (memcmp(decrypted_text, plaintext[0], msglen) == 0) {
        printf("DECRYPT SUCCESS!\n");
    } else {
        printf("DECRYPT FAILURE!\n");
    }
    return 0;
}

// prints string as hex
static void phex(uint8_t *str)
{

#if defined(AES256)
    uint8_t len = 32;
#elif defined(AES192)
    uint8_t len = 24;
#elif defined(AES128)
    uint8_t len = 16;
#endif

    unsigned char i;
    for (i = 0; i < len; ++i)
        printf("%.2x", str[i]);
    printf("\n");
}
