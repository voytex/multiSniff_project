# Status Vectors Memory Map
| Name | Flash Base address | RAM Base Address | Size | Value |
| ---- | ------------------ | ---------------- | ---- | ----- |
| MAC address        | 0x50000 | `N/A`      | 6  | e.g. `00 00 00 11 22 33` |
| Using DHCP         | 0x50006 | 0x20013500 | 1  | `0x59` = Yes / `0x4E` = No |
| Device IP address  | 0x50007 | 0x20013501 | 4  | e.g. `C0 A8 05 11` |
| Gateway Address    | 0x5000B | 0x20013505 | 4  | e.g. `C0 A8 05 01` |
| Network Mask       | 0x5000F | 0x20013509 | 4  | e.g. `FF FF FF 00` |
| Target IP address  | 0x50010 | 0x2001350D | 4  | e.g. `C0 A8 05 02` |
| RF Protocol        | 0x50014 | 0x20013511 | 1  | `0xB5` = Bluetooth LE / `0x15` = IEEE 802.15.4 | 
| Running status     | 0x50015 | 0x20013512 | 1  | `0x52` = **R**unning / `0x00`= St**o**pped
| Signal RF change   | `N/A`   | 0x20013513 | 1  | `0x00`= No change | `0xFF` = Change requested |