from ipaddress import IPv4Network
from functools import cmp_to_key
from pathlib import Path
import sys


def generate_code(ordered_list, variable_name):
    len_list = len(ordered_list)
    print("#define {0}_cidr_arr_rows {1}".format(variable_name, len_list))
    print("#define {0}_cidr_arr_cols {1}".format(variable_name, 2))
    print("static u32 {0}_cidr_blocks[][{1}] = {{".format(variable_name, 2))
    for i, item in enumerate(ordered_list):
        print("\t{{{0},{1}}}{2}".format(item[0], item[1], "," if i < len(ordered_list) - 1 else ""))
    print("};")


def compare_cidr(item1, item2):
    # We compare the lower bounds of each item which will be on the first position of each array.
    # We also assume that no cloud provider will publish a conflicting CIDR block range as this
    # would be considered as false information and very unlikely to happen tbh.
    return item1[0] - item2[0]


def convert_cidr(cidr_str_list):
    cidr_int_list = []
    for cidr in cidr_str_list:
        cidr_int_list.append([int(IPv4Network(cidr)[0]), int(IPv4Network(cidr)[-1])])
    return cidr_int_list


def read_file(path):
    lines = []
    with open(path, 'r') as f:
        for line in f:
            lines.append(line.rstrip("\n"))
    # remove duplicates before returning it
    return list(dict.fromkeys(lines))


def init():
    if len(sys.argv) != 2:
        print("This script takes exactly one parameter which is the file containing the CIDR blocks from a particular"
              "cloud provider. Nothing more, nothing less...exiting")
        sys.exit(-1)

    lines = read_file(sys.argv[1])
    cidr_unordered_list = convert_cidr(lines)  # unordered yet
    cidr_ordered_list = sorted(cidr_unordered_list, key=cmp_to_key(compare_cidr))
    variable_name = Path(sys.argv[1]).stem  # Python3.4+ required
    generate_code(cidr_ordered_list, variable_name)


if __name__ == '__main__':
    init()
