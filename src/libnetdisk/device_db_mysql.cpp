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
#include "device_db_mysql.h"

#include <mysql/mysql.h>

#include <iostream>
#include <string>

#include "url.h"

namespace netdisk {

DeviceDBMySQL::DeviceDBMySQL(Logger& logger, URL& dbUrl) : _logger(logger), _dbUrl(dbUrl) {}

bool DeviceDBMySQL::initialise() {
  conn = mysql_init(nullptr);
  if (!conn) {
    _logger.error("MySQL initialization failed.");
    return false;
  }

  const char* user;
  const char* password;

  if(_dbUrl.user) user = _dbUrl.user.value().c_str();
  if(_dbUrl.password) password = _dbUrl.password.value().c_str();

  _logger.debug("connecting...");

  if (!mysql_real_connect(conn, _dbUrl.host.c_str(), user, password, _dbUrl.path.substr(1).c_str(), 0, nullptr, 0)) {
    _logger.error("Connection failed: " + std::string(mysql_error(conn)));
    return false;
  }

  _logger.info("Connected");

  return true;
}

std::shared_ptr<Host> DeviceDBMySQL::get_host(uint64_t host_id) {
  if (mysql_query(conn, "SELECT * FROM host")) {
    _logger.error("Query failed: " + std::string(mysql_error(conn)));
    return NULL;
  }

  MYSQL_RES* result = mysql_store_result(conn);
  if (!result) {
    _logger.error("Bad Result Set: " + std::string(mysql_error(conn)));
    return NULL;
  }

  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;

  while ((row = mysql_fetch_row(result))) {
    for (int i = 0; i < num_fields; i++) {
      std::cout << (row[i] ? row[i] : "NULL") << " ";
    }
    std::cout << std::endl;
  }

  mysql_free_result(result);
  return NULL;
}

std::shared_ptr<Device> DeviceDBMySQL::get_device(uint64_t device_id) {
    return NULL;
}

std::vector<std::shared_ptr<Device>> DeviceDBMySQL::get_host_devices(uint64_t host_id) {
    return std::vector<std::shared_ptr<Device>>();
}

bool DeviceDBMySQL::close() {
  _logger.debug("closing...");
  if (conn) {
    mysql_close(conn);
    conn = nullptr;
  }
  _logger.info("Closed");
}

}  // namespace netdisk
