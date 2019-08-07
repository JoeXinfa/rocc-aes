#include <stdio.h>
#include "uint128_t.h"
#include <inttypes.h>

int main() {
    uint128_t a(0, 4);
    uint128_t b(0, 2);
    uint128_t c;
    //c = a + b;
    //c = a - b;
    //c = a * b;
    c = a / b;
    //uint128_t c(0xe037d35a8b160eb7LL,  0xf11919bfef440917LL);
    printf("Lower = %" PRIu64 "\n", c.lower());
    printf("Upper = %" PRIu64 "\n", c.upper());
    return 0;
}
