// #include <gtk/gtk.h>
#include "./include/simulation.h"

static struct art
{
    int x;
    int y;
} ar;

void func()
{
    struct art p;
    p.x = 1;
    p.y = 1;

    ar.x = p.x;
    ar.y = p.y;
}

int main(int argc, char *argv[])
{
    // martian_t m;

    // m.id = 1;
    // m.max_energy = 10;
    // m.name = "lol";
    // m.period = 5;
    // m.position.x = 0;
    // m.position.y = 4;
    // // m.previous_position = {-1,-1};
    // addMartian(m);
    // printf("\nEl nombre del marciano es %s\n", getMartian(0).name);
    func();

    printf("\nEl valor del struct es x: %d  y: %d\n", ar.x, ar.y);
    return 0;
}