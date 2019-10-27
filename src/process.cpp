#include <unistd.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, float active_jiffies, float jiffies)
    : pid_(pid), last_active_jiffies_(active_jiffies), last_jiffies_(jiffies) {
  user_ = LinuxParser::User(pid);
  command_ = LinuxParser::Command(pid);
  ram_ = LinuxParser::Ram(pid);
  uptime_ = LinuxParser::UpTime(pid);

  auto current_active_jiffies = (float)LinuxParser::ActiveJiffies(pid_);
  auto current_jiffies = (float)LinuxParser::Jiffies();

  utilization_ = (current_active_jiffies - last_active_jiffies_) /
                 (current_jiffies - last_jiffies_);

  last_active_jiffies_ = current_active_jiffies;
  last_jiffies_ = current_jiffies;

}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  return utilization_;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process& a [[maybe_unused]]) {
  return CpuUtilization() > a.CpuUtilization();
}
std::pair<float, float> Process::get_last_jiffies() {
  return std::make_pair(last_active_jiffies_, last_jiffies_);
}
