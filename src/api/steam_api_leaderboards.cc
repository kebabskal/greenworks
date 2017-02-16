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

void RegisterAPIs(v8::Handle<v8::Object> exports) {
  Nan::Set(exports,
           Nan::New("FindLeaderboard").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(FindLeaderboard)->GetFunction());
  Nan::Set(exports,
           Nan::New("UploadLeaderboardScore").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(UploadLeaderboardScore)->GetFunction());
}

SteamAPIRegistry::Add X(RegisterAPIs);

}  // namespace
}  // namespace api
}  // namespace greenworks
