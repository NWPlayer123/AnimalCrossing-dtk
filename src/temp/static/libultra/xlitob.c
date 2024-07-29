#include <stdlib.h> /* lldiv & lldiv_t */
#include "_mem.h"   /* memcpy */
#include "libultra/xprintf.h"

#define BUFF_LEN 0x18

u8 ldigs[] = "0123456789abcdef";
u8 udigs[] = "0123456789ABCDEF";

static void _Litob(_Pft* args, u8 type) {
    u8* numMap;
    int base;
    int idx;
    u64 num;
    lldiv_t test;
    char buff[BUFF_LEN];

    if (type == 'X') {
        numMap = udigs;
    } else {
        numMap = ldigs;
    }

    base = (type == 'o') ? 8 : ((type != 'x' && type != 'X') ? 10 : 16);
    idx = BUFF_LEN;
    num = args->v.ll;

    if ((type == 'd' || type == 'i') && args->v.ll < 0) {
        num = -num;
    }

    if (num != 0 || args->prec != 0) {
        buff[--idx] = numMap[num % base];
    }

    args->v.ll = num / base;

    while (args->v.ll > 0 && idx > 0) {
        const lldiv_t quotrem = lldiv(args->v.ll, base);
        args->v.ll = quotrem.quot;
        buff[--idx] = numMap[quotrem.rem];
    }

    args->n1 = BUFF_LEN - idx;

    memcpy(args->s, buff + idx, args->n1);

    if (args->n1 < args->prec) {
        args->nz0 = args->prec - args->n1;
    }

    if (args->prec < 0 && (args->flags & (FLAGS_ZERO | FLAGS_MINUS)) == FLAGS_ZERO) {
        idx = args->width - args->n0 - args->nz0 - args->n1;
        if (idx > 0) {
            args->nz0 += idx;
        }
    }
}
