#ifndef _GUI_H
#define _GUI_H

#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkx.h>


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
GtkWidget *buttonDetener;
GtkWidget *labelModo;
GtkWidget *buttonAutomatico;
GtkWidget *buttonManual;
GtkWidget *labelVelocidad;
GtkWidget *buttonAumentarVelocidad;
GtkWidget *buttonDisminuirVelocidad;
GtkWidget *canvasMatriz;
GtkWidget *treeEnergiaMarciano;
GtkBuilder *builder;


//Initialize GUI components
int inicializeGUI();

#endif