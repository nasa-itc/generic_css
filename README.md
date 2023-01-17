# Generic_css - NOS3 Component
This repository contains the NOS3 Generic_css Component.
This includes flight software (FSW), ground software (GSW), simulation, and support directories.

## Overview
This generic_css component is an I2C device that accepts multiple commands including requests for telemetry. The available FSW is for use in the core Flight System (cFS) while the GSw supports COSMOS. A NOS3 simulation is available which includes both generic_css and 42 data providers.

# Device Communications
The protocol, commands, and responses of the component are captured below.

## Protocol
The protocol in use is Inter-Integrated Circuit (I2C). All communications with the device require / contain a header of 0xDEAD and a footer of 0xBEEF.

I2C is a synchronous serial communication protocol that supports slaves being controlled by multiple masters, as well as masters controlling multiple slaves. This is a two wire communication protocol with one wire used for data and the other used for a clock signal. For our purposes we will treat the generic_css as a single slave controlled by a master software bus.

I2C transfers data using messages, and these message are broken up into frames. The format of a _typical_ I2c message is as follows:

[start condition][7 or 8 bits address frame][r/w bit][ack/nack bit][8 bits data frame n][ack/nack bit][8 bits data frame n+1][ack/nack bit][8 bits data frame n+2][ack/nack bit]...[stop condition]

Data sent in data frames are always MSB first.

Our format is described below, as well as in the "Commands" section:

* Start Condition: uint16 0xDEAD
* Address + R/W bit: uint8, CSS Address is 7 bits 0b00110000 or 0x30, LSB is R/W bit (0 for write, 1 for read)
  * This means a write to the CSS would be 0x30, and a read from the CSS would be 0x31.
* Ack/Nack bit: uint8 (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack).)
* Data Frame (Command Identifier): uint8 (0x11 for Get Housekeeping, 0x12 for Get Generic_eps, 0x13 for Set Configuration)
* Ack/Nack bit: uint8 (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack).)
* Subsequent Data Frames based on command received ...
* Subsequent Ack/Nack bit responses on command received ...
* Stop Condition: 0xBEEF

Once the generic_css has received the start condition 0xDEAD and verified the message contains its address (0x30) in the address frame it will respond with an ACK bit to the master and save the R/W bit from the message. Other I2C devices connected to this master will be expected to respond with a NACK bit as they are not being addressed. 

This will open up a connection between the master and the slave, and operation will be based on the R/W bit signifying if the master is requesting a read or write operation.

When the generic_css is put into write mode, the master will send data frames one at a time while waiting for an ACK/NACK bit from the generic_css in response. If an ACK bit is received, then another data frame may be sent from the master. If a NACK bit is received, then the master knows the data frame was not successfully received and can either resend the data frame or abort operation with the stop condition 0xBEEF.

When the generic_css is put into read mode, the master will receive data frames one at a time from the generic_css and send its own ACK/NACK bit in response. The generic_css will treat an ACK bit as a signal to move on and send the next data frame, and a NACK bit as a signal to resend the current data frame. This operation will end when the generic_css receives the stop condition 0xBEEF; typically after the last data frame has been read on the generic_css, sent to the master, and the master has responded with an ACK bit.

## Commands
The command formats are as follows:
* uint16, 0xDEAD
* uint8, Address + R/W bit
 * CSS Address is 7 most significant bits 0b00110000 or 0x30
 * R/W bit is LSB (0 for write, 1 for read)
  * This means a write to the CSS would be 0x30, and a read from the CSS would be 0x31.
 * uint8 ACK/NACK bit is expected in response from the generic_css to this portion of the command (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack)).
* uint8, Command Identifier
  * (0x11) Get Housekeeping
  * (0x12) Get Generic_eps
  * (0x13) Set Configuration
  * uint8 ACK/NACK bit is expected in response from the generic_css to this portion of the command (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack)).
* uint8, Command Payload
 * Unused for all but the Set Configuration command.
 * 0x00 NOOP
 * 0x01 RESET COUNTERS
 * 0x02 TURN ON
 * 0x03 TURN OFF
 * uint8 ACK/NACK bit is expected in response from the generic_css to this portion of the command (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack)).
* uint16, 0xBEEF

## Response
Response formats are as follows:
* Housekeeping
 * uint8, Command Counter
  * uint8 ACK/NACK bit is expected in response from the master to this portion of the command (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack)).
 * uint8, CommandErrorCount
  * uint8 ACK/NACK bit is expected in response from the master to this portion of the command (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack)).
 * uint32, Status
  * Self reported status of the component where zero is completely healthy and each bit represents different errors
  * No means to clear / set status except for a power cycle to the device
  * uint8 ACK/NACK bit is expected in response from the master to this portion of the command (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack)).
 * 32 uint32, Values
  * 4*8 ADC channels with float (32 bit size) values in each = 32 uint32s (1024 bytes)
  * uint8 ACK/NACK bit is expected in response from the master to this portion of the command (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack)).
* Generic_eps
 * uint16, Voltage
  * uint8 ACK/NACK bit is expected in response from the master to this portion of the command (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack)).
 * uint8, Error (1 = an error occurred on this channel)
  * uint8 ACK/NACK bit is expected in response from the master to this portion of the command (0x00 for acknowledged (ack), 0x01 for not acknowledged (nack)).


# Configuration
The various configuration parameters available for each portion of the component are captured below.

## FSW
Refer to the file [fsw/platform_inc/generic_eps_platform_cfg.h](fsw/platform_inc/generic_eps_platform_cfg.h) for the default
configuration settings, as well as a summary on overriding parameters in mission-specific repositories.

## Simulation

## 42


# Documentation
If this generic_css application had an ICD and/or test procedure, they would be linked here.

## Releases
We use [SemVer](http://semver.org/) for versioning. For the versions available, see the tags on this repository.
