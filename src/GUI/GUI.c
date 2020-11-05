#include "../include/GUI.h"

const gchar *valueEnergia;
const gchar *valuePeriodo;

GtkWidget *arrayButtons[LAB_HEIGHT][LAB_WIDTH];
GtkWidget *arrayImagenes[LAB_HEIGHT][LAB_WIDTH];

GtkWidget *arrayMartianButton[MAX_MARTIANS];
GtkWidget *arrayMartianImage[MAX_MARTIANS];
GtkWidget *arrayNameButton[MAX_MARTIANS];
GtkWidget *arrayEnergyButton[MAX_MARTIANS];

const char *arraySprites[6] = {"GUI/img/red.png",
                               "GUI/img/green.png",
                               "GUI/img/blue.png",
                               "GUI/img/yellow.png",
                               "GUI/img/magenta.png",
                               "GUI/img/cyan.png"};

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

            if (_labyrinth[row][column] == 0)
            {
                arrayImagenes[row][column] = gtk_image_new_from_file("GUI/img/white.png");
            }
            else
            {
                arrayImagenes[row][column] = gtk_image_new_from_file("GUI/img/black.png");
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

    if (getSimulationState() == SIM_INITIAL)
    {
        initReport();
    }

    setSimulationState(SIM_RUNNING);

    // start simulation loop in a thread !!!!!!!!!!!!!!!!!!!!
    pthread_create(&_running_sim_thread, NULL, simulation_loop, NULL);
}

void on_click_pause_simulation()
{
    pauseSimulation();
}

void on_click_stop_simulation()
{
    endSimulation();
    resetMartian();
    resetEnergyGrid();
    resetEnergyGrid();
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
    int energy = atoi(valueEnergia);
    int period = atoi(valuePeriodo);

    martian_t martian;
    martian.max_energy = energy;
    martian.period = period;
    addMartian(martian);

    martian_t *martianList = getMartianList();
    int numMartians = getNumMartians();

    //Agrega Display del Sprite del Marciano
    arrayMartianButton[numMartians - 1] = gtk_button_new();
    arrayMartianImage[numMartians - 1] = gtk_image_new_from_file(arraySprites[numMartians - 1]);
    gtk_button_set_image((GtkButton *)arrayMartianButton[numMartians - 1], (GtkWidget *)arrayMartianImage[numMartians - 1]);
    gtk_grid_attach((GtkGrid *)energyGrid, arrayMartianButton[numMartians - 1], 0, numMartians, 1, 1);

    //Agrega Display del Nombre del Marciano
    arrayNameButton[numMartians - 1] = gtk_button_new();
    gtk_grid_attach((GtkGrid *)energyGrid, arrayNameButton[numMartians - 1], 1, numMartians, 1, 1);
    gtk_button_set_label((GtkButton *)arrayNameButton[numMartians - 1], martianList[numMartians - 1].name);

    //Agrega Display del Valor de Energia del Marciano
    arrayEnergyButton[numMartians - 1] = gtk_button_new();
    gtk_grid_attach((GtkGrid *)energyGrid, arrayEnergyButton[numMartians - 1], 2, numMartians, 1, 1);
    gtk_button_set_label((GtkButton *)arrayEnergyButton[numMartians - 1], valueEnergia);

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

    gtk_image_set_from_file((GtkImage *)arrayImagenes[m->previous_position.y][m->previous_position.x], "GUI/img/white.png");
    gtk_image_set_from_file((GtkImage *)arrayImagenes[m->position.y][m->position.x], arraySprites[m->id]);
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
            gtk_image_set_from_file((GtkImage *)arrayImagenes[martianList[i].position.y][martianList[i].position.x], "GUI/img/white.png");
            gtk_button_set_label((GtkButton *)arrayEnergyButton, "DONE");
        }
    }
    return;
}

void resetMartian()
{
    martian_t *martianList = getMartianList();
    int numMartians = getNumMartians();

    for (int i = 0; i < numMartians; ++i)
    {
        gtk_image_set_from_file((GtkImage *)arrayImagenes[martianList[i].position.y][martianList[i].position.x], "GUI/img/white.png");
    }
    return;
}

void resetEnergyGrid()
{
    for (int i = 1; i < MAX_MARTIANS; ++i)
    {
        gtk_grid_remove_row((GtkGrid *)energyGrid, i);
    }
    return;
}

void launchReport()
{
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

void *simulation_loop()
{
    enum sim_state state;
    while (getSimulationState() == SIM_RUNNING)
    {
        simulationStep();
        setTimer();
        doneMartian();
        drawMartian();
        state = getSimulationState();
        if (state == SIM_FINISHED || state == SIM_ERROR)
        {
            endReport();
            break;
        }

        usleep(getTimeStep());
    }
    return 0;
}
