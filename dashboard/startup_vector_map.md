# Startup Vectors Memory Map
| Name | Base address | Size | Value |
| ---- | ------------ | ---- | ----- |
| MAC address | 0x50000 | 6  | e.g. `00 00 00 11 22 33` |
| Device IP address  | 0x50006 | 4  | e.g. `C0 A8 05 11` |
| Target IP address  | 0x5000A | 4  | e.g. `C0 A8 05 02` |
| Using DHCP         | 0x5000E | 1  | `59` = Yes / `4E` = No |
| Protocol           | 0x5000F | 1  | 