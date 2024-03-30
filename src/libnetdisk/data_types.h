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

#include <iostream>
#include <string>
#include <vector>

namespace netdisk {

typedef uint8_t[AES_KEY_SIZE] aes_key_t;

typedef struct host {
  uint64_t id;
  std::string name;
  aes_key_t key;
  std::vector<uint64_t> devices;
} host_t;

typedef struct device {
  uint64_t id;
  std::string name;
  std::string filename;
  uint32_t block_size;
  uint64_t block_total;
  bool read_only;
} host_t;

}  // namespace netdisk