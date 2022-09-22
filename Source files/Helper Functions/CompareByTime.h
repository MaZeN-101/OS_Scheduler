#pragma once
#include "process_DataType.h"


/* compare each process by the others to order them according to arrival time */
bool compareByArriveTime(const process_struct& a, const process_struct& b);

/* compare each process by the others to order them according to burst time */
bool compareByBurstTime(const process_struct& a, const process_struct& b);

/* compare each process by the others to order them according to priority */
bool compareByPriority(const process_struct& a, const process_struct& b);