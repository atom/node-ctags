#include "tag-reader.h"

void TagReader::Execute() {
  tags.clear();

  for (int i = 0; i < chunkSize; i++) {
    tagEntry entry;
    if (tagsNext(file, &entry) == TagSuccess)
      tags.push_back(Tag(entry));
    else
      break;
  }
}

void TagReader::HandleOKCallback() {
  Nan::HandleScope handle_scope;

  Local<Array> array = Nan::New<Array>(tags.size());
  for (size_t i = 0; i < tags.size(); i++) {
    array->Set(i, tags[i].toV8Object());
  }

  Local<Value> argv[] = { Nan::Null(), array };
  callback->Call(2, argv);
}
