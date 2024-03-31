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

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <memory>
#include <thread>
#include <unordered_map>

#include "logger.h"

namespace netdisk {

class TcpServer {
 public:
  TcpServer(Logger& logger, short port);

  void start();
  void stop();

 private:
  void _handle_accept(const boost::system::error_code& error, std::shared_ptr<boost::asio::ip::tcp::socket> new_connection);
  void start_accept();

  boost::asio::io_context _io_context;
  boost::asio::ip::tcp::acceptor _acceptor;
  std::unordered_map<int, std::shared_ptr<boost::asio::ip::tcp::socket>> connections_;
  int next_connection_id_ = 0;
  uint16_t _port;
  std::thread _thread;

  Logger& _logger;
};

}  // namespace netdisk
