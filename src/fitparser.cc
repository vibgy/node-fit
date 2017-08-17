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

#include <cstdlib>
#include <ctime>

#include "../libfit/fit_encode.hpp"
#include "../libfit/fit_mesg_broadcaster.hpp"
#include "../libfit/fit_file_id_mesg.hpp"
#include "../libfit/fit_date_time.hpp"

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


void FitParser::EncodeSync(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  FitParser* obj = ObjectWrap::Unwrap<FitParser>(args.Holder());
  obj->value_ += 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}

int mainRun();
int EncodeActivityFile(Isolate* isolate, Local<Object> inputJson);

void FitParser::Encode(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();


  FitParser* obj = ObjectWrap::Unwrap<FitParser>(args.Holder());
  obj->value_ += 1;

  const unsigned argc = 2;
  Handle<Value> argv[argc] = {
    String::NewFromUtf8(isolate, "done"), // event name
    Integer::New(isolate, obj->value_) //Local<NumberValue>(1)  // argument
  };

  Local<Object> inputJson = args[0]->ToObject();
  unsigned int i = inputJson->Get(String::NewFromUtf8(isolate, "localTimestamp"))->Uint32Value();
  printf("localTimestamp %d\n", i);

  //Local<String> ts = Local<String>::Cast(inputJson->Get(String::NewFromUtf8(isolate, "timestamp")));
  //char* str = get(inputJson->Get(String::NewFromUtf8(isolate, "timestamp")));
  std::string str(*String::Utf8Value(inputJson->Get(String::NewFromUtf8(isolate, "timestamp"))));
  printf("timestamp %s\n", str.c_str());
  // cout << "Really " << str ;

  EncodeActivityFile(isolate, inputJson);
  MakeCallback(isolate, args.This(), "emit", 2, argv);
/*
  Local<Object> inputJson = arg[0]->ToObject();
  Local<Object> bufferObj =

  const unsigned argc = 2;
  Local<Value> argv[argc] = {
    String::NewFromUtf8(isolate, "on"), // event name
    buffer //Local<NumberValue>(1)  // argument
  };
*/
  //Local<Function> cb = Local<Function>::Cast(args[1]);
  //cb->Call(Null(isolate), argc, argv);
}


time_t ParseDate(const char * str)
{
    struct tm ti;
    if(strptime(str, "%a %b %d %H:%M:%S %Y", &ti) == NULL)
    //if(strptime("6 Dec 2001 12:33:45", "%d %b %Y %H:%M:%S", &ti) == NULL)
    {
        /* ... error parsing ... */
      printf("Error parsing the date %s\n", str);
    }
    return mktime(&ti);
}

#define GET_STR(s) std::string(*String::Utf8Value(inputJson->Get(String::NewFromUtf8(isolate, s)))).c_str()
#define GET_INT(s) inputJson->Get(String::NewFromUtf8(isolate, s))->Uint32Value()
#define GET_NUM(s) inputJson->Get(String::NewFromUtf8(isolate, s))->NumberValue()

#define GET_SSTR(s) std::string(*String::Utf8Value(inputSession->Get(String::NewFromUtf8(isolate, s)))).c_str()
#define GET_SINT(s) inputSession->Get(String::NewFromUtf8(isolate, s))->Uint32Value()
#define GET_SNUM(s) inputSession->Get(String::NewFromUtf8(isolate, s))->NumberValue()

#define GET_RSTR(s) std::string(*String::Utf8Value(inputRecord->Get(String::NewFromUtf8(isolate, s)))).c_str()
#define GET_RINT(s) inputRecord->Get(String::NewFromUtf8(isolate, s))->Uint32Value()
#define GET_RNUM(s) inputRecord->Get(String::NewFromUtf8(isolate, s))->NumberValue()

int EncodeActivityFile(Isolate* isolate, Local<Object> inputJson)
{
   fit::Encode encode;
   std::fstream file;

   time_t current_time_unix = time(0);
   fit::DateTime initTime(current_time_unix);

   file.open("ExampleActivityFile.fit", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

   if (!file.is_open())
   {
      printf("Error opening file ExampleActivityFile.fit\n");
      return -1;
   }

   encode.Open(file);

   fit::FileIdMesg fileIdMesg; // Every FIT file requires a File ID message
   fileIdMesg.SetType(FIT_FILE_ACTIVITY);
   // todo: set manufacturer. We need a manufacturer ID

  // fileIdMesg.SetManufacturer(FIT_MANUFACTURER_RECON);
   fileIdMesg.SetProduct(1001);

   // todo: set serial number to app version
   fileIdMesg.SetSerialNumber(54321);

   encode.Write(fileIdMesg);

   // todo: get device data from activity
   fit::DeviceInfoMesg deviceInfoMesg;
   deviceInfoMesg.SetTimestamp(initTime.GetTimeStamp()); // Convert to FIT time and write timestamp.
   deviceInfoMesg.SetBatteryStatus(FIT_BATTERY_STATUS_GOOD);

   encode.Write(deviceInfoMesg);

   fit::ActivityMesg activityMesg;
   //time_t current_time_unix = time(0);
   printf("timestamp %s\n", GET_STR("timestamp"));
   printf("localtimestamp %u\n", GET_INT("localTimestamp"));
   printf("sessions %u\n", GET_INT("numSessions"));

   fit::DateTime iTime(ParseDate(GET_STR("timestamp")));
   activityMesg.SetTimestamp(iTime.GetTimeStamp());
   activityMesg.SetLocalTimestamp(GET_INT("localTimestamp"));
   activityMesg.SetNumSessions(GET_INT("numSessions"));
   activityMesg.SetType(FIT_ACTIVITY_MANUAL);//GET_STR("type"));
   activityMesg.SetEvent(FIT_EVENT_ACTIVITY);//GET_STR("event"));
   activityMesg.SetEventType(FIT_EVENT_TYPE_START);//GET_STR("eventType"));
   encode.Write(activityMesg);

   // get sessions object
   int len = 0;
   Local<Array> sessions = Local<Array>::Cast(inputJson->Get(String::NewFromUtf8(isolate, "sessions")));
   if (sessions->IsArray()) {
     //len = sessions->Get(String::NewFromUtf8(isolate, "length"))->ToObject()->Uint32Value();
    len = sessions->Length();
    printf("length %u\n", len);
   }


  for (int i = 0; i < len; i++) {
    fit::SessionMesg sessionMsg;
    Local<Object> inputSession = Local<Object>::Cast(sessions->Get(i));

    printf("timestamp %s\n", GET_SSTR("timestamp"));
    printf("timestamp %s\n", GET_SSTR("startTime"));
    printf("sessions %f\n", GET_SNUM("totalElapsedTime"));

    fit::DateTime its(ParseDate(GET_SSTR("timestamp")));
    sessionMsg.SetTimestamp(its.GetTimeStamp());
    fit::DateTime sTime(ParseDate(GET_SSTR("startTime")));
    sessionMsg.SetStartTime(sTime.GetTimeStamp());
    sessionMsg.SetTotalElapsedTime(GET_SNUM("totalElapsedTime"));

    // todo: get sport from activity json
    sessionMsg.SetSport(FIT_SPORT_CYCLING);
    // this doesn't quite work
    // switch (GET_SSTR("sport")) {
    //   case "RUNNING":
    //   sessionMsg.SetSport(FIT_SPORT_RUNNING);
    //   break;
    //   case "CYCLING":
    //   sessionMsg.SetSport(FIT_SPORT_CYCLING);
    //   break;
    //   default:
    //   sessionMsg.SetSport(FIT_SPORT_CYCLING);
    // }
    sessionMsg.SetEvent(FIT_EVENT_ACTIVITY);
    // todo: set avg speed, max speed, calories and other summary values

    // todo: add laps to session. Add them when target power goes from rest to active or vise versa



    // Not sure what this should be or if we need it
    // sessionMsg.SetEventType(FIT_EVENT_TYPE_STOP);
    encode.Write(sessionMsg);
  }
  // get records array
   int recordslen = 0;
   Local<Array> records = Local<Array>::Cast(inputJson->Get(String::NewFromUtf8(isolate, "records")));
   if (records->IsArray()) {
     //len = sessions->Get(String::NewFromUtf8(isolate, "length"))->ToObject()->Uint32Value();
    recordslen = records->Length();
    printf("records length %u\n", recordslen);
   }

   // create the records
   for (int i = 0; i < recordslen; i++) {
    fit::RecordMesg recordMsg;
    Local<Object> inputRecord = Local<Object>::Cast(records->Get(i));

    printf("timestamp %s\n", GET_RSTR("timestamp"));
    printf("power %d\n", GET_RINT("power"));
    printf("speed %f\n", GET_RNUM("speed"));

    printf("cadence %d\n", GET_RINT("cadence"));\

    fit::DateTime its(ParseDate(GET_RSTR("timestamp")));
    recordMsg.SetTimestamp(its.GetTimeStamp());
    recordMsg.SetPower(GET_RINT("power"));
    recordMsg.SetSpeed(GET_RNUM("speed"));
    recordMsg.SetCadence(GET_RINT("cadence"));
    recordMsg.SetHeartRate(GET_RINT("heart_rate"));

    encode.Write(recordMsg);
  }

   if (!encode.Close())
   {
      printf("Error closing encode.\n");
      return -1;
   }
   file.close();

   printf("Encoded FIT file ExampleActivityFile.fit.\n");
   return 0;
}

int EncodeSettingsFile()
{
   fit::Encode encode;
   std::fstream file;

   file.open("ExampleSettings.fit", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

   if (!file.is_open())
   {
      printf("Error opening file ExampleSettings.fit\n");
      return -1;
   }

   fit::FileIdMesg fileIdMesg; // Every FIT file requires a File ID message
   fileIdMesg.SetType(FIT_FILE_SETTINGS);
   fileIdMesg.SetManufacturer(FIT_MANUFACTURER_DYNASTREAM);
   fileIdMesg.SetProduct(1000);
   fileIdMesg.SetSerialNumber(12345);

   fit::UserProfileMesg userProfileMesg;
   userProfileMesg.SetGender(FIT_GENDER_FEMALE);
   userProfileMesg.SetWeight((FIT_FLOAT32)63.1);
   userProfileMesg.SetAge(99);
   std::wstring wstring_name(L"TestUser");
   userProfileMesg.SetFriendlyName(wstring_name);

   encode.Open(file);
   encode.Write(fileIdMesg);
   encode.Write(userProfileMesg);

   if (!encode.Close())
   {
      printf("Error closing encode.\n");
      return -1;
   }
   file.close();

   printf("Encoded FIT file ExampleSettings.fit.\n");
   return 0;
}

int EncodeMonitoringFile()
{
   fit::Encode encode;
   std::fstream file;

   time_t current_time_unix = time(0);
   fit::DateTime initTime(current_time_unix);

   file.open("ExampleMonitoringFile.fit", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

   if (!file.is_open())
   {
      printf("Error opening file ExampleMonitoringFile.fit\n");
      return -1;
   }

   encode.Open(file);

   fit::FileIdMesg fileIdMesg; // Every FIT file requires a File ID message
   fileIdMesg.SetType(FIT_FILE_MONITORING_B);
   fileIdMesg.SetManufacturer(FIT_MANUFACTURER_DYNASTREAM);
   fileIdMesg.SetProduct(1001);
   fileIdMesg.SetSerialNumber(54321);

   encode.Write(fileIdMesg);

   fit::DeviceInfoMesg deviceInfoMesg;
   deviceInfoMesg.SetTimestamp(initTime.GetTimeStamp()); // Convert to FIT time and write timestamp.
   deviceInfoMesg.SetBatteryStatus(FIT_BATTERY_STATUS_GOOD);

   encode.Write(deviceInfoMesg);

   fit::MonitoringMesg monitoringMesg;

   // By default, each time a new message is written the Local Message Type 0 will be redefined to match the new message.
   // In this case,to avoid having a definition message each time there is a DeviceInfoMesg, we can manually set the Local Message Type of the MonitoringMessage to '1'.
   // By doing this we avoid an additional 7 definition messages in our FIT file.
   monitoringMesg.SetLocalNum(1);

   monitoringMesg.SetTimestamp(initTime.GetTimeStamp()); // Initialise Timestamp to now
   monitoringMesg.SetCycles(0); // Initialise Cycles to 0
   for(int i = 0; i < 4; i++) // This loop represents 1/6 of a day
   {
      for(int j = 0; j < 4; j++) // Each one of these loops represent 1 hour
      {
         fit::DateTime walkingTime(current_time_unix);
         monitoringMesg.SetTimestamp(walkingTime.GetTimeStamp());
         monitoringMesg.SetActivityType(FIT_ACTIVITY_TYPE_WALKING); // By setting this to WALKING, the Cycles field will be interpretted as Steps
         monitoringMesg.SetCycles(monitoringMesg.GetCycles() + (rand()%1000+1)); // Cycles are accumulated (i.e. must be increasing)
         encode.Write(monitoringMesg);
         current_time_unix += (time_t)(3600); //Add an hour to our contrieved timestamp
      }
      fit::DateTime statusTime(current_time_unix);
      deviceInfoMesg.SetTimestamp(statusTime.GetTimeStamp());
      deviceInfoMesg.SetBatteryStatus(FIT_BATTERY_STATUS_GOOD);
      encode.Write(deviceInfoMesg);

   }

   if (!encode.Close())
   {
      printf("Error closing encode.\n");
      return -1;
   }
   file.close();

   printf("Encoded FIT file ExampleMonitoringFile.fit.\n");
   return 0;
}

int mainRun()
{
   printf("FIT Encode Example Application\n");

   int returnValue = 0;

   returnValue += EncodeSettingsFile();
   returnValue += EncodeMonitoringFile();

   return returnValue;
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

