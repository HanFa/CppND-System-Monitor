#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  auto current_active_jiffies = (float)LinuxParser::ActiveJiffies();
  auto current_jiffies = (float)LinuxParser::Jiffies();

  utilization_ = (current_active_jiffies - last_active_jiffies) /
                 (current_jiffies - last_jiffies);

  last_active_jiffies = current_active_jiffies;
  last_jiffies = current_jiffies;
  return utilization_;
}