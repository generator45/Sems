from math import *
from my_utils import *


class Cache:

    # Cache_size in kB and block_size in B
    def __init__(self, cache_size, no_of_ways, block_size):
        self.no_of_cache_lines = int(cache_size * 1024 / block_size*no_of_ways)
        self.byte_offset_bits = int(log2(block_size))
        self.index_bits = int(log2(self.no_of_cache_lines))
        self.tag_bits = 32 - self.byte_offset_bits - self.index_bits
        self.no_of_ways = no_of_ways
        self.block_size = block_size
        self.cache_size = cache_size
        self.hit = 0
        self.miss = 0
        self.cache_table = []
        for i in range(self.no_of_cache_lines):
            cache_line = []
            for j in range(self.no_of_ways):
                cache_line.append(CacheLine())
            self.cache_table.append(cache_line)

    def cache_hit(self):
        self.hit += 1

    def cache_miss(self):
        self.miss += 1

    def hit_rate(self):
        return (self.hit / (self.hit + self.miss)) * 100

    def miss_rate(self):
        return (self.miss / (self.hit + self.miss)) * 100

    def mem_access(self, address):
        address = hex_to_bin(address)
        tag = address[: self.tag_bits]
        index = bin_to_int(address[self.tag_bits : self.tag_bits + self.index_bits])
        byte_offset = address[self.tag_bits + self.index_bits :]

        match_found = False
        MRU_bit = 0

        # Tag match each block in the row
        for i in range(self.no_of_ways):

            # Block is valid
            if self.cache_table[index][i].is_valid() and self.cache_table[index][
                i
            ].tag_match(tag):
                self.cache_hit()
                match_found = True
                MRU_bit = i
                break

        # No match found in all ways

        if not match_found:
            self.cache_miss()

            for i in range(self.no_of_ways - 1, 0, -1):
                self.cache_table[index][i] = self.cache_table[index][i - 1]

            self.cache_table[index][0].set_tag(tag)

        else:
            temp = self.cache_table[index][MRU_bit]

            for i in range(MRU_bit, 0, -1):
                self.cache_table[index][i] = self.cache_table[index][i - 1]

            self.cache_table[index][0] = temp


class CacheLine:
    def __init__(self):
        self.valid = False
        self.tag = None

    def is_valid(self):
        return self.valid

    def set_tag(self, tag):
        self.valid = True
        self.tag = tag

    def tag_match(self, tag):
        return self.tag == tag
