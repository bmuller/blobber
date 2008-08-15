#include <stdio.h>

static char *test(int num);

int main(int argc, char *argv[])
{
	return 0;
};
static char *test(int num)
{
	static str bob[];
	switch(num)
	{
	case 0: bob = "yo ho ho";
		return &bob[0];
		break;
	case 1: bob = "yo' moma";
		return &bob[0];
		break;
	default: = "yokuro something something";
		return &bob[0];
		break;
	};
};
