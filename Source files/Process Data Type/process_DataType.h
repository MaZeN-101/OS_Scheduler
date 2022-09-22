#pragma once
#include <string>

using namespace std;

typedef struct
{
    string process_name;
    float burst_time = 0;
    float arrival_time = 0;
    int priority = 0;
}process_struct;

struct find_process
{
    string id;
    find_process(string id) : id(id) {}
    bool operator () (const process_struct& m) const
    {
        return m.process_name == id;
    }
};
