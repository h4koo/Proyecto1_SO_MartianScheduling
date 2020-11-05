#ifndef _GUI_H
#define _GUI_H

#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkx.h>
#include "simulation.h"
#include <string.h>

GtkWidget *mainWindow;
GtkWidget *mainGrid;
GtkWidget *entryEnergia;
GtkWidget *entryPeriodo;
GtkWidget *buttonAgregar;
GtkWidget *labelEnergia;
GtkWidget *labelPeriodo;
GtkWidget *labelAgregarMarciano;
GtkWidget *labelSimulacion;
GtkWidget *buttonComenzar;
GtkWidget *buttonPausar;
GtkWidget *buttonDetener;
GtkWidget *labelModo;
GtkWidget *buttonAutomatico;
GtkWidget *buttonManual;
GtkWidget *labelVelocidad;
GtkWidget *buttonAumentarVelocidad;
GtkWidget *buttonDisminuirVelocidad;
GtkWidget *treeEnergiaMarciano;
GtkWidget *buttonRM;
GtkWidget *buttonEDF;
GtkWidget *matrixGrid;
GtkWidget *energyGrid;
GtkBuilder *builder;

//Initialize GUI components
int inicializeGUI();

void initializeEnergyDisplay();

void insertButtons();

//Start Simulation
void on_click_start_simulation();

//Stop Simulation
void on_click_stop_simulation();

//Increase Speed of Simulation
void on_click_increase_speed();

//Decrease Speed of Simulation
void on_click_reduce_speed();

//Automatic Mode Select
void select_automatic();

//Manual Mode Select
void select_manual();

//Select RM Algorithm
void select_RM();

//Select EDF Algorithm
void select_EDF();

void drawMartian();

void energyDisplay();

//Simulation loop
void *simulation_loop();

#endif