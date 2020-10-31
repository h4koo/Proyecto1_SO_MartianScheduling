#ifndef _MARTIAN_H
#define _MARTIAN_H

#include "position.h"

enum martian_state
{
    MRTN_RUNNING,
    MRTN_COMPLETED
};
//
typedef struct
{
    /* data */
    int id;
    char *name;
    enum martian_state state;
    int max_energy;
    int remaining_energy;
    int period;
    position_t position;
    position_t previous_position;

} martian_t;

#endif