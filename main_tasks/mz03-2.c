#include <stdio.h>



int
main(int argc, char *argv[])
{
    long long psum = 0;
    long long nsum = 0;
    for (int i = 1; i < argc; ++i) {
        int num;
        if (sscanf(argv[i], "%d", &num) > 0) {
            if (num >= 0) {
                psum += num;
            } else {
                nsum += num;
            }
        } else {
            fprintf(stderr, "wrong value\n");
            return 1;
        }
    }
    printf("%lld\n%lld\n", psum, nsum);
    return 0;
}
