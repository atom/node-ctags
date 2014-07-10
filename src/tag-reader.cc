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
			for (std::map<std::string, std::string>::const_iterator it=tags[i].fields.begin(), end=tags[i].fields.end(); it != end; ++it) {
				fieldEntry->Set(NanSymbol(((*it).first).data()), NanSymbol(((*it).second).data()));
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
