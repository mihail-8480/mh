pkgver=0.3.0
pkgrel=0
epoch=1
pkgname='libmh'
url="https://github.com/mihail-8480/mh"
pkgdesc="A C library that is supposed to make my life easier."
arch=('x86_64')
license=('MIT')
depends=('libc.so' 'libm.so' 'libpthread.so' 'libdl.so')
makedepends=('cmake' 'gcc' 'make')

build() {
    cmake -B build \
        -DCMAKE_BUILD_TYPE='Release' \
        -DCMAKE_INSTALL_PREFIX='/usr' \
        -Wno-dev
    make -C build
}

package() {
    cmake --install build
}