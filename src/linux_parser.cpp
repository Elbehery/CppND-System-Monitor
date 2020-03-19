#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "linux_parser.h"
#include "process.h"
#include "system.h"


using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { string line;
    string key;
    string value;
    std::map<std::string, float> localMap;
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()) {
        int counterGuard = 2;
        while (std::getline(filestream, line) && counterGuard > 0) {
            std::istringstream linestream(line);
            while (std::getline(linestream, key, ':')) {
                if (key == "MemTotal" || key == "MemFree") {
                    linestream >> value;
                    value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
                    value.erase(value.size() - 2);
                    localMap.insert(std::make_pair(key, stof(value)));
                }
            }
            counterGuard--;
        }
    }

    float utilization = (localMap["MemTotal"] - localMap["MemFree"]) / localMap["MemTotal"];
    return utilization; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() { 
    string line;
    string key;
    std::vector<string> cpuData = {};
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        while (std::getline(linestream, key, ' ')) {
          key.erase(std::remove(key.begin(), key.end(),' '),key.end());
            cpuData.push_back(key);
        }
    }
  
    return cpuData;
}

int LinuxParser::TotalProcesses() { 
  	string line;
    string key;
    string value;
    std::map<std::string, float> localMap;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::istringstream linestream(line);
            std::vector<std::string> tokens = {};
            while (std::getline(linestream, key, '_')) {
                tokens.push_back(key);
            }

            if (tokens.size() == 2 && tokens[0] == "processes") {
                return stoi(tokens[1]);
            }
        }
    }
    return 0;
}

int LinuxParser::RunningProcesses() { 
    string line;
    string key;
    string value;
    std::map<std::string, float> localMap;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::istringstream linestream(line);
            std::vector<std::string> tokens = {};
            while (std::getline(linestream, key, '_')) {
                tokens.push_back(key);
            }

            if (tokens.size() == 3 && tokens[0] == "procs" && tokens[1] == "running") {
                return stoi(tokens[2]);
            }
        }
    }
    return 0;
}

string LinuxParser::Command(int pid) {
    vector<Process> processes = System::Processes();
    for(auto proc : processes) {
        if(proc.Pid() == pid){
            return proc.Command();
        }
    }
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }