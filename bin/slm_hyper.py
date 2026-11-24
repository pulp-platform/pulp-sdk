#!/usr/bin/python3

#Written by ABA to update the format of the slm file to be compliant with hyperflash model used in testbench
import os
import os.path
import argparse
import sys

parser = argparse.ArgumentParser(description='Generate hyper memory image file from slm')

parser.add_argument("--input", dest="input_file", default=None, help="Specify input file (ex. ./build/pulpissimo/slm_files/flash_stim.slm)")
parser.add_argument("--output", dest="output_file", default=None, help="Specify output file (ex. ./build/pulpissimo/slm_files/hyper_flash_stim.slm)")

args = parser.parse_args()

if args.input_file is None:
    raise Exception('Specify the input file with --input=<path> (ex. --input=./build/pulpissimo/slm_files/flash_stim.slm)')

if args.output_file is None:
    raise Exception('Specify the output file with --output=<path> (ex. --output=./build/pulpissimo/slm_files/hyper_flash_stim.slm')

delimiter=" "
with open(args.input_file, "r") as fi:
    data = list(map(lambda x:x.split(delimiter), fi.read().strip().split("\n")))
fo=open(args.output_file, "w")

# Write the header
fo.write('@000000\n')

# Iterate over rows in pairs
for i in range(0, len(data)-1, 2):  # subtract 1 to avoid IndexError if odd number of rows
    fo.write(f'{data[i+1][1]}{data[i][1]}\n')
