#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <pthread.h>
#include <stdio.h>  // For printf()
#include <stdlib.h> // For exit()
#include <string.h> // For strlen()
#include <unistd.h> // For sleep()

#include "martian.h"

#define TIME_STEP 500 // Simulation time step of 500
#define MAX_MARTIANS 27

#define LAB_EMPTY 0
#define LAB_WALL 1
#define LAB_MARTIAN 2

#define LAB_HEIGHT 10
#define LAB_WIDTH 20


static int _labyrinth[LAB_HEIGHT][LAB_WIDTH] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                               {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                                               {1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
                                               {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                                               {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
                                               {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
                                               {1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
                                               {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1},
                                               {1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1},
                                               {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

// ++++ External funcs ++++
int addMartian(martian_t new_martian);

// starts the simulation loop
int startSimulation();

martian_t getMartian(int id);

int getNumMartians();

// pauses simulation loop
int pauseSimulation();

// ends the simulation
int endSimulation();

// makes the simulation go faster (0 < t_mult < 1) or slower  (1 < t_mult)
int changeSimulationSpeed(int t_mult);

// ++++ Internal funcs ++++

// returns index of the next martian to move acording to RM scheduling
int rateMonotonicScheduling();

// returns index of the next martian to move acording to EDF scheduling
int earliestDeadlineFirst();

// makes a semi-random movement of a mratian in the labyrinth
int moveMartian(int martian_index);

// loop that simulates CPU clock. In charge of managing martians energy and moving them and checking
int simulationLoop();

#endif
