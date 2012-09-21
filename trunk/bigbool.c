#include "bigbool.h"

int bigbool_create(bigbool *pbig, int n)
{
    unsigned int i;

    if(n < 0)
	return -1;
    if(pbig -> pbool)
    {
	free(pbig -> pbool);
	pbig -> pbool = NULL;
    }
    pbig -> size_bit = n;
    pbig -> size_base = (n + BIGBOOL_SZ_BIT - 1) / BIGBOOL_SZ_BIT;
    if(n == 0)
    {
	pbig -> pbool = NULL;
	return 0;
    }
    pbig -> pbool = (BIGBOOL_BASE*) malloc(n * sizeof(BIGBOOL_BASE));
    if(pbig -> pbool == NULL)
	return -1;
    for(i=0; i < pbig -> size_base; i++)
	pbig -> pbool[i] = 0;
    return 0;
}

void bigbool_init(bigbool *pbig)
{
    pbig -> pbool = NULL;
    pbig -> size_bit = 0;
    pbig -> size_base = 0;
}

void bigbool_delete(bigbool *pbig)
{
    if(pbig -> pbool)
    {
	free(pbig -> pbool);
	pbig -> pbool = NULL;
	pbig -> size_bit = 0;
	pbig -> size_base = 0;
    }
}


int bigbool_get(const bigbool *pbig,  unsigned int n)
{
    unsigned int d = n / BIGBOOL_SZ_BIT;
    unsigned int m = n % (BIGBOOL_SZ_BIT);
    BIGBOOL_BASE k=1;
    if(pbig -> pbool[d] & (k << m))
	return 1;
    return 0;
}

void bigbool_set(bigbool *pbig, unsigned int position, int value)
{
    int d, m;
    BIGBOOL_BASE t;

    value = !(!value);
    
    if(position >= pbig -> size_bit)
	return;
    d = position / BIGBOOL_SZ_BIT;
    m = position % BIGBOOL_SZ_BIT;
    t = (BIGBOOL_BASE) value;
    t = t << m;
    if(t)
	pbig -> pbool[d] |= t;
    else
    {
	t = ~(1 << m);
	pbig -> pbool[d] &=t;
    }
}


int bigbool_deikstre(bigbool *pbig)
{
    unsigned int i, j;

    for(i=0; i<pbig->size_bit; i++)
    {
	if(bigbool_get(pbig, i) == 0)
	{
    	    bigbool_set(pbig, i, 1);
	    j=0;
	    for(; j<i; j++)
    		bigbool_set(pbig, j, 0);
	    return 0;
	}
    }
    if(i == pbig -> size_bit)
    {
	if(pbig -> size_bit % BIGBOOL_SZ_BIT)
	{
	    pbig -> size_bit++;
	    bigbool_set(pbig, pbig -> size_bit-1, 1);
    	    for(i=0; i < pbig -> size_bit-1; i++)
		bigbool_set(pbig, i, 0);
	}
	else
	{
    	    BIGBOOL_BASE *tmp = (BIGBOOL_BASE*) malloc(sizeof(BIGBOOL_BASE) * (pbig -> size_base + 1));
	    if(tmp == NULL)
		return -1;
	    for(i=0; i < pbig->size_base; i++)
		tmp[i] = 0;
	    pbig -> size_bit++;
	    pbig -> size_base++;
	    if(pbig -> pbool)
		free(pbig -> pbool);
	    pbig -> pbool = tmp;
	    bigbool_set(pbig, pbig -> size_bit-1, 1);
	}
    }
    return 0;
}

int bigbool_copy(bigbool *pdest, const bigbool *psrc)
{
    unsigned int i;
    
    bigbool_delete(pdest);
    if(bigbool_create(pdest, psrc -> size_bit) == -1)
	return -1;
    
    for(i=0; i<psrc -> size_bit; i++)
    {
	int value = bigbool_get(psrc, i);
	bigbool_set(pdest, i, value);
    }
    return 0;
}

int bigbool_create_str(bigbool *pbig, const char *str)
{
    unsigned int len, i;

    bigbool_delete(pbig);
    len = strlen(str);
    if(bigbool_create(pbig, len) == -1)
	return -1;
    
    for(i=0; i<len; i++)
	bigbool_set(pbig, i, str[i] - '0');
    return 0;
}

int bigbool_equal(const bigbool *pbig1, const bigbool *pbig2)
{
    unsigned int i, n;
    BIGBOOL_BASE mask;

    if(pbig1 == pbig2)
	return 0;

    if(pbig1 -> size_bit != pbig2 -> size_bit)
	return 1;

    for(i=0; i<pbig1 -> size_base - 1; i++)
	if(pbig1 -> pbool[i] != pbig2 -> pbool[i])
	    return 1;
    
    n = (pbig1 -> size_bit) % BIGBOOL_SZ_BIT;
    mask = 1;
    for(i=0; i<n; i++, mask <<= 1)
	if((pbig1 -> pbool[pbig1 -> size_base - 1] & mask) != (pbig2 -> pbool[pbig2 -> size_base - 1] & mask))
	    return 1;

    return 0;
}
