from file_parsing import read_trace_file
import pandas as pd
from os import listdir;
if __name__ == "__main__":
    # Example usage
    # trace_file_path = "../traces/twolf.trace"
    cache_sizes = [2**x for x in range(7,13)]
    block_sizes = [2**x for x in range(1,8)]
    associativity = [2**x for x in range(1,7)]
    s = "../traces/"
    trace_files = [s +x for x in listdir("../traces")]
    cache_table = {}
    block_table = {}
    associativity_table = {}
    cache_table['cache_size'] = cache_sizes;
    block_table['block_size'] = block_sizes;
    associativity_table['associativity'] = associativity;
    
    for file in trace_files:
        cache_table[file[len(s):]] = []
        for size in cache_sizes:
            hit_rate = read_trace_file(file,cache_size=size)
            cache_table[file[len(s):]].append(hit_rate)
        block_table[file[len(s):]] = []
        for size in block_sizes:
            hit_rate = read_trace_file(file,block_size=size)
            block_table[file[len(s):]].append(hit_rate)
        associativity_table[file[len(s):]] = []
        for size in associativity:
            hit_rate = read_trace_file(file,associativity=size)
            associativity_table[file[len(s):]].append(hit_rate)
    
    pd.DataFrame(cache_table).to_csv("../data/cache_data.csv",index=False)
    pd.DataFrame(block_table).to_csv("../data/block_data.csv",index=False)
    pd.DataFrame(associativity_table).to_csv("../data/associativity_data.csv",index=False)