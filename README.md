# multiSniff Masters Project

## General
Repository for my Master`s project called __[Multiprotocol Sniffer And Analyzer](https://www.vut.cz/studenti/zav-prace/detail/159103)__. 

The subject of this work is design of a device that captures Bluetooth Low Energy or IEEE 802.15.4 frames and sends them via Ethernet bus to a computer for subsequent analysis using Wireshark. The device is based on the CC2652RB microchip from Texas Instruments, which is accompanied by the WIZnet W5500 Ethernet interface. This repository includes MCU [software](./source), HTML code for web-based [control panel](./flashbins/index.html) and Wireshark [dissector](./wireshark/msniff.lua).

## Hardware Used
* TI CC2652RB LaunchPad for prototyping and SW development
* WIZnet W5500 with RJ-45 connector for Ethernet interface (SPI communication)
* SSD1306 OLED display (I2C communication)

## Results
### Prototype photo
![prototype_w_display](https://github.com/user-attachments/assets/e76a7554-7867-44dd-a42f-ceac308abcfa)

### Dashboard screenshot
![dashboard_portrait_bez_addbaru](https://github.com/user-attachments/assets/c06d26f7-fce3-4508-acda-459742ae5cd6)

### Captured BLE traffic
![msniff_test_capture_211572](https://github.com/user-attachments/assets/94134058-dc19-4b60-9e88-44828610d694)

<table>
  <tr>
    <td>
      black line
    </td>
    <td>
      sum of all BLE traffic (advertising channel)  
    </td>
  </tr>
  <tr>
    <td>
      blue line
    </td>  
    <td>
      advertising of BLE enabled coffee machine      
    </td>
  </tr>
  <tr>
    <td>
      red line    
    </td>
    <td>
    advertising of ESP32 turned on and off during test capture    
  </td>
  </tr>
</table>

## Other Links
* [Bachelor`s Project (MIDI over Ethernet)](https://github.com/voytex/MoE_firmware)
    



