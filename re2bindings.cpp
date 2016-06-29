#include <emscripten/bind.h>
#include "re2/re2.h"

using namespace emscripten;

bool RE2_FullMatch(std::string text, std::string regex) {
  // todo: handle captures, if needed.
  return RE2::FullMatch(text, regex);
}

bool RE2_PartialMatch(std::string text, std::string regex) {
  // todo: handle captures, if needed.
  return RE2::PartialMatch(text, regex);
}

EMSCRIPTEN_BINDINGS(my_module) {
  function("RE2_FullMatch", &RE2_FullMatch);
  function("RE2_PartialMatch", &RE2_PartialMatch);
}
