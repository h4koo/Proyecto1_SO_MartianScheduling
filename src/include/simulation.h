#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <pthread.h>
#include <stdio.h>  // For printf()
#include <stdlib.h> // For exit()
#include <string.h> // For strlen()
#include <unistd.h> // For sleep()

#include "martian.h"
#include "report.h"

#define TIME_STEP 500 // Simulation time step of 500
#define MAX_MARTIANS 6

#define LAB_EMPTY 0
#define LAB_WALL 1
#define LAB_MARTIAN 2

#define LAB_HEIGHT 10
#define LAB_WIDTH 20

enum sim_state
{
    SIM_INITIAL,
    SIM_RUNNING,
    SIM_PAUSED,
    SIM_FINISHED,
    SIM_ERROR
};

extern int _labyrinth[LAB_HEIGHT][LAB_WIDTH];

// ++++ External funcs ++++
int addMartian(martian_t new_martian);

// starts the simulation loop
void startSimulation();

martian_t getMartian(int id);

martian_t *getMartianList();

int getNumMartians();

// pauses simulation loop
void pauseSimulation();

// ends the simulation
void endSimulation();

// makes the simulation go faster (0 < t_mult < 1) or slower  (1 < t_mult)
int changeSimulationSpeed(int t_mult);

void makeSimulationSlower();

void makeSimulationFaster();

// ++++ Internal funcs ++++

// returns index of the next martian to move acording to RM scheduling
int rateMonotonicScheduling();

// returns index of the next martian to move acording to EDF scheduling
int earliestDeadlineFirst();

// makes a semi-random movement of a mratian in the labyrinth
int moveMartian(int martian_index, martian_t *martian_list);

// loop that simulates CPU clock. In charge of managing martians energy and moving them and checking
void *simulationLoop();

void simulationStep();

void selectAlgRM();

void selectAlgEDF();

void selectModeAutomatic();

void selectModeManual();

int getSimulationState();

void setSimulationState(enum sim_state state);

int getTimeStep();
void resetSimulation();

#endif
