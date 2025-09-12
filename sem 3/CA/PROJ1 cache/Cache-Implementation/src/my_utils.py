def hex_to_bin(hex):
    return bin(int(hex, 16))[2:].zfill(32)


def bin_to_int(bin):
    return int(bin, 2)


def hex_to_int(hex):
    return bin_to_int(hex_to_bin(hex))
