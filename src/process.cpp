#include "process.h"
#include "linux_parser.h"

#include <unistd.h>
#include <cctype>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid_t) {
    pid_ = pid_t;
    user_ = captureProcessUser(pid_);
    command_ = captureProcessCommand(pid_);
    uptime_ = UpTime();
}

std::string Process::captureProcessUser(int pid) {

    string pidString = std::to_string(pid);
    // get user ID
    string uid;
    string line;
    string key;
    string value;
    std::ifstream filestream(LinuxParser::kProcDirectory + pidString + LinuxParser::kStatusFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "uid") {
                    std::istringstream iss(value);
                    iss >> uid;
                }
            }
        }
    }

    filestream.close();

    // get user name
    string username;
    std::ifstream usernameStream("/etc/passwd");
    if (usernameStream.is_open()) {
        while (std::getline(usernameStream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> username;
            while (linestream >> value) {
                if (value == uid) {
                    return username;
                }
            }
        }
    }

    usernameStream.close();
    return username;
}

std::string Process::captureProcessCommand(int pid) {
    string cmd;
    string pidString = std::to_string(pid);
    std::ifstream filestream(LinuxParser::kProcDirectory + pidString + LinuxParser::kStatusFilename);
    if (filestream.is_open()) {
        std::getline(filestream, cmd);
    }
    filestream.close();
    return cmd;
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { 
// fetch this process's cpu metrics
    vector<int> cpuMetrics = {};
    string line, metric;
    std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid_) + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        int index = 1;
       while (getline(linestream, metric, ' ')) {
            if ((index == 14 || index == 15 || index == 16 || index == 17 || index == 22) &&
                std::all_of(metric.begin(), metric.end(), isdigit)) {
                cpuMetrics.push_back(stoi(metric));
            }
            index++;
        }
    }
  	filestream.close();
  
 // calculate process CpuUtilization
    long total_time = cpuMetrics[0] + cpuMetrics[1] + cpuMetrics[2] + cpuMetrics[3];
    float seconds = UpTime() - (cpuMetrics[4] / CLOCKS_PER_SEC);
    float cpuUsage = 100 * ((total_time / CLOCKS_PER_SEC) / seconds);

    return cpuUsage;
  
}

string Process::Command() { return command_; }

string Process::Ram() { return string(); }

string Process::User() { return user_; }

long int Process::UpTime() {    
  int clockTick;
    string line, metric;
    std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid_) + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        int index = 1;
        while (getline(linestream, metric, ' ')) {
            if (index == 22 &&
                std::all_of(metric.begin(), metric.end(), isdigit)) {
                clockTick = stoi(metric);
                break;
            }
            index++;
        }
    }

    // set uptime
    uptime_ = clockTick / sysconf(_SC_CLK_TCK);
    return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a [[maybe_unused]]) const {
    return true;
}