#include <string>
#include <string.h>
#include <node.h>
#include <v8.h>

#include <fstream>
#include <iostream>
#include "listener.h"
#include "../libfit/fit_decode.hpp"
#include "../libfit/fit_mesg_broadcaster.hpp"

using namespace std;
/*
// Convert a wide string to a V8 string.
v8::Handle<v8::String> Listener::GetV8String(const std::wstring& str)
{
  return v8::String::NewFromUtf8(this->isolate,
      str.c_str());
}

// Convert a string to a V8 string.
v8::Handle<v8::String> Listener::GetV8String(const std::string& str)
{
  return v8::String::NewFromUtf8(this->isolate,
      str.c_str());
      //reinterpret_cast<const uint16_t*>(str.c_str()), str.length());
}
*/

// Convert a FIT DATE TIME to a V8 Date.
// FIT date time is seconds since UTC 00:00 Dec 31 1989
// v8 date time is milliseconds since UTC 00:00 Jan 1 1970
static double GetMillisecondsSinceEpoch(const uint32_t seconds)
{
   const uint32_t secondsSinceEpoch = 631065600;
   return (static_cast<double>(seconds) + secondsSinceEpoch)*1000;
}

Listener::Listener (const v8::FunctionCallbackInfo<v8::Value>& args) {
   self = args.This();
   isolate = args.GetIsolate();
}


void Listener::OnMesg(fit::RecordMesg& mesg)
{
   Local<Object> obj = Object::New(this->isolate);
   if (mesg.GetHeartRate() != FIT_DATE_TIME_INVALID)
      obj->Set(String::NewFromUtf8(isolate, "timestamp", v8::String::kInternalizedString), Date::New(this->isolate, GetMillisecondsSinceEpoch(mesg.GetTimestamp())));
   if (mesg.GetPositionLat() != FIT_SINT32_INVALID)
      obj->Set(String::NewFromUtf8(isolate, "position_lat", v8::String::kInternalizedString), Int32::New(this->isolate, mesg.GetPositionLat()));
   if (mesg.GetPositionLong() != FIT_SINT32_INVALID)
      obj->Set(String::NewFromUtf8(isolate, "position_long", v8::String::kInternalizedString), Int32::New(this->isolate, mesg.GetPositionLong()));
   if (mesg.GetAltitude() != FIT_FLOAT32_INVALID)
      obj->Set(String::NewFromUtf8(isolate, "altitude", v8::String::kInternalizedString),  NumberObject::New(this->isolate, mesg.GetAltitude()));
   if (mesg.GetHeartRate() != FIT_UINT8_INVALID)
      obj->Set(String::NewFromUtf8(isolate, "heart_rate", v8::String::kInternalizedString), Int32::New(this->isolate, mesg.GetHeartRate()));
   if (mesg.GetCadence() != FIT_UINT8_INVALID)
      obj->Set(String::NewFromUtf8(isolate, "cadence", v8::String::kInternalizedString), Int32::New(this->isolate, mesg.GetCadence()));
   if (mesg.GetDistance() != FIT_FLOAT32_INVALID)
      obj->Set(String::NewFromUtf8(isolate, "distance", v8::String::kInternalizedString), NumberObject::New(this->isolate, mesg.GetDistance()));
   if (mesg.GetSpeed() != FIT_FLOAT32_INVALID)
      obj->Set(String::NewFromUtf8(isolate, "speed", v8::String::kInternalizedString), NumberObject::New(this->isolate, mesg.GetSpeed()));
   if (mesg.GetPower() != FIT_UINT16_INVALID)
      obj->Set(String::NewFromUtf8(isolate, "power", v8::String::kInternalizedString), Int32::New(this->isolate, mesg.GetPower()));

    // obj->Set(String::NewFromUtf8(isolate, "compressed_speed_distance"), mesg.GetNumCompressedSpeedDistance());
   // obj->Set(String::NewFromUtf8(isolate, "grade"), mesg.GetGrade());
   // obj->Set(String::NewFromUtf8(isolate, "resistance"), mesg.GetResistance());
   // obj->Set(String::NewFromUtf8(isolate, "time_from_course"), mesg.GetTimeFromCourse());
   // obj->Set(String::NewFromUtf8(isolate, "cycle_length"), mesg.GetCycleLength());
   // obj->Set(String::NewFromUtf8(isolate, "temperature"), mesg.GetTemperature());
   // obj->Set(String::NewFromUtf8(isolate, "speed_1s"), mesg.GetNumSpeed1s());
   // obj->Set(String::NewFromUtf8(isolate, "cycles"), mesg.GetCycles());
   // obj->Set(String::NewFromUtf8(isolate, "total_cycles"), mesg.GetTotalCycles());
   // obj->Set(String::NewFromUtf8(isolate, "compressed_accumulated_power"), mesg.GetCompressedAccumulatedPower());
   // obj->Set(String::NewFromUtf8(isolate, "accumulated_power"), mesg.GetAccumulatedPower());
   // obj->Set(String::NewFromUtf8(isolate, "left_right_balance"), mesg.GetLeftRightBalance());
   // obj->Set(String::NewFromUtf8(isolate, "gps_accuracy"), mesg.GetGpsAccuracy());
   // obj->Set(String::NewFromUtf8(isolate, "vertical_speed"), mesg.GetVerticalSpeed());
   // obj->Set(String::NewFromUtf8(isolate, "calories"), mesg.GetCalories());
   Handle<Value> argv[2] = {
      String::NewFromUtf8(isolate, "record"), // event name
      obj  // argument
   };

   MakeCallback(this->isolate, self, "emit", 2, argv);
}

void Listener::OnMesg(fit::Mesg& mesg) {
   //printf("Message [%s]:\n", mesg.GetName());
   for (int i = 0; i < mesg.GetNumFields(); i++) {
      fit::Field* field = mesg.GetFieldByIndex(i);
      int subFieldIndex = mesg.GetActiveSubFieldIndex((*field).GetNum());
      //std::cout << "   Name : " << (*field).GetName(subFieldIndex) << std::endl;
      std::wstring value = (*field).GetSTRINGValue(0, subFieldIndex);

      for (int fieldElement = 1; fieldElement < (*field).GetNumValues(); fieldElement++) {
         value += L"|";

         std::wstring nextValue = (*field).GetSTRINGValue(fieldElement, subFieldIndex);
         value += nextValue;
      }

      //std::wcout << "   Value : " << value << std::endl;
      //std::cout << "   Units : " << (*field).GetUnits(subFieldIndex) << std::endl;
      std::string units = (*field).GetUnits(subFieldIndex);
      Local<Object> obj = Object::New(isolate);
      obj->Set(String::NewFromUtf8(isolate, "type"), String::NewFromUtf8(isolate, mesg.GetName().c_str()));
      //obj->Set(String::NewFromUtf8(isolate, "value"), this.GetV8String(value));
      obj->Set(String::NewFromUtf8(isolate, "units"), String::NewFromUtf8(isolate, units.c_str()));
      Handle<Value> argv[2] = {
         String::NewFromUtf8(isolate, "message"), // event name
         obj  // argument
      };

      MakeCallback(isolate, self, "emit", 2, argv);
   }
}
