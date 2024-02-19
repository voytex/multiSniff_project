STV_MAC_ADDRESS = bytearray([0x00, 0x00, 0x00, 0x11, 0x22, 0x33])
STV_USING_DHCP = bytearray([0x59])
STV_DEVICE_IP_ADDRESS = bytearray([192, 168, 5, 11])
STV_GATEWAY_IP_ADDRESS = bytearray([192, 168, 5, 1])
STV_NETWORK_MASK = bytearray([255, 255, 255, 0])
STV_TARGET_IP_ADDRESS = bytearray([192, 168, 5, 2])
STV_RF_PROTO = bytearray([0xB5])
STV_RUNNING_STATUS = bytearray([0x00])


FILENAME = "status_vectors.bin"

with open(FILENAME, "wb") as f:
    f.write(STV_MAC_ADDRESS)
    f.write(STV_USING_DHCP)
    f.write(STV_DEVICE_IP_ADDRESS)
    f.write(STV_GATEWAY_IP_ADDRESS)
    f.write(STV_NETWORK_MASK)
    f.write(STV_TARGET_IP_ADDRESS)
    f.write(STV_RF_PROTO)
    f.write(STV_RUNNING_STATUS)