#include <stdio.h>
#include "uint128_t.h"
#include <inttypes.h>
#include "rsa.h"

int main() {
    /* Simple test exponentiation and modulo
    uint128_t a(0, 2);
    uint128_t b(0, 3);
    uint128_t c(0, 5);
    uint128_t d = mod_exp(a, b, c);
    printf("Lower = %" PRIu64 "\n", d.lower());
    printf("Upper = %" PRIu64 "\n", d.upper());
    */

    uint32_t len = 1;
    uint8_t msg[len];
    uint128_t public_key(0, 5);
    uint128_t private_key(0, 5);
    uint128_t modulo(0, 21);

    msg[0] = 12;
    printf("Message %d \n", msg[0]);

    uint128_t enc[1];
    rsa_encrypt(msg, len, public_key, modulo, enc);

    printf("Encypted Lower = %" PRIu64 "\n", enc[0].lower());
    printf("Encypted Upper = %" PRIu64 "\n", enc[0].upper());

    uint8_t dec[1];
    rsa_decrypt(enc, len, private_key, modulo, dec);

    printf("Decrypted %d \n", dec[0]);

    return 0;
}
