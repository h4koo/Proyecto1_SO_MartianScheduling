#include "./include/GUI.h"
#include "./include/simulation.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv); // init Gtk

    builder = gtk_builder_new_from_file("GUI/GUI.glade");

    mainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);

    //Initialize GUI components
    inicializeGUI();

    gtk_widget_show_all(mainWindow);
    gtk_main();
    return EXIT_SUCCESS;

    return 0;
}