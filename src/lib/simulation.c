#include "../include/simulation.h"

static enum sim_state {
    SIM_INITIAL,
    SIM_RUNNING,
    SIM_PAUSED,
    SIM_FINISHED,
    SIM_ERROR
} _simulation_state = SIM_INITIAL;

static enum schd_alg {
    RATE_MONOTONIC,
    EARLIEST_DEADLINE_FIRST
} _selected_alg = RATE_MONOTONIC;

static enum app_mode {
    AUTO,
    MANUAL
} _sim_mode;

static martian_t _martians[MAX_MARTIANS];
static int _num_martians = 0;

static int _sim_timer = 0;
static int _time_multiplier = 1;

static position_t _start_position = {0, 4};
static position_t _end_position = {19, 6};


// ++++ External funcs ++++
int addMartian(martian_t new_martian)
{
    if (_num_martians == MAX_MARTIANS)
    {

        printf("*****\nERROR: No se pueden agregar mas de %i marcianos a la simulación\n", MAX_MARTIANS);
        return -1;
    }
    new_martian.state = MRTN_RUNNING;
    _martians[_num_martians] = new_martian;
    _num_martians++;

    return 0;
}

martian_t getMartian(int id)
{

    return _martians[id];
}

int getNumMartians()
{
    return _num_martians;
}

// starts the simulation loop
int startSimulation();

// pauses simulation loop
int pauseSimulation();

// ends the simulation
int endSimulation();

// makes the simulation go faster (0 < t_mult < 1) or slower  (1 < t_mult)
int changeSimulationSpeed(int t_mult);

// ++++ Internal funcs ++++

// returns index of the next martian to move acording to RM scheduling
int rateMonotonicScheduling()
{
    martian_t *mrt;
    int selected_id = -1;
    int lowest_period = 9999999999;

    for (int i = 0; i < _num_martians; i++)
    {
        mrt = _martians + i;

        // if the martian already completed
        if (mrt->state == MRTN_COMPLETED)
            continue;

        // check if the martian is due for another scheduling
        if ((_sim_timer % mrt->period) == 0)
        {
            // if the martian still has remaining energy
            if (mrt->remaining_energy > 0)
            {
                printf("ERROR SCHEDULING RM: Martian %s was unable to complete past work before starting new one\n", mrt->name);
                _simulation_state = SIM_ERROR;
                return -2;
            }
            else
            {
                //refill the energy
                mrt->remaining_energy = mrt->max_energy;
            }
        }

        if (mrt->remaining_energy > 0)
        {
            if (mrt->period < lowest_period)
            {
                lowest_period = mrt->period;
                selected_id = i;
            }
        }
    } // end for

    return selected_id;
}

// returns index of the next martian to move acording to EDF scheduling
int earliestDeadlineFirst()
{

    martian_t *mrt;
    int deadline, num_cycles;
    int selected_id = -1;
    int earliest_deadline = 9999999999;

    for (int i = 0; i < _num_martians; i++)
    {
        mrt = _martians + i;

        // if the martian already completed
        if (mrt->state == MRTN_COMPLETED)
            continue;
        // check if the martian is due for another scheduling
        if ((_sim_timer % mrt->period) == 0)
        {
            // if the martian still has remaining energy
            if (mrt->remaining_energy > 0)
            {
                printf("ERROR SCHEDULING RM: Martian %s was unable to complete past work before starting new one\n", mrt->name);
                _simulation_state = SIM_ERROR;
                return -2;
            }
            else
            {
                //refill the energy
                mrt->remaining_energy = mrt->max_energy;
            }
        }

        // if the martian has energy
        if (mrt->remaining_energy > 0)
        {
            num_cycles = _sim_timer / mrt->period;
            deadline = (mrt->period * (num_cycles + 1)) - _sim_timer;
            if (deadline < earliest_deadline)
            {
                earliest_deadline = deadline;
                selected_id = i;
            }
        }
    } // end for

    return selected_id;
}

// makes a semi-random movement of a martian in the labyrinth
int moveMartian(int martian_index)
{

    martian_t *martian = _martians + martian_index;
    position_t up = martian->position, down = martian->position, right = martian->position, left = martian->position;
    up.y--;
    down.y++;
    right.x++;
    left.x--;

    // // check where the martian is relative to the end position
    // int x_diff = end_position.x - martian->position.x;
    // int y_diff = end_position.y - martian->position.y;

    // if (y_diff < 0) // the end position is going up
    // {
    // }
    // else if (y_diff == 0) // we are in the same row as the end position
    // {
    //     /* code */
    // }
    // else //the end position is going down
    // {
    // }

    //try moving up
    if (up.y >= 0 && up.y < LAB_HEIGHT && _labyrinth[up.y][up.x] == 0 && !(up.y == martian->previous_position.y && up.x == martian->previous_position.x))
    {
        // move up
        martian->previous_position = martian->position;
        _labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = up;
        _labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
    }

    //try moving right
    else if (right.x >= 0 && right.x < LAB_WIDTH && _labyrinth[right.y][right.x] == 0 && !(right.y == martian->previous_position.y && right.x == martian->previous_position.x))
    {
        // move up
        martian->previous_position = martian->position;
        _labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = right;
        _labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
    }

    // try moving down
    else if (down.y >= 0 && down.y < LAB_HEIGHT && _labyrinth[down.y][down.x] == 0 && !(down.y == martian->previous_position.y && down.x == martian->previous_position.x))
    {
        // move up
        martian->previous_position = martian->position;
        _labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = down;
        _labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
    }

    //try moving left
    else if (left.x >= 0 && left.x < LAB_WIDTH && _labyrinth[left.y][left.x] == 0 && !(left.y == martian->previous_position.y && left.x == martian->previous_position.x))
    {
        // move up
        martian->previous_position = martian->position;
        _labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = left;
        _labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
    }

    // the martian couldn't move anywhere
    else
    {
        printf("ERROR!!!: Unable to move Martian %s\n", martian->name);
        martian->remaining_energy--;
        return -1;
    }

    // reduce remaining_energy of the martian
    martian->remaining_energy--;
    return 0;
}

// loop that simulates CPU clock. In charge of managing martians energy and moving them and checking
int simulationLoop()
{
    int selected_martian_id;
    while (_simulation_state == SIM_RUNNING)
    {
        if (_selected_alg == RATE_MONOTONIC)
        {
            selected_martian_id = rateMonotonicScheduling();
        }
        else
        {
            selected_martian_id = earliestDeadlineFirst();
        }

        _sim_timer++;
    } // end while

    if (_simulation_state == SIM_FINISHED || _simulation_state == SIM_ERROR)
    {
        /* code */
    }
}
