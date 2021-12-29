#include <stdlib.h>

typedef struct lil_t lil_t;
int mno_scm_main(int argc, char *argv[],
                 void (*load)(lil_t),
                 void (*clean)(lil_t));

int main(int argc, char *argv[])
{
    return mno_scm_main(argc, argv, NULL, NULL);
}
