// Copyright (c) 2016 Greenheart Games Pty. Ltd. All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "nan.h"
#include "steam/steam_api.h"
#include "v8.h"

#include "greenworks_utils.h"
#include "steam_api_registry.h"
#include "steam_id.h"

namespace greenworks {
namespace api {
namespace {

void InitFriendFlags(v8::Handle<v8::Object> exports) {
  v8::Local<v8::Object> friend_flags = Nan::New<v8::Object>();
  SET_TYPE(friend_flags, "None", k_EFriendFlagNone);
  SET_TYPE(friend_flags, "Blocked", k_EFriendFlagBlocked);
  SET_TYPE(friend_flags, "FriendshipRequested",
           k_EFriendFlagFriendshipRequested);
  SET_TYPE(friend_flags, "Immediate", k_EFriendFlagImmediate);
  SET_TYPE(friend_flags, "ClanMember", k_EFriendFlagClanMember);
  SET_TYPE(friend_flags, "OnGameServer", k_EFriendFlagOnGameServer);
  SET_TYPE(friend_flags, "RequestingFriendship",
           k_EFriendFlagRequestingFriendship);
  SET_TYPE(friend_flags, "RequestingInfo", k_EFriendFlagRequestingInfo);
  SET_TYPE(friend_flags, "Ignored", k_EFriendFlagIgnored);
  SET_TYPE(friend_flags, "IgnoredFriend", k_EFriendFlagIgnoredFriend);
  SET_TYPE(friend_flags, "Suggested", k_EFriendFlagSuggested);
  SET_TYPE(friend_flags, "ChatMember", k_EFriendFlagChatMember);
  SET_TYPE(friend_flags, "All", k_EFriendFlagAll);
  Nan::Persistent<v8::Object> constructor;
  constructor.Reset(friend_flags);
  Nan::Set(exports,
           Nan::New("FriendFlags").ToLocalChecked(),
           friend_flags);
}

void InitFriendRelationship(v8::Handle<v8::Object> exports) {
  v8::Local<v8::Object> relationship = Nan::New<v8::Object>();
  SET_TYPE(relationship, "None", k_EFriendRelationshipNone);
  SET_TYPE(relationship, "Blocked", k_EFriendRelationshipBlocked);
  SET_TYPE(relationship, "RequestRecipient",
           k_EFriendRelationshipRequestRecipient);
  SET_TYPE(relationship, "Friend", k_EFriendRelationshipFriend);
  SET_TYPE(relationship, "RequestInitiator",
           k_EFriendRelationshipRequestInitiator);
  SET_TYPE(relationship, "Ignored", k_EFriendRelationshipIgnored);
  SET_TYPE(relationship, "IgnoredFriend", k_EFriendRelationshipIgnoredFriend);
  SET_TYPE(relationship, "Suggested", k_EFriendRelationshipSuggested);
  Nan::Persistent<v8::Object> constructor;
  constructor.Reset(relationship);
  Nan::Set(exports,
           Nan::New("FriendRelationship").ToLocalChecked(),
           relationship);
}

void InitFriendPersonaChange(v8::Handle<v8::Object> exports) {
  v8::Local<v8::Object> persona_change = Nan::New<v8::Object>();
  SET_TYPE(persona_change, "Name", k_EPersonaChangeName);
  SET_TYPE(persona_change, "Status", k_EPersonaChangeStatus);
  SET_TYPE(persona_change, "ComeOnline", k_EPersonaChangeComeOnline);
  SET_TYPE(persona_change, "GoneOffline", k_EPersonaChangeGoneOffline);
  SET_TYPE(persona_change, "GamePlayed", k_EPersonaChangeGamePlayed);
  SET_TYPE(persona_change, "GameServer", k_EPersonaChangeGameServer);
  SET_TYPE(persona_change, "Avator", k_EPersonaChangeAvatar);
  SET_TYPE(persona_change, "JoinedSource", k_EPersonaChangeJoinedSource);
  SET_TYPE(persona_change, "LeftSource", k_EPersonaChangeLeftSource);
  SET_TYPE(persona_change, "RelationshipChanged",
           k_EPersonaChangeRelationshipChanged);
  SET_TYPE(persona_change, "NameFirstSet", k_EPersonaChangeNameFirstSet);
  SET_TYPE(persona_change, "FacebookInfo", k_EPersonaChangeFacebookInfo);
  SET_TYPE(persona_change, "NickName", k_EPersonaChangeNickname);
  SET_TYPE(persona_change, "SteamLevel", k_EPersonaChangeSteamLevel);
  Nan::Persistent<v8::Object> constructor;
  constructor.Reset(persona_change);
  Nan::Set(exports,
           Nan::New("PersonaChange").ToLocalChecked(),
           persona_change);
}

void InitAccountType(v8::Handle<v8::Object> exports) {
  v8::Local<v8::Object> account_type = Nan::New<v8::Object>();
  SET_TYPE(account_type, "Invalid", k_EAccountTypeInvalid);
  SET_TYPE(account_type, "Individual", k_EAccountTypeIndividual);
  SET_TYPE(account_type, "Multiseat", k_EAccountTypeMultiseat);
  SET_TYPE(account_type, "GameServer", k_EAccountTypeGameServer);
  SET_TYPE(account_type, "AnonymousGameServer", k_EAccountTypeAnonGameServer);
  SET_TYPE(account_type, "Pending", k_EAccountTypePending);
  SET_TYPE(account_type, "ContentServer", k_EAccountTypeContentServer);
  SET_TYPE(account_type, "Clan", k_EAccountTypeClan);
  SET_TYPE(account_type, "Chat", k_EAccountTypeChat);
  SET_TYPE(account_type, "ConsoleUser", k_EAccountTypeConsoleUser);
  SET_TYPE(account_type, "AnonymousUser", k_EAccountTypeAnonUser);
  Nan::Persistent<v8::Object> constructor;
  constructor.Reset(account_type);
  Nan::Set(exports,
           Nan::New("AccountType").ToLocalChecked(),
           account_type);
}

NAN_METHOD(GetFriendCount) {
  Nan::HandleScope scope;
  if (info.Length() < 1 || !info[0]->IsInt32()) {
    THROW_BAD_ARGS("Bad arguments");
  }
  EFriendFlags friend_flag = static_cast<EFriendFlags>(info[0]->Int32Value());

  info.GetReturnValue().Set(Nan::New<v8::Integer>(
    SteamFriends()->GetFriendCount(friend_flag)));
}

NAN_METHOD(GetFriends) {
  Nan::HandleScope scope;
  if (info.Length() < 1 || !info[0]->IsInt32()) {
    THROW_BAD_ARGS("Bad arguments");
  }
  EFriendFlags friend_flag = static_cast<EFriendFlags>(info[0]->Int32Value());
  int friends_count = SteamFriends()->GetFriendCount(friend_flag);
  v8::Local<v8::Array> friends = Nan::New<v8::Array>(friends_count);

  for (int i = 0; i < friends_count; ++i) {
    CSteamID steam_id = SteamFriends()->GetFriendByIndex(i, friend_flag);
    friends->Set(i, greenworks::SteamID::Create(steam_id));
  }
  info.GetReturnValue().Set(friends);
}

NAN_METHOD(GetSmallFriendAvatar) {
  Nan::HandleScope scope;
  if (info.Length() < 1 || !info[0]->IsString()) {
    THROW_BAD_ARGS("Bad arguments");
  }
  std::string steam_id_str(*(v8::String::Utf8Value(info[0])));
  CSteamID steam_id(utils::strToUint64(steam_id_str));
  if (!steam_id.IsValid()) {
    THROW_BAD_ARGS("Steam ID is invalid");
  }
  info.GetReturnValue().Set(
      SteamFriends()->GetSmallFriendAvatar(steam_id));
}

NAN_METHOD(GetMediumFriendAvatar) {
  Nan::HandleScope scope;
  if (info.Length() < 1 || !info[0]->IsString()) {
    THROW_BAD_ARGS("Bad arguments");
  }
  std::string steam_id_str(*(v8::String::Utf8Value(info[0])));
  CSteamID steam_id(utils::strToUint64(steam_id_str));
  if (!steam_id.IsValid()) {
    THROW_BAD_ARGS("Steam ID is invalid");
  }
  info.GetReturnValue().Set(
      SteamFriends()->GetMediumFriendAvatar(steam_id));
}

NAN_METHOD(GetLargeFriendAvatar) {
  Nan::HandleScope scope;
  if (info.Length() < 1 || !info[0]->IsString()) {
    THROW_BAD_ARGS("Bad arguments");
  }
  std::string steam_id_str(*(v8::String::Utf8Value(info[0])));
  CSteamID steam_id(utils::strToUint64(steam_id_str));
  if (!steam_id.IsValid()) {
    THROW_BAD_ARGS("Steam ID is invalid");
  }
  info.GetReturnValue().Set(
      SteamFriends()->GetLargeFriendAvatar(steam_id));
}

NAN_METHOD(RequestUserInformation) {
  Nan::HandleScope scope;
  if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsBoolean()) {
    THROW_BAD_ARGS("Bad arguments");
  }
  std::string steam_id_str(*(v8::String::Utf8Value(info[0])));
  bool require_name_only = info[1]->BooleanValue();
  CSteamID steam_id(utils::strToUint64(steam_id_str));
  if (!steam_id.IsValid()) {
    THROW_BAD_ARGS("Steam ID is invalid");
  }
  SteamFriends()->RequestUserInformation(steam_id, require_name_only);
}

void RegisterAPIs(v8::Handle<v8::Object> exports) {
  InitFriendFlags(exports);
  InitFriendRelationship(exports);
  InitFriendPersonaChange(exports);
  InitAccountType(exports);

  Nan::Set(exports,
           Nan::New("getFriendCount").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(GetFriendCount)->GetFunction());
  Nan::Set(exports,
           Nan::New("getFriends").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(GetFriends)->GetFunction());
  Nan::Set(exports, Nan::New("getSmallFriendAvatar").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(GetSmallFriendAvatar)->GetFunction());
  Nan::Set(
      exports, Nan::New("getMediumFriendAvatar").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(GetMediumFriendAvatar)->GetFunction());
  Nan::Set(
      exports, Nan::New("getLargeFriendAvatar").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(GetLargeFriendAvatar)->GetFunction());
  Nan::Set(
      exports, Nan::New("requestUserInformation").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(RequestUserInformation)->GetFunction());
}

SteamAPIRegistry::Add X(RegisterAPIs);

}  // namespace
}  // namespace api
}  // namespace greenworks