# Netdisk - Networking - Operations

## Basic Packet Structure

Once two endpoints are in the NETDISK_CONNECTED state, packets may be freely exchanged over the encrypted channel. The essential packet format is as follows:

| Field     | Offset | Length | Description                                      |
|:----------|:-------|:-------|:-------------------------------------------------|
| length    | 0      | 2      | Data packet length >> 4 (num. of 16 byte blocks) |
| operation | 2      | 2      | Operation                                        |
| payload   | 4      | var.   | Operation Payload depending on 'Operation' value |

Furthermore, the 16-bit *operation* field has structure, as follows:

| Field     | Bits      | Description             |
|:----------|:----------|-------------------------|
| is_reply  | 15        | Command - 0, Reply - 1  |
| operation | 14 - 0    | Operation Number        |

As all packets are encrypted using AES-256, every packet **MUST** have a length exactly divisible by 16. The *length* field in every packet is not the number of
bytes in the packet, rather the number of 16-byte chunks. As such, *length* can be calculated by taking the length in bytes and shifting left by 4 bits, i.e. dividing by 16.
For variable length packets which do not total an exact size divisible by 16, the total bytes represented by *length* **MUST** be larger than the packet, and the extra
bytes should be padded with random values. In each case where this is possible, the overall packet context defines the actual length of the data (generally, the device block size in bytes).

## Operations

### Command Operations

#### NETDISK_OPERATION_LIST_DEVICES

This operation requests a server endpoint to list the devices available to the client. The packet format is as follows:

| Field      | Offset | Length | Description                                      |
|:-----------|:-------|:-------|:-------------------------------------------------|
| length     | 0      | 2      | Data packet length == 2                          |
| operation  | 2      | 2      | Operation == NETDISK_OPERATION_LIST_DEVICES      |
| request_id | 4      | 8      | A 64-bit user request ID                         |
| client_id  | 12     | 8      | The 64-bit client ID                             |
| _reserved  | 20     | 12     | -Reserved-                                       |

The operation fields:

* *request_id* is unused by the server, and included in any reply pertaining to this operation
* *client_id* is the preconfigured ID of the client, and MAY be used to restrict access to some or all devices. 

#### NETDISK_OPERATION_DEVICE_LIST

This operation is sent in reply to NETDISK_OPERATION_LIST_DEVICES, detailing the devices available to the client. The packet format is as follows:

| Field       | Offset | Length | Description                                      |
|:------------|:-------|:-------|:-------------------------------------------------|
| length      | 0      | 2      | Data packet length >> 4                          |
| operation   | 2      | 2      | Operation == NETDISK_OPERATION_LIST_DEVICES      |
| request_id  | 4      | 8      | A 64-bit user request ID                         |
| client_id   | 12     | 8      | The 64-bit client ID                             |
| count       | 20     | 2      | The number of devices                            |
| devices...  | 22     | var.   | A packed list of *count* devices                 |

Each device in *devices* is defined by the following:

| Field       | Offset | Length | Description                                      |
|:------------|:-------|:-------|:-------------------------------------------------|
| device_id   | 0      | 8      | The 64-bit ID of the device                      |
| block_size  | 8      | 4      | The size in bytes of the device blocks           |
| block_total | 12     | 8      | The total number of blocks in the device         |
| name_length | 20     | 2      | The length in bytes of the name of the device    |
| name        | 22     | var.   | The name of the device                           |

**NOTE:** The packet MUST be an exact multiple of 16 bytes long, and the *length* field must be set accordingly. If the main packet plus all devices totals a size of less than a multiple of 16 bytes, random padding should be used, and ignored by the server.

#### NETDISK_OPERATION_READ

This operation is used to read from a device.

| Field      | Offset | Length | Description                                      |
|:-----------|:-------|:-------|:-------------------------------------------------|
| length     | 0      | 2      | Data packet length == 4                          |
| operation  | 2      | 2      | Operation == NETDISK_OPERATION_READ              |
| request_id | 4      | 8      | A 64-bit user request ID                         |
| client_id  | 12     | 8      | The 64-bit client ID                             |
| device_id  | 20     | 8      | The 64-bit ID of the device                      |
| block_id   | 28     | 8      | The block ID                                     |
| _reserved  | 36     | 12     | -Reserved-                                       |

The operation fields:

* *request_id* is unused by the server, and included in any reply pertaining to this operation
* *client_id* is the preconfigured ID of the client, and MAY be used to restrict access to some or all devices. 
* *device_id* is the ID of the device
* *block_id* is the id or offset of the block

#### NETDISK_OPERATION_READ_OK

This operation is sent in reply to NETDISK_OPERATION_READ when the read has succeded:

| Field      | Offset | Length | Description                                      |
|:-----------|:-------|:-------|:-------------------------------------------------|
| length     | 0      | 2      | Data packet length >> 4                          |
| operation  | 2      | 2      | Operation == NETDISK_OPERATION_READ_OK           |
| request_id | 4      | 8      | A 64-bit user request ID                         |
| client_id  | 12     | 8      | The 64-bit client ID                             |
| device_id  | 20     | 8      | The 64-bit ID of the device                      |
| block_id   | 28     | 8      | The block ID                                     |
| data       | 36     | var.   | The data for the block                           |

* *request_id* is the value of the *request_id* in the originating NETDISK_OPERATION_READ.
* *client_id* is the preconfigured ID of the client, and MAY be used to restrict access to some or all devices. 
* *device_id* is the ID of the device
* *block_id* is the id or offset of the block
* *data* is the data read from the device, which should be the exact length of the block size specified in NETDISK_OPERATION_DEVICE_LIST for this device_id.

**NOTE:** The packet MUST be an exact multiple of 16 bytes long, and the *length* field must be set accordingly. If the main packet plus the data totals a size of less than a multiple of 16 bytes, random padding should be used, and ignored by the client.

#### NETDISK_OPERATION_WRITE

This operation is used to write to a device.

| Field      | Offset | Length | Description                                      |
|:-----------|:-------|:-------|:-------------------------------------------------|
| length     | 0      | 2      | Data packet length >> 4                          |
| operation  | 2      | 2      | Operation == NETDISK_OPERATION_WRITE             |
| request_id | 4      | 8      | A 64-bit user request ID                         |
| client_id  | 12     | 8      | The 64-bit client ID                             |
| device_id  | 20     | 8      | The 64-bit ID of the device                      |
| block_id   | 28     | 8      | The block ID                                     |
| data       | 36     | var.   | The data for the block                           |

* *request_id* is unused by the server, and included in any reply pertaining to this operation
* *client_id* is the preconfigured ID of the client, and MAY be used to restrict access to some or all devices. 
* *device_id* is the ID of the device
* *block_id* is the id or offset of the block
* *data* is the data to write, which should be the exact length of the block size specified in NETDISK_OPERATION_DEVICE_LIST for this device_id.

**NOTE:** The packet MUST be an exact multiple of 16 bytes long, and the *length* field must be set accordingly. If the main packet plus the data totals a size of less than a multiple of 16 bytes, random padding should be used, and ignored by the server.

#### NETDISK_OPERATION_WRITE_OK

This operation is sent in reply to NETDISK_OPERATION_WRITE when the write has succeded:

| Field      | Offset | Length | Description                                      |
|:-----------|:-------|:-------|:-------------------------------------------------|
| length     | 0      | 2      | Data packet length == 4                          |
| operation  | 2      | 2      | Operation == NETDISK_OPERATION_WRITE_OK          |
| request_id | 4      | 8      | A 64-bit user request ID                         |
| client_id  | 12     | 8      | The 64-bit client ID                             |
| device_id  | 20     | 8      | The 64-bit ID of the device                      |
| block_id   | 28     | 8      | The block ID                                     |
| _reserved  | 36     | 12     | -Reserved-                                       |

* *request_id* is the value of the *request_id* in the originating NETDISK_OPERATION_WRITE.
* *client_id* is the preconfigured ID of the client, and MAY be used to restrict access to some or all devices. 
* *device_id* is the ID of the device
* *block_id* is the id or offset of the block

