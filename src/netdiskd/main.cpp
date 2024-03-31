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
#include <boost/program_options.hpp>
#include <iostream>
#include <optional>
#include <string>

#include "logger_scoped.h"
#include "logger_stdio.h"
#include "url.h"
#include "version.h"

using namespace std;
using namespace netdisk;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  LoggerStdIO mainLogger(LogLevel::DEBUG);

  LoggerScoped serverLogger("server", &mainLogger);

  mainLogger.info("-----------------------------------");
  mainLogger.info("netdiskd v" + Version::getVersionString());
  mainLogger.info("-----------------------------------");

  try {
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "Help")("db_url", po::value<std::string>(), "A Database URL (mysql://user:password@host/database, etc)");

    po::variables_map vm;

    // Create a command line parser that applies the 'desc' options
    po::command_line_parser parser(argc, argv);
    parser.options(desc).allow_unregistered();  // Allow unknown options
    po::parsed_options parsed_options = parser.run();

    // Store and notify parsed options
    po::store(parsed_options, vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 1;
    }

    // Process options
    if (vm.count("db_url")) {
      std::string db_url = vm["db_url"].as<std::string>();
      URL dbUrl(db_url);

      if (dbUrl.is_valid()) {
        serverLogger.info("scheme = " + dbUrl.scheme);
        if (dbUrl.user) {
          serverLogger.info("user = " + dbUrl.user.value());
        }
        if (dbUrl.password) {
          serverLogger.info("password = " + dbUrl.password.value());
        }
        serverLogger.info("host = " + dbUrl.host);
        if (dbUrl.port) {
          serverLogger.info("port = " + std::to_string(dbUrl.port.value()));
        }
        serverLogger.info("path = " + dbUrl.path);
        serverLogger.info("query = " + dbUrl.query);
        serverLogger.info("fragment = " + dbUrl.fragment);
      } else {
        serverLogger.warn("db_url is not a valid URL");
      }
    }

    // Collect all unrecognized options from the parsed information
    std::vector<std::string> unrecognized_opts = po::collect_unrecognized(parsed_options.options, po::include_positional);

    // Handle or display unrecognized options
    if (!unrecognized_opts.empty()) {
      mainLogger.error("Unrecognized options:");
      for (const auto& opt : unrecognized_opts) {
        mainLogger.error(opt);
      }
      return 1;
    }

  } catch (const po::error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  } catch (const std::exception& e) {
    std::cerr << "Unhandled Exception reached the top of main: " << e.what() << ", application will now exit" << std::endl;
    return 99;
  }
}
