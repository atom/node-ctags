#include "tag-finder.h"

void TagFinder::Execute() {
  tagEntry entry;
  if (tagsFind(file, &entry, tag.data(), options) == TagSuccess) {
    matches.push_back(Tag(entry));
    while (tagsFindNext(file, &entry) == TagSuccess)
      matches.push_back(Tag(entry));
  }
}

void TagFinder::HandleOKCallback() {
  Nan::HandleScope handle_scope;

  Local<Array> array = Nan::New<Array>(matches.size());
  for (size_t i = 0; i < matches.size(); i++) {
    Local<Object> tagObject = Nan::New<Object>();
    Nan::Set(tagObject, Nan::New<String>("name").ToLocalChecked(),
                   Nan::New<String>(matches[i].name.data()).ToLocalChecked());
    Nan::Set(tagObject, Nan::New<String>("file").ToLocalChecked(),
                   Nan::New<String>(matches[i].file.data()).ToLocalChecked());
    Nan::Set(tagObject, Nan::New<String>("kind").ToLocalChecked(),
                   Nan::New<String>(matches[i].kind.data()).ToLocalChecked());
    Nan::Set(tagObject, Nan::New<String>("lineNumber").ToLocalChecked(),
                   Nan::New<Integer>((int32_t)matches[i].lineNumber));
    if (matches[i].pattern.length() > 0)
      Nan::Set(tagObject,
          Nan::New<String>("pattern").ToLocalChecked(),
          Nan::New<String>(matches[i].pattern.data()).ToLocalChecked());
    Nan::Set(array, i, tagObject);
  }

  Local<Value> argv[] = { Nan::Null(), array };
  callback->Call(2, argv);
}
