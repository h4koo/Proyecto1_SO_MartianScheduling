#ifndef _REPORT_H
#define _REPORT_H

#include <stdio.h>
#include "martian.h"

int initReport();

int endReport();

int logMartian(martian_t *martian);

int logNOP();

#endif