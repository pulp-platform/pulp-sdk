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
TRANSFER_SIZE = 128
MAX_STRIDE    = 10
MAX_LENGTH    = 10
MAX_REPS     = 5
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
def write_defs_array_2d(f, name, val, name_size, length, name_src_stride, name_dst_stride, name_struct):
    f.write('#define %s %d\n\n' % (name,val))
    f.write ('typedef struct { \n')
    f.write ('  unsigned int %s;\n' % name_size)
    f.write ('  unsigned int %s;\n' % length)
    f.write ('  unsigned int %s;\n' % name_src_stride)
    f.write ('  unsigned int %s;\n' % name_dst_stride)
    f.write ('} %s;\n\n' %name_struct)
    return

# The following function writes an array of structures containing the parameters for the 2-dimensional transfers to be executed
def write_transfer_params_2d(f, struct_name, params_array_name, arr):
    f.write ('%s %s[] = {\n' % (struct_name, params_array_name))
    for v in arr:
        length  = random.randint(1, MAX_LENGTH)
        size       = random.randint(1, TRANSFER_SIZE) + length
        src_stride  = random.randint(1, MAX_STRIDE) + length
        dst_stride  = random.randint(1, MAX_STRIDE) + length
        f.write('{%d, %d, %d, %d},\n' % (size, length, src_stride, dst_stride))
    f.write('};\n\n')
    return

#########################################
# 3D STIMULI GENERATION
#########################################

# The following function writes the needed defines and the structure containing the parameter fields for a 2-dimensional transfer
def write_defs_array_3d(f, name, val, name_size, length, name_src_stride_2d, name_dst_stride_2d, name_src_stride_3d, name_dst_stride_3d, num_reps_3d, name_struct):
    f.write('#define %s %d\n\n' % (name,val))
    f.write ('typedef struct { \n')
    f.write ('  unsigned int %s;\n' % name_size)
    f.write ('  unsigned int %s;\n' % length)
    f.write ('  unsigned int %s;\n' % name_src_stride_2d)
    f.write ('  unsigned int %s;\n' % name_dst_stride_2d)
    f.write ('  unsigned int %s;\n' % name_src_stride_3d)
    f.write ('  unsigned int %s;\n' % name_dst_stride_3d)
    f.write ('  unsigned int %s;\n' % num_reps_3d)
    f.write ('} %s;\n\n' %name_struct)
    return

# The following function writes an array of structures containing the parameters for the 2-dimensional transfers to be executed
def write_transfer_params_3d(f, struct_name, params_array_name, arr):
    f.write ('%s %s[] = {\n' % (struct_name, params_array_name))
    for v in arr:
        length  = random.randint(1, MAX_LENGTH)
        size       = random.randint(1, TRANSFER_SIZE) + length
        src_stride_2d  = random.randint(1, MAX_STRIDE) + length
        dst_stride_2d  = random.randint(1, MAX_STRIDE) + length
        src_stride_3d  = random.randint(1, MAX_STRIDE) + length
        dst_stride_3d  = random.randint(1, MAX_STRIDE) + length
        num_reps_3d  = random.randint(1, MAX_REPS)
        f.write('{%d, %d, %d, %d, %d, %d, %d},\n' % (size, length, src_stride_2d, dst_stride_2d, src_stride_3d, dst_stride_3d, num_reps_3d))
    f.write('};\n\n')
    return


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
    length_field_name = 'length'
    src_stride_field_name = 'src_stride_2d'
    dst_stride_field_name = 'dst_stride_2d'
    params_array_name = 'params_2d'
    write_defs_array_2d(f_def, 'NB_TRANSFERS', NB_TRANSFERS, size_field_name, length_field_name, src_stride_field_name, dst_stride_field_name, struct_name)
    write_transfer_params_2d(f_param, struct_name, params_array_name, transfers)

def gen_stim_3d(f_param, f_def, testcase):
    print ("Generate stimuli for 3d testcase")
    struct_name = 'transfer_3d'
    size_field_name = 'size_3d'
    length_field_name = 'length'
    src_stride_2d_field_name = 'src_stride_2d'
    src_stride_3d_field_name = 'src_stride_3d'
    dst_stride_2d_field_name = 'dst_stride_2d'
    dst_stride_3d_field_name = 'dst_stride_3d'
    num_reps_3d_field_name = 'num_reps_3d'
    params_array_name = 'params_3d'
    write_defs_array_3d(f_def, 'NB_TRANSFERS', NB_TRANSFERS, size_field_name, length_field_name, src_stride_2d_field_name, dst_stride_2d_field_name, src_stride_3d_field_name, dst_stride_3d_field_name, num_reps_3d_field_name, struct_name)
    write_transfer_params_3d(f_param, struct_name, params_array_name, transfers)


# Selecting the testcase depending on what was specified through command line (defaulting to 1D)

if (testcase == '1D'):
    print ("Generating stimuli for 1D case")
    f_param_1d = open('idma_param_1d.h', 'w')
    f_def_1d = open('idma_def_1d.h', 'w')
    gen_stim_1d(f_param_1d, f_def_1d, testcase)
elif (testcase == '2D'):
    print ("Generating stimuli for 2D case")
    f_param_2d = open('idma_param_2d.h', 'w')
    f_def_2d = open('idma_def_2d.h', 'w')
    gen_stim_2d(f_param_2d, f_def_2d, testcase)
elif (testcase == '3D'):
    print ("Generating stimuli for 3D case")
    f_param_3d = open('idma_param_3d.h', 'w')
    f_def_3d = open('idma_def_3d.h', 'w')
    gen_stim_3d(f_param_3d, f_def_3d, testcase)
else:
    print ("No testcase specified --> generating stimuli for 1d case")
    f_param_1d = open('idma_param_1d.h', 'w')
    f_def_1d = open('idma_def_1d.h', 'w')
    gen_stim_1d(f_param_1d, f_def_1d, testcase)
