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
#include <string>
#include <sstream>

#include "version.h"

namespace netdisk {

netdisk_version_t Version::getVersion() {
	netdisk_version_t thisVersion;
	thisVersion.major = NETDISK_MAJOR_VERSION;
	thisVersion.minor = NETDISK_MINOR_VERSION;
	thisVersion.patch = NETDISK_PATCH_VERSION;

	return thisVersion;
}

std::string Version::getVersionString() {
	netdisk_version_t thisVersion = Version::getVersion();
	return Version::getVersionString(thisVersion);
}

std::string Version::getVersionString(netdisk_version_t version) {
	std::ostringstream versionString;
	versionString << (uint32_t)version.major << "." << (uint32_t)version.minor << "." << (uint32_t)version.patch;
    return versionString.str();
}

}