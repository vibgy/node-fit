#include <node.h>
#include <v8.h>
#include "fitparser.h"

using namespace v8;


extern "C" void init(Local<Object> exports) {
  FitParser::Init(exports);
}

NODE_MODULE(binding, init);