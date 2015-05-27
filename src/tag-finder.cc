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

  Handle<Array> array = NanNew<Array>(matches.size());
  for (size_t i = 0; i < matches.size(); i++) {
    Local<Object> tagObject = NanNew<Object>();
    tagObject->Set(NanNew<String>("name"),
                   NanNew<String>(matches[i].name.data()));
    tagObject->Set(NanNew<String>("file"),
                   NanNew<String>(matches[i].file.data()));
    tagObject->Set(NanNew<String>("kind"),
                   NanNew<String>(matches[i].kind.data()));
    tagObject->Set(NanNew<String>("lineNumber"),
                   NanNew<Integer>((int32_t)matches[i].lineNumber));
    if (matches[i].pattern.length() > 0)
      tagObject->Set(NanNew<String>("pattern"),
                     NanNew<String>(matches[i].pattern.data()));
    array->Set(i, tagObject);
  }

  Local<Value> argv[] = {
    NanNull(),
    NanNew(array)
  };
  callback->Call(2, argv);
}
