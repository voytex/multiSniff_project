# multiSniff software design

## General

Device SW shall be built on TI-RTOS using tasking and semaphore functionality. Software shall be dependent upon several libraries and modules, namely:

- _TBD_: TI: Packet queue source code from TI Packet Sniffer 2
- _TBD_: _author_: Ported W5500 Arduino driver for TI-RTOS

## Software design

### Radio API

Main purpose of **Radio API** is to improve readability and code clarity since RF Core of **CC2652RB** is controlled by providing it with various **commands** (`structs`).

1. Enumeration for RF protocol

   - For convenient between (among) multiple RF protocols.
   - Use whenever possible

   ```C
   typedef enum RF_Protocol {
         BluetoothLowEnergy = 0,
         IEEE_802_15_4      = 1
   } RF_Protocol_t;

   ```

1. `Radio_openRadioCore`

   - Initializes Radio Core of MCU according to parameters.
   - **Parameters**:
     - `pParams[in]` pointer to parameters structure
     - `pObj[in]` pointer to object storing internal configuration
     - `pMode{in]` pointer to global RF mode (e.g. BLE or IEEE)
     - `pSetup[in]` pointer to radio setup structure
     - `pHandle[out]` pointer to RF_Handle to control Radio Core

1. `Radio_setFrequencySynthesizer`

   - Sets frequency synthesizer according in order to tune to a given BLE or IEEE channel
   - **Parameters**:
     - `pHandle[in]` handle to Radio Core
     - `pFsCmd[in]` pointer to Frequency Synthesizer Command
   - **Returns**:
     - `RF_EventMask` Event mask signalling status of the command

1. `Radio_initRXCmd`

   - Initializes RX commands of given protocol for sniffing.
   - **Parameters**:
     - `pRXCmd[in,out]` pointer to Command struct (BLE/IEEE)
     - `proto[in]` protocol to determine typecasting

1. `Radio_beginRX`
   - Posts (sends and does not wait for execution end) RX command according to selected protocol.
   - Main purpose is to shadow TI's API for better readability
   - **Parameters**:
     - `pHandle[in]` handle to Radio Core
     - `proto[in]` protocol whose RX cmd will be sent
     - `callbackFunction[in]` pointer to callback function that executes when one or more of `<events>` occur(s).
     - `events` events on which `<callbackFunction>` gets called
   - **Returns**:
     - `RF_CmdHandle` handle to control given command
