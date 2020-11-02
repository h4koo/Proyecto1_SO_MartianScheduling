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
    martian_t m;

    m.id = 1;
    m.max_energy = 5;
    m.name = "Foo";
    m.period = 10;
    addMartian(m);

    m.max_energy = 5;
    m.name = "Bar";
    m.period = 15;
    addMartian(m);

    startSimulation();

    sleep(5);
    pauseSimulation();
    sleep(2);
    // printf("\nEl nombre del marciano es %s\n", getMartian(0).name);
    // func();

    printf("\nfin del main\n");
    return 0;
}