#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
    Process(int pid);
    int Pid();
    std::string User();
    std::string UserID();
    std::string Command();
    float CpuUtilization();
    std::string Ram();
    long int UpTime();
    bool operator<(Process const& a) const;

private:
    int pid_;
    int uptime_;
    std::string uid_;
    std::string user_;
    std::string command_;
    std::string captureProcessUser(int pid);
    std::string captureProcessCommand(int pid);
};

#endif