#include "rocc.h"
#include "mmu.h"
#include "extension.h"
#include "rsa.h"
#include "uint128_t.h"

// Calculate exponentiation and modulo
uint128_t rsa_t::mod_exp(uint128_t base, uint128_t expo, uint128_t mod) {
    uint128_t result; // base**expo mod(mod)
    result = base;
    for (uint128_t i = 1; i < expo; i++)
        result *= base;
    result %= mod;
    return result;
}


int rsa_t::rsa_encrypt(uint8_t *msg, uint32_t len, uint128_t expo,
    uint128_t mod, uint128_t *output) {
    uint32_t i;
    for (i = 0; i < len; i++) {
        uint128_t base = (uint128_t) msg[i];
        output[i] = mod_exp(base, expo, mod);
    }
    return 0;
}


int rsa_t::rsa_decrypt(uint128_t *ciphertext, uint32_t len, uint128_t expo,
    uint128_t mod, uint8_t *output) {
    uint32_t i;
    for (i = 0; i < len; i++) {
        uint128_t res = mod_exp(ciphertext[i], expo, mod);
        output[i] = (uint8_t) res;
    }
    return 0;
}
