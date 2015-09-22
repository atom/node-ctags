#include <string>
#include "tags.h"
#include "tag-finder.h"
#include "tag-reader.h"

void Tags::Init(Handle<Object> target) {
  Nan::HandleScope handle_scope;

  Local<FunctionTemplate> newTemplate = Nan::New<FunctionTemplate>(Tags::New);
  newTemplate->SetClassName(Nan::New<String>("Tags").ToLocalChecked());
  newTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> proto = newTemplate->PrototypeTemplate();
  Nan::SetMethod(proto, "end", Tags::End);
  Nan::SetMethod(proto, "exists", Tags::Exists);
  Nan::SetMethod(proto, "findTags", Tags::FindTags);
  Nan::SetMethod(proto, "getTags", Tags::GetTags);

  target->Set(Nan::New<String>("Tags").ToLocalChecked(),
              newTemplate->GetFunction());
}

NODE_MODULE(ctags, Tags::Init)

NAN_METHOD(Tags::New) {
  Tags *tags = new Tags(Local<String>::Cast(info[0]));
  tags->Wrap(info.This());
  info.GetReturnValue().SetUndefined();
}

tagFile* Tags::GetFile(v8::Local<v8::Object> obj) {
  return node::ObjectWrap::Unwrap<Tags>(obj)->file;
}

NAN_METHOD(Tags::GetTags) {
  tagFile *tagFile = GetFile(info.This());
  int chunkSize = info[0]->Uint32Value();
  Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
  Nan::AsyncQueueWorker(new TagReader(callback, chunkSize, tagFile));
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Tags::FindTags) {
  Nan::HandleScope handle_scope;

  tagFile *tagFile = GetFile(info.This());
  std::string tag(*String::Utf8Value(info[0]));
  int options = 0;
  if (info[1]->BooleanValue())
    options |= TAG_PARTIALMATCH;
  else
    options |= TAG_FULLMATCH;

  if (info[2]->BooleanValue())
    options |= TAG_IGNORECASE;
  else
    options |= TAG_OBSERVECASE;

  Nan::Callback *callback = new Nan::Callback(info[3].As<Function>());
  Nan::AsyncQueueWorker(new TagFinder(callback, tag, options, tagFile));
  info.GetReturnValue().SetUndefined();
}

Tags::Tags(Handle<String> path) {
  Nan::HandleScope handle_scope;

  std::string filePath(*String::Utf8Value(path));
  tagFileInfo info;
  file = tagsOpen(filePath.data(), &info);
  if (!info.status.opened)
    file = NULL;
}

NAN_METHOD(Tags::Exists) {
  info.GetReturnValue().Set(GetFile(info.This()) != NULL);
}

NAN_METHOD(Tags::End) {
  tagFile *file = GetFile(info.This());
  if (file != NULL) {
    tagsClose(file);
    node::ObjectWrap::Unwrap<Tags>(info.This())->file = NULL;
  }

  info.GetReturnValue().SetUndefined();
}
