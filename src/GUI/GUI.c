#include "../include/GUI.h"
#include "../include/simulation.h"

int inicializeGUI(){

    mainGrid = GTK_WIDGET(gtk_builder_get_object(builder,"mainGrid"));
    entryEnergia = GTK_WIDGET(gtk_builder_get_object(builder,"entryEnergia"));
    entryPeriodo = GTK_WIDGET(gtk_builder_get_object(builder,"entryPeriodo"));
    buttonAgregar = GTK_WIDGET(gtk_builder_get_object(builder,"buttonAgregar"));
    labelEnergia = GTK_WIDGET(gtk_builder_get_object(builder,"labelEnergia"));
    labelPeriodo = GTK_WIDGET(gtk_builder_get_object(builder,"labelPeriodo"));
    labelAgregarMarciano = GTK_WIDGET(gtk_builder_get_object(builder,"labelAgregarMarciano"));
    labelSimulacion = GTK_WIDGET(gtk_builder_get_object(builder,"labelSimulacion"));
    buttonComenzar = GTK_WIDGET(gtk_builder_get_object(builder,"buttonComenzar"));
    buttonDetener = GTK_WIDGET(gtk_builder_get_object(builder,"buttonDetener"));
    labelModo = GTK_WIDGET(gtk_builder_get_object(builder,"labelModo"));
    buttonAutomatico = GTK_WIDGET(gtk_builder_get_object(builder,"buttonAutomatico"));
    buttonManual = GTK_WIDGET(gtk_builder_get_object(builder,"buttonManual"));
    labelVelocidad = GTK_WIDGET(gtk_builder_get_object(builder,"labelVelocidad"));
    buttonAumentarVelocidad = GTK_WIDGET(gtk_builder_get_object(builder,"buttonAumentarVelocidad"));
    buttonDisminuirVelocidad = GTK_WIDGET(gtk_builder_get_object(builder,"buttonDisminuirVelocidad"));
    canvasMatriz = GTK_WIDGET(gtk_builder_get_object(builder,"canvasMatriz"));
    treeEnergiaMarciano = GTK_WIDGET(gtk_builder_get_object(builder,"treeEnergiaMarciano"));
    builder = GTK_WIDGET(gtk_builder_get_object(builder,"builder"));

    return 0
}