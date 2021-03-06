// Copyright (c) 2014 Greenheart Games Pty. Ltd. All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SRC_GREENWORKS_ASYNC_WORKERS_H_
#define SRC_GREENWORKS_ASYNC_WORKERS_H_

#include <string>
#include <vector>

#include "steam/steam_api.h"

#include "steam_async_worker.h"
#include "greenworks_utils.h"
#include "greenworks_workshop_workers.h"

namespace greenworks {

class FileContentSaveWorker : public SteamAsyncWorker {
 public:
  FileContentSaveWorker(Nan::Callback* success_callback,
                        Nan::Callback* error_callback,
                        std::string file_name,
                        std::string content);

  // Override NanAsyncWorker methods.
  virtual void Execute();

 private:
  std::string file_name_;
  std::string content_;
};

class FilesSaveWorker : public SteamAsyncWorker {
 public:
  FilesSaveWorker(Nan::Callback* success_callback,
                  Nan::Callback* error_callback,
                  const std::vector<std::string>& files_path);

  // Override NanAsyncWorker methods.
  virtual void Execute();

 private:
  std::vector<std::string> files_path_;
};

class FileReadWorker : public SteamAsyncWorker {
 public:
  FileReadWorker(Nan::Callback* success_callback, Nan::Callback* error_callback,
      std::string file_name);

  // Override NanAsyncWorker methods.
  virtual void Execute();
  virtual void HandleOKCallback();

 private:
  std::string file_name_;
  std::string content_;
};

class FileDeleteWorker : public SteamAsyncWorker {
 public:
  FileDeleteWorker(Nan::Callback* success_callback,
                   Nan::Callback* error_callback,
                   std::string file_name);

  // Override NanAsyncWorker methods.
  virtual void Execute();

 private:
  std::string file_name_;
};

class CloudQuotaGetWorker : public SteamAsyncWorker {
 public:
  CloudQuotaGetWorker(Nan::Callback* success_callback,
      Nan::Callback* error_callback);

  // Override NanAsyncWorker methods.
  virtual void Execute();
  virtual void HandleOKCallback();

 private:
  uint64 total_bytes_;
  uint64 available_bytes_;
};

class ActivateAchievementWorker : public SteamAsyncWorker {
 public:
  ActivateAchievementWorker(Nan::Callback* success_callback,
      Nan::Callback* error_callback, const std::string& achievement);

  // Override NanAsyncWorker methods.
  virtual void Execute();

 private:
  std::string achievement_;
};

class GetAchievementWorker : public SteamAsyncWorker {
 public:
  GetAchievementWorker(Nan::Callback* success_callback,
                       Nan::Callback* error_callback,
                       const std::string& achievement);

  // Override NanAsyncWorker methods.
  virtual void Execute();
  virtual void HandleOKCallback();

 private:
  std::string achievement_;
  bool is_achieved_;
};

class ClearAchievementWorker : public SteamAsyncWorker {
 public:
  ClearAchievementWorker(Nan::Callback* success_callback,
                       Nan::Callback* error_callback,
                       const std::string& achievement);

  // Override NanAsyncWorker methods.
  virtual void Execute();

 private:
  std::string achievement_;
  bool success_;
};

class GetNumberOfPlayersWorker : public SteamCallbackAsyncWorker {
 public:
  GetNumberOfPlayersWorker(Nan::Callback* success_callback,
                           Nan::Callback* error_callback);
  void OnGetNumberOfPlayersCompleted(NumberOfCurrentPlayers_t* result,
                                     bool io_failure);
  // Override NanAsyncWorker methods.
  virtual void Execute();
  virtual void HandleOKCallback();

 private:
  int num_of_players_;
  CCallResult<GetNumberOfPlayersWorker, NumberOfCurrentPlayers_t> call_result_;
};

class CreateArchiveWorker : public SteamAsyncWorker {
 public:
  CreateArchiveWorker(Nan::Callback* success_callback,
                      Nan::Callback* error_callback,
                      const std::string& zip_file_path,
                      const std::string& source_dir,
                      const std::string& password,
                      int compress_level);

  // Override NanAsyncWorker methods.
  virtual void Execute();

 private:
  std::string zip_file_path_;
  std::string source_dir_;
  std::string password_;
  int compress_level_;
};

class ExtractArchiveWorker : public SteamAsyncWorker {
 public:
  ExtractArchiveWorker(Nan::Callback* success_callback,
                       Nan::Callback* error_callback,
                       const std::string& zip_file_path,
                       const std::string& extract_path,
                       const std::string& password);

  // Override NanAsyncWorker methods.
  virtual void Execute();

 private:
  std::string zip_file_path_;
  std::string extract_path_;
  std::string password_;
};

class GetAuthSessionTicketWorker : public SteamCallbackAsyncWorker {
 public:
  GetAuthSessionTicketWorker(Nan::Callback* success_callback,
                             Nan::Callback* error_callback);
  STEAM_CALLBACK(GetAuthSessionTicketWorker,
                 OnGetAuthSessionCompleted,
                 GetAuthSessionTicketResponse_t,
                 result);
  virtual void Execute();
  virtual void HandleOKCallback();

 private:
  HAuthTicket handle_;
  unsigned int ticket_buf_size_;
  uint8 ticket_buf_[2048];
};

class RequestEncryptedAppTicketWorker : public SteamCallbackAsyncWorker {
 public:
  RequestEncryptedAppTicketWorker(std::string user_data,
                                  Nan::Callback* success_callback,
                                  Nan::Callback* error_callback);
  void OnRequestEncryptedAppTicketCompleted(
      EncryptedAppTicketResponse_t*, bool);
  virtual void Execute();
  virtual void HandleOKCallback();

 private:
  std::string user_data_;
  unsigned int ticket_buf_size_;
  uint8 ticket_buf_[4096];
  CCallResult< RequestEncryptedAppTicketWorker, EncryptedAppTicketResponse_t >
      call_result_;
};

class FindLeaderboardWorker : public SteamCallbackAsyncWorker {
  public:
    FindLeaderboardWorker(std::string leaderboard_name,
                          Nan::Callback* success_callback,
                          Nan::Callback* error_callback);
    void OnFindLeaderboardCompleted(
      LeaderboardFindResult_t*, bool);
    virtual void Execute();
    virtual void HandleOKCallback();
  private:
    std::string leaderboard_name_;
    uint64 leaderboard_handle_;
    CCallResult< FindLeaderboardWorker, LeaderboardFindResult_t >
      call_result_;
};

class UploadLeaderboardScoreWorker : public SteamCallbackAsyncWorker {
  public:
    UploadLeaderboardScoreWorker(uint64 leaderboard_handle,
                      int score,
                      bool force,
                      Nan::Callback* success_callback,
                      Nan::Callback* error_callback);
    void OnUploadLeaderboardScoreCompleted(
      LeaderboardScoreUploaded_t*, bool);
    virtual void Execute();
    virtual void HandleOKCallback();
  private:
    int score_;
    bool force_;
    bool success_;
    uint64 leaderboard_handle_;
    CCallResult< UploadLeaderboardScoreWorker, LeaderboardScoreUploaded_t >
      call_result_;
};

class DownloadLeaderboardEntriesWorker : public SteamCallbackAsyncWorker {
  public:
    DownloadLeaderboardEntriesWorker(uint64 leaderboard_handle,
                      int request_type,
                      int range_start,
                      int range_end,
                      Nan::Callback* success_callback,
                      Nan::Callback* error_callback);
    void OnDownloadLeaderboardEntriesCompleted(
      LeaderboardScoresDownloaded_t*, bool);
    virtual void Execute();
    virtual void HandleOKCallback();
  private:
    uint64 leaderboard_handle_;
    int request_type_;
    int range_start_;
    int range_end_;
    LeaderboardEntry_t* entries_;
    int entries_count_;
    CCallResult< DownloadLeaderboardEntriesWorker, LeaderboardScoresDownloaded_t >
      call_result_;
};

class AttachLeaderboardUGCWorker : public SteamCallbackAsyncWorker {
  public:
    AttachLeaderboardUGCWorker(uint64 leaderboard_handle,
                      uint64 ugc_handle,
                      Nan::Callback* success_callback,
                      Nan::Callback* error_callback);
    void OnAttachLeaderboardUGCCompleted(
      LeaderboardUGCSet_t*, bool);
    virtual void Execute();
    virtual void HandleOKCallback();
  private:
    uint64 leaderboard_handle_;
    uint64 ugc_handle_;
    CCallResult< AttachLeaderboardUGCWorker, LeaderboardUGCSet_t >
      call_result_;
    bool result_;
};

}  // namespace greenworks

#endif  // SRC_GREENWORKS_ASYNC_WORKERS_H_
