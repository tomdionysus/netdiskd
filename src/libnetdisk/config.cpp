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

#include "config.h"

namespace po = boost::program_options;

namespace netdisk {

Config::Config(Logger& logger) : _logger(logger) {}

Config::Config(Logger& logger, int argc, char* argv[]) : _logger(logger) { parse_cmd_line(argc, argv); }

void Config::parse_cmd_line(int argc, char* argv[]) {
  _valid = false;

  _logger.debug("Parsing command line...");

  try {
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "Help")("db_mode", "Device Database Mode (mysql)")("db_url", po::value<std::string>(),
                                                                                    "A Database URL (mysql://user:password@host/database, etc)");

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
      return;
    }

    // Process options
    if (vm.count("db_url")) {
      std::string db_url = vm["db_url"].as<std::string>();
      URL dbUrl(db_url);

      if (dbUrl.is_valid()) {
        _logger.info("scheme = " + dbUrl.scheme);
        if (dbUrl.user) {
          _logger.info("user = " + dbUrl.user.value());
        }
        if (dbUrl.password) {
          _logger.info("password = " + dbUrl.password.value());
        }
        _logger.info("host = " + dbUrl.host);
        if (dbUrl.port) {
          _logger.info("port = " + std::to_string(dbUrl.port.value()));
        }
        _logger.info("path = " + dbUrl.path);
        _logger.info("query = " + dbUrl.query);
        _logger.info("fragment = " + dbUrl.fragment);
        _logger.info("URL = " + dbUrl);
      } else {
        _logger.warn("db_url is not a valid URL");
      }
    }

    // Collect all unrecognized options from the parsed information
    std::vector<std::string> unrecognized_opts = po::collect_unrecognized(parsed_options.options, po::include_positional);

    // Handle or display unrecognized options
    if (!unrecognized_opts.empty()) {
      _logger.error("Unrecognized options:");
      for (const auto& opt : unrecognized_opts) {
        _logger.error(opt);
      }
      return;
    }

    _valid = true;

  } catch (const po::error& e) {
    _logger.error("Error: " + std::string(e.what()));
    return;
  }
}

bool Config::is_valid() { return _valid; }

}  // namespace netdisk