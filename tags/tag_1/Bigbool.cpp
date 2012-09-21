
void new_memory()
{
    try
    {
Bool = new base[size_base];
    }
    catch(std::bad_alloc)
    {
throw(2);
    }
}

Bigbool::Bigbool(int n)
{
    size_bit = n;
    size_base = (n + sz_bit - 1) / sz_bit;
    new_memory();
    for(int i=0; i<size_base; i++)
	Bool[i] = 0;
}

Bigbool::Bigbool(const char *str)
{
    int size = strlen(str);
    size_bit = size;
    size_base = (size + sz_bit - 1) / sz_bit;
    new_memory();
    for(int i=0; i<size; i++)
	set(str[i] - '0', i);
}

Bigbool::Bigbool(const Bigbool &obj)
{
    size_bit = obj.size_bit;
    size_base = obj.size_base;
    new_memory();
    for(int i=0; i<size_base; i++)
	Bool[i] = obj.Bool[i];
}

int Bigbool::get(int n)
{
    unsigned int d = n / sz_bit;
    unsigned int m = n % (sz_bit);
    base k=1;
    if( Bool[d] & (k << m))
	return 1;
    return 0;
}
void Bigbool::print(char *str, int len)
{
    int i;
    for(i=0; (i<size_bit) && (i<len-1); i++)
	str[i] = get(i) + 48;
    str[i] = 0;
}
void Bigbool::set(int value, int n)
{
    if(!((value == 0) || (value == 1)))
	throw(3);
    if((n<0) || (n >= size_bit))
        throw(4);
    int d = n / sz_bit;
    int m = n % sz_bit;
    base t = value;
    t = t << m;
    if(t)
	Bool[d] |= t;
    else
    {
	t = ~(1 << m);
	Bool[d] &=t;
    }
}

Bigbool Bigbool::operator =(const Bigbool &obj)
{
    if(&obj == this)
	return *this;
    if(Bool)
	delete [] Bool;
    size_bit = obj.size_bit;
    size_base = obj.size_base;
    new_memory();
    for(int i=0; i<size_base; i++)
	Bool[i] = obj.Bool[i];
    return *this;
}

unsigned int Bigbool::size()
{
    return size_bit;
}

void Bigbool::bust()
{
    int i;
    for(i=0; i<size_bit; i++)
    {
	if(get(i) == 0)
	{
	    set(1, i);
	    for(int j=0; j<i; j++)
	        set(0, j);
	    return;
	}
    }
    if(i == size_bit)
    {
	if(size_bit % sz_bit)
	{
	    size_bit++;
	    set(1, size_bit-1);
	    for(int i=0; i<size_bit-1; i++)
		set(0, i);

	}
	else
	{
    	    base *tmp = new base[size_base + 1];
	    for(int i=0; i<size_base; i++)
		tmp[i] = 0;
	    size_bit++;
	    size_base++;
	    delete [] Bool;
	    Bool = tmp;
	    set(1, size_bit-1);
	}
    }
}
