#define _NOT_REDEF_
#include "catch_lib.h"

manager catch_distributor;

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
int resource_init(resource *pres, const char *res_name, const char *bool_str)
{
    int len = strlen(res_name) + 1;

    pres -> action_after_end = RETURNED_END_BOOL;
    pres -> name = (char *) malloc(len * sizeof(char)); 
    if(pres -> name == NULL)
      return -1;
    strcpy(pres -> name, res_name);
    bigbool_init(& pres -> bool_describe);
    if(bigbool_create_str(&pres -> bool_describe, bool_str) == -1)
  return -1;
    pres -> size = pres -> bool_describe.size_bit;
    pres -> position = 0;
    return 0;
}

int resource_current_action(resource *pres)
{
    int value;

    if(pres -> position == pres -> size)
        return pres -> action_after_end;

    value = bigbool_get(&pres -> bool_describe, pres -> position);
    pres -> position++;
    return value;
}

void resource_delete(resource *pres)
{
    if(pres -> name)
    {
       free(pres -> name);
   pres -> name = NULL;
    }
    bigbool_delete(&pres -> bool_describe);
}

int resource_is_my(resource *pres, const char *name)
{
    int result;

    if(pres -> name == NULL)
  return 0;

    result = strcmp(pres -> name, name);
    return !result;
}

int resource_copy(resource *pdest, const resource *psrc)
{
    if(pdest == psrc)
      return 0;
    if(bigbool_copy(&pdest -> bool_describe, &psrc -> bool_describe) == -1)
        return -1;
    pdest -> position = psrc -> position;
    pdest -> size = psrc -> size;
    if(psrc -> name == NULL)
   return 0;
    free(pdest -> name);
    pdest -> name = (char *) malloc(sizeof(char) * (strlen(psrc -> name) + 1));
    if(pdest == NULL)
      return -1;
    strcpy(pdest -> name, psrc -> name);
    pdest -> action_after_end = psrc -> action_after_end;
    return 0;
}

void resource_init_null(resource *pres)
{
    pres -> name = NULL;
    pres -> action_after_end = RETURNED_END_BOOL;
    bigbool_init(&pres -> bool_describe);
}
void manager_init(manager *pman)
{
    pman -> returned_resource_not_find = RETURNED_RESOURCE_NOT_FIND;
    pman -> presources = NULL;
    pman -> size = 0;
    pman -> next_changs = 0;
}


int manager_add(manager *pman, const resource *pres)
{
    resource *tmp;
    unsigned int i;

    if(pres == NULL)
  return -1;

    for(i=0; i < pman -> size; i++)
        if(resource_is_my(&pman -> presources[i], pres -> name))
          return -1;

    tmp = (resource *) malloc(sizeof(resource) * (pman -> size + 1));
    if(tmp == NULL)
     return -1;
    for(i=0; i<pman -> size + 1; i++)
     resource_init_null(&tmp[i]);
    for(i=0; i<pman -> size; i++)
       if(resource_copy(&tmp[i], &pman -> presources[i]) == -1)
     return -1;
    if(resource_copy(&tmp[i], pres) == -1)
    return -1;
    if(pman -> presources)
        free(pman -> presources);
    pman -> presources = tmp;
    pman -> size++;
    return 0;
}


int manager_add_str(manager *pman, const char *name, const char *str_bool)
{
    resource *tmp;
    unsigned int i;

    if(name == NULL || str_bool == NULL)
     return -1;

    for(i=0; i < pman -> size; i++)
     if(resource_is_my(&pman->presources[i], name))
       return -1;

    tmp = (resource *) malloc(sizeof(resource) * (pman -> size + 1));
    if(tmp == NULL)
  return -1;
    for(i=0; i<pman -> size + 1; i++)
     resource_init_null(&tmp[i]);
    for(i=0; i<pman -> size; i++)
  if(resource_copy(&tmp[i], &pman -> presources[i]) == -1)
          return -1;
    if(resource_init(&tmp[i], name, str_bool) == -1)
     return -1;
    if(pman -> presources)
        free(pman -> presources);
    pman -> presources = tmp;
    pman -> size++;

    return 0;
}


int manager_get_action(const manager *pman, const char *resource_name)
{
    unsigned int i;
    char f;
    int value;
    
    switch(pman -> act)
    {
 case(complete_rejection):
            value =  0;
           break;

  case(rejection):
          value = pman -> returned_resource_not_find;
           for(i=0; i<pman -> size; i++)
       if(resource_is_my(&pman->presources[i], resource_name))
                    value =  0;
           break;


        case(handling):
           value = pman -> returned_resource_not_find;
      for(i=0; i<pman -> size; i++)
             if(resource_is_my(&pman->presources[i], resource_name))
               {
                 value = resource_current_action(&pman -> presources[i]);
                 break;
            }
         break;

  case(random_behavior):
            value = rand() % 2;
           break;

  case(user_request):
          for(f=1;f == 1;)
      {
         f=0;
          fprintf(stderr, "Resource %s\nAction: ", resource_name);
              fscanf(stdin, "%d", &value);
             if(value != 0 && value != 1)
          {
                 f = 1;
                fprintf(stderr, "Error. Action is a 1 or 0\n");
           }
         }   
    };

    return value;
}

int manager_all_end(const manager *pman)
{
    unsigned int i=0;
    for(i=0; i<pman -> size; i++)
        if(pman -> presources[i].position < pman -> presources[i].size)
      return 0;
    return 1;
}

void manager_delete(manager *pman)
{
    unsigned int i;

    if(pman -> presources == NULL)
   return;

    for(i=0; i<pman -> size; i++)
     resource_delete(&pman -> presources[i]);
    free(pman -> presources);
}

void manager_next_state_bust(manager *pman)
{
    if(pman -> size == 0)
  return;

    bigbool_deikstre(&pman -> presources[pman -> next_changs].bool_describe);
    pman -> next_changs = (pman -> next_changs + 1) % pman -> size;
}
int catch_start(int argc, char **argv)
{
    extern manager catch_distributor;
	 catch_distributor.act = rejection;
	 atexit(catch_normal_exit);
    int i=0;
    manager_init(&catch_distributor);
    if(argc && (strcmp(argv[0], "complete_rejection") == 0 || strcmp(argv[0], "rejection") == 0 || strcmp(argv[0], "random_behavior") == 0 || strcmp(argv[0], "user_request") == 0 || strcmp(argv[0], "handling") == 0 ))
    {
        argc--;
        if(strcmp(argv[0], "complete_rejection") == 0)
            catch_distributor.act = complete_rejection;
        else if(strcmp(argv[0], "rejection") == 0)
            catch_distributor.act = rejection;
        else if(strcmp(argv[0], "handling") == 0 )
            catch_distributor.act = handling;
	     else if(strcmp(argv[0], "random_behavior") == 0)
		 catch_distributor.act=random_behavior;
	     else
		 catch_distributor.act=user_request;
        argv++;
    }
    for(; i<argc; i+= 2)
        if(manager_add_str(&catch_distributor, argv[i], argv[i+1]) == -1)
	    return -1;
    return 0;
}
void catch_normal_exit(void)
{
    manager_delete(&catch_distributor);
    exit(0);
}
FILE * catch_fopen(const char * arg0, const char * arg1)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, "fopen") == 0)
	return NULL;
    else
	return fopen(arg0, arg1);
}
int catch_open(const char * arg0, int arg1, ...)
{
    va_list ap;
    unsigned int s[16];
    int i=0;
    int size = 1;
    extern manager catch_distributor;
    va_start(ap, arg1);
    for(;i<size;i++ )
        s[i] = va_arg(ap, unsigned int);
    va_end(ap);
    if(manager_get_action(&catch_distributor, "open") == 0)
	return -1;
    else
	return open(arg0, arg1, s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10], s[11], s[12], s[13], s[14], s[15]);
}
void * catch_malloc(size_t arg0)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, "malloc") == 0)
	return NULL;
    else
	return malloc(arg0);
}
void * catch_calloc(size_t arg0, size_t arg1)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, "calloc") == 0)
	return NULL;
    else
	return calloc(arg0, arg1);
}
void * catch_realloc(void * arg0, size_t arg1)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, "realloc") == 0)
	return NULL;
    else
	return realloc(arg0, arg1);
}
