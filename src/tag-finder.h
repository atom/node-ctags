#ifndef SRC_TAG_FINDER_H_
#define SRC_TAG_FINDER_H_

#include <string>
#include <vector>
#include "nan.h"
#include "tag.h"
#include "readtags.h"

using namespace v8;

class TagFinder : public Nan::AsyncWorker {
 public:
  TagFinder(Nan::Callback* callback,
            std::string tag,
            int options,
            size_t limit,
            tagFile* file)
      : Nan::AsyncWorker(callback),
        options(options),
        limit(limit),
        tag(tag),
        file(file) {}

  ~TagFinder() {}

  void Execute();
  void HandleOKCallback();

 private:
  int options;
  size_t limit;
  std::string tag;
  std::vector< Tag > matches;
  tagFile *file;
};

#endif  // SRC_TAG_FINDER_H_
