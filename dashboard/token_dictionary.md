# Token dictionary table
For token-value relations within dashboard and software.

| Token | Type | Value |
| ----- | ---- | ----- |
| `$m`  | `char[17]` | MAC address of the Device | 
| `$d`  | `char[15]` | Device IP address |
| `$g`  | `char[15]` | Gateway IP address |
| `$s`  | `char[15]` | Network mask |
| `$r`  | `char[1]` `("1"\|"0")` | Is the device sniffing? |
| `$p`  | `char[1]` `("1"\|"0")` | Protocol (0 = `BLE`; 1 =  `802_15_4`) |
| `$v`  | `char[17]` | Number of RXOK BLE frames | 
| `$w`  | `char[17]` | Number of RXNOK BLE frames |
| `$x`  | `char[17]` | Number of RXOK 802_15_4 frames |
| `$y`  | `char[17]` | Number of RXNOK 802_15_4 frames |
| 
