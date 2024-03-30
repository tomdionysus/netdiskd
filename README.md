# netdisk

A SAN-like remote block device server/client for Linux in C/C++.

## Goals

* A server (netdiskd) capable of serving multiple devices to multiple hosts.
* 256 bit AES CBC encryption on all transfers.
* A Linux kernel module (netdisk.ko) capable of connecting to a netdiskd instance and serving multiple devices to its host.
* Read/Write and read only devices.
* Automatic reconnect and retry when a connection is broken.
* An abstracted host/device store for netdiskd that can be:
	* Flat file configuration
	* MySQL
	* PostgreSQL
	* Redis

## Documentation

* [Networking](docs/networking.md) - The Netdisk binary protocol and details of encryption.