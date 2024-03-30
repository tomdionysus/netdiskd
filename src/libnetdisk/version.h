//
// netdisk
//
// Copyright (C) 2024 Tom Cully
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
//
#pragma once

#include <cstdint>
#include <sstream>
#include <string>

namespace netdisk {

#define NETDISK_MAJOR_VERSION 0
#define NETDISK_MINOR_VERSION 1
#define NETDISK_PATCH_VERSION 0

typedef struct netdisk_version {
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
} netdisk_version_t;

class Version {
 public:
  static netdisk_version_t getVersion();
  static std::string getVersionString();
  static std::string getVersionString(netdisk_version_t version);
};

}  // namespace netdisk