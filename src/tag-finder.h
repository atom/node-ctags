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
  TagFinder(Nan::Callback *callback, std::string tag, int options, tagFile *file)
    : Nan::AsyncWorker(callback), options(options), tag(tag), file(file) {}

  ~TagFinder() {}

  void Execute();
  void HandleOKCallback();

 private:
  int options;
  std::string tag;
  std::vector< Tag > matches;
  tagFile *file;
};

#endif  // SRC_TAG_FINDER_H_
