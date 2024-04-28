msniff_protocol = Proto("mSniff", "mSniff parser")

proto_switch = ProtoField.int8("mSniff.proto_switch", "RF protocol", base.DEC, { [0] = "Bluetooth Low Energy", [1] = "IEEE 802.15.4" })

msniff_protocol.fields = { proto_switch }

local ble_dissector  = Dissector.get("btle")
local ieee_dissector = Dissector.get("wpan")

function msniff_protocol.dissector(buffer, pinfo, tree)
    length = buffer:len()

    if length == 0 then return end

    --pinfo.cols.protocol = msniff_protocol.name
    local proto = buffer(0,1):uint()
    local subtree = tree:add(msniff_protocol, buffer(), "mSniff Parser")
    subtree:add(proto_switch, proto)
    if proto == 0 then
        ble_dissector:call(buffer(1):tvb(), pinfo, tree)
    elseif proto == 1 then  
        ieee_dissector:call(buffer(1):tvb(), pinfo, tree)
    end
end

local udp_port = DissectorTable.get("udp.port")
udp_port:add(2014, msniff_protocol)