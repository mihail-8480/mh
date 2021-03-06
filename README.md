# mh

## Installation

To build and install `mh` you need to have `gcc`, `cmake` and `make` installed.

```shell
git clone https://github.com/mihail-8480/mh
cd mh
cmake .
cmake --build .
cmake --install .
```

*You might need root permissions for `cmake --install .` because it tries to copy files into `/usr/local/lib` by
default.*

### On Arch Linux-based distributions
You can build and install it using [`mhpkg`](https://github.com/mihail-8480/mhpkg).
```sh
mhpkg install mh
```

## Testing

To test if the installation is successful you can run the default tests:

```shell
cd bin
mhtest ./libmh-default-tests.so
```

*You need to replace the `.so` extension with `.dll` if you are on Windows or `.dylib` if you are on Mac.*

If the test ends with `Finished testing and all tests passed.` the library should work on your system as expected.

## Examples

| Repository | Description |
| --- | --- |
| [mihail-8480/mhserv](https://github.com/mihail-8480/mhserv) | A web server that loads dynamic libraries that implement the function specified in the header `<mh_http_handler.h>`. |

## Dependencies

| Repository | License | Used in Component |
| --- | --- | --- |
| [tidwall/hashmap.c](https://github.com/tidwall/hashmap.c)| [MIT](https://github.com/tidwall/hashmap.c/blob/master/LICENSE) | `libext.a` (`mh/ext/hashmap`) for `<mh_map.h>` |
