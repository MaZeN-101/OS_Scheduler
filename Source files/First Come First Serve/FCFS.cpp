#include "FCFS.h"
#include "CompareByTime.h"
#include "GanttChartView.h"


float FCFS(int num_of_processes, process_struct *process)
{
    queue <process_struct> Schedule;                //queue the processes according to arrival time 
    queue <process_struct> arrive_schedule;         //store all process arrive times 
    vector <process_struct> Gantt_chart;            //queue processes time line
    vector <process_struct> arrange_arrive_time;      //arrange processes according to arrival time

    int iterations;
    float time_taken = 0, Average_wait = 0;

    iterations = num_of_processes;

    /* enter each process for arranging and processing */
    for (int i = 0; i < iterations; i++)
    {
        arrange_arrive_time.push_back(process[i]);
    }

    /* sort processes according to arrival time */
    sort(arrange_arrive_time.begin(), arrange_arrive_time.end(), compareByArriveTime);

    /* queue the sorted processes into queue */
    for (const auto& e : arrange_arrive_time)
        arrive_schedule.push(e);

    Schedule.push(arrive_schedule.front());
    arrive_schedule.pop();


    /* schuduling processes */
    for (int i = 0; i < iterations; i++)
    {
        Gantt_chart.push_back(Schedule.front());

        /* calculate time taken for each process */
        if (Schedule.front().arrival_time > time_taken)
            time_taken = Schedule.front().burst_time + Schedule.front().arrival_time;
        else
            time_taken += Schedule.front().burst_time;

        /* calculate waiting time for each process */
        Average_wait += time_taken - Schedule.front().arrival_time - Schedule.front().burst_time;
        Schedule.pop();


        /* if a process's arrival time came */
        while (!arrive_schedule.empty())
        {
            if (arrive_schedule.front().arrival_time <= time_taken)
            {
                Schedule.push(arrive_schedule.front());
                arrive_schedule.pop();
            }
            else
                break;
        }

        if (!arrive_schedule.empty() && Schedule.empty())
        {
            Schedule.push(arrive_schedule.front());
            arrive_schedule.pop();
        }
    }

    /* calculate average waiting time */
    Average_wait = Average_wait / num_of_processes;


    /* generate gantt chart using graphViz*/
    ofstream file("FCFS.dot");

    if (file.is_open())
    {
        file << GanttChartView(Gantt_chart);
        file.close();
    }

    system("dot -Tpng FCFS.dot -o FCFS.png");

    system("FCFS.png");

    return Average_wait;
}