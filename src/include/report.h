#ifndef _REPORT_H
#define _REPORT_H

#include <stdio.h>
#include "martian.h"

FILE report;

int initReport();

int endReport();

int logMartian(martian_t martian);

#endif