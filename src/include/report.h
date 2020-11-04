#ifndef _REPORT_H
#define _REPORT_H

#include <stdio.h>
#include "martian.h"

int initReport();

int endReport();

int logRMMartian(martian_t *martian);
int logEDFMartian(martian_t *martian);

int logRMNOP();
int logEDFNOP();

#endif