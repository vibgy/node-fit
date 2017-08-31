#ifndef LISTENER_H
#define LISTENER_H

#include "../libfit/fit_decode.hpp"
#include "../libfit/fit_mesg_broadcaster.hpp"
#include <node.h>
#include <v8.h>

using namespace v8;
using namespace node;
using v8::Isolate;

class Listener : 
	public fit::MesgListener, 
	public fit::RecordMesgListener {
   public:
      Listener (const v8::FunctionCallbackInfo<v8::Value>& args);
      void OnMesg(fit::Mesg& mesg);
      void OnMesg(fit::RecordMesg& mesg);

   private:
      Local<Object> self;
      Isolate* isolate;
//      v8::Handle<v8::String> GetV8String(const std::string& str);
//      v8::Handle<v8::String> GetV8String(const std::wstring& str);
};

#endif
