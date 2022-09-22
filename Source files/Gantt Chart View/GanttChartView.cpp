#include "GanttChartView.h"

/* set percision of float number to 1 */
string percision(float x)
{
    int y = (x * 10);
    y %= 10;
    if (y == 0)
    {
        return to_string((int)x);
    }
    else
    {
        return (to_string((int)x) + "." + to_string(y));
    }
}


string GanttChartView(vector <process_struct> ganttchart)
{
    /* output file in dot extension */
    string my_ganttChart = "digraph structs{subgraph cluster_p{label = Gantt_Chart;node[shape = plaintext];struct1[label = <<TABLE><TR>";
    string find_process_name, process_timeline;
    float find_burst_time, time_line = 0, find_arrive_time;
    int index = -1;

    /* parsing first process */
    find_process_name = ganttchart[0].process_name;
    find_arrive_time = ganttchart[0].arrival_time;
    find_burst_time = ganttchart[0].burst_time;

    /* if process isn't starting at 0 */
    if (find_arrive_time != 0)
    {
        process_timeline = "<TD BGCOLOR=\"red\"><FONT POINT-SIZE=\"20.0\">  <BR/>0   "+ 
                            percision(find_arrive_time) + "</FONT></TD> ";
        my_ganttChart.append(process_timeline);
    }

    ganttchart.erase(ganttchart.begin());

    /* get busrt time of a process according to its name */
    auto it = find_if(ganttchart.begin(), ganttchart.end(), find_process(find_process_name));
    if (it != ganttchart.end())
        index = it - ganttchart.begin();

    /* if process doesn't exist more */
    if (index != -1)
    {
        find_burst_time -= ganttchart[index].burst_time;
        index = -1;
    }

    /* calculate time line */
    time_line = find_arrive_time + find_burst_time;

    process_timeline = "<TD BGCOLOR=\"grey\"><FONT POINT-SIZE=\"20.0\">" + find_process_name + "<BR/>" +
                        percision(find_arrive_time) + "   " + percision(time_line) + "</FONT></TD>";

    my_ganttChart.append(process_timeline);

    while(!ganttchart.empty())
    {
        /* parsing each process */
        find_process_name = ganttchart[0].process_name;
        find_arrive_time = ganttchart[0].arrival_time;
        find_burst_time = ganttchart[0].burst_time;

        ganttchart.erase(ganttchart.begin());

        /* get busrt time of a process according to its name */
        auto it = find_if(ganttchart.begin(), ganttchart.end(), find_process(find_process_name));
        if (it != ganttchart.end())
            index = it - ganttchart.begin();

        /* if process doesn't exist more */
        if (index != -1)
        {
            find_burst_time -= ganttchart[index].burst_time;
            index = -1;
        }

        /* if process still not arrived */
        if (find_arrive_time > time_line)
        {
            process_timeline = "<TD BGCOLOR=\"red\"><FONT POINT-SIZE=\"20.0\">  <BR/>" + percision(time_line) + "   " +
                                percision(find_arrive_time) + "</FONT></TD> ";

            my_ganttChart.append(process_timeline);

            time_line = find_arrive_time;
        }


        process_timeline = "<TD BGCOLOR=\"grey\"><FONT POINT-SIZE=\"20.0\">" + find_process_name + "<BR/>" +
                            percision(time_line) + "   " + percision(time_line + find_burst_time) + "</FONT></TD>";

        my_ganttChart.append(process_timeline);

        /* calculate time line */
        time_line += find_burst_time;

    }

    my_ganttChart.append("</TR></TABLE>>];}}");
    return my_ganttChart;
}
