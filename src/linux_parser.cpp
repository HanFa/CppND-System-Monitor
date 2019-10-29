#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream stream(kOSPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
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
  string os, kernel, _;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> _ >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value;
  int total_mem = 0, available_mem = 0, buffers = 0;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream iss(line);
      iss >> key >> value;

      if (key == "MemTotal:") {
        total_mem = std::stoi(value);
      } else if (key == "MemAvailable:") {
        available_mem = std::stoi(value);
      } else if (key == "Buffers:") {
        buffers = std::stoi(value);
      }
    }
  }
  return 1 - (float) available_mem / (float)(total_mem - buffers);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::string uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    stream >> uptime;
    return std::stol(uptime);
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> information_tokens;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatFilename);

  std::string line, token;

  if (stream.is_open()) {
    while (stream >> token) {
      information_tokens.push_back(token);
    }
  }

  return (information_tokens.size() < 21)
             ? 0
             : std::stol(information_tokens[13]) +
                   std::stol(information_tokens[14]) +
                   std::stol(information_tokens[15]) +
                   std::stol(information_tokens[16]);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::vector<string> utilizations = CpuUtilization();
  return std::stol(utilizations[CPUStates::kUser_]) +
         std::stol(utilizations[CPUStates::kNice_]) +
         std::stol(utilizations[CPUStates::kSystem_]) +
         std::stol(utilizations[CPUStates::kIRQ_]) +
         std::stol(utilizations[CPUStates::kSoftIRQ_]) +
         std::stol(utilizations[CPUStates::kSteal_]) +
         std::stol(utilizations[CPUStates::kGuest_]) +
         std::stol(utilizations[CPUStates::kGuestNice_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<string> utilizations = CpuUtilization();
  return std::stol(utilizations[CPUStates::kIdle_]) +
         std::stol(utilizations[CPUStates::kIOwait_]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::string line, token;
  std::vector<string> utilizations{};
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream) {
    while (std::getline(stream, line)) {
      std::istringstream first_line_stream(line);
      first_line_stream >> token;
      if (token == "cpu") {
        while (first_line_stream >> token) {
          utilizations.push_back(token);
        }
      }
    }
  }
  return utilizations;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line, token;
  int processes_num = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream) {
    while (std::getline(stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> token;
      if (token == "processes") {
        line_stream >> processes_num;
        return processes_num;
      }
    }
  }

  return processes_num;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line, token;
  int running_processes_num = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream) {
    while (std::getline(stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> token;
      if (token == "procs_running") {
        line_stream >> running_processes_num;
        return running_processes_num;
      }
    }
  }

  return running_processes_num;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::string line{};
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      return line;
    }
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatusFilename);
  string line, token;
  while (std::getline(stream, line)) {
    std::istringstream line_stream(line);
    line_stream >> token;
    if (token == "VmSize:") {
      line_stream >> token;
      return std::to_string(stoi(token) / 1024);
    }
  }
  return "0";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatusFilename);
  std::string token, line;

  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "Uid:") {
        if (stream >> token) {
          return token;
        }
      }
    }
  }

  return std::string("");
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::ifstream stream(kPasswordPath);
  std::string search_token, line;
  search_token = ":x:" + Uid(pid);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      size_t marker = line.find(search_token);
      if (marker != std::string::npos) {
        return line.substr(0, marker);
      }
    }
  }
  return std::string("");
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatFilename);

  std::string token;

  if (stream.is_open()) {
    for (int i = 0; stream >> token; ++i) {
      if (i == 13)  // (14) utime
      {
        return std::stol(token);
      }
    }
  }
  return 0;
}
