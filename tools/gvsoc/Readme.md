# Install the riscv toolchain 
```
git clone git@github.com:pulp-platform/pulp-riscv-gnu-toolchain.git
```
Tag used is v1.0.16
# Set the following path variables 
```
export PATH=/scratch/prasadar/v1.0.16-pulp-riscv-gcc-centos-7:$PATH
export PULP_RISCV_GCC_TOOLCHAIN=/scratch/prasadar/v1.0.16-pulp-riscv-gcc-centos-7
````
# cd <neureka_gvsoc>
```
source configs/pulp-open.sh
```
# Build 
```
CC=gcc-9.2.0 CXX=g++-9.2.0 MAGICK_ROOT=/usr/pack/graphicsmagick-1.3.36-kgf make build
```
# Build GVSoC
```
CC=gcc-9.2.0 CXX=g++-9.2.0 MAGICK_ROOT=/usr/pack/graphicsmagick-1.3.36-kgf make gvsoc.build
```
# Test the NEUREKA application 
```
cd tests/1x1x1x1_fs1
make clean all run platform=gvsoc runner_args="--trace=.*:log.txt"
```
