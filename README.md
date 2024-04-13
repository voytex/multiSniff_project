# multiSniff software design

## General

- _TBD_: TI: Packet queue source code from TI Packet Sniffer 2
- _TBD_: _author_: Ported W5500 Arduino driver for TI-RTOS

### Dashboard

HTML code of dashboard shall be saved on device memory in special section. A Python script shall be implemented so that every time a build of software is involved, HTML code is converted to binary file and loaded onto the device as well.
