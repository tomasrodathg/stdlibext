# stdlibext

An extension on the `C`'s `stdlib`, with features of safe memory allocation, freeing memory of a stack by introducing a `Safe Main` proxy via a preprocessor macro. It also adds some of the CPP objects, with the option to keep them close to `C` primitives for raw manipulation (I'm using the terms `Safe` and `Unsafe` from 'Rust').

## `String`

Current look of a 'stringified' `String`:
```
string:
	val: This is a new Str
	cap: 18
	len: 18
	stack pos: 0
	growth_factor: 2
string:
	val: This is a much longer string than before
	cap: 62
	len: 41
	stack pos: 0
	growth_factor: 2
string:
	val: This is an eeeeeeeveeeeennnn longer stringgggggggg! It's hugeeeeeeeeee! I can't believe it!
	cap: 120
	len: 92
	stack pos: 0
	growth_factor: 2
string:
	val:
	cap: 120
	len: 0
	stack pos: 0
	growth_factor: 2
string:
	val: Same string, different length!
	cap: 120
	len: 31
	stack pos: 0
	growth_factor: 2
```
