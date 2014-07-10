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
  NanScope();

  Handle<Array> array = Array::New(matches.size());
  for (size_t i = 0; i < matches.size(); i++) {
    Local<Object> tagObject = Object::New();
    tagObject->Set(NanSymbol("name"), NanSymbol(matches[i].name.data()));
    tagObject->Set(NanSymbol("file"), NanSymbol(matches[i].file.data()));

    if (matches[i].kind.length() > 0)
      tagObject->Set(NanSymbol("kind"), NanSymbol(matches[i].kind.data()));
    if (matches[i].pattern.length() > 0)
      tagObject->Set(NanSymbol("pattern"), NanSymbol(matches[i].pattern.data()))
      ; { }

    if (matches[i].fields.size() > 0) {
      Local<Object> fieldEntry = Object::New();
      map_iterator it = matches[i].fields.begin();
      map_iterator end = matches[i].fields.end();
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
