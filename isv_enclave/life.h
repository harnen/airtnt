#define CELL(I,J) (field[size*(I)+(J)])
#define ALIVE(I,J) t[size*(I)+(J)] = 1
#define DEAD(I,J)  t[size*(I)+(J)] = 0
 
int count_alive(const char *field, int i, int j, int size);
 
void evolve(const char *field, char *t, int size);

typedef struct life_input
{
    uint8_t size;
    uint8_t steps;
    char array[];
} life_input_t;

