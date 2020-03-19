#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <iostream>
using std::string;
using std::vector;
using std::stol;


float Processor::Utilization() {
    vector<string> cpuMeasures = LinuxParser::CpuUtilization();

    long userRead = stol(cpuMeasures[2]);
    long niceRead = stol(cpuMeasures[3]);
    long systemRead = stol(cpuMeasures[4]);
    long idleRead = stol(cpuMeasures[5]);
    long iowaitRead = stol(cpuMeasures[6]);
    long irqRead = stol(cpuMeasures[7]);
    long softirqRead = stol(cpuMeasures[8]);
    long stealRead = stol(cpuMeasures[9]);
    long guesRead = stol(cpuMeasures[10]);
    long guestniceRead = stol(cpuMeasures[11]);
    // more accurate values for calculations.
    long user = userRead - guesRead;
    long nice = niceRead - guestniceRead;
    // substitute all values
    long idle = idleRead + iowaitRead;
    long non_idle = user + nice + systemRead + irqRead + softirqRead + stealRead;
    long total = idle + non_idle;

    return non_idle / total;
}