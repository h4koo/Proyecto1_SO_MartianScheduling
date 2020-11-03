#include "../include/simulation.h"

#define MAX_TIME_STEP 1000000
#define MIN_TIME_STEP 100000
#define NO_SCHEDULING -1
#define SCHEDULING_ERROR -2

int _labyrinth[LAB_HEIGHT][LAB_WIDTH] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                                         {1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
                                         {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                                         {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
                                         {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
                                         {1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
                                         {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1},
                                         {1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

static enum sim_state _simulation_state = SIM_INITIAL;

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
static int _completed_martians = 0;
static pthread_t _running_sim_thread;

static int _sim_timer = 0;
static unsigned int _time_step = 500000;

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
    new_martian.remaining_energy = 0;
    new_martian.previous_position.x = -1;
    new_martian.previous_position.y = -1;
    new_martian.position.x = _start_position.x;
    new_martian.position.y = _start_position.y;
    _martians[_num_martians] = new_martian;
    _num_martians++;

    // check if martians are still schedulable !!!!!!!!!!!!!!!!!!!!!!!!!

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
void startSimulation()
{
    if (_simulation_state == SIM_INITIAL)
    {
        initReport();
    }

    _simulation_state = SIM_RUNNING;

    // start simulation loop in a thread !!!!!!!!!!!!!!!!!!!!
    pthread_create(&_running_sim_thread, NULL, simulationLoop, NULL);
    // simulationLoop();
}

// pauses simulation loop
void pauseSimulation()
{
    _simulation_state = SIM_PAUSED;
}

// ends the simulation
void endSimulation()
{
    _simulation_state = SIM_FINISHED;
}

// makes the simulation go faster (0 < t_mult < 1) or slower  (1 < t_mult)
// int changeSimulationSpeed(int t_mult);
void makeSimulationSlower()
{
    _time_step += 100000;
    if (_time_step > MAX_TIME_STEP)
    {
        _time_step = MAX_TIME_STEP - 1;
        printf("The slowest setting has been reached");
    }
}
void makeSimulationFaster()
{
    _time_step -= 100000;
    if (_time_step < MIN_TIME_STEP)
    {
        _time_step = MIN_TIME_STEP;
        printf("The fastest setting has been reached");
    }
}

// ++++ Internal funcs ++++

// returns the index of the next martian to move acording to RM scheduling
// also refills the martians energy if needed and throws error if not able to reschedule
int rateMonotonicScheduling()
{
    martian_t *mrt;
    int completed_mrt = 0;
    int selected_id = NO_SCHEDULING;
    int lowest_period = 999999;

    for (int i = 0; i < _num_martians; ++i)
    {
        mrt = _martians + i;

        // if the martian already completed
        if (mrt->state == MRTN_COMPLETED)
        {
            ++completed_mrt;
            continue;
        }

        // check if the martian is due for another scheduling
        if ((_sim_timer % mrt->period) == 0)
        {
            // if the martian still has remaining energy
            if (mrt->remaining_energy > 0)
            {
                printf("ERROR SCHEDULING RM: Martian %s was unable to complete past work before starting new one\n", mrt->name);
                _simulation_state = SIM_ERROR;
                return SCHEDULING_ERROR;
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
    _completed_martians = completed_mrt;
    return selected_id;
}

// returns index of the next martian to move acording to EDF scheduling
// also refills the martians energy if needed and throws error if not able to reschedule
int earliestDeadlineFirst()
{

    martian_t *mrt;
    int completed_mrt = 0;
    int deadline, num_cycles;
    int selected_id = NO_SCHEDULING; //if -1 is returned no scheduling is needed
    int earliest_deadline = 999999;

    for (int i = 0; i < _num_martians; ++i)
    {
        mrt = _martians + i;

        // if the martian already completed
        if (mrt->state == MRTN_COMPLETED)
        {
            ++completed_mrt;
            continue;
        }
        // check if the martian is due for another scheduling
        if ((_sim_timer % mrt->period) == 0)
        {
            // if the martian still has remaining energy
            if (mrt->remaining_energy > 0)
            {
                printf("ERROR SCHEDULING RM: Martian %s was unable to complete past work before starting new one\n", mrt->name);
                _simulation_state = SIM_ERROR;
                return SCHEDULING_ERROR;
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

    _completed_martians = completed_mrt;
    return selected_id;
}

// makes a semi-random movement of a martian in the labyrinth
int moveMartian(int martian_index)
{

    martian_t *martian = _martians + martian_index;
    position_t up = martian->position, down = martian->position, right = martian->position, left = martian->position;
    --up.y;
    ++down.y;
    ++right.x;
    --left.x;

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
    else if (right.x >= 0 && right.x < LAB_WIDTH && _labyrinth[right.y][right.x] == LAB_EMPTY && !(right.y == martian->previous_position.y && right.x == martian->previous_position.x))
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
    else if (left.x >= 0 && left.x < LAB_WIDTH && _labyrinth[left.y][left.x] == LAB_EMPTY && !(left.y == martian->previous_position.y && left.x == martian->previous_position.x))
    {
        // move up
        martian->previous_position = martian->position;
        _labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = left;
        _labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
    }

    // the martian couldn't move anywhere to a new position, try moving to previous position
    else if (_labyrinth[martian->previous_position.y][martian->previous_position.x] == LAB_EMPTY)
    {
        left = martian->position;
        _labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = martian->previous_position;
        martian->previous_position = left;
        _labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
    }

    // the martian couldn't move
    else
    {
        printf("ERROR!!!: Unable to move Martian %s, seems like he's trapped\n", martian->name);
        --martian->remaining_energy;
        return -1;
    }

    // reduce remaining_energy of the martian
    --martian->remaining_energy;
    return 0;
}

// loop that simulates CPU clock. In charge of managing martians energy and moving them and checking
void *simulationLoop()
{
    int selected_martian_id;
    martian_t *mrt;

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

        if (selected_martian_id == SCHEDULING_ERROR)
            break;
        if (selected_martian_id != NO_SCHEDULING)
        {
            moveMartian(selected_martian_id);
            mrt = _martians + selected_martian_id;
            printf("Moved martian %s to position x: %d, y: %d \n", mrt->name, mrt->position.x, mrt->position.y);
            if (mrt->position.x == _end_position.x && mrt->position.y == _end_position.y)
            {
                mrt->state = MRTN_COMPLETED;
                if (++_completed_martians == _num_martians)
                {
                    _simulation_state = SIM_FINISHED;
                    break;
                }
            }
            // log the moving martian
            logMartian(mrt);
        }
        else
        {
            logNOP();
        }

        usleep(_time_step);
        printf("Simulation time is: %d \n", _sim_timer);
        _sim_timer++;
    } // end while
    printf("Completed simulation time is: %d \n", _sim_timer);

    if (_simulation_state == SIM_FINISHED || _simulation_state == SIM_ERROR)
    {
        /* code */
    }
    return 0;
}

void simulationStep()
{
    int selected_martian_id;
    martian_t *mrt;

    if (_selected_alg == RATE_MONOTONIC)
    {
        selected_martian_id = rateMonotonicScheduling();
    }
    else
    {
        selected_martian_id = earliestDeadlineFirst();
    }

    if (selected_martian_id == SCHEDULING_ERROR)
    {
        _simulation_state = SIM_ERROR;
    }
    else if (selected_martian_id != NO_SCHEDULING)
    {
        moveMartian(selected_martian_id);
        mrt = _martians + selected_martian_id;
        printf("Moved martian %s to position x: %d, y: %d \n", mrt->name, mrt->position.x, mrt->position.y);
        if (mrt->position.x == _end_position.x && mrt->position.y == _end_position.y)
        {
            mrt->state = MRTN_COMPLETED;
            if (++_completed_martians == _num_martians)
            {
                _simulation_state = SIM_FINISHED;
            }
        }
        // log the moving martian
        logMartian(mrt);
    }
    else
    {
        logNOP();
        printf("No scheduling\n");
    }
    printf("Simulation time is: %d \n", _sim_timer);
    ++_sim_timer;
}

void selectAlgRM()
{
    _selected_alg = RATE_MONOTONIC;
}

void selectAlgEDF()
{
    _selected_alg = EARLIEST_DEADLINE_FIRST;
}

void selectModeAutomatic()
{
    _sim_mode = AUTO;
}

void selectModeManual()
{
    _sim_mode = MANUAL;
}

int getSimulationState()
{
    return _simulation_state;
}

void setSimulationState(enum sim_state state)
{
    _simulation_state = state;
}

int getTimeStep()
{
    return _time_step;
}

void resetSimulation()
{

    martian_t *mrt;
    for (size_t i = 0; i < _num_martians; i++)
    {
        mrt = _martians + i;
        mrt->remaining_energy = 0;
        mrt->state = MRTN_RUNNING;
        mrt->position.x = _start_position.x;
        mrt->position.y = _start_position.y;
        mrt->previous_position.x = -1;
        mrt->previous_position.y = -1;
    }
}