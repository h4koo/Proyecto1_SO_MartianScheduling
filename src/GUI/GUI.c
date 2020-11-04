#include "../include/GUI.h"

const gchar* valueEnergia;
const gchar* valuePeriodo;

GtkWidget* arrayButtons[LAB_HEIGHT][LAB_WIDTH];
GtkWidget* arrayImagenes[LAB_HEIGHT][LAB_WIDTH];

char* arraySprites[6] =    {"/home/beer/Desktop/REPOS/Proyecto1_SO_MartianScheduling/src/GUI/img/red.png",
                            "/home/beer/Desktop/REPOS/Proyecto1_SO_MartianScheduling/src/GUI/img/green.png",
                            "/home/beer/Desktop/REPOS/Proyecto1_SO_MartianScheduling/src/GUI/img/blue.png",
                            "/home/beer/Desktop/REPOS/Proyecto1_SO_MartianScheduling/src/GUI/img/yellow.png",
                            "/home/beer/Desktop/REPOS/Proyecto1_SO_MartianScheduling/src/GUI/img/magenta.png",
                            "/home/beer/Desktop/REPOS/Proyecto1_SO_MartianScheduling/src/GUI/img/cyan.png"};

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

    matrixGrid = GTK_WIDGET(gtk_builder_get_object(builder, "matrixGrid"));

    builder = GTK_BUILDER(gtk_builder_get_object(builder, "builder"));

    insertButtons();

    return 0;
}

void insertButtons()
{
    for (int row = 0 ; row < LAB_HEIGHT ; ++row)
    {
        for(int column = 0 ; column < LAB_WIDTH ; ++column)
        {
            arrayButtons[row][column] = gtk_button_new();

            if (_labyrinth[row][column] == 0)
            {
                arrayImagenes[row][column] = gtk_image_new_from_file ("/home/beer/Desktop/REPOS/Proyecto1_SO_MartianScheduling/src/GUI/img/white.png");
            }else{
                arrayImagenes[row][column] = gtk_image_new_from_file ("/home/beer/Desktop/REPOS/Proyecto1_SO_MartianScheduling/src/GUI/img/black.png");
            }

            gtk_button_set_image((GtkButton*)arrayButtons[row][column],(GtkWidget*)arrayImagenes[row][column]);


            gtk_grid_attach((GtkGrid*) matrixGrid, arrayButtons[row][column], column, row, 1,1);
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
}

void getEnergia(GtkEntry *entryEnergia)
{
    valueEnergia = gtk_entry_get_text(GTK_ENTRY(entryEnergia));
}

void getPeriodo(GtkEntry *entryPeriodo)
{
    valuePeriodo = gtk_entry_get_text(GTK_ENTRY(entryPeriodo));
}

void drawMartian(){
    martian_t* arrayMartians = getMartianList();
    int numMartians = getNumMartians();

    for (int i = 0; i < numMartians; i++)
    {
        gtk_image_set_from_file((GtkImage*)arrayImagenes[arrayMartians[i].previous_position.y][arrayMartians[i].previous_position.x],
        "/home/beer/Desktop/REPOS/Proyecto1_SO_MartianScheduling/src/GUI/img/white.png");
        gtk_image_set_from_file((GtkImage*)arrayImagenes[arrayMartians[i].position.y][arrayMartians[i].position.x],arraySprites[i]);
    }
}


void *simulation_loop()
{
    enum sim_state state;
    while (getSimulationState() == SIM_RUNNING)
    {
        simulationStep();
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
