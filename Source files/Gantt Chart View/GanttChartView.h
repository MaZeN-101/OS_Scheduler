#pragma once
#include <vector>
#include <fstream>
#include "process_DataType.h"
using namespace std;

string GanttChartView(vector <process_struct> ganttchart);
float precision(float f, int places);