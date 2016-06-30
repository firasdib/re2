#include <emscripten/bind.h>
#include "re2/re2.h"

using std::vector;
using re2::RE2;
using re2::StringPiece;

using namespace emscripten;

std::vector<int> RE2_Match(std::string regex, std::string text, int startpos, int endpos, int captureGroupCount) {
  RE2 re(regex);

  std::vector<StringPiece> groups(captureGroupCount + 1);
  std::vector<int> result(captureGroupCount + 1);

  bool didMatch = re.Match(text, startpos, endpos, RE2::UNANCHORED, &groups[0], groups.size());

  if (didMatch) {
    for (size_t i = 0, n = groups.size(); i < n; ++i) {
      const StringPiece& item = groups[i];
      result[i] = item.size();
    }

    return result;
  } else {
    return vector<int>();
  }
}

EMSCRIPTEN_BINDINGS(my_module) {
  register_vector<int>("VectorInt");

  // this may or may not work, let's try directly binding the static funcs
  function("RE2_Match", &RE2_Match);
}

// utilities

inline size_t getUtf8Length(const uint16_t* from, const uint16_t* to) {
	size_t n = 0;
	while (from != to) {
		uint16_t ch = *from++;
		if (ch <= 0x7F) ++n;
		else if (ch <= 0x7FF) n += 2;
		else if (0xD800 <= ch && ch <= 0xDFFF) n += 4;
		else if (ch < 0xFFFF) n += 3;
		else n += 4;
	}
	return n;
}

inline size_t getUtf16Length(const char* from, const char* to) {
	size_t n = 0;
	while (from != to) {
		unsigned ch = *from & 0xFF;
		if (ch < 0xF0) {
			if (ch < 0x80) {
				++from;
			} else {
				if (ch < 0xE0) {
					from += 2;
				} else {
					from += 3;
				}
			}
			++n;
		} else {
			from += 4;
			n += 2;
		}
	}
	return n;
}

inline size_t getUtf8CharSize(char ch) {
	return ((0xE5000000 >> ((ch >> 3) & 0x1E)) & 3) + 1;
}
