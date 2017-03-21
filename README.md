uTelemetry
==========

## Protocol
### Structure
N: Packet size in byte
| byte No.      | Data hex      | Description  |
| ------------- |--------------| -------------|
| 0 | ff | Start of uTelemetry packet |
| 1 | cc | Start of uTelemetry packet |
| 2 | | total packet length LSB |
| 3 | | total packet length MSB |
| 4 | | Package type |
| n | | Optional: Data |
| N-2 | cc | End of uTelemetry packet |
| N-1 | ff | End of uTelemetry packet |

### Package types
| Package type code hex  | Description  | Has Data |
| ------------- |--------------| -------------|
| 00 | Start of uTelemetry | Yes |
| 01 | End of uTelemetry | No |
| 02 | Measurement Declaration | Yes |
| 03 | Measurement Data | Yes |

### Start packet
| byte No.      | Data hex      | Description  |
| ------------- |--------------| -------------|
| 4 | 00 | Start of uTelemetry  |
| 5 | xx | Major protocol version | 
| 6 | xx | Minor protocol version|

### End packet
| byte No.      | Data hex      | Description  |
| ------------- |--------------| -------------|
| 4 | 01 | End of uTelemetry  |

### Measurement Declaration
| byte No.      | Data hex      | Description  |
| ------------- |--------------| -------------|
| 4 | 02 | Measurement Declaration  |
| 5 | xx | Unique Measurement ID LSB |
| 6 | xx | Unique Measurement ID MSB | 
| 7 | xx | X Data type | 
| 8 | xx | Y Data type | 
| 9 | xx | Measurement Name string NULL terminated | 
|   | xx | Measurement Unit string NULL terminated | 

#### Data types
Data is always transmitted LSB FIRST (little-endian)
| Data hex | Data size bytes | Data type |
|----------|-----------------|-----------|
| 00 | 1 | uint8_t |
| 01 | 1 | int8_t |
| 02 | 2 | uint16_t |
| 03 | 2 | int16_t |
| 04 | 4 | uint32_t |
| 05 | 4 | int32_t |
| 06 | 8 | uint64_t |
| 07 | 8 | int64_t |
| 08 | 4 | float |
| 09 | 8 | double |

### Measurement Data
Number of X and Y data must always be same length
| byte No.      | Data hex      | Description  |
| ------------- |--------------| -------------|
| 4 | 03 | Measurement Data  |
| 5 | xx | Unique Measurement ID LSB |
| 6 | xx | Unique Measurement ID MSB | 
| 7 | xx | Number of data points LSB | 
| 8 | xx | Number of data points MSB | 
| 9 | xx | X Data | 
|   | xx | Y Data | 

