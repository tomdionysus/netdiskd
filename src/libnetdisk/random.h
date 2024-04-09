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
#include <fstream>

namespace netdisk {

class Random {
 public:
  Random();
  ~Random();

  int8_t getInt8();
  int16_t getInt16();
  int32_t getInt32();
  int64_t getInt64();
  void getRandom(void* ptr, size_t size);

 private:
  std::ifstream randFile;
};

}  // namespace netdisk
