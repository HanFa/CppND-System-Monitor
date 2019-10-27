#include <unistd.h>
#include <cstddef>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {

}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();

  processes_.clear();
  processes_.reserve(pids.size());
  for (const int & pid : pids) {
    if (pid_to_jiffies_.find(pid) != pid_to_jiffies_.end()) {
      auto p = pid_to_jiffies_[pid];
      processes_.emplace_back(pid, p.first, p.second);
    } else {
      processes_.emplace_back(pid, 0, 0);
    }
  }
  std::sort(processes_.begin(), processes_.end());

  pid_to_jiffies_.clear();
  for (auto & process : processes_) {
    pid_to_jiffies_[process.Pid()] = process.get_last_jiffies();
  }

  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
  return LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
  return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  return LinuxParser::UpTime();
}
