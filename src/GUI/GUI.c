#include "../include/GUI.h"

const gchar *valueEnergia;
const gchar *valuePeriodo;

GtkWidget *arrayButtons[LAB_HEIGHT][LAB_WIDTH];
GtkWidget *arrayImagenes[LAB_HEIGHT][LAB_WIDTH];

GtkWidget *arrayMartianButton[MAX_MARTIANS];
GtkWidget *arrayMartianImage[MAX_MARTIANS];
GtkWidget *arrayNameButton[MAX_MARTIANS];
GtkWidget *arrayEnergyLabel[MAX_MARTIANS];
GError **error;

GdkPixbuf *red_image;
GdkPixbuf *green_image;
GdkPixbuf *blue_image;
GdkPixbuf *yellow_image;
GdkPixbuf *cyan_image;
GdkPixbuf *magenta_image;

GdkPixbuf *white_image;
GdkPixbuf *black_image;

const char *arraySprites[6] = {"GUI/img/red.png",
                               "GUI/img/green.png",
                               "GUI/img/blue.png",
                               "GUI/img/yellow.png",
                               "GUI/img/magenta.png",
                               "GUI/img/cyan.png"};
GdkPixbuf *sprite_images[MAX_MARTIANS];
static pthread_t _running_sim_thread;

int inicializeGUI()
{
    mainGrid = GTK_WIDGET(gtk_builder_get_object(builder, "mainGrid"));
    entryEnergia = GTK_WIDGET(gtk_builder_get_object(builder, "entryEnergia"));
    entryPeriodo = GTK_WIDGET(gtk_builder_get_object(builder, "entryPeriodo"));
    buttonAgregar = GTK_WIDGET(gtk_builder_get_object(builder, "buttonAgregar"));
    labelEnergia = GTK_WIDGET(gtk_builder_get_object(builder, "labelEnergia"));
    labelPeriodo = GTK_WIDGET(gtk_builder_get_object(builder, "labelPeriodo"));
    labelAgregarMarciano = GTK_WIDGET(gtk_builder_get_object(builder, "labelAgregarMarciano"));
    labelSimulacion = GTK_WIDGET(gtk_builder_get_object(builder, "labelSimulacion"));
    buttonComenzar = GTK_WIDGET(gtk_builder_get_object(builder, "buttonComenzar"));
    buttonPausar = GTK_WIDGET(gtk_builder_get_object(builder, "buttonPausar"));
    buttonDetener = GTK_WIDGET(gtk_builder_get_object(builder, "buttonDetener"));
    labelModo = GTK_WIDGET(gtk_builder_get_object(builder, "labelModo"));
    buttonAutomatico = GTK_WIDGET(gtk_builder_get_object(builder, "buttonAutomatico"));
    buttonManual = GTK_WIDGET(gtk_builder_get_object(builder, "buttonManual"));
    labelVelocidad = GTK_WIDGET(gtk_builder_get_object(builder, "labelVelocidad"));
    buttonAumentarVelocidad = GTK_WIDGET(gtk_builder_get_object(builder, "buttonAumentarVelocidad"));
    buttonDisminuirVelocidad = GTK_WIDGET(gtk_builder_get_object(builder, "buttonDisminuirVelocidad"));
    treeEnergiaMarciano = GTK_WIDGET(gtk_builder_get_object(builder, "treeEnergiaMarciano"));
    buttonRM = GTK_WIDGET(gtk_builder_get_object(builder, "buttonRM"));
    buttonEDF = GTK_WIDGET(gtk_builder_get_object(builder, "buttonEDF"));
    timerLabel = GTK_WIDGET(gtk_builder_get_object(builder, "timerLabel"));
    messageLabel = GTK_WIDGET(gtk_builder_get_object(builder, "messageLabel"));

    matrixGrid = GTK_WIDGET(gtk_builder_get_object(builder, "matrixGrid"));
    energyGrid = GTK_WIDGET(gtk_builder_get_object(builder, "energyGrid"));

    builder = GTK_BUILDER(gtk_builder_get_object(builder, "builder"));

    red_image = gdk_pixbuf_new_from_file("GUI/img/red.png", error);
    green_image = gdk_pixbuf_new_from_file("GUI/img/green.png", error);
    blue_image = gdk_pixbuf_new_from_file("GUI/img/blue.png", error);
    yellow_image = gdk_pixbuf_new_from_file("GUI/img/yellow.png", error);
    cyan_image = gdk_pixbuf_new_from_file("GUI/img/cyan.png", error);
    magenta_image = gdk_pixbuf_new_from_file("GUI/img/magenta.png", error);

    white_image = gdk_pixbuf_new_from_file("GUI/img/white.png", error);
    black_image = gdk_pixbuf_new_from_file("GUI/img/black.png", error);

    //set up the Image pixbufs
    sprite_images[0] = red_image;
    sprite_images[1] = green_image;
    sprite_images[2] = blue_image;
    sprite_images[3] = yellow_image;
    sprite_images[4] = cyan_image;
    sprite_images[5] = magenta_image;
    // sprite_images[0] = red_image;
    // , white_image, black_image, red_image, white_image, black_image};

    g_signal_connect(G_OBJECT(mainGrid), "key_press_event", G_CALLBACK(key_pressed), NULL);

    insertButtons();

    return 0;
}

gboolean key_pressed(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    if (event->keyval == GDK_KEY_x)
    {
        on_click_stop_simulation();
        return TRUE;
    }
    return FALSE;
}

void insertButtons()
{
    for (int row = 0; row < LAB_HEIGHT; ++row)
    {
        for (int column = 0; column < LAB_WIDTH; ++column)
        {
            arrayButtons[row][column] = gtk_button_new();

            if (_rm_labyrinth[row][column] == 0)
            {
                // arrayImagenes[row][column] = gtk_image_new_from_file("GUI/img/white.png");
                arrayImagenes[row][column] = gtk_image_new_from_pixbuf(white_image);
            }
            else
            {
                arrayImagenes[row][column] = gtk_image_new_from_pixbuf(black_image);
                // arrayImagenes[row][column] = gtk_image_new_from_file("GUI/img/black.png");
            }

            gtk_button_set_image((GtkButton *)arrayButtons[row][column], (GtkWidget *)arrayImagenes[row][column]);

            gtk_grid_attach((GtkGrid *)matrixGrid, arrayButtons[row][column], column, row, 1, 1);
        }
    }
}

void on_click_start_simulation()
{
    enum sim_state state = getSimulationState();
    if (state == SIM_RUNNING)
        return;

    int num_martians = getNumMartians();
    if (num_martians == 0)
    {
        printf("No Martians have been added, no need to start the simulation");
        return;
    }

    if (state == SIM_FINISHED || state == SIM_ERROR)
    { //  if the state is finished or error
        resetMartian();
        restartSimulation();
        initReport();
        // reset Button label, timer, remainign energies and clean up board of martians
    }

    if (state == SIM_INITIAL)
    {
        initReport();
    }

    setSimulationState(SIM_RUNNING);
    gtk_button_set_label((GtkButton *)buttonComenzar, "Running");
    gtk_button_set_label((GtkButton *)buttonDetener, "Stop");
    // start simulation loop in a thread !!!!!!!!!!!!!!!!!!!!
    pthread_create(&_running_sim_thread, NULL, simulation_loop, NULL);
}

void on_click_pause_simulation()
{
    enum sim_state state = getSimulationState();

    if (state == SIM_RUNNING)
    {
        pauseSimulation();
        gtk_button_set_label((GtkButton *)buttonComenzar, "Resume");
    }
}

void on_click_stop_simulation()
{
    enum sim_state state = getSimulationState();

    if (state == SIM_RUNNING)
    {
        endSimulation();
        gtk_button_set_label((GtkButton *)buttonComenzar, "Restart");
        gtk_button_set_label((GtkButton *)buttonDetener, "Reset");
        // launchReport();
    }

    else if (state == SIM_ERROR || state == SIM_FINISHED)
    {
        resetMartian();
        resetEnergyGrid();
        resetEnergyGrid();
        removeAllMartians();

        gtk_button_set_label((GtkButton *)buttonComenzar, "Start");
        gtk_button_set_label((GtkButton *)buttonDetener, "Stop");
    }

    // // resetMartian();
    // // resetEnergyGrid();
    // // resetEnergyGrid();

    // gtk_button_set_label((GtkButton *)buttonComenzar, "Restart");
}

void on_click_increase_speed()
{
    makeSimulationFaster();
}

void on_click_reduce_speed()
{
    makeSimulationSlower();
}

void select_automatic()
{
    selectModeAutomatic();
}

void select_manual()
{
    selectModeManual();
}

void select_RM()
{
    selectAlgRM();
}

void select_EDF()
{
    selectAlgEDF();
}

void add_martian()
{
    enum app_mode sel_mode = getSelectedMode();
    enum sim_state state = getSimulationState();
    if (sel_mode == AUTO && state == SIM_RUNNING)
        return;

    if (valueEnergia == NULL || valuePeriodo == NULL)
        return;
    int energy = atoi(valueEnergia);
    int period = atoi(valuePeriodo);

    martian_t martian;
    martian.max_energy = energy;
    martian.period = period;
    int res_add = addMartian(martian);

    if (res_add < 0)
    {
        return;
    }

    martian_t *martianList = getMartianList();
    int numMartians = getNumMartians();

    //Agrega Display del Sprite del Marciano
    arrayMartianButton[numMartians - 1] = gtk_button_new();
    arrayMartianImage[numMartians - 1] = gtk_image_new_from_pixbuf(sprite_images[numMartians - 1]);
    gtk_button_set_image((GtkButton *)arrayMartianButton[numMartians - 1], (GtkWidget *)arrayMartianImage[numMartians - 1]);
    gtk_grid_attach((GtkGrid *)energyGrid, arrayMartianButton[numMartians - 1], 0, numMartians, 1, 1);

    //Agrega Display del Nombre del Marciano
    arrayNameButton[numMartians - 1] = gtk_button_new();
    gtk_grid_attach((GtkGrid *)energyGrid, arrayNameButton[numMartians - 1], 1, numMartians, 1, 1);
    gtk_button_set_label((GtkButton *)arrayNameButton[numMartians - 1], martianList[numMartians - 1].name);

    //Agrega Display del Valor de Energia del Marciano
    arrayEnergyLabel[numMartians - 1] = gtk_label_new("");
    gtk_grid_attach((GtkGrid *)energyGrid, arrayEnergyLabel[numMartians - 1], 2, numMartians, 1, 1);
    gtk_label_set_text((GtkLabel *)arrayEnergyLabel[numMartians - 1], valueEnergia);
    // gtk_button_set_label((GtkButton *)arrayEnergyLabel[numMartians - 1], valueEnergia);

    //Muestra los nuevos Display (botones)
    gtk_widget_show_all(mainWindow);
}

void getEnergia(GtkEntry *entryEnergia)
{
    valueEnergia = gtk_entry_get_text(GTK_ENTRY(entryEnergia));
}

void getPeriodo(GtkEntry *entryPeriodo)
{
    valuePeriodo = gtk_entry_get_text(GTK_ENTRY(entryPeriodo));
}

void drawMartian()
{
    martian_t *m = getRunningMartian();

    if (m == NULL)
    {
        return;
    }
    if (m->previous_position.y != -1)
    {
        gtk_image_set_from_pixbuf((GtkImage *)arrayImagenes[m->previous_position.y][m->previous_position.x], white_image);
    }

    if (m->state == MRTN_COMPLETED)
    {
        gtk_image_set_from_pixbuf((GtkImage *)arrayImagenes[m->position.y][m->position.x], white_image);
        gtk_label_set_text((GtkLabel *)arrayEnergyLabel[m->id], "DONE");
    }

    else
    {
        gtk_image_set_from_pixbuf((GtkImage *)arrayImagenes[m->position.y][m->position.x], sprite_images[m->id]);

        char rem_energy[4];
        sprintf(rem_energy, "%d", m->remaining_energy);
        gtk_label_set_text((GtkLabel *)arrayEnergyLabel[m->id], rem_energy);
    }

    // gtk_button_set_label((GtkButton *)arrayEnergyLabel[m->id], rem_energy);

    gtk_widget_show_all(mainWindow);
}
void doneMartian()
{
    martian_t *martianList = getMartianList();
    int numMartians = getNumMartians();

    for (int i = 0; i < numMartians; ++i)
    {
        if (martianList[i].state == MRTN_COMPLETED)
        {
            gtk_image_set_from_pixbuf((GtkImage *)arrayImagenes[martianList[i].position.y][martianList[i].position.x], white_image);
            gtk_label_set_text((GtkLabel *)arrayEnergyLabel[martianList[i].id], "DONE");
            // gtk_button_set_label((GtkButton *)arrayEnergyLabel[martianList[i].id], "DONE");
        }

        printf("martian id i=%d: %d\n", i, martianList[i].id);

        int remEnergy = martianList[i].remaining_energy;
        char energyDisplay[4];
        sprintf(energyDisplay, "%d", remEnergy);

        gtk_button_set_label((GtkButton *)arrayEnergyLabel[i], energyDisplay);
    }
    return;
}

void resetMartian()
{
    martian_t *martianList = getMartianList();
    int numMartians = getNumMartians();

    for (int i = 0; i < numMartians; ++i)
    {
        gtk_image_set_from_pixbuf((GtkImage *)arrayImagenes[martianList[i].position.y][martianList[i].position.x], white_image);
    }
    return;
}

void resetEnergyGrid()
{
    int n = getNumMartians();
    for (int i = 1; i <= n; ++i)
    {
        gtk_grid_remove_row((GtkGrid *)energyGrid, i);
    }
    return;
}

void launchReport()
{
    endReport();
    system("python3 ../visual/main.py");
    return;
}

void setTimer()
{
    int timer = getSimulationTime();
    char stimer[10];
    sprintf(stimer, "%d", timer);

    gtk_label_set_text((GtkLabel *)timerLabel, stimer);

    return;
}
void controlButtonsSimEnd()
{
    // gtk_button_set_label((GtkButton *)buttonComenzar, "Restart");
    // gtk_button_set_label((GtkButton *)buttonDetener, "Reset");
    // gtk_widget_show_all(mainWindow);
}

void *simulation_loop()
{
    enum sim_state state = getSimulationState();

    while (state == SIM_RUNNING)
    {
        simulationStep();
        setTimer();
        // doneMartian();
        drawMartian();
        usleep(getTimeStep());

        state = getSimulationState();

        if (state == SIM_FINISHED || state == SIM_ERROR)
        {
            launchReport();

            break;
        }
    }
    // gtk_button_set_label((GtkButton *)buttonComenzar, "Restart");
    // gtk_button_set_label((GtkButton *)buttonDetener, "Reset");
    // gtk_widget_show_all(mainWindow);
    return 0;
}
