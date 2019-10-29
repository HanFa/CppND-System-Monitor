#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

string pad(const string& s) { return s.size() < 2 ? "0" + s : s; }

string Format::ElapsedTime(long seconds) {
  int SS = (int)seconds % 60;
  int MM = ((int)seconds / 60) % 60;
  int HH = (int)seconds / 3600;
  string SS_str = pad(to_string(SS));
  string MM_str = pad(to_string(MM));
  string HH_str = pad(to_string(HH));
  return HH_str + ":" + MM_str + ":" + SS_str;
}