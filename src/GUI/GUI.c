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
    builder = GTK_BUILDER(gtk_builder_get_object(builder,"builder"));

    return 0;
}

gboolean drawMaze(GtkDrawingArea *widget, cairo_t *cr){
    for (int i = 0; i < LAB_HEIGHT; i++){
        for (int j = 0; j <LAB_WIDTH; j++){
            if (_labyrinth[i][j] == 0){
                //draw white
                cairo_set_source_rgb(cr,1.0,1.0,1.0); //RGB
                cairo_rectangle(cr,j*30,i*30,30,30);
                cairo_fill(cr);
                cairo_stroke(cr);
            }
            else{
                cairo_set_source_rgb(cr,0.0,0.0,0.0); //RGB
                cairo_rectangle(cr,j*30,i*30,30,30);
                cairo_fill(cr);
                cairo_stroke(cr);
            }
        }
    }
}

void on_click_start_simulation(){
    startSimulation();
}

void on_click_stop_simulation(){
    endSimulation();
}

void on_click_increase_speed(){
    makeSimulationFaster();
}

void on_click_reduce_speed(){
    makeSimulationSlower();
}

void select_automatic(){
    modo = true;
}

void select_manual(){
    modo = false;
}