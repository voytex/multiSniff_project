# Token dictionary table
For token-value relations within dashboard and software.

| Token | Type | Value | Dash access |
| ----- | ---- | ----- | ----------- |
| `$t`  | `char[15]` | IP address of the Target | `R/W` |
| `$m`  | `char[17]` | MAC address of the Device | `R`|
| `$d`  | `char[15]` | Device IP address | `R/W`|
| `$g`  | `char[15]` | Gateway IP address | `R/W`|
| `$s`  | `char[15]` | Network mask | `R/W`|
| `$h`  | `char[1]` `("1"\|"0")` | Is the device using DHCP? | `R/W`|
| `$r`  | `char[1]` `("1"\|"0")` | Is the device sniffing? | `R/W`|
| `$p`  | `char[1]` `("1"\|"0")` | Protocol (0 = `BLE`; 1 =  `802_15_4`) | `R/W` |
| `$k`  | `char[2]`  | RX Channel | `R/W`|
| `$v`  | `char[17]` | Number of RXOK BLE frames | `R` |
| `$w`  | `char[17]` | Number of RXNOK BLE frames | `R` |
| `$x`  | `char[17]` | Number of RXOK 802_15_4 frames | `R` |
| `$y`  | `char[17]` | Number of RXNOK 802_15_4 frames | `R` |
| `$z`  | `char[17]` | Last frame's RSSI | `R` |

