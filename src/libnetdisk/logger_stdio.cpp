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
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

#include "logger_stdio.h"

namespace netdisk {

LoggerStdIO::LoggerStdIO(LogLevel log_level) {
	_log_level = log_level;
}

void LoggerStdIO::debug(std::string log) {
    if(_log_level>LogLevel::DEBUG) return;
	std::cout << _getDateTime() << " [DEBUG] " << log << std::endl;
}

void LoggerStdIO::info(std::string log) {
    if(_log_level>LogLevel::INFO) return;
	std::cout << _getDateTime() << " [INFO ] " << log << std::endl;
}

void LoggerStdIO::warn(std::string log) {
    if(_log_level>LogLevel::WARN) return;
	std::cout << _getDateTime() << " [WARN ] " << log << std::endl;
}

void LoggerStdIO::error(std::string log) {
	std::cout << _getDateTime() << " [ERROR] " << log << std::endl;
}

std::string LoggerStdIO::_getDateTime() {
    // Get current time as time_point
    auto now = std::chrono::system_clock::now();
    // Convert to a time_t
    auto now_c = std::chrono::system_clock::to_time_t(now);
    // Convert to tm struct in UTC
    std::tm now_tm = *std::gmtime(&now_c);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

}