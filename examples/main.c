#include "../inc/mh_console.h"

int main(void) {
    MH_WRITE("a: ");
    mh_unsigned_number_t a = mh_read_uint(MH_INPUT);
    MH_WRITE("b: ");
    mh_unsigned_number_t b = mh_read_uint(MH_INPUT);
    MH_WRITE("{}+{}={}\n", MH_FMT_UINT(a), MH_FMT_UINT(b), MH_FMT_UINT(a+b));
}
