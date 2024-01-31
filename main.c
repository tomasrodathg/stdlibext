#include "./str.h"
#include <stdio.h>
#include "./stdlibext.h"

int smain(void) 
{
	string *str = string_from("This is a new Str", Safe);
	str->print(str);
	str->pushstr(str, "This is a much longer string than before");
	str->print(str);
	str->pushstr(str, "This is an eeeeeeeveeeeennnn longer stringgggggggg! It's hugeeeeeeeeee! I can't believe it!");
	str->print(str);
	clear(str);
	str->print(str);
	str->pushstr(str, "Same string, different length!");
	str->print(str);
	return 0;
}
__SAFE_MAIN__(smain())
