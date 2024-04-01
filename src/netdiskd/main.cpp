//
// netdiskd
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
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <optional>
#include <string>

#include "config.h"
#include "device_db_mysql.h"
#include "logger_scoped.h"
#include "logger_stdio.h"
#include "tcp_server.h"
#include "url.h"
#include "version.h"
#include "device_db_mysql.h"

using namespace std;
using namespace netdisk;

int main(int argc, char* argv[]) {
  LoggerStdIO mainLogger(LogLevel::DEBUG);

  LoggerScoped configLogger("config", &mainLogger);
  LoggerScoped serverLogger("server", &mainLogger);
  LoggerScoped devicedbLogger("devicedb", &mainLogger);

  mainLogger.info("-----------------------------------");
  mainLogger.info("netdiskd v" + Version::getVersionString());
  mainLogger.info("-----------------------------------");

  Config config(configLogger, argc, argv);

  if (!config.is_valid()) {
    mainLogger.error("Invalid Configuration");
    return 99;
  }

  DeviceDBMySQL ddMysql(devicedbLogger, config.dbUrl);
  DeviceDB& deviceDb = ddMysql;

  // Create the device database
  deviceDb.initialise();

  // Create the TcpServer instance with the logger and start it on the specified port
  TcpServer server(serverLogger, 26547);

  // Wait for SIGINT
  boost::asio::io_context signal_wait_context;
  boost::asio::signal_set signals(signal_wait_context, SIGINT);

  signals.async_wait([&](const boost::system::error_code& error, int signal_number) {
    if (!error) {
      mainLogger.debug("SIGINT received");
      server.stop();
      deviceDb.close();
    }
  });
  // Start TCP server
  server.start();

  // Do the SIGINT Wait
  signal_wait_context.run();
}
