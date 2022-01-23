#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum
{
    X0 = 1234,
    A = 1103515245,
    C = 12345,
    MOD = 1U << 31
};

struct RandomGenerator;
void destroy(struct RandomGenerator *);
int next(struct RandomGenerator *);

typedef struct RandomOperations
{
    void (*destroy)(struct RandomGenerator *);
    int (*next)(struct RandomGenerator *);
} RandomOperations;

typedef struct RandomGenerator
{
    int32_t x;
    uint32_t a;
    uint32_t c;
    uint32_t mod;
    RandomOperations *ops;
} RandomGenerator;    

void 
destroy(RandomGenerator *gen)
{
    free(gen->ops);
    free(gen);
}


int
next(RandomGenerator *gen)
{
    gen->x = (gen->a * gen->x + gen->c) % gen->mod;
    return gen->x;
}


RandomGenerator *
random_create(int seed)
{
    RandomGenerator *gen = realloc(NULL, sizeof(RandomGenerator));
    if (gen == NULL) {
        fprintf(stderr, "realloc error\n");
        return NULL;
    }
    gen->ops = realloc(NULL, sizeof(RandomOperations));
    if (gen->ops == NULL) {
        fprintf(stderr, "realloc error\n");
        return NULL;
    }
    gen->x = seed;
    gen->a = A;
    gen->c = C;
    gen->mod = MOD;
    gen->ops->destroy = destroy;
    gen->ops->next = next;
    return gen;
}

