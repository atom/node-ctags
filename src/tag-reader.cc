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
    Local<Object> tagObject = Nan::New<Object>();
    Nan::Set(tagObject,
        Nan::New<String>("name").ToLocalChecked(),
        Nan::New<String>(tags[i].name.data()).ToLocalChecked());
    Nan::Set(tagObject,
        Nan::New<String>("file").ToLocalChecked(),
        Nan::New<String>(tags[i].file.data()).ToLocalChecked());
    Nan::Set(tagObject,
        Nan::New<String>("kind").ToLocalChecked(),
        Nan::New<String>(tags[i].kind.data()).ToLocalChecked());
    Nan::Set(tagObject, Nan::New<String>("lineNumber").ToLocalChecked(),
                   Nan::New<Integer>((int32_t)tags[i].lineNumber));
    if (tags[i].pattern.length() > 0)
      Nan::Set(tagObject,
          Nan::New<String>("pattern").ToLocalChecked(),
          Nan::New<String>(tags[i].pattern.data()).ToLocalChecked());
      Nan::Set(array, i, tagObject);
  }

  Local<Value> argv[] = { Nan::Null(), array };
  callback->Call(2, argv);
}
