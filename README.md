# multiSniff software design

## General

Device SW shall be built on TI-RTOS using tasking and semaphore functionality. Software shall be dependent upon several libraries and modules, namely:

- _TBD_: TI: Packet queue source code from TI Packet Sniffer 2
- _TBD_: _author_: Ported W5500 Arduino driver for TI-RTOS

## Software design

### Radio API

Main purpose of **Radio API** is to improve readability and code clarity since RF Core of **CC2652RB** is controlled by providing it with various **commands** (`structs`).

1. `Radio_openRadioCore`
   - Initializes Radio Core of MCU according to parameters.
   - **Parameters**:
     - `pParams[in]` pointer to parameters structure
     - `pObj[in]` pointer to object storing internal configuration
     - `pMode{in]` pointer to global RF mode (e.g. BLE or IEEE)
     - `pSetup[in]` pointer to radio setup structure
     - `pHandle[out]` pointer to RF_Handle to control Radio Core
