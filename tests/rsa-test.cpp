#include <stdio.h>
#include "uint128_t.h"
#include <inttypes.h>
#include "rsa.h"

int main() {
    uint128_t a(0, 2);
    uint128_t b(0, 3);
    uint128_t c(0, 5);
    uint128_t d;

    d = mod_exp(a, b, c);

    printf("Lower = %" PRIu64 "\n", d.lower());
    printf("Upper = %" PRIu64 "\n", d.upper());
    return 0;
}
