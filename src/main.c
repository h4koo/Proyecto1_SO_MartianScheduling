#include "./include/GUI.h"
#include "./include/simulation.h"

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


    //Initialize GUI components
    int inicializeGUI();


    gtk_widget_show(mainWindow);
    gtk_main();
    return EXIT_SUCCESS;
}