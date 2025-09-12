from cache import *


def read_trace_file(file_path,cache_size = 1024,associativity = 4,block_size = 4):
    # count = 0
    # obj = Cache(4096, 4, 4)
    obj = Cache(cache_size,associativity, block_size)
    with open(file_path, "r") as file:
        for line in file:
            address = str(line.strip().split(" ")[1])
            obj.mem_access(address)

    return obj.hit_rate()
