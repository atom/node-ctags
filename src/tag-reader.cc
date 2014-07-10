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

  Handle<Array> array = Array::New(tags.size());
  for (size_t i = 0; i < tags.size(); i++) {
    Local<Object> tagObject = Object::New();
    tagObject->Set(NanSymbol("name"), NanSymbol(tags[i].name.data()));
    tagObject->Set(NanSymbol("file"), NanSymbol(tags[i].file.data()));

    if (tags[i].kind.length() > 0)
      tagObject->Set(NanSymbol("kind"), NanSymbol(tags[i].kind.data()));
    if (tags[i].pattern.length() > 0)
      tagObject->Set(NanSymbol("pattern"), NanSymbol(tags[i].pattern.data()));

    if (tags[i].fields.size() > 0) {
      Local<Object> fieldEntry = Object::New();
      map_iterator it = tags[i].fields.begin();
      map_iterator end = tags[i].fields.end();
      for (; it != end; ++it) {
        const char *key = ((*it).first).data();
        const char *value = ((*it).second).data();
        fieldEntry->Set(NanSymbol(key), NanSymbol(value));
      }
      tagObject->Set(NanSymbol("fields"), fieldEntry);
    }

    array->Set(i, tagObject);
  }

  Local<Value> argv[] = {
    Local<Value>::New(Null()),
    Local<Value>::New(array)
  };
  callback->Call(2, argv);
}
