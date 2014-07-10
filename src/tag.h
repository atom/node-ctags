#ifndef SRC_TAG_H_
#define SRC_TAG_H_

#include <string>
#include <map>
#include "readtags.h"

class Tag {
 public:
  Tag(tagEntry entry) {
    name = entry.name;
    file = entry.file;
    kind = entry.kind != NULL ? entry.kind : "";
    pattern = entry.address.pattern != NULL ? entry.address.pattern : "";

    if (entry.fields.count > 0) {
    	for (size_t i = 0; i < entry.fields.count; i++) {
				tagExtensionField field = entry.fields.list[i];
				fields[std::string(field.key)] = std::string(field.value);
			}
		}
  }

  std::string name;
  std::string file;
  std::string kind;
  std::string pattern;
  std::map<std::string, std::string> fields;
};

#endif  // SRC_TAG_H_
