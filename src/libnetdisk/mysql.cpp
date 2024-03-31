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
#include <mysql/mysql.h>

#include <iostream>
#include <string>

class HostReader {
 public:
  HostReader(const std::string& server, const std::string& user, const std::string& password, const std::string& database)
      : server(server), user(user), password(password), database(database) {}

  ~HostReader() {
    if (conn) {
      mysql_close(conn);
    }
  }

  bool connect() {
    conn = mysql_init(nullptr);
    if (!conn) {
      std::cerr << "MySQL initialization failed." << std::endl;
      return false;
    }

    if (!mysql_real_connect(conn, server.c_str(), user.c_str(), password.c_str(), database.c_str(), 0, nullptr, 0)) {
      std::cerr << "Connection failed: " << mysql_error(conn) << std::endl;
      return false;
    }

    return true;
  }

  void readHostData() {
    if (!connect()) {
      return;
    }

    if (mysql_query(conn, "SELECT * FROM host")) {
      std::cerr << "Query failed: " << mysql_error(conn) << std::endl;
      return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
      std::cerr << "Failed to store result set: " << mysql_error(conn) << std::endl;
      return;
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
  }

 private:
  MYSQL* conn = nullptr;
  std::string server;
  std::string user;
  std::string password;
  std::string database;
};
