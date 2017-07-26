// Copyright (c) 2016 Greenheart Games Pty. Ltd. All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include <map>
#include <sstream>

#include "nan.h"
#include "steam/steam_api.h"
#include "v8.h"

#include "greenworks_async_workers.h"
#include "greenworks_utils.h"
#include "greenworks_version.h"
#include "steam_api_registry.h"
#include "steam_client.h"
#include "steam_event.h"
#include "steam_id.h"

namespace greenworks {
namespace api {
namespace {

NAN_METHOD(FindLeaderboard) {
  Nan::HandleScope scope;
  if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsFunction()) {
    THROW_BAD_ARGS("Bad arguments");
  }

  std::string leaderboard_name(*(v8::String::Utf8Value(info[0])));
  Nan::Callback* success_callback = new Nan::Callback(info[1].As<v8::Function>());
  Nan::Callback* error_callback = new Nan::Callback(info[2].As<v8::Function>());
  
  Nan::AsyncQueueWorker(new greenworks::FindLeaderboardWorker(leaderboard_name,
                                                              success_callback,
                                                              error_callback));
  
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(UploadLeaderboardScore) {
  Nan::HandleScope scope;
  if (info.Length() < 4 || 
      !info[0]->IsInt32() || 
      !info[1]->IsInt32() || 
      !info[2]->IsBoolean() || 
      !info[3]->IsFunction()) {
    THROW_BAD_ARGS("Bad arguments");
  }

  int leaderboard_handle = info[0]->Int32Value();
  int score = info[1]->Int32Value();
  bool force = info[2]->BooleanValue();
  Nan::Callback* success_callback = new Nan::Callback(info[3].As<v8::Function>());
  Nan::Callback* error_callback = new Nan::Callback(info[4].As<v8::Function>());
  
  Nan::AsyncQueueWorker(new greenworks::UploadLeaderboardScoreWorker(leaderboard_handle,
                                                          score,
                                                          force,
                                                          success_callback,
                                                          error_callback));

  info.GetReturnValue().Set(Nan::Undefined());
}
  
NAN_METHOD(DownloadLeaderboardEntries) {
  Nan::HandleScope scope;
  if (info.Length() < 5 || 
      !info[0]->IsInt32() || // leaderboard_handle
      !info[1]->IsInt32() || // request_type
      !info[2]->IsInt32() || // range_start
      !info[3]->IsInt32() || // range_end
      !info[4]->IsFunction()) { // success_callback
    THROW_BAD_ARGS("Bad arguments");
  }

  int leaderboard_handle = info[0]->Int32Value();
  int request_type = info[1]->Int32Value();
  int range_start = info[2]->Int32Value();
  int range_end = info[3]->Int32Value();
  Nan::Callback* success_callback = new Nan::Callback(info[4].As<v8::Function>());
  Nan::Callback* error_callback = new Nan::Callback(info[5].As<v8::Function>());

  Nan::AsyncQueueWorker(new greenworks::DownloadLeaderboardEntriesWorker(leaderboard_handle,
                                                          request_type,
                                                          range_start,
                                                          range_end,
                                                          success_callback,
                                                          error_callback));

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(AttachLeaderboardUGC) {
  Nan::HandleScope scope;
  if (info.Length() < 3 ||
    !info[0]->IsString() || // leaderboard_handle
    !info[1]->IsString() || // hUGC
    !info[2]->IsFunction()) { // success_callback
    THROW_BAD_ARGS("Bad arguments");
  }

  int leaderboard_handle = utils::strToUint64(*(v8::String::Utf8Value(info[0])));
  int ugc_handle = utils::strToUint64(*(v8::String::Utf8Value(info[1])));
  Nan::Callback* success_callback = new Nan::Callback(info[2].As<v8::Function>());
  Nan::Callback* error_callback = new Nan::Callback(info[3].As<v8::Function>());

  Nan::AsyncQueueWorker(new greenworks::AttachLeaderboardUGCWorker(leaderboard_handle,
                                                                   ugc_handle,
                                                                   success_callback,
                                                                   error_callback));

  info.GetReturnValue().Set(Nan::Undefined());
}

void RegisterAPIs(v8::Handle<v8::Object> exports) {
  Nan::Set(exports,
           Nan::New("FindLeaderboard").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(FindLeaderboard)->GetFunction());
  Nan::Set(exports,
           Nan::New("UploadLeaderboardScore").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(UploadLeaderboardScore)->GetFunction());
  Nan::Set(exports,
           Nan::New("DownloadLeaderboardEntries").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(DownloadLeaderboardEntries)->GetFunction());
  Nan::Set(exports,
           Nan::New("AttachLeaderboardUGC").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(AttachLeaderboardUGC)->GetFunction());

}

SteamAPIRegistry::Add X(RegisterAPIs);

}  // namespace
}  // namespace api
}  // namespace greenworks
