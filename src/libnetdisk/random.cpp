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
#include "random.h"

#include <iostream>

namespace netdisk {

Random::Random() {
  randFile.open("/dev/random", std::ios::in | std::ios::binary);
  if (!randFile) {
    std::cerr << "Cannot open /dev/random" << std::endl;
    throw std::runtime_error("Failed to open /dev/random");
  }
}

Random::~Random() {
  if (randFile.is_open()) {
    randFile.close();
  }
}

int8_t Random::getInt8() {
  int8_t value;
  getRandom(&value, sizeof(value));
  return value;
}

int16_t Random::getInt16() {
  int16_t value;
  getRandom(&value, sizeof(value));
  return value;
}

int32_t Random::getInt32() {
  int32_t value;
  getRandom(&value, sizeof(value));
  return value;
}

int64_t Random::getInt64() {
  int64_t value;
  getRandom(&value, sizeof(value));
  return value;
}

void Random::getRandom(void* ptr, size_t size) {
  randFile.read(static_cast<char*>(ptr), size);
  if (!randFile) {
    std::cerr << "Error reading from /dev/random" << std::endl;
    throw std::runtime_error("Failed to read from /dev/random");
  }
}

}  // namespace netdisk