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
#include "session.h"

#include <iostream>
#include <functional>

namespace netdisk {

Session::Session() {
  _running = false;
  _thread = nullptr;
}

Session::~Session() {

}

void Session::initialise() {

}

void Session::start() {
  if(!_running) {
    _thread = new std::thread(std::bind(&Session::_execute, this, 0));
  }
}

void Session::stop() {
  if(_running) {
    // Signal Thread
    _running = false;
    
    // Wait for quit
    if(_thread->joinable()) {
      _thread->join();
    }
  }
}

void Session::_execute(int id) {
  _running = true;

  while(_running) {

  }

  _thread = nullptr;
}


}  // namespace netdisk