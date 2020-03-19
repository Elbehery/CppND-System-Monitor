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
    if (filestream.is_open()) {
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

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a [[maybe_unused]]) const {
    return true;
}