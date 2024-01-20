STV_MAC_ADDRESS = bytearray([0x00, 0x00, 0x00, 0x11, 0x22, 0x33])
STV_DEVICE_IP_ADDRESS = bytearray([192, 168, 5, 11])
STV_TARGET_IP_ADDRESS = bytearray([192, 168, 5, 2])
STV_USING_DHCP = bytearray([0x59])
STV_RF_PROTO = bytearray([0xB5])

FILENAME = "startup_vectors.bin"

with open(FILENAME, "wb") as f:
    f.write(STV_MAC_ADDRESS)
    f.write(STV_DEVICE_IP_ADDRESS)
    f.write(STV_TARGET_IP_ADDRESS)
    f.write(STV_USING_DHCP)
    f.write(STV_RF_PROTO)