//see LICENSE for license
#ifndef _RISCV_AES_ROCC_H
#define _RISCV_AES_ROCC_H

#include "rocc.h"
#include "mmu.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define AES_KEYLEN 32
#define AES_IVLEN 16

class aes_t : public rocc_t
{
public:
  aes_t() {};

  const char* name() { return "aes"; }

  void reset()
  {
    key_addr = 0;
    iv_addr = 0;
    msg_addr = 0;
    cipher_addr = 0;
    msg_len = 0;
  }

  reg_t custom0(rocc_insn_t insn, reg_t xs1, reg_t xs2)
  {
    switch (insn.funct)
    {
      case 0: // setup plaintext and ciphertext address
        msg_addr = xs1;
        cipher_addr = xs2;
        break;

      case 1: // setup key and iv address
        key_addr = xs1;
        iv_addr = xs2;
        break;

      case 2: // setup msg length and run
        msg_len = xs1;

        // read key into buffer
	uint8_t key[AES_KEYLEN];
        for(uint32_t i = 0; i < AES_KEYLEN; i++)
          key[i] = p->get_mmu()->load_uint8(key_addr + i);

        // read iv into buffer
	uint8_t iv[AES_IVLEN];
        for(uint32_t i = 0; i < AES_IVLEN; i++)
          iv[i] = p->get_mmu()->load_uint8(iv_addr + i);

        // read message into buffer
	uint8_t *input;
        input  = (uint8_t*)malloc(msg_len*sizeof(uint8_t));
        for(uint32_t i = 0; i < msg_len; i++)
          input[i] = p->get_mmu()->load_uint8(msg_addr + i);
          
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CTR_xcrypt_buffer(&ctx, input, 32);
	// result is writen to input

        // write output
        for(uint32_t i = 0; i < msg_len; i++)
          p->get_mmu()->store_uint8(cipher_addr + i, input[i]);
        
        // clean up
        free(input);

        break;

      default:
        illegal_instruction();
    }

    return -1; // accelerator currently returns nothing
  }

private:
  reg_t key_addr;
  reg_t iv_addr;
  reg_t msg_addr;
  reg_t cipher_addr;
  reg_t msg_len;

// #define the macros below to 1/0 to enable/disable the mode of operation.
//
// CBC enables AES encryption in CBC-mode of operation.
// CTR enables encryption in counter-mode.
// ECB enables the basic ECB 16-byte block algorithm. All can be enabled simultaneously.

// The #ifndef-guard allows it to be configured before #include'ing or at compile time.
#ifndef CBC
  #define CBC 1
#endif

#ifndef ECB
  #define ECB 1
#endif

#ifndef CTR
  #define CTR 1
#endif


//#define AES128 1
//#define AES192 1
#define AES256 1

#define AES_BLOCKLEN 16 //Block length in bytes AES is 128b block only

#if defined(AES256) && (AES256 == 1)
    #define AES_KEYLEN 32
    #define AES_keyExpSize 240
#elif defined(AES192) && (AES192 == 1)
    #define AES_KEYLEN 24
    #define AES_keyExpSize 208
#else
    #define AES_KEYLEN 16   // Key length in bytes
    #define AES_keyExpSize 176
#endif

struct AES_ctx
{
  uint8_t RoundKey[AES_keyExpSize];
#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
  uint8_t Iv[AES_BLOCKLEN];
#endif
};

void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);
#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
void AES_init_ctx_iv(struct AES_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void AES_ctx_set_iv(struct AES_ctx* ctx, const uint8_t* iv);
#endif

#if defined(ECB) && (ECB == 1)
// buffer size is exactly AES_BLOCKLEN bytes; 
// you need only AES_init_ctx as IV is not used in ECB 
// NB: ECB is considered insecure for most uses
void AES_ECB_encrypt(const struct AES_ctx* ctx, uint8_t* buf);
void AES_ECB_decrypt(const struct AES_ctx* ctx, uint8_t* buf);
#endif // #if defined(ECB) && (ECB == 1)

#if defined(CBC) && (CBC == 1)
// buffer size MUST be mutile of AES_BLOCKLEN;
// Suggest https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
// NOTES: you need to set IV in ctx via AES_init_ctx_iv() or AES_ctx_set_iv()
//        no IV should ever be reused with the same key 
void AES_CBC_encrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, uint32_t length);
void AES_CBC_decrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, uint32_t length);
#endif // #if defined(CBC) && (CBC == 1)

#if defined(CTR) && (CTR == 1)
// Same function for encrypting as for decrypting. 
// IV is incremented for every block, and used after encryption as XOR-compliment for output
// Suggesting https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
// NOTES: you need to set IV in ctx with AES_init_ctx_iv() or AES_ctx_set_iv()
//        no IV should ever be reused with the same key 
void AES_CTR_xcrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, uint32_t length);
#endif // #if defined(CTR) && (CTR == 1)

typedef uint8_t aes_state_t[4][4];
void KeyExpansion(uint8_t* RoundKey, const uint8_t* Key);
void AddRoundKey(uint8_t round, aes_state_t* state, const uint8_t* RoundKey);
void SubBytes(aes_state_t* state);
void ShiftRows(aes_state_t* state);
uint8_t xtime(uint8_t x);
void MixColumns(aes_state_t* state);
void InvMixColumns(aes_state_t* state);
void InvSubBytes(aes_state_t* state);
void InvShiftRows(aes_state_t* state);
void Cipher(aes_state_t* state, const uint8_t* RoundKey);
void InvCipher(aes_state_t* state, const uint8_t* RoundKey);
void XorWithIv(uint8_t* buf, const uint8_t* Iv);

};
REGISTER_EXTENSION(aes, []() { return new aes_t; })
#endif
