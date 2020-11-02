#include "./include/simulation.h"
#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkx.h>


//Global Variables
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
//


//Functions

// static struct art
// {
//     int x;
//     int y;
// } ar;

// void func()
// {
//     struct art p;
//     p.x = 1;
//     p.y = 1;

//     ar.x = p.x;
//     ar.y = p.y;
// }



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


//


int main(int argc, char *argv[])
{
    // martian_t m;
    // m.id = 1;
    // m.max_energy = 10;
    // m.name = "lol";
    // m.period = 5;
    // m.position.x = 0;
    // m.position.y = 4;
    // // m.previous_position = {-1,-1};
    // addMartian(m);
    // printf("\nEl nombre del marciano es %s\n", getMartian(0).name);
    // func();
    // printf("\nEl valor del struct es x: %d  y: %d\n", ar.x, ar.y);
    
    gtk_init(&argc,&argv); // init Gtk

    builder = gtk_builder_new_from_file ("GUI/GUI.glade");
    
    mainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit),NULL);
    gtk_builder_connect_signals(builder,NULL);


    //Initialize variables
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

    gtk_widget_show(mainWindow);
    gtk_main();
    return EXIT_SUCCESS;
}