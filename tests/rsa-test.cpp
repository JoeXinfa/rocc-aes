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

    uint32_t len = 7;
    uint8_t msg[len];
    uint128_t public_key(0, 5);
    uint128_t private_key(0, 5);
    uint128_t modulo(0, 21);

    msg[0] = 2;
    msg[1] = 5;
    msg[2] = 8;
    msg[3] = 9;
    msg[4] = 12; 
    msg[5] = 17; 
    msg[6] = 20;

    uint128_t enc[len];
    rsa_encrypt(msg, len, public_key, modulo, enc);

    uint8_t dec[len];
    rsa_decrypt(enc, len, private_key, modulo, dec);

    uint32_t i;
    for (i = 0; i < len; i++) {
        printf("-----------------------\n");
        printf("Message %d \n", msg[i]);
        printf("Encypted Lower = %" PRIu64 "\n", enc[i].lower());
        printf("Encypted Upper = %" PRIu64 "\n", enc[i].upper());
        printf("Decrypted %d \n", dec[i]);
    }

    return 0;
}
