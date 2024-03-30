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
#include "logger.h"
#include "logger_scoped.h"

namespace netdisk {

LoggerScoped::LoggerScoped(std::string scope, Logger *logger) {
	_scope = scope;
	_logger = logger;
}

void LoggerScoped::debug(std::string str) {
	_logger->debug("("+_scope+") "+str);
}

void LoggerScoped::info(std::string str) {
	_logger->info("("+_scope+") "+str);
}

void LoggerScoped::warn(std::string str) {
	_logger->warn("("+_scope+") "+str);
}

void LoggerScoped::error(std::string str) {
	_logger->error("("+_scope+") "+str);
}

}