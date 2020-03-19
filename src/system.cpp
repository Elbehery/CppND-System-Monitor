#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "system.h"
#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


std::vector<Process> System::processes_ = {};

System::System(){
    vector<int> pids = LinuxParser::Pids();
    for(auto pid : pids){
        Process process = Process(pid);
        processes_.push_back(process);
    }
}

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { return processes_; }

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }