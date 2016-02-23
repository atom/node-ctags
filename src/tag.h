#ifndef SRC_TAG_H_
#define SRC_TAG_H_

#include <string>
#include <utility>
#include <vector>
#include "nan.h"
#include "readtags.h"
#include "v8.h"

class Tag {
 public:
  Tag(tagEntry entry) {
    name = entry.name;
    file = entry.file;
    kind = entry.kind != NULL ? entry.kind : "";
    pattern = entry.address.pattern != NULL ? entry.address.pattern : "";
    lineNumber = entry.address.lineNumber;
    for (size_t i = 0; i < entry.fields.count; i++) {
      fields.push_back(std::make_pair(
        std::string(entry.fields.list[i].key),
        std::string(entry.fields.list[i].value)
      ));
    }
  }

  v8::Local<v8::Object> toV8Object() {
    using namespace v8;

    Local<Object> tagObject = Nan::New<Object>();
    tagObject->Set(Nan::New<String>("name").ToLocalChecked(),
                   Nan::New<String>(name.data()).ToLocalChecked());
    tagObject->Set(Nan::New<String>("file").ToLocalChecked(),
                   Nan::New<String>(file.data()).ToLocalChecked());
    tagObject->Set(Nan::New<String>("kind").ToLocalChecked(),
                   Nan::New<String>(kind.data()).ToLocalChecked());
    tagObject->Set(Nan::New<String>("lineNumber").ToLocalChecked(),
                   Nan::New<Integer>((int32_t)lineNumber));
    if (pattern.length() > 0)
      tagObject->Set(Nan::New<String>("pattern").ToLocalChecked(),
                     Nan::New<String>(pattern.data()).ToLocalChecked());
    if (fields.size() > 0) {
      Local<Object> fieldsObj = Nan::New<Object>();
      for (size_t j = 0; j < fields.size(); j++) {
        fieldsObj->Set(Nan::New<String>(fields[j].first).ToLocalChecked(),
                       Nan::New<String>(fields[j].second).ToLocalChecked());
      }
      tagObject->Set(Nan::New<String>("fields").ToLocalChecked(), fieldsObj);
    }
    return tagObject;
  }

  std::string name;
  std::string file;
  std::string kind;
  std::string pattern;
  unsigned long lineNumber;
  std::vector<std::pair<std::string, std::string> > fields;
};

#endif  // SRC_TAG_H_
