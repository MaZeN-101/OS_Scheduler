#include "RoundRobin.h"
#include "CompareByTime.h"
#include "GanttChartView.h"



float RoundRobin(int num_of_processes, process_struct* process, float quantum)
{
    
    queue <process_struct> Schedule;                //queue the processes according to arrival time 
    queue <process_struct> arrive_schedule;         //store all process arrive times 
    vector <process_struct> Gantt_chart;            //queue processes time line
    vector <process_struct> arrange_arrive_time;      //arrange processes according to arrival time

    process_struct temp_store;                      //store a process to schedule another one

    int iterations, index = 0, flag = 0;
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
        /* if burst time of a process <= quantum */
        if (Schedule.front().burst_time <= quantum)
        {
            Gantt_chart.push_back(Schedule.front());

            /* calculate time taken for each process */
            if (Schedule.front().arrival_time > time_taken)
                time_taken = Schedule.front().burst_time + Schedule.front().arrival_time;
            else
                time_taken += Schedule.front().burst_time;

            /* get busrt time of a process according to its name */
            auto it = find_if(arrange_arrive_time.begin(), arrange_arrive_time.end(), find_process(Schedule.front().process_name));
            if (it != arrange_arrive_time.end())
                index = it - arrange_arrive_time.begin();

            /* calculate waiting time for each process */
            Average_wait += time_taken - Schedule.front().arrival_time - arrange_arrive_time[index].burst_time;
            Schedule.pop();

            if (!arrive_schedule.empty() && Schedule.empty())
            {
                Schedule.push(arrive_schedule.front());
                arrive_schedule.pop();
                continue;
            }
            flag = 1;
        }
        /* if burst time of a process > quantum */
        else
        {
            Gantt_chart.push_back(Schedule.front());

            /* calculate time taken for each process */
            if (Schedule.front().arrival_time > time_taken)
                time_taken = Schedule.front().arrival_time + quantum;
            else
                time_taken += quantum;

            Schedule.front().burst_time -= quantum;

            while (!arrive_schedule.empty())
            {
                if (arrive_schedule.front().arrival_time > time_taken && !Schedule.empty())
                {
                    Schedule.push(Schedule.front());
                    Schedule.pop();

                    Gantt_chart.push_back(Schedule.front());

                    /* calculate time taken for each process */
                    if (Schedule.front().burst_time >= quantum)
                        time_taken += quantum;
                    else
                        time_taken += Schedule.front().burst_time;


                    if (Schedule.front().burst_time >= quantum)
                        Schedule.front().burst_time -= quantum;
                    else
                        Schedule.front().burst_time = 0;

                    /* if the process's burst time reached 0 so process must be terminated */
                    if (Schedule.front().burst_time == 0)
                    {
                        /* get busrt time of a process according to its name */
                        auto it = find_if(arrange_arrive_time.begin(), arrange_arrive_time.end(), find_process(Schedule.front().process_name));
                        if (it != arrange_arrive_time.end())
                            index = it - arrange_arrive_time.begin();

                        /* calculate waiting time for each process */
                        Average_wait += time_taken - Schedule.front().arrival_time - arrange_arrive_time[index].burst_time;
                        Schedule.pop();
                    }
                }
                else
                    break;
            }
            if (!Schedule.empty() && arrive_schedule.empty())
            {
                Schedule.push(Schedule.front());
                Schedule.pop();
            }
            else if (Schedule.empty() && arrive_schedule.front().arrival_time > time_taken)
            {
                Schedule.push(arrive_schedule.front());
                arrive_schedule.pop();
            }
            iterations += (Schedule.size() + arrive_schedule.size()) - (iterations - (i + 1));
            flag = 0;
        }

        /* if a process's arrival time came */
        if (!arrive_schedule.empty() && arrive_schedule.front().arrival_time <= time_taken)
        {
            /* if only one process in the schedule */
            if (Schedule.size() == 1)
            {
                temp_store = Schedule.front();
                Schedule.pop();

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

                Schedule.push(temp_store);
            }
            /* if more than one process in the schedule */
            else
            {
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
                if (flag == 0)
                {
                    Schedule.push(Schedule.front());
                    Schedule.pop();
                }
            }
        }
    }

    /* calculate average waiting time */
    Average_wait = Average_wait / num_of_processes;


    /* generate gantt chart using graphViz*/
    ofstream file("RoundRobin.dot");

    if (file.is_open())
    {
        file << GanttChartView(Gantt_chart);
        file.close();
    }

    system("dot -Tpng RoundRobin.dot -o RoundRobin.png");

    system("RoundRobin.png");

    return Average_wait;
}