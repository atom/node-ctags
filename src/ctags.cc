#include <string>
#include <vector>

#include "nan.h"
using namespace v8;  // NOLINT

#include "readtags.h"

namespace {

Local<Object> ParseEntry(const tagEntry& entry) {
  Local<Object> tagEntry = Object::New();
  tagEntry->Set(NanSymbol("name"), NanSymbol(entry.name));
  tagEntry->Set(NanSymbol("file"), NanSymbol(entry.file));
  if (entry.address.pattern != NULL)
    tagEntry->Set(NanSymbol("pattern"),
                  NanSymbol(entry.address.pattern));
  return tagEntry;
}

NAN_METHOD(FindTags) {
  NanScope();

  if (args.Length() < 2)
    NanReturnValue(Array::New(0));

  std::string path(*String::Utf8Value(args[0]));
  std::string tag(*String::Utf8Value(args[1]));

  int tagFlags = 0;
  if (args[2]->IsObject()) {
    Local<Object> options(Local<Object>::Cast(args[2]));
    if (options->Get(NanSymbol("partialMatch"))->BooleanValue())
      tagFlags |= TAG_PARTIALMATCH;
    else
      tagFlags |= TAG_FULLMATCH;

    if (options->Get(NanSymbol("caseInsensitive"))->BooleanValue())
      tagFlags |= TAG_IGNORECASE;
    else
      tagFlags |= TAG_OBSERVECASE;
  } else
    tagFlags = TAG_OBSERVECASE | TAG_FULLMATCH;

  tagFileInfo info;
  tagFile* tagFile;
  tagFile = tagsOpen(path.data(), &info);
  if (!info.status.opened)
    NanReturnValue(Array::New(0));

  tagEntry entry;
  std::vector< Local<Object> > entries;
  if (tagsFind(tagFile, &entry, tag.data(), tagFlags) == TagSuccess) {
    entries.push_back(ParseEntry(entry));
    while (tagsFindNext(tagFile, &entry) == TagSuccess)
      entries.push_back(ParseEntry(entry));
  }
  tagsClose(tagFile);

  Handle<Array> array = Array::New(entries.size());
  for (size_t i = 0; i < entries.size(); i++)
    array->Set(i, entries[i]);
  NanReturnValue(array);
}

NAN_METHOD(GetTags) {
  NanScope();

  if (args.Length() == 0)
    NanReturnValue(Array::New(0));

  std::string path(*String::Utf8Value(args[0]));

  tagFileInfo info;
  tagFile* tagFile;
  tagFile = tagsOpen(path.c_str(), &info);
  if (!info.status.opened)
    NanReturnValue(Array::New(0));

  tagEntry entry;
  std::vector< Local<Object> > entries;
  while (tagsNext(tagFile, &entry) == TagSuccess)
    entries.push_back(ParseEntry(entry));
  tagsClose(tagFile);

  Handle<Array> array = Array::New(entries.size());
  for (size_t i = 0; i < entries.size(); i++)
    array->Set(i, entries[i]);
  NanReturnValue(array);
}

void init(Handle<Object> target) {
  NODE_SET_METHOD(target, "findTags", FindTags);
  NODE_SET_METHOD(target, "getTags", GetTags);
}

}  // namespace

NODE_MODULE(ctags, init)
