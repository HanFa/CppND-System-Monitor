#ifndef PROCESS_H
#define PROCESS_H

#include <unordered_map>
#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:

  explicit Process(int pid, float active_jiffies, float jiffies);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process & a);        // TODO: See src/process.cpp
  std::pair<float, float> get_last_jiffies();

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_{""};
  std::string command_{""};
  std::string ram_{""};
  long int uptime_{0};

  float utilization_;
  float last_active_jiffies_;
  float last_jiffies_;
};

#endif