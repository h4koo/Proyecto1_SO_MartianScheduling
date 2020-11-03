#include "../include/report.h"

static FILE *_report;

int initReport()
{
    _report = fopen("report.txt", "w");
    if (_report == NULL)
    {
        printf("Error trying to create the report file");
        return -1;
        // exit(1);
    }
    return 0;
}

int endReport()
{

    int i;
    i = fclose(_report);
    if (i != 0)
    {
        printf("Error trying to close the report file");
        return -1;
    }
    return 0;
}

int logMartian(martian_t *martian)
{
    int wr_byte = fprintf(_report, "%s\n", martian->name);

    if (wr_byte < 0)
        return -1;
    return 0;
}

int logNOP()
{
    int wr_byte = fprintf(_report, "NOP\n");
    if (wr_byte < 0)
        return -1;
    return 0;
}