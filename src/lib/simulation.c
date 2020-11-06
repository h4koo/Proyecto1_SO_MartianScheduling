#include "../include/simulation.h"

#define MAX_TIME_STEP 1000000
#define MIN_TIME_STEP 100000
#define NO_SCHEDULING -1
#define SCHEDULING_ERROR -2
#define EW_MESSAGE_LENGTH 256

int _rm_labyrinth[LAB_HEIGHT][LAB_WIDTH] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                            {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                                            {1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
                                            {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
                                            {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
                                            {1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
                                            {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1},
                                            {1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
                                            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

int _edf_labyrinth[LAB_HEIGHT][LAB_WIDTH] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                             {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                                             {1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
                                             {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
                                             {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
                                             {1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
                                             {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1},
                                             {1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
                                             {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
;

static char *_names[MAX_MARTIANS] = {"A", "B", "C", "D", "E", "F"};

int _rm_last_moved_martian;
int _edf_last_moved_martian;

char _rm_message[EW_MESSAGE_LENGTH];
char _edf_message[EW_MESSAGE_LENGTH];

static enum sim_state _rm_simulation_state = SIM_INITIAL;
static enum sim_state _edf_simulation_state = SIM_INITIAL;

static enum schd_alg _selected_alg = RATE_MONOTONIC;

static enum app_mode _sim_mode;

static martian_t _rm_martians[MAX_MARTIANS];
static martian_t _edf_martians[MAX_MARTIANS];
static int _num_martians = 0;

static int _rm_completed_martians = 0;
static int _edf_completed_martians = 0;
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
    if (new_martian.period < new_martian.max_energy || new_martian.max_energy <= 0)
    {
        printf("*****\nERROR: Al agregar marciano, valores incorrectos de entrada --> Periodo: %d  MxEnergia: %d\n", new_martian.period, new_martian.max_energy);
        return -1;
    }

    new_martian.state = MRTN_RUNNING;
    new_martian.name = _names[_num_martians];
    new_martian.remaining_energy = 0;
    new_martian.id = _num_martians;
    new_martian.previous_position.x = -1;
    new_martian.previous_position.y = -1;
    new_martian.position.x = _start_position.x;
    new_martian.position.y = _start_position.y;
    _edf_martians[_num_martians] = new_martian;
    _rm_martians[_num_martians] = new_martian;
    _num_martians++;

    // check if martians are still schedulable !!!!!!!!!!!!!!!!!!!!!!!!!

    return 0;
}

martian_t getMartian(int id)
{
    if (_selected_alg == RATE_MONOTONIC)
    {
        return _rm_martians[id];
    }
    else
    {
        return _edf_martians[id];
    }
}

martian_t *getRunningMartian()
{
    if (_selected_alg == RATE_MONOTONIC)
    {

        if (_rm_last_moved_martian < 0)
            return NULL;
        return &_rm_martians[_rm_last_moved_martian];
    }
    else
    {

        if (_edf_last_moved_martian < 0)
            return NULL;
        return &_edf_martians[_edf_last_moved_martian];
    }
}

martian_t *getMartianList()
{
    if (_selected_alg == RATE_MONOTONIC)
    {
        return _rm_martians;
    }
    else
    {
        return _edf_martians;
    }
}

int getNumMartians()
{
    return _num_martians;
}

int getSimulationTime()
{
    return _sim_timer;
}

// starts the simulation loop
void startSimulation()
{
    if (_rm_simulation_state == SIM_INITIAL)
    {
        initReport();
    }

    _rm_simulation_state = SIM_RUNNING;
    _edf_simulation_state = SIM_RUNNING;

    // start simulation loop in a thread !!!!!!!!!!!!!!!!!!!!
    pthread_create(&_running_sim_thread, NULL, simulationLoop, NULL);
    // simulationLoop();
}

// pauses simulation loop
void pauseSimulation()
{
    _rm_simulation_state = SIM_PAUSED;
    _edf_simulation_state = SIM_PAUSED;
}

// ends the simulation
void endSimulation()
{
    _rm_simulation_state = SIM_FINISHED;
    _edf_simulation_state = SIM_FINISHED;
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
        mrt = _rm_martians + i;

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
                _rm_simulation_state = SIM_ERROR;
                sprintf(_rm_message, "ERROR SCHEDULING RM: \nMartian %s was unable to complete past work before starting new one\n", mrt->name);
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
    _rm_completed_martians = completed_mrt;
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
        mrt = _edf_martians + i;

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
                _edf_simulation_state = SIM_ERROR;
                sprintf(_edf_message, "ERROR SCHEDULING EDF: \nMartian %s was unable to complete past work before starting new one\n", mrt->name);
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

    _edf_completed_martians = completed_mrt;
    return selected_id;
}

// makes a semi-random movement of a martian in the labyrinth
int moveMartian(int martian_index, martian_t *martian_list, int labyrinth[LAB_HEIGHT][LAB_WIDTH])
{

    martian_t *martian = martian_list + martian_index;
    position_t up = martian->position, down = martian->position, right = martian->position, left = martian->position;
    --up.y;
    ++down.y;
    ++right.x;
    --left.x;

    // printf("up x:%d, y:%d\n", up.x, up.y);
    // printf("left x:%d, y:%d   ", left.x, left.y);
    // printf(" -- x:%d, y:%d  --  ", martian->position.x, martian->position.y);
    // printf("   right x:%d, y:%d\n", right.x, right.y);
    // printf("down x:%d, y:%d\n", down.x, down.y);

    // try moving right
    if (right.x >= 0 && right.x < LAB_WIDTH && labyrinth[right.y][right.x] == LAB_EMPTY && !(right.y == martian->previous_position.y && right.x == martian->previous_position.x))
    {
        // move up
        martian->previous_position = martian->position;
        labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = right;
        labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
    }

    //try moving up
    else if (up.y >= 0 && up.y < LAB_HEIGHT && labyrinth[up.y][up.x] == 0 && !(up.y == martian->previous_position.y && up.x == martian->previous_position.x))
    {
        // move up
        martian->previous_position = martian->position;
        labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = up;
        labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
    }

    // try moving down
    else if (down.y >= 0 && down.y < LAB_HEIGHT && labyrinth[down.y][down.x] == 0 && !(down.y == martian->previous_position.y && down.x == martian->previous_position.x))
    {
        // move up
        martian->previous_position = martian->position;
        labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = down;
        labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
    }

    //try moving left
    else if (left.x >= 0 && left.x < LAB_WIDTH && labyrinth[left.y][left.x] == LAB_EMPTY && !(left.y == martian->previous_position.y && left.x == martian->previous_position.x))
    {
        // move up
        martian->previous_position = martian->position;
        labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = left;
        labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
    }

    // the martian couldn't move anywhere to a new position, try moving to previous position
    else if (labyrinth[martian->previous_position.y][martian->previous_position.x] == LAB_EMPTY)
    {
        left = martian->position;
        labyrinth[martian->position.y][martian->position.x] = LAB_EMPTY;
        martian->position = martian->previous_position;
        martian->previous_position = left;
        labyrinth[martian->position.y][martian->position.x] = LAB_MARTIAN;
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

    while (_rm_simulation_state == SIM_RUNNING)
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
            moveMartian(selected_martian_id, _rm_martians, _rm_labyrinth);
            mrt = _rm_martians + selected_martian_id;
            // printf("Moved martian %s to position x: %d, y: %d \n", mrt->name, mrt->position.x, mrt->position.y);
            if (mrt->position.x == _end_position.x && mrt->position.y == _end_position.y)
            {
                mrt->state = MRTN_COMPLETED;
                if (++_rm_completed_martians == _num_martians)
                {
                    _rm_simulation_state = SIM_FINISHED;
                    break;
                }
            }
            // log the moving martian
            logRMMartian(mrt);
        }
        else
        {
            logRMNOP();
        }

        usleep(_time_step);
        // printf("Simulation time is: %d \n", _sim_timer);
        _sim_timer++;
    } // end while
    // printf("Completed simulation time is: %d \n", _sim_timer);

    if (_rm_simulation_state == SIM_FINISHED || _rm_simulation_state == SIM_ERROR)
    {
        /* code */
    }
    return 0;
}

void simulationStep()
{
    // int selected_martian_id;
    martian_t *mrt;

    int rm_martian_id = rateMonotonicScheduling();
    int edf_martian_id = earliestDeadlineFirst();

    _rm_last_moved_martian = rm_martian_id;
    _edf_last_moved_martian = edf_martian_id;

    // make loggin for both algorithms

    if (_rm_simulation_state != SIM_FINISHED)
    {
        if (rm_martian_id == NO_SCHEDULING)
            logRMNOP();
        else if (rm_martian_id != SCHEDULING_ERROR)
        {
            logRMMartian(_rm_martians + rm_martian_id);

            moveMartian(rm_martian_id, _rm_martians, _rm_labyrinth);
            mrt = _rm_martians + rm_martian_id;
            // printf("Moved martian %s to position x: %d, y: %d \n", mrt->name, mrt->position.x, mrt->position.y);
            // printf("It's previous position was x: %d, y: %d \n", mrt->previous_position.x, mrt->previous_position.y);
            if (mrt->position.x == _end_position.x && mrt->position.y == _end_position.y)
            {
                _rm_labyrinth[_end_position.y][_end_position.x] = 0;
                mrt->state = MRTN_COMPLETED;
                if ((++_rm_completed_martians) == _num_martians)
                {
                    _rm_simulation_state = SIM_FINISHED;
                }
            }
        }
        else
        {
            _rm_simulation_state = SIM_ERROR;
        }
    }

    if (_edf_simulation_state != SIM_FINISHED)
    {
        /* code */

        if (edf_martian_id == NO_SCHEDULING)
            logEDFNOP();
        else if (edf_martian_id != SCHEDULING_ERROR)
        {
            logEDFMartian(_edf_martians + edf_martian_id);

            moveMartian(edf_martian_id, _edf_martians, _edf_labyrinth);
            mrt = _edf_martians + edf_martian_id;
            // printf("Moved martian %s to position x: %d, y: %d \n", mrt->name, mrt->position.x, mrt->position.y);
            if (mrt->position.x == _end_position.x && mrt->position.y == _end_position.y)
            {
                _edf_labyrinth[_end_position.y][_end_position.x] = 0;
                mrt->state = MRTN_COMPLETED;
                if ((++_edf_completed_martians) == _num_martians)
                {
                    _edf_simulation_state = SIM_FINISHED;
                }
            }
        }
        else
        {
            _edf_simulation_state = SIM_ERROR;
        }
    }

    // printf("Simulation time is: %d \n", _sim_timer);
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

enum app_mode getSelectedMode()
{
    return _sim_mode;
}

int getSimulationState()
{
    return _selected_alg == RATE_MONOTONIC ? _rm_simulation_state : _edf_simulation_state;
}

void setSimulationState(enum sim_state state)
{
    if (_selected_alg == RATE_MONOTONIC)
    {
        _rm_simulation_state = state;
    }
    else

        _edf_simulation_state = state;
}

int getTimeStep()
{
    return _time_step;
}

char *getSimErrorMessage()
{
    return _selected_alg == RATE_MONOTONIC ? _rm_message : _edf_message;
}
void removeAllMartians()
{
    _num_martians = 0;
}
void resetSimulation()
{

    martian_t *mrt;
    for (size_t i = 0; i < _num_martians; i++)
    {
        mrt = _rm_martians + i;
        mrt->remaining_energy = 0;
        mrt->state = MRTN_RUNNING;
        mrt->position.x = _start_position.x;
        mrt->position.y = _start_position.y;
        mrt->previous_position.x = -1;
        mrt->previous_position.y = -1;
    }

    for (size_t i = 0; i < LAB_HEIGHT; ++i)
    {
        for (size_t j = 0; j < LAB_WIDTH; ++j)
        {
            if (_rm_labyrinth[i][j] > 1)
            {
                _rm_labyrinth[i][j] = 0;
            }
            if (_edf_labyrinth[i][j] > 1)
            {
                _edf_labyrinth[i][j] = 0;
            }
        }
    }

    _sim_timer = 0;
    _rm_simulation_state = SIM_INITIAL;
    _edf_simulation_state = SIM_INITIAL;
}