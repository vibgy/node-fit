#include <string>
#include <node.h>
#include <v8.h>
#include <nan.h>

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

#include <clocale>

#include "fitparser.h"
#include "../libfit/fit_decode.hpp"
#include "../libfit/fit_mesg_broadcaster.hpp"

#include "../libfit/fit_encode.hpp"
#include "../libfit/fit_mesg_broadcaster.hpp"
#include "../libfit/fit_file_id_mesg.hpp"
#include "../libfit/fit_date_time.hpp"

using namespace std;
using namespace v8;
using namespace node;
using namespace Nan;
using v8::Isolate;
using node::AtExit;

#define GET_STR(s) std::string(*String::Utf8Value(inputJson->Get(String::NewFromUtf8(isolate, s)))).c_str()
#define GET_INT(s) inputJson->Get(String::NewFromUtf8(isolate, s))->Uint32Value()
#define GET_NUM(s) inputJson->Get(String::NewFromUtf8(isolate, s))->NumberValue()

#define GET_SSTR(s) std::string(*String::Utf8Value(inputSession->Get(String::NewFromUtf8(isolate, s)))).c_str()
#define GET_SINT(s) inputSession->Get(String::NewFromUtf8(isolate, s))->Uint32Value()
#define GET_SNUM(s) inputSession->Get(String::NewFromUtf8(isolate, s))->NumberValue()

#define GET_RSTR(s) std::string(*String::Utf8Value(inputRecord->Get(String::NewFromUtf8(isolate, s)))).c_str()
#define GET_RINT(s) inputRecord->Get(String::NewFromUtf8(isolate, s))->Uint32Value()
#define GET_RNUM(s) inputRecord->Get(String::NewFromUtf8(isolate, s))->NumberValue()

#define GET_LSTR(s) std::string(*String::Utf8Value(inputLap->Get(String::NewFromUtf8(isolate, s)))).c_str()
#define GET_LINT(s) inputLap->Get(String::NewFromUtf8(isolate, s))->Uint32Value()
#define GET_LNUM(s) inputLap->Get(String::NewFromUtf8(isolate, s))->NumberValue()

v8::Persistent<v8::Function> FitParser::constructor;

FitParser::FitParser(){};
FitParser::~FitParser(){};

class Listener : public fit::MesgListener,
                 public fit::RecordMesgListener
{
public:
  Isolate* isolateListener;
  Local<Function> cbListener;

  void Init(Isolate* isolate, Local<Function> cb) {
    isolateListener = isolate;
    cbListener = cb;
  }

  wstring PrintValues(const fit::FieldBase &field)
  {
    std::wstringstream str;

    for (FIT_UINT8 j = 0; j < (FIT_UINT8)field.GetNumValues(); j++)
    {
      // std::wcout << L"       Val" << j << L": ";
      switch (field.GetType())
      {
      // Get float 64 values for numeric types to receive values that have
      // their scale and offset properly applied.
      case FIT_BASE_TYPE_ENUM:
      case FIT_BASE_TYPE_BYTE:
      case FIT_BASE_TYPE_SINT8:
      case FIT_BASE_TYPE_UINT8:
      case FIT_BASE_TYPE_SINT16:
      case FIT_BASE_TYPE_UINT16:
      case FIT_BASE_TYPE_SINT32:
      case FIT_BASE_TYPE_UINT32:
      case FIT_BASE_TYPE_SINT64:
      case FIT_BASE_TYPE_UINT64:
      case FIT_BASE_TYPE_UINT8Z:
      case FIT_BASE_TYPE_UINT16Z:
      case FIT_BASE_TYPE_UINT32Z:
      case FIT_BASE_TYPE_UINT64Z:
      case FIT_BASE_TYPE_FLOAT32:
      case FIT_BASE_TYPE_FLOAT64:
        // std::wcout << field.GetFLOAT64Value(j);
        // str << std::scientific << field.GetFLOAT64Value(j);
        str << field.GetFLOAT64Value(j);
        break;
      case FIT_BASE_TYPE_STRING:
        // std::wcout << field.GetSTRINGValue(j);
        str << field.GetSTRINGValue(j);
        break;
      default:
        break;
      }
      // std::wcout << L" " << field.GetUnits().c_str() << L"\n";
      // str << " " << field.GetUnits().c_str();
    }

    return str.str();
  }

  Local<Object> PrintOverrideValues(const fit::Mesg &mesg, FIT_UINT8 fieldNum, Local<Object> obj)
  {
    std::vector<const fit::FieldBase *> fields = mesg.GetOverrideFields(fieldNum);
    const fit::Profile::FIELD *profileField = fit::Profile::GetField(mesg.GetNum(), fieldNum);
    FIT_BOOL namePrinted = FIT_FALSE;

    Local<Object> localObj;
    if (obj.IsEmpty()) {
      localObj = Object::New(isolateListener);
    } else {
      localObj = obj;
    }

    for (const fit::FieldBase *field : fields)
    {
      if (!namePrinted)
      {
        // printf("   %s:\n", profileField->name.c_str());
        namePrinted = FIT_TRUE;
      }

      // if (FIT_NULL != dynamic_cast<const fit::Field *>(field))
      // {
      //   // Native Field
      //   printf("      native: ");
      // }
      // else
      // {
      //   // Developer Field
      //   printf("      override: ");
      // }

      switch (field->GetType())
      {
      // Get float 64 values for numeric types to receive values that have
      // their scale and offset properly applied.
      case FIT_BASE_TYPE_ENUM:
      case FIT_BASE_TYPE_BYTE:
      case FIT_BASE_TYPE_SINT8:
      case FIT_BASE_TYPE_UINT8:
      case FIT_BASE_TYPE_SINT16:
      case FIT_BASE_TYPE_UINT16:
      case FIT_BASE_TYPE_SINT32:
      case FIT_BASE_TYPE_UINT32:
      case FIT_BASE_TYPE_SINT64:
      case FIT_BASE_TYPE_UINT64:
      case FIT_BASE_TYPE_UINT8Z:
      case FIT_BASE_TYPE_UINT16Z:
      case FIT_BASE_TYPE_UINT32Z:
      case FIT_BASE_TYPE_UINT64Z:
      case FIT_BASE_TYPE_FLOAT32:
      case FIT_BASE_TYPE_FLOAT64:
        // printf("%f\n", field->GetFLOAT64Value());
        localObj->Set(String::NewFromUtf8(isolateListener, profileField->name.c_str()),
          Number::New(isolateListener, field->GetFLOAT64Value()));
        break;
      case FIT_BASE_TYPE_STRING:
        // printf("%ls\n", field->GetSTRINGValue().c_str());
        char charValue[500];
        sprintf(charValue, "%ls\n", field->GetSTRINGValue().c_str());
        localObj->Set(String::NewFromUtf8(isolateListener, profileField->name.c_str()),
          v8::String::NewFromUtf8(isolateListener, charValue));
        break;
      default:
        break;
      }
    }

    return localObj;
  }

  void OnMesg(fit::Mesg &mesg) override
  {
    // const unsigned argc = 1;
    Local<Object> obj;

    printf("On Mesg:\n");
    // std::wcout << L"   New Mesg: " << mesg.GetName().c_str() << L".  It has " << mesg.GetNumFields() << L" field(s) and " << mesg.GetNumDevFields() << " developer field(s).\n";

    for (FIT_UINT16 i = 0; i < (FIT_UINT16)mesg.GetNumFields(); i++)
    {
      if (obj.IsEmpty()) {
        obj = Object::New(isolateListener);
      }

      fit::Field *field = mesg.GetFieldByIndex(i);
      // std::wcout << L"   Field" << i << " (" << field->GetName().c_str() << ") has " << field->GetNumValues() << L" value(s)\n";
      const wstring strValue = PrintValues(*field);
      // std::wcout << strValue;

      char charValue[1000];
      sprintf(charValue, "%ls\n", &strValue);
      obj->Set(String::NewFromUtf8(isolateListener, field->GetName().c_str()),
        v8::String::NewFromUtf8(isolateListener, charValue));
    }

    for (auto devField : mesg.GetDeveloperFields())
    {
      if (obj.IsEmpty()) {
        obj = Object::New(isolateListener);
      }

      // std::wcout << L"   Developer Field(" << devField.GetName().c_str() << ") has " << devField.GetNumValues() << L" value(s)\n";
      // PrintValues(devField);
      wstring strValue = PrintValues(devField);
      // std::wcout << strValue;

      char charValue[500];
      sprintf(charValue, "%ls\n", &strValue);

      obj->Set(String::NewFromUtf8(isolateListener, devField.GetName().c_str()),
        v8::String::NewFromUtf8(isolateListener, charValue));
    }

    /*Local<Value> argv[argc] = obj;
    cbListener->Call(Null(isolateListener), argc, argv);*/
  }

  void OnMesg(fit::RecordMesg &record) override
  {
    // const unsigned argc = 1;
    Local<Object> obj;

    // printf("Records:\n");
    obj = PrintOverrideValues(record, fit::RecordMesg::FieldDefNum::HeartRate, obj);
    obj = PrintOverrideValues(record, fit::RecordMesg::FieldDefNum::Cadence, obj);
    obj = PrintOverrideValues(record, fit::RecordMesg::FieldDefNum::Distance, obj);
    obj = PrintOverrideValues(record, fit::RecordMesg::FieldDefNum::Speed, obj);

    /*Local<Value> argv[argc] = obj;
    cbListener->Call(Null(isolateListener), argc, argv);*/
  }
};

time_t ParseDate(const char *str)
{
  struct tm ti;
  return mktime(&ti);
}

void FitParser::Init(v8::Local<v8::Object> exports)
{
  Isolate *isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "FitParser"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "encode", Encode);
  NODE_SET_PROTOTYPE_METHOD(tpl, "decode", Decode);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "FitParser"), tpl->GetFunction());
}

void FitParser::New(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  Isolate *isolate = args.GetIsolate();

  if (args.IsConstructCall())
  {
    // Invoked as constructor: `new MyObject(...)`
    // double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    // FitParser *obj = new FitParser(value);
    FitParser *obj = new FitParser();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  }
  else
  {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = {args[0]};
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }
}

void FitParser::Encode(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  const unsigned argc = 1;
  Isolate *isolate = args.GetIsolate();
  Local<Object> inputJson = args[0]->ToObject();
  Local<Function> cb = Local<Function>::Cast(args[1]);

  fit::Encode encode(fit::ProtocolVersion::V20);
  std::fstream file;
  std::list<fit::RecordMesg> records;

  time_t current_time_unix = time(0);
  fit::DateTime initTime(current_time_unix);
  fit::DateTime iTime(ParseDate(GET_STR("timestamp")));

  int sessionsLen = 0;
  int jsonRecordsLen = 0;
  int jsonLapsLen = 0;

  file.open("ExampleActivityFile.fit", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

  if (!file.is_open())
  {
    // cout << "Error opening file ExampleActivityFile.fit" << endl;
    Local<Value> argv[argc] = {String::NewFromUtf8(isolate, "Error opening file ExampleActivityFile.fit")};
    cb->Call(Null(isolate), argc, argv);
    return;
  }

  encode.Open(file);
  // FileIdMesg SECTION
  fit::FileIdMesg fileIdMesg; // Every FIT file requires a File ID message
  fileIdMesg.SetType(FIT_FILE_ACTIVITY);
  fileIdMesg.SetManufacturer(FIT_MANUFACTURER_THE_SUFFERFEST);
  fileIdMesg.SetProduct(1231);
  fileIdMesg.SetSerialNumber(12345);
  fileIdMesg.SetTimeCreated(GET_INT("timeCreated"));

  encode.Write(fileIdMesg);

  // deviceInfoMesg SECTION
  // todo: get device data from activity
  fit::DeviceInfoMesg deviceInfoMesg;
  deviceInfoMesg.SetTimestamp(initTime.GetTimeStamp()); // Convert to FIT time and write timestamp.
  deviceInfoMesg.SetBatteryStatus(FIT_BATTERY_STATUS_GOOD);

  encode.Write(deviceInfoMesg);

  //ActivityMesg SECTION
  fit::ActivityMesg activityMesg;
  activityMesg.SetTimestamp(GET_INT("timestamp"));
  activityMesg.SetLocalTimestamp(GET_INT("localTimestamp"));
  activityMesg.SetNumSessions(GET_INT("numSessions"));
  activityMesg.SetType(FIT_ACTIVITY_MANUAL);
  activityMesg.SetEvent(FIT_EVENT_ACTIVITY);       //GET_STR("event"));
  activityMesg.SetEventType(FIT_EVENT_TYPE_START); //GET_STR("eventType"));

  encode.Write(activityMesg);

  // sessionMsg SECTION
  Local<Array> sessions = Local<Array>::Cast(inputJson->Get(String::NewFromUtf8(isolate, "sessions")));
  if (sessions->IsArray()) {
    sessionsLen = sessions->Length();
  }

  // cout << "Sessions" << endl;
  for (int i = 0; i < sessionsLen; i++)
  {
    fit::SessionMesg sessionMsg;
    Local<Object> inputSession = Local<Object>::Cast(sessions->Get(i));

    // cout << "[timestampS] " << GET_SINT("timestamp") << endl;

    sessionMsg.SetTimestamp(GET_SINT("timestamp"));
    sessionMsg.SetStartTime(GET_SINT("startTime"));
    sessionMsg.SetTotalElapsedTime(GET_SNUM("totalElapsedTime"));
    sessionMsg.SetTotalTimerTime(GET_SNUM("totalTimerTime"));
    sessionMsg.SetTotalDistance(GET_SINT("totalDistance"));
    sessionMsg.SetTrainingStressScore(GET_SINT("trainingStressScore"));
    sessionMsg.SetIntensityFactor(GET_SINT("intensityFactor"));
    // Fixes for Garmin Physio
    sessionMsg.SetTotalMovingTime(GET_SNUM("totalTimerTime"));
    // message index (0)
    sessionMsg.SetMessageIndex(0);

    // calories
    sessionMsg.SetTotalCalories(GET_SINT("totalCalories"));
    // todo: get sport from activity json
    // sessionMsg.SetSport(FIT_SPORT_CYCLING);

    // this doesn't quite work
    std::string sport = GET_SSTR("sport");
    // cout << GET_SSTR("sport");
    if (sport == "RUNNING") {
      sessionMsg.SetSport(FIT_SPORT_RUNNING);
      sessionMsg.SetSubSport(FIT_SUB_SPORT_INDOOR_RUNNING);
    }
    else if (sport == "CYCLING") {
      sessionMsg.SetSport(FIT_SPORT_CYCLING);
      sessionMsg.SetSubSport(FIT_SUB_SPORT_INDOOR_CYCLING);
    }
    else if (sport == "MULTISPORT") {
      sessionMsg.SetSport(FIT_SPORT_MULTISPORT);
      sessionMsg.SetSubSport(FIT_SUB_SPORT_INDOOR_CYCLING);
    }
    else if (sport == "MENTAL") {
      sessionMsg.SetSport(FIT_SPORT_GENERIC);
    }
    else if (sport == "STRENGTH") {
      sessionMsg.SetSport(FIT_SPORT_TRAINING);
      sessionMsg.SetSubSport(FIT_SUB_SPORT_STRENGTH_TRAINING);
    }
    else if (sport == "YOGA") {
      sessionMsg.SetSport(FIT_SPORT_TRAINING);
      sessionMsg.SetSubSport(FIT_SUB_SPORT_YOGA);
    }

    sessionMsg.SetEvent(FIT_EVENT_ACTIVITY);
    // todo: set avg speed, max speed, calories and other summary values
    // set average/max speed
    sessionMsg.SetAvgSpeed(GET_SNUM("avgSpeed"));
    sessionMsg.SetMaxSpeed(GET_SNUM("maxSpeed"));
    // set average/max cadence
    sessionMsg.SetAvgCadence(GET_SNUM("avgCadence"));
    sessionMsg.SetMaxCadence(GET_SNUM("maxCadence"));
    // set average/max power
    sessionMsg.SetAvgPower(GET_SNUM("avgPower"));
    sessionMsg.SetMaxPower(GET_SNUM("maxPower"));
    // set average/max heartRate
    sessionMsg.SetAvgHeartRate(GET_SNUM("avgHeartRate"));
    sessionMsg.SetMaxHeartRate(GET_SNUM("maxHeartRate"));
    // set threshold power
    sessionMsg.SetThresholdPower(GET_SNUM("thresholdPower"));
    // set total work
    sessionMsg.SetTotalWork(GET_SNUM("totalWork"));

    // todo: add laps to session. Add them when target power goes from rest to active or vise versa

    Local<Array> jsonLaps = Local<Array>::Cast(inputSession->Get(String::NewFromUtf8(isolate, "laps")));
    if (jsonLaps->IsArray())
    {
      jsonLapsLen = jsonLaps->Length();
    }

    for (int i = 0; i < jsonLapsLen; i++)
    {
      fit::LapMesg lapMsg;
      Local<Object> inputLap = Local<Object>::Cast(jsonLaps->Get(i));
      lapMsg.SetTimestamp(GET_LNUM("timestamp"));      
      lapMsg.SetStartTime(GET_LNUM("startTime"));
      lapMsg.SetTotalElapsedTime(GET_LNUM("totalElapsedTime"));
      lapMsg.SetTotalDistance(GET_LNUM("totalDistance"));
      lapMsg.SetMaxSpeed(GET_LNUM("maxSpeed"));
      lapMsg.SetAvgSpeed(GET_LNUM("avgSpeed"));
      lapMsg.SetMaxCadence(GET_LNUM("maxCadence"));
      lapMsg.SetAvgCadence(GET_LNUM("avgCadence"));
      lapMsg.SetMaxPower(GET_LNUM("maxPower"));
      lapMsg.SetAvgPower(GET_LNUM("avgPower"));
      lapMsg.SetMaxHeartRate(GET_LNUM("maxHeartRate"));
      lapMsg.SetAvgHeartRate(GET_LNUM("avgHeartRate"));
      lapMsg.SetTotalWork(GET_LNUM("totalWork"));
      encode.Write(lapMsg);
    }

    // Not sure what this should be or if we need it
    // sessionMsg.SetEventType(FIT_EVENT_TYPE_STOP);
    encode.Write(sessionMsg);
  }




  // cout << "Records" << endl;
  Local<Array> jsonRecords = Local<Array>::Cast(inputJson->Get(String::NewFromUtf8(isolate, "records")));
  if (jsonRecords->IsArray())
  {
    jsonRecordsLen = jsonRecords->Length();
  }

  for (int i = 0; i < jsonRecordsLen; i++)
  {
    fit::RecordMesg recordMsg;
    Local<Object> inputRecord = Local<Object>::Cast(jsonRecords->Get(i));

    //cout << "[timestamp] " << GET_RINT("timestamp") << endl;

    recordMsg.SetTimestamp(GET_RINT("timestamp"));
    recordMsg.SetPower(GET_RINT("power"));
    recordMsg.SetSpeed(GET_RNUM("speed"));
    recordMsg.SetDistance(GET_RINT("distance"));
    recordMsg.SetCadence(GET_RINT("cadence"));
    recordMsg.SetHeartRate(GET_RINT("heart_rate"));

    encode.Write(recordMsg);
  }




  if (!encode.Close())
  {
    // cout << "Error closing encode.\n"
    //      << endl;
    Local<Value> argv[argc] = {String::NewFromUtf8(isolate, "Error closing encode.")};
    cb->Call(Null(isolate), argc, argv);
    return;
  }


  std::streamsize size = file.tellg();
  file.seekg(0, file.beg);
  std::vector<char> buffer(size);
  if (file.read(buffer.data(), size))  {
  }


  file.close();

  // Local<Value> argv[argc] = Nan::NewBuffer(buffer, sizeof(file)).ToLocalChecked();
  /*Local<Value> argv[argc] = Nan::Encode(buffer, sizeof(file), Nan::Encoding::BINARY);
  cb->Call(Null(isolate), argc, argv);*/

  args.GetReturnValue().Set(Nan::Encode(buffer.data(), size, Nan::Encoding::BINARY));
}

void FitParser::Decode(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  const unsigned argc = 1;
  fit::Decode decode;
  fit::MesgBroadcaster mesgBroadcaster;
  std::fstream file;

  Isolate *isolate = args.GetIsolate();
  String::Utf8Value fileName(args[0]->ToString());
  Local<Function> cb = Local<Function>::Cast(args[1]);

  Listener listener;
  listener.Init(isolate, cb);

  file.open(strdup(*fileName), std::ios::in | std::ios::binary);

  if (!file.is_open())
  {
    Local<Value> argv[argc] = {String::NewFromUtf8(isolate, "File is not open")};
    cb->Call(Null(isolate), argc, argv);
    return;
  }

  if (!decode.IsFIT(file))
  {
    Local<Value> argv[argc] = {String::NewFromUtf8(isolate, "File is not FIT")};
    cb->Call(Null(isolate), argc, argv);
    return;
  }

  if (!decode.CheckIntegrity(file))
  {
    Local<Value> argv[argc] = {String::NewFromUtf8(isolate, "FIT file integrity failed.\nAttempting to decode...")};
    cb->Call(Null(isolate), argc, argv);
    return;
  }

  mesgBroadcaster.AddListener((fit::RecordMesgListener &)listener);

  // This listener is not working properly
  // mesgBroadcaster.AddListener((fit::MesgListener &)listener);

  try {
    decode.Read(file, mesgBroadcaster);

  } catch (const fit::RuntimeException &e) {
    printf("Exception decoding file: %s\n", e.what());
    Local<Value> argv[argc] = {String::NewFromUtf8(isolate, "Exception decoding file")};
    cb->Call(Null(isolate), argc, argv);
  }
}
