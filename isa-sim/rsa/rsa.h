//see LICENSE for license
#ifndef _RISCV_RSA_ROCC_H
#define _RISCV_RSA_ROCC_H

#include "rocc.h"
#include "mmu.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uint128_t.h"

class rsa_t : public rocc_t
{
public:
  rsa_t() {};

  const char* name() { return "rsa"; }

  void reset()
  {
    msg_addr = 0;
    cipher_addr = 0;
    key = 0;
    mod = 0;
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

      case 1: // setup key and modulo values
        key = xs1;
        mod = xs2;
        break;

      case 2: // setup msg length and run encrypt
        msg_len = xs1;

        // read message into buffer
	uint8_t *input;
        input  = (uint8_t*)malloc(msg_len*sizeof(uint8_t));
        for(uint32_t i = 0; i < msg_len; i++)
          input[i] = p->get_mmu()->load_uint8(msg_addr + i);

        uint128_t enc[msg_len];
        rsa_encrypt(input, msg_len, key, mod, enc);

        // write output
        for(uint32_t i = 0; i < msg_len; i++)
          p->get_mmu()->store_uint128(cipher_addr + i, enc[i]);
        
        //clean up
        free(input);

        break;

      case 3: // setup msg length and run decrypt
        msg_len = xs1;

        // read message into buffer
	uint128_t *input_ct;
        input_ct  = (uint128_t*)malloc(msg_len*sizeof(uint128_t));
        for(uint32_t i = 0; i < msg_len; i++)
          input_ct[i] = p->get_mmu()->load_uint128(msg_addr + i);

        uint8_t dec[msg_len];
        rsa_decrypt(input_ct, msg_len, key, mod, dec);

        // write output
        for(uint32_t i = 0; i < msg_len; i++)
          p->get_mmu()->store_uint8(cipher_addr + i, dec[i]);
        
        //clean up
        free(input_ct);

        break;

      default:
        illegal_instruction();

    }

    return -1; // accelerator currently returns nothing
  }

private:
  reg_t msg_addr;
  reg_t cipher_addr;
  reg_t key;
  reg_t mod;
  reg_t msg_len;


uint128_t mod_exp(uint128_t base, uint128_t expo, uint128_t mod);
int rsa_encrypt(uint8_t *msg, uint32_t len, uint128_t expo,
    uint128_t mod, uint128_t *output);
int rsa_decrypt(uint128_t *ciphertext, uint32_t len, uint128_t expo,
    uint128_t mod, uint8_t *output); 

};
REGISTER_EXTENSION(rsa, []() { return new rsa_t; })
#endif
