#include "uint128_t.h"

// Calculate exponentiation and modulo
uint128_t mod_exp(uint128_t base, uint128_t expo, uint128_t mod) {
    uint128_t result; // base**expo mod(mod)
    result = base;
    for (uint128_t i = 1; i < expo; i++)
        result *= base;
    result %= mod;
    return result;
}
