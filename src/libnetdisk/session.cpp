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

#include <functional>
#include <iostream>

#include "logger_scoped.h"

namespace netdisk {

Session::Session(Logger *logger, std::shared_ptr<boost::asio::ip::tcp::socket> connection) {
  _running = false;
  _thread = nullptr;
  _connection = connection;
  auto endpoint = _connection->remote_endpoint();
  _logger = new LoggerScoped(endpoint.address().to_string() + ":" + std::to_string(endpoint.port()), logger);

  _thread = new std::thread(std::bind(&Session::_execute, this, 0));
}

Session::~Session() {
  stop();
  delete _logger;
}

void Session::stop() {
  if (_running) {
    // Signal Thread
    _running = false;

    // Wait for quit
    if (_thread->joinable()) {
      _thread->join();
    }
  }
}

void Session::_execute(int id) {
  _running = true;

  _logger->info("Connected");

  boost::system::error_code ec;

  while (_running) {
    ec = _read_with_timeout();

    if (ec) {
      if (ec == boost::asio::error::operation_aborted) {
        _running = false;
      } else {
        _running = false;
      }
    } else {
      _logger->info("Read Bytes");
    }
  }

  _logger->info("Closed");

  _connection->close();

  _thread = nullptr;
}

boost::system::error_code Session::_read_with_timeout() {
  boost::asio::steady_timer timer(_connection->get_executor(), boost::asio::chrono::seconds(10));
  boost::system::error_code ec;

  timer.async_wait([this](const boost::system::error_code &) {
    _connection->cancel();  // This will cancel the blocking read operation
  });

  // Buffer to store data
  char data[1024];
  size_t length = _connection->read_some(boost::asio::buffer(data), ec);

  // Cancel the timer if the operation completes before the timeout
  timer.cancel();

  return ec;
}

}  // namespace netdisk