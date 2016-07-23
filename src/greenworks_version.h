// Copyright (c) 2015 Greenheart Games Pty. Ltd. All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SRC_STEAM_VERSION_H_
#define SRC_STEAM_VERSION_H_

#define GREENWORKS_MAJOR_VERSION 0
#define GREENWORKS_MINOR_VERSION 5
#define GREENWORKS_PATCH_VERSION 3

#ifndef GREENWORKS_STRINGIFY
#define GREENWORKS_STRINGIFY(n) GREENWORKS_STRINGIFY_HELPER(n)
#define GREENWORKS_STRINGIFY_HELPER(n) #n
#endif

#define GREENWORKS_VERSION GREENWORKS_STRINGIFY(GREENWORKS_MAJOR_VERSION) "." \
                           GREENWORKS_STRINGIFY(GREENWORKS_MINOR_VERSION) "." \
                           GREENWORKS_STRINGIFY(GREENWORKS_PATCH_VERSION)

#endif  // SRC_STEAM_VERSION_H_