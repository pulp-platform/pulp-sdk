#!/usr/bin/env python

import sys
import random
import argparse
import math
import re

# Extracting testcase
if len(sys.argv) > 1:
    testcase = sys.argv[1]
else:
    testcase = None

# Define max values for size, strides and length
TRANSFER_SIZE = 1024
MAX_STRIDE    = 10
MAX_LENGTH    = 10
# Randomize the number of transfers to execute
NB_TRANSFERS = random.randint(1, 10)
# Create array for sizes of each transfer
transfers = [None] * NB_TRANSFERS


#########################################
# 1D STIMULI GENERATION
#########################################

# The following function writes the needed defines and the structure containing the parameter fields for a 1-dimensional transfer
def write_transfer_params_1d(f, struct_name, params_array_name, arr):
    f.write ('%s %s[] = {\n' % (struct_name, params_array_name))
    for v in arr:
        size        = random.randint(1, TRANSFER_SIZE)
        f.write('{%d},\n' % size)
    f.write('};\n\n')
    return

# The following function writes an array of structures containing the parameters for the 1-dimensional transfers to be executed
def write_defs_array_1d(f, name, val, name_size, name_struct):
    f.write('#define %s %d\n\n' % (name,val))
    f.write ('typedef struct { \n')
    f.write ('  unsigned int %s;\n' % name_size)
    f.write ('} %s;\n\n' %name_struct)
    return

#########################################
# 2D STIMULI GENERATION
#########################################

# The following function writes the needed defines and the structure containing the parameter fields for a 2-dimensional transfer
def write_defs_array_2d(f, name, val, name_size, name_l1_length, name_l1_stride, name_l2_length, name_l2_stride, name_struct):
    f.write('#define %s %d\n\n' % (name,val))
    f.write ('typedef struct { \n')
    f.write ('  unsigned int %s;\n' % name_size)
    f.write ('  unsigned int %s;\n' % name_l1_length)
    f.write ('  unsigned int %s;\n' % name_l1_stride)
    f.write ('  unsigned int %s;\n' % name_l2_length)
    f.write ('  unsigned int %s;\n' % name_l2_stride)
    f.write ('} %s;\n\n' %name_struct)
    return

# The following function writes an array of structures containing the parameters for the 2-dimensional transfers to be executed
def write_transfer_params_2d(f, struct_name, params_array_name, arr):
    f.write ('%s %s[] = {\n' % (struct_name, params_array_name))
    for v in arr:
        l1_length  = random.randint(1, MAX_LENGTH)
        l2_length  = random.randint(1, MAX_LENGTH)
        size       = random.randint(1, TRANSFER_SIZE) + max(l1_length, l2_length)
        l1_stride  = random.randint(1, MAX_STRIDE) + l1_length
        l2_stride  = random.randint(1, MAX_STRIDE) + l2_length
        f.write('{%d, %d, %d, %d, %d},\n' % (size, l1_length, l1_stride, l2_length, l2_stride))
    f.write('};\n\n')
    return

# Calling the stimuli-generation functions depending on the transfer dimensions

def gen_stim_1d(f_param, f_def, testcase):
    print ("Generate stimuli for 1d testcase")
    struct_name = 'transfer_1d'
    size_field_name = 'size_1d'
    params_array_name = 'params_1d'
    write_defs_array_1d(f_def, 'NB_TRANSFERS', NB_TRANSFERS, size_field_name, struct_name)
    write_transfer_params_1d(f_param, struct_name, params_array_name, transfers)

def gen_stim_2d(f_param, f_def, testcase):
    print ("Generate stimuli for 2d testcase")
    struct_name = 'transfer_2d'
    size_field_name = 'size_2d'
    l1_length_field_name = 'l1_length_2d'
    l1_stride_field_name = 'l1_stride_2d'
    l2_length_field_name = 'l2_length_2d'
    l2_stride_field_name = 'l2_stride_2d'
    params_array_name = 'params_2d'
    write_defs_array_2d(f_def, 'NB_TRANSFERS', NB_TRANSFERS, size_field_name, l1_length_field_name, l1_stride_field_name, l2_length_field_name, l2_stride_field_name, struct_name)
    write_transfer_params_2d(f_param, struct_name, params_array_name, transfers)

# Selecting the testcase depending on what was specified through command line (defaulting to 1D)

if (testcase == '1D'):
    print ("Generating stimuli for 1D case")
    f_param_1d = open('mchan_param_1d.h', 'w')
    f_def_1d = open('mchan_def_1d.h', 'w')
    gen_stim_1d(f_param_1d, f_def_1d, testcase)
elif (testcase == '2D'):
    print ("Generating stimuli for 2D case")
    f_param_2d = open('mchan_param_2d.h', 'w')
    f_def_2d = open('mchan_def_2d.h', 'w')
    gen_stim_2d(f_param_2d, f_def_2d, testcase)
else:
    print ("No testcase specified --> generating stimuli for 1d case")
    f_param_1d = open('mchan_param_1d.h', 'w')
    f_def_1d = open('mchan_def_1d.h', 'w')
    gen_stim_1d(f_param_1d, f_def_1d, testcase)
