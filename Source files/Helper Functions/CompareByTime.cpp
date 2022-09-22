#include "CompareByTime.h"

/* compare each process by the others to order them according to arrival time */
bool compareByArriveTime(const process_struct& a, const process_struct& b)
{
    return a.arrival_time < b.arrival_time;
}

/* compare each process by the others to order them according to burst time */
bool compareByBurstTime(const process_struct& a, const process_struct& b)
{
    return a.burst_time < b.burst_time;
}

/* compare each process by the others to order them according to priority */
bool compareByPriority(const process_struct& a, const process_struct& b)
{
    return a.priority < b.priority;
}
