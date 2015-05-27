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
  NanScope();

  Handle<Array> array = NanNew<Array>(tags.size());
  for (size_t i = 0; i < tags.size(); i++) {
    Local<Object> tagObject = NanNew<Object>();
    tagObject->Set(NanNew<String>("name"), NanNew<String>(tags[i].name.data()));
    tagObject->Set(NanNew<String>("file"), NanNew<String>(tags[i].file.data()));
    tagObject->Set(NanNew<String>("kind"), NanNew<String>(tags[i].kind.data()));
    tagObject->Set(NanNew<String>("lineNumber"),
                   NanNew<Integer>((int32_t)tags[i].lineNumber));
    if (tags[i].pattern.length() > 0)
      tagObject->Set(NanNew<String>("pattern"),
                     NanNew<String>(tags[i].pattern.data()));
    array->Set(i, tagObject);
  }

  Local<Value> argv[] = {
    NanNull(),
    NanNew(array)
  };
  callback->Call(2, argv);
}
