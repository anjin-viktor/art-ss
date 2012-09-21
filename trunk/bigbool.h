#ifndef _BIGBOOL_H_
#define _BIGBOOL_H_

#include <string.h>
#include <stdlib.h>

#define BIGBOOL_BASE unsigned char

#define  BIGBOOL_SZ sizeof(BIGBOOL_BASE);

#define  BIGBOOL_SZ_BIT  (sizeof(BIGBOOL_BASE) * 8)

typedef struct
{
    BIGBOOL_BASE *pbool;
    unsigned int size_base;
    unsigned int size_bit;
} bigbool;

int bigbool_create(bigbool *pbig, int n);
void bigbool_init(bigbool *pbig);   /* create NULL bool */
void bigbool_delete(bigbool *pbig);
int bigbool_get(const bigbool *pbig, unsigned int n);
void bigbool_set(bigbool *pbig, unsigned int position, int value);
int bigbool_deikstre(bigbool *pbig);    /* next vector of Deikstre */
int bigbool_copy(bigbool *pdest, const bigbool *psrc);
int bigbool_create_str(bigbool *pbig, const char *str);
int bigbool_equal(const bigbool *pbig1, const bigbool *pbig2); /*return 0 if equal*/

#endif
