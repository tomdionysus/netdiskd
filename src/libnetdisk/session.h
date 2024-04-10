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

#include <atomic>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <cstdint>
#include <fstream>
#include <thread>

#include "logger.h"

namespace netdisk {

class Session {
 public:
  Session(Logger *logger, std::shared_ptr<boost::asio::ip::tcp::socket> connection);
  ~Session();

  void stop();

 private:
  Logger *_logger;

  std::thread *_thread;
  std::atomic<bool> _running;

  std::shared_ptr<boost::asio::ip::tcp::socket> _connection;

  void _execute(int id);
  boost::system::error_code _read_with_timeout();
};

}  // namespace netdisk
