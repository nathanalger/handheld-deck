import serial
import struct

SYNC0 = 0xAA
SYNC1 = 0x55

PKT_PING = 0x01


def crc8(crc, byte):
    crc ^= byte

    for _ in range(8):
        if crc & 0x80:
            crc = ((crc << 1) ^ 0x07) & 0xFF
        else:
            crc = (crc << 1) & 0xFF

    return crc


def build_packet(seq, pkt_type, payload=b""):
    size = len(payload)

    packet = bytearray()

    packet.append(SYNC0)
    packet.append(SYNC1)

    crc = 0

    packet.append(seq)
    crc = crc8(crc, seq)

    packet.append(pkt_type)
    crc = crc8(crc, pkt_type)

    size_lo = size & 0xFF
    size_hi = (size >> 8) & 0xFF

    packet.append(size_lo)
    crc = crc8(crc, size_lo)

    packet.append(size_hi)
    crc = crc8(crc, size_hi)

    for b in payload:
        packet.append(b)
        crc = crc8(crc, b)

    packet.append(crc)

    return packet


def main():
    ser = serial.Serial(
        "/dev/serial0",
        baudrate=115200,
        bytesize=8,
        parity="N",
        stopbits=1,
        timeout=1
    )

    seq = 0

   while True:
      value = int(input("Value: "))

      payload = struct.pack("<H", value)

      pkt = build_packet(seq, PKT_PING, payload)

      ser.write(pkt)

      print("Sent:", pkt.hex(" "))

      seq = (seq + 1) & 0xFF


if __name__ == "__main__":
    main()