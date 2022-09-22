#include "PPriority.h"
#include "CompareByTime.h"
#include "GanttChartView.h"


float PPriority(int num_of_processes, process_struct* process)
{

    queue <process_struct> Schedule;                //queue the processes according to arrival time 
    queue <process_struct> arrive_schedule;         //store all process arrive times 
    vector <process_struct> Gantt_chart;            //queue processes time line
    vector <process_struct> arrange_arrive_time;    //arrange processes according to arrival time
    vector <process_struct> arrange_priority;       //arrange processes according to lowest priority

    int index = 0, iterations;
    float time_taken = 0, Average_wait = 0;


    iterations = num_of_processes;

    /* enter each process for arranging and processing */
    for (int i = 0; i < iterations; i++)
    {
        arrange_arrive_time.push_back(process[i]);
    }

    /* sort processes according to arrival time */
    sort(arrange_arrive_time.begin(), arrange_arrive_time.end(), compareByArriveTime);


    /* sort processes according to priority if they are equal in arrive time */
    int i = 1;
    while (i != num_of_processes)
    {
        if (arrange_arrive_time[0].arrival_time == arrange_arrive_time[i].arrival_time)
        {
            if (arrange_arrive_time[0].priority > arrange_arrive_time[i].priority)
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

                                        /* schuduling processes in case of preemptive scheduling */

    for (i = 0; i < iterations; i++)
    {
        Gantt_chart.push_back(Schedule.front());


        if (arrive_schedule.empty() && !Schedule.empty())
        {
            /* calculate time taken for each process */
            if (Schedule.front().arrival_time > time_taken)
                time_taken = Schedule.front().arrival_time + Schedule.front().burst_time;
            else
                time_taken += Schedule.front().burst_time;


            /* get busrt time of a process according to its name */
            auto it = find_if(arrange_arrive_time.begin(), arrange_arrive_time.end(), find_process(Schedule.front().process_name));
            if (it != arrange_arrive_time.end())
                index = it - arrange_arrive_time.begin();

            /* calculate waiting time for each process */
            Average_wait += time_taken - Schedule.front().arrival_time - arrange_arrive_time[index].burst_time;

            Schedule.pop();
        }

        while (!arrive_schedule.empty())
        {
            if (arrive_schedule.front().arrival_time > time_taken)
            {
                /* calculate time taken for each process */
                if (Schedule.front().arrival_time > time_taken)
                    time_taken = Schedule.front().arrival_time + 1;
                else
                    time_taken++;

                Schedule.front().burst_time--;

                /* calculate waiting time for each process */
                if (Schedule.front().burst_time == 0)
                {
                    /* get busrt time of a process according to its name */
                    auto it = find_if(arrange_arrive_time.begin(), arrange_arrive_time.end(), find_process(Schedule.front().process_name));
                    if (it != arrange_arrive_time.end())
                        index = it - arrange_arrive_time.begin();

                    /* calculate waiting time for each process */
                    Average_wait += time_taken - Schedule.front().arrival_time - arrange_arrive_time[index].burst_time;

                    Schedule.pop();
                    break;
                }
            }
            else
                break;
        }

        /* if a process's arrival time came */
        while (!arrive_schedule.empty())
        {
            if (arrive_schedule.front().arrival_time <= time_taken)
            {
                arrange_priority.push_back(arrive_schedule.front());
                arrive_schedule.pop();

                if (Schedule.empty())
                {
                    /* enquque process in schudule queue */
                    Schedule.push(arrange_priority.front());

                    arrange_priority.clear();
                }
                else
                {
                    while (!Schedule.empty())
                    {
                        arrange_priority.push_back(Schedule.front());
                        Schedule.pop();
                    }

                    /* sort processes according to priority */
                    sort(arrange_priority.begin(), arrange_priority.end(), compareByPriority);


                    for (unsigned int j = 0; j < arrange_priority.size() - 1; j++)
                    {
                        if (arrange_priority[j].priority == arrange_priority[j + 1].priority)
                        {
                            vector <process_struct> arrange;
                            arrange.push_back(arrange_priority[j]);
                            arrange.push_back(arrange_priority[j + 1]);

                            /* sort processes according to arrival time */
                            sort(arrange.begin(), arrange.end(), compareByArriveTime);

                            arrange_priority[j] = arrange.front();
                            arrange_priority[j + 1] = arrange.back();
                        }
                    }

                    /* enquque processes in schudule queue */
                    for (const auto& e : arrange_priority)
                        Schedule.push(e);

                    arrange_priority.clear();
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
        iterations += (Schedule.size() + arrive_schedule.size() + arrange_priority.size()) - (iterations - (i + 1));
    }


    /* calculate average waiting time */
    Average_wait = Average_wait / num_of_processes;


    /* generate gantt chart using graphViz*/
    ofstream file("PPriority.dot");

    if (file.is_open())
    {
        file << GanttChartView(Gantt_chart);
        file.close();
    }

    system("dot -Tpng PPriority.dot -o PPriority.png");

    system("PPriority.png");

    return Average_wait;
}