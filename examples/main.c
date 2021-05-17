#include "../inc/mh_console.h"

int main(void) {
    MH_WRITE("a: ");
    int a = mh_read_int(MH_INPUT);
    MH_WRITE("b: ");
    int b = mh_read_int(MH_INPUT);
    MH_WRITE("({}) + ({}) = {}\n", MH_FMT_INT(a), MH_FMT_INT(b), MH_FMT_INT(a + b));
}
