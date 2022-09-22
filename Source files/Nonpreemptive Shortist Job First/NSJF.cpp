#include "NSJF.h"
#include "CompareByTime.h"
#include "GanttChartView.h"


float NSJF(int num_of_processes, process_struct* process)
{

    queue <process_struct> Schedule;                //queue the processes according to arrival time 
    queue <process_struct> arrive_schedule;         //store all process arrive times 
    vector <process_struct> Gantt_chart;            //queue processes time line
    vector <process_struct> arrange_arrive_time;    //arrange processes according to arrival time
    vector <process_struct> arrange_burst_time;     //arrange processes according to burst time

    int index = 0, iterations;
    float time_taken = 0, Average_wait = 0;

    
    iterations = num_of_processes;
    
    /* enter each process name, arrival time and burst time */
    for (int i = 0; i < iterations; i++)
    {
        arrange_arrive_time.push_back(process[i]);
    }


    /* sort processes according to arrival time */
    sort(arrange_arrive_time.begin(), arrange_arrive_time.end(), compareByArriveTime);


    /* sort processes according to burst time if they are equal in arrive time */
    int i = 1;
    while (i != num_of_processes)
    {
        if (arrange_arrive_time[0].arrival_time == arrange_arrive_time[i].arrival_time)
        {
            if (arrange_arrive_time[0].burst_time > arrange_arrive_time[i].burst_time)
                swap(arrange_arrive_time[0], arrange_arrive_time[i]);
            i++;
        }
        else
            break;
    }

    /* queue the sorted processes into queue */
    for (const auto& e : arrange_arrive_time)
        arrive_schedule.push(e);

    Schedule.push(arrive_schedule.front());
    arrive_schedule.pop();

                                    /* schuduling processes in case of non preemptive scheduling */
  
    for (i = 0; i < iterations; i++)
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
                arrange_burst_time.push_back(arrive_schedule.front());
                arrive_schedule.pop();

                if (Schedule.empty())
                {
                    /* enquque process in schudule queue */
                    Schedule.push(arrange_burst_time.front());

                    arrange_burst_time.clear();
                }
                else
                {
                    while (!Schedule.empty())
                    {
                        arrange_burst_time.push_back(Schedule.front());
                        Schedule.pop();
                    }

                    /* sort processes according to shortest burst time */
                    sort(arrange_burst_time.begin(), arrange_burst_time.end(), compareByBurstTime);


                    for (unsigned int j = 0; j < arrange_burst_time.size() - 1; j++)
                    {
                        if (arrange_burst_time[j].burst_time == arrange_burst_time[j + 1].burst_time)
                        {
                            vector <process_struct> arrange;
                            arrange.push_back(arrange_burst_time[j]);
                            arrange.push_back(arrange_burst_time[j + 1]);

                            /* sort processes according to shortest burst time */
                            sort(arrange.begin(), arrange.end(), compareByArriveTime);

                            arrange_burst_time[j] = arrange.front();
                            arrange_burst_time[j + 1] = arrange.back();
                        }
                    }

                    /* enquque processes in schudule queue */
                    for (const auto& e : arrange_burst_time)
                        Schedule.push(e);

                    arrange_burst_time.clear();
                }

            }
            else
                break;
        }

        if (!arrive_schedule.empty() && Schedule.empty())
        {
            Schedule.push(arrive_schedule.front());
            arrive_schedule.pop();
        }
        iterations += (Schedule.size() + arrive_schedule.size() + arrange_burst_time.size()) - (iterations - (i + 1));
    }

    /* calculate average waiting time */
    Average_wait = Average_wait / num_of_processes;


    /* generate gantt chart using graphViz*/
    ofstream file("NSJF.dot");

    if (file.is_open())
    {
        file << GanttChartView(Gantt_chart);
        file.close();
    }

    system("dot -Tpng NSJF.dot -o NSJF.png");

    system("NSJF.png");

    return Average_wait;
}