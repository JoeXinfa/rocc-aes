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

    uint128_t public_key(0, 5);
    uint128_t private_key(0, 5);
    uint128_t modulo(0, 21);

    const char ptext[] = "HELLO";
    uint32_t len = 5;
    uint8_t msg[len];
    //uint8_t *msg = (uint8_t *) ptext;
    uint32_t i;
    for (i = 0; i < len; i++) {
        msg[i] = ptext[i];
        printf("Message %d char %d value %c \n", i, msg[i], msg[i]);
        // Shift 60 to make values fall in [0,20] for using the keys
        msg[i] -= 60;
    }

    uint128_t enc[len];
    rsa_encrypt(msg, len, public_key, modulo, enc);

    uint8_t dec[len];
    rsa_decrypt(enc, len, private_key, modulo, dec);

    for (i = 0; i < len; i++) {
        dec[i] += 60; // Shift back
        printf("Decrypted %d char %d value %c \n", i, dec[i], dec[i]);
    }

    return 0;
}
