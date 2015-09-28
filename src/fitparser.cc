#include <string>
#include <string.h>
#include <node.h>
#include <v8.h>

#include <fstream>
#include <iostream>

#include "fitparser.h"
#include "listener.h"
#include "../libfit/fit_decode.hpp"
#include "../libfit/fit_mesg_broadcaster.hpp"

using namespace std;
using namespace v8;
using namespace node;
using v8::Isolate;
using node::AtExit;

Persistent<Function> FitParser::constructor;

FitParser::FitParser() {};
FitParser::FitParser(double value) : value_ (value) {

};
FitParser::~FitParser() {};

void FitParser::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "FitParser"));
   tpl->InstanceTemplate()->SetInternalFieldCount(1);
   // Prototype
   NODE_SET_PROTOTYPE_METHOD(tpl, "decode", Decode);
   NODE_SET_PROTOTYPE_METHOD(tpl, "encode", Encode);

   constructor.Reset(isolate, tpl->GetFunction());
   exports->Set(String::NewFromUtf8(isolate, "FitParser"), tpl->GetFunction());
}

void FitParser::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();

 if (args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    FitParser* obj = new FitParser(value);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void FitParser::Decode(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  FitParser* obj = ObjectWrap::Unwrap<FitParser>(args.Holder());
  obj->value_ -= 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}

void FitParser::Encode(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Locker locker(isolate);
  HandleScope scope(isolate);

  printf("%d\n", args.Length());
  printf("%d\n", args[0]->IntegerValue());
  //printf("%s\n", args[1]);

  Persistent<Function> callback;
  callback.Reset(isolate, args[1].As<Function>());


  FitParser* obj = ObjectWrap::Unwrap<FitParser>(args.Holder());
  obj->value_ += 1;

  const unsigned argc = 2;
  //Local<Int> val = obj->value_;
  Local<Value> argv[argc] = {
    String::NewFromUtf8(isolate, "record"), // event name
    Integer::New(isolate, 5) //Local<NumberValue>(1)  // argument
  };

  Local<Function> f = Local<Function>::New(isolate, callback);
  f->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  callback.Reset();

  //args.GetReturnValue().Set(Number::New(isolate, obj->value_));

}

void FitParser::EncodeSync(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  FitParser* obj = ObjectWrap::Unwrap<FitParser>(args.Holder());
  obj->value_ += 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}

/*
Handle<Value> FitParser::Decode(const v8::FunctionCallbackInfo<v8::Value>& args) {
   HandleScope scope;
   fit::Decode decode;
   fit::MesgBroadcaster mesgBroadcaster;
   Listener* listener = new Listener(args);
   std::fstream file;

   if (args.Length() < 1) {
      Handle<Value> argv[2] = {
       String::New("error"), // event name
       Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments"))  // argument
      };

      MakeCallback(args.This(), "emit", 2, argv);
      return scope.Close(Undefined());
   }

   if (!args[0]->IsString()) {
      Handle<Value> argv[2] = {
       String::New("error"), // event name
       Exception::TypeError(String::NewFromUtf8(isolate, "Argument must be a string."))  // argument
      };

      MakeCallback(isolate, args.This(), "emit", 2, argv);
      return scope.Close(Undefined());
   }

   String::Utf8Value fileName(args[0]->ToString());
   
   file.open(strdup(*fileName), std::ios::in | std::ios::binary);

   if (!file.is_open())
   {
      Handle<Value> argv[2] = {
       String::New("error"), // event name
       Exception::TypeError(String::NewFromUtf8(isolate, "Error opening file."))  // argument
      };

      MakeCallback(isolate, args.This(), "emit", 2, argv);
      return scope.Close(Undefined());
   }

   if (!decode.CheckIntegrity(file))
   {
      Handle<Value> argv[2] = {
       String::New("error"), // event name
       Exception::TypeError(String::NewFromUtf8(isolate, "FIT file integrity failed."))  // argument
      };

      MakeCallback(isolate, args.This(), "emit", 2, argv);
      return scope.Close(Undefined());
   }

   mesgBroadcaster.AddListener((fit::MesgListener &)*listener);
   mesgBroadcaster.AddListener((fit::RecordMesgListener &)*listener);

   try
   {
      mesgBroadcaster.Run(file);
   }
   catch (const fit::RuntimeException& e)
   {
      Handle<Value> argv[2] = {
       String::New("error"), // event name
       Exception::TypeError(String::NewFromUtf8(isolate, "Exception while decoding file."))  // argument
      };

      MakeCallback(args.This(), "emit", 2, argv);
      return scope.Close(Undefined());
   }

   Handle<Value> argv[2] = {
    String::New("end"), // event name
    args[0]->ToString()  // argument
   };

   MakeCallback(isolate, args.This(), "emit", 2, argv);

   return Undefined();
}
*/
