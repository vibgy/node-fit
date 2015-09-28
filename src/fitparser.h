#ifndef FITPARSER_H
#define FITPARSER_H

#include <node.h>
#include <node_object_wrap.h>

class FitParser : public node::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  FitParser();
  explicit FitParser(double v);
  ~FitParser();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Decode(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void EncodeSync(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Encode(const v8::FunctionCallbackInfo<v8::Value>& args);
  static v8::Persistent<v8::Function> constructor;

  double value_;
};

#endif
