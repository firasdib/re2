#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "re2/re2.h"
// #include "util/flags.h"

using std::vector;
using re2::RE2;
using re2::StringPiece;

using namespace emscripten;

// DECLARE_bool(trace_re2);

class RegexMatch {
  public:
    RegexMatch ();
    RegexMatch (int,int);
    int start;
    int end;
    int getStart() { return start; };
    int getEnd() { return end; };
};

RegexMatch::RegexMatch () {}

RegexMatch::RegexMatch (int a, int b) {
  start = a;
  end = b;
}

int RE2_CreateRegex(int regex_pointer) {
  const char* regex = (char*)regex_pointer;

  return (int)(new RE2(regex));
}

std::vector<RegexMatch> RE2_Match(int regex_pointer, int text_pointer, int startpos, int endpos, int captureGroupCount) {
  // Debug only!
  // re2::FLAGS_trace_re2 = true;

  const char* match_text = (char*)text_pointer;

  std::vector<StringPiece> groups(captureGroupCount + 1);
  std::vector<RegexMatch> result(captureGroupCount + 1);

  bool did_match = ((RE2*)regex_pointer)->Match(match_text, startpos, endpos, RE2::UNANCHORED, &groups[0], groups.size());

  if (did_match) {
    for (size_t i = 0, n = groups.size(); i < n; ++i) {
      const StringPiece& item = groups[i];

      int match_start = item.begin() - match_text;
      int match_end = match_start + item.size();

      RegexMatch rm(match_start, match_end);

      result[i] = rm;
    }

    return result;
  } else {
    return vector<RegexMatch>();
  }
}

EMSCRIPTEN_BINDINGS(my_module) {
  register_vector<RegexMatch>("VectorRegexMatch");

  function("RE2_Match", &RE2_Match);
  function("RE2_CreateRegex", &RE2_CreateRegex);

  class_<RegexMatch>("RegexMatch")
    .constructor<int,int>()
    .function("getStart", &RegexMatch::getStart)
    .function("getEnd", &RegexMatch::getEnd);
}
