#include "readtags.h"
#include <string>
#include <node.h>
#include <v8.h>
#include <vector>

using namespace v8;
using namespace std;

Local<Object> ParseEntry(tagEntry entry) {
  Local<Object> tagEntry = Object::New();
  tagEntry->Set(String::NewSymbol("name"), String::NewSymbol(entry.name));
  tagEntry->Set(String::NewSymbol("file"), String::NewSymbol(entry.file));
  if (entry.address.pattern != NULL)
    tagEntry->Set(String::NewSymbol("pattern"), String::NewSymbol(entry.address.pattern));
  return tagEntry;
}

Handle<Value> FindTag(const Arguments& args) {
  if (args.Length() < 2) {
    HandleScope scope;
    return scope.Close(Array::New(0));
  }

  String::Utf8Value utf8Path(Local<String>::Cast(args[0]));
  string path(*utf8Path);
  String::Utf8Value utf8Tag(Local<String>::Cast(args[1]));
  string tag(*utf8Tag);

  tagFileInfo info;
  tagFile* tagFile;
  tagFile = tagsOpen(path.data(), &info);
  if (!info.status.opened) {
    HandleScope scope;
    return scope.Close(Array::New(0));
  }

  tagEntry entry;
  vector< Local<Object> > entries;
  if (tagsFind(tagFile, &entry, tag.data(), TAG_FULLMATCH | TAG_OBSERVECASE) == TagSuccess) {
    entries.push_back(ParseEntry(entry));
    while (tagsFindNext(tagFile, &entry) == TagSuccess)
      entries.push_back(ParseEntry(entry));
  }
  tagsClose(tagFile);

  Handle<Array> array = Array::New(entries.size());
  for (size_t i = 0; i < entries.size(); i++)
    array->Set(i, entries[i]);
  HandleScope scope;
  return scope.Close(array);
}

Handle<Value> GetTags(const Arguments& args) {
  if (args.Length() == 0) {
    HandleScope scope;
    return scope.Close(Array::New(0));
  }

  String::Utf8Value utf8Path(Local<String>::Cast(args[0]));
  string path(*utf8Path);

  tagFileInfo info;
  tagFile* tagFile;
  tagFile = tagsOpen(path.c_str(), &info);
  if (!info.status.opened) {
    HandleScope scope;
    return scope.Close(Array::New(0));
  }

  tagEntry entry;
  vector< Local<Object> > entries;
  while (tagsNext(tagFile, &entry) == TagSuccess)
    entries.push_back(ParseEntry(entry));
  tagsClose(tagFile);

  Handle<Array> array = Array::New(entries.size());
  for (size_t i = 0; i < entries.size(); i++)
    array->Set(i, entries[i]);
  HandleScope scope;
  return scope.Close(array);
}

void init(Handle<Object> target) {
  target->Set(String::NewSymbol("findTag"), FunctionTemplate::New(FindTag)->GetFunction());
  target->Set(String::NewSymbol("getTags"), FunctionTemplate::New(GetTags)->GetFunction());
}
NODE_MODULE(ctags, init)
