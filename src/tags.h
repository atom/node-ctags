#ifndef SRC_TAGS_H_
#define SRC_TAGS_H_

#include <string>
#include "nan.h"
#include "readtags.h"

using namespace v8;  // NOLINT

class Tags : public node::ObjectWrap {
 public:
  static void Init(Local<Object> target);

 private:
  static NAN_METHOD(End);
  static NAN_METHOD(Exists);
  static NAN_METHOD(FindTags);
  static NAN_METHOD(GetTags);
  static NAN_METHOD(New);

  static tagFile* GetFile(v8::Local<v8::Object> obj);

  explicit Tags(Local<String> path);
  ~Tags() {
    if (file != NULL) {
      tagsClose(file);
      file = NULL;
    }
  };

  tagFile* file;
};

#endif  // SRC_TAGS_H_
