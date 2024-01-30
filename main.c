#include "./str.h"
#include <stdio.h>
#include "./stdlibext.h"

int smain(void) {
	Str *str = str_from("This is a new Str", Safe);
	str->print(str);
	str->pushstr(str, "This is a much longer string than before");
	str->print(str);
	str->pushstr(str, "This is an eeeeeeeveeeeennnn longer stringgggggggg! It's hugeeeeeeeeee! I can't believe it!");
	str->print(str);
	return 0;
}
__SAFE_MAIN__(smain())
