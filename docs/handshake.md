# Netdisk - Networking - Handshake

All parts of the system communicate using a simple binary, AES encrypted protocol over TCP. A client or server endpoint can be in one of a number of states:

## NETDISK_STATE_DISCONNECTED

The server or client is currently not connected via TCP. 

* Both endpoints should reset and clear the AES context and stored IV pertaining to the other endpoint.
* A server should wait for connections from clients.
* A client should make a configurable number of attempts to reconnect, using a configurable random backoff timeout.

When a TCP connection is connected or accepted, the endpoint moves to the NETDISK_INIT state.

## NETDISK_INIT_A

The two INIT states allow the endpoints to generate and exchange AES Initialisation Vectors (IVs). 

First, both endpoints generate and transmit a secure random 256bit IV (A) in cleartext:

| Field     | Offset | Length | Description                       |
|:----------|:-------|--------|-----------------------------------|
| iv_a      | 0      | 32     | AES CBC Initialisation Vector     |

On receipt, each endpoint moves to the NETDISK_INIT_B state.

## NETDISK_INIT_B

Secondly, both endpoints generate _another_ secure random 256bit IV (B), and XOR it with the received IV (A) from the other side. The new IV is then transmitted using the same format: 

| Field     | Offset | Length | Description                       |
|:----------|:-------|--------|-----------------------------------|
| iv_b      | 0      | 32     | AES CBC Initialisation Vector     |

On receipt, Each endpoint sets up an encryption context based on the pre-shared key, and the received IV (B). All traffic past this state is encrypted, updating the
IV at the end of every transfer to produce a continuous CBC stream:

* Each endpoint must encrypt and decrypt using two different CBC contexts, one for RX and one for TX.
* Each data packet length must be an exact multiple of 16 bytes (the AES blocksize) long.  

The endpoint moves to the NETDISK_HANDSHAKE state.

## NETDISK_HANDSHAKE

Both endpoints transmit a handshake packet, consisting of the following:

| Field     | Offset | Length | Description                       |
|:----------|:-------|--------|-----------------------------------|
| length    | 0      | 2      | Data packet length == 1           |
| operation | 2      | 2      | Operation == NETDISK_HANDSHAKE    |
| magic     | 4      | 7      | 'NETDISK'                         |
| major     | 11     | 1      | Major Version                     |
| minor     | 12     | 1      | Minor Version                     |
| patch     | 13     | 1      | Patch Version                     |
| _reserved | 14     | 2      | -Reserved-                        |

On receipt, each endpoint should check:

* The *length* is equal to 1
* The *operation* is equal to NETDISK_HANDSHAKE
* The *magic* 7 byte number is equal to 'NETDISK'

If any of these conditions is not met, the endpoint should assume that the pre shared AES key is incorrect, and immediately reset the TCP connection and return to the NETDISK_STATE_DISCONNECTED state.

Additionally, each endpoint MAY compare the major, minor and patch version numbers to ensure compatibility; if the endpoints are not compatible, the objecting endpoint should transmit NETDISK_OPERATION_INCOMPATIBLE_VERSION and then disconnect, returning to NETDISK_STATE_DISCONNECTED:

| Field     | Offset | Length | Description                                         |
|:----------|:-------|--------|-----------------------------------------------------|
| length    | 0      | 2      | Data packet length == 1                             |
| operation | 2      | 2      | Operation == NETDISK_OPERATION_INCOMPATIBLE_VERSION |
| payload   | 4      | 12     | -Reserved-                                          |

If all is well, each endpoint moves to the NETDISK_CONNECTED state.

## NETDISK_CONNECTED

Each endpoint may now transmit packets in the following format:

| Field     | Offset | Length | Description                                      |
|:----------|:-------|--------|--------------------------------------------------|
| length    | 0      | 2      | Data packet length >> 4 (num. of 16 byte blocks) |
| operation | 2      | 2      | Operation == NETDISK_HANDSHAKE                   |
| payload   | 4      | var.   | Operation Payload depending on 'Operation' value |

The length of these packets is variable depending on the *operation* value - but must always be a multiple of 16. The *length* is the count of 16 byte chunks, i.e. the byte length divided by 16, or (len >> 4). The length represents the whole packet, including the *length* and *operation* fields, and as such has a minimum value of 1. 

If the TCP connection is reset, the state moves back to NETDISK_STATE_DISCONNECTED. 
