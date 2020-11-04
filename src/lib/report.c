#include "../include/report.h"

static FILE *_rm_report;
static FILE *_edf_report;

int initReport()
{
    _rm_report = fopen("rm_report.txt", "w");
    _edf_report = fopen("edf_report.txt", "w");
    if (_rm_report == NULL)
    {
        printf("Error trying to create the RM report file");
        return -1;
        // exit(1);
    }
    if (_edf_report == NULL)
    {
        printf("Error trying to create the EDF report file");
        return -1;
        // exit(1);
    }
    return 0;
}

int endReport()
{

    int i, j;
    i = fclose(_rm_report);
    j = fclose(_edf_report);
    if (i != 0)
    {
        printf("Error trying to close the RM report file");
        return -1;
    }
    if (j != 0)
    {
        printf("Error trying to close the EDF report file");
        return -1;
    }
    return 0;
}

int logEDFMartian(martian_t *martian)
{
    int wr_byte = fprintf(_edf_report, "%s\n", martian->name);

    if (wr_byte < 0)
        return -1;
    return 0;
}

int logRMMartian(martian_t *martian)
{
    int wr_byte = fprintf(_rm_report, "%s\n", martian->name);

    if (wr_byte < 0)
        return -1;
    return 0;
}

int logRMNOP()
{
    int wr_byte = fprintf(_rm_report, "NOP\n");
    if (wr_byte < 0)
        return -1;
    return 0;
}
int logEDFNOP()
{
    int wr_byte = fprintf(_edf_report, "NOP\n");
    if (wr_byte < 0)
        return -1;
    return 0;
}