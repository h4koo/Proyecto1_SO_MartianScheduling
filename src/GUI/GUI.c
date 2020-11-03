#include "../include/GUI.h"

static int firstLoop = 1;

const gchar* valueEnergia;
const gchar* valuePeriodo;

typedef struct{
    float r;
    float g;
    float b;
}color_t;

//order of array: red, green, blue, yellow, cyan
color_t colorArray[5] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,1.0,0.0},{0.0,1.0,1.0}};
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
    canvasMatriz = GTK_WIDGET(gtk_builder_get_object(builder, "canvasMatriz"));
    treeEnergiaMarciano = GTK_WIDGET(gtk_builder_get_object(builder, "treeEnergiaMarciano"));
    buttonRM = GTK_WIDGET(gtk_builder_get_object(builder, "buttonRM"));
    buttonEDF = GTK_WIDGET(gtk_builder_get_object(builder, "buttonEDF"));
    builder = GTK_BUILDER(gtk_builder_get_object(builder, "builder"));

    return 0;
}

gboolean drawMaze(GtkDrawingArea *canvasMatriz, cairo_t *cr){
    if (firstLoop == 1){
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
    return 1;
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

// void on_click_start_simulation()
// {
//     startSimulation();
// }

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

void add_martian(){
    int energy = atoi(valueEnergia);
    int period = atoi(valuePeriodo);

    martian_t martian;
    martian.max_energy = energy;
    martian.period = period;
    addMartian(martian);
}

void getEnergia(GtkEntry *entryEnergia){
    valueEnergia = gtk_entry_get_text(GTK_ENTRY(entryEnergia));
}

void getPeriodo(GtkEntry *entryPeriodo){
    valuePeriodo = gtk_entry_get_text(GTK_ENTRY(entryPeriodo));
}
void *simulation_loop()
{
    enum sim_state state;
    while (getSimulationState() == SIM_RUNNING)
    {
        simulationStep();
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
