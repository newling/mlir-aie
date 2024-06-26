##===- Makefile -----------------------------------------------------------===##
# 
# This file licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# 
##===----------------------------------------------------------------------===##

srcdir := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

include ${srcdir}/../../makefile-common

VPATH := ${srcdir}/../../../aie_kernels/generic

PASSTHROUGH_WIDTH = 1920
PASSTHROUGH_HEIGHT = 1080

#PASSTHROUGH_WIDTH = 512
#PASSTHROUGH_HEIGHT = 9

targetname = passThrough

.PHONY: all template clean

all: build/final_${PASSTHROUGH_WIDTH}.xclbin

build/aie2_lineBased_8b_${PASSTHROUGH_WIDTH}.mlir: ${srcdir}/aie2.py
	mkdir -p ${@D}
	python3 $< ${PASSTHROUGH_WIDTH} ${PASSTHROUGH_HEIGHT} > $@

build/passThrough.cc.o: passThrough.cc
	mkdir -p ${@D}
	cd ${@D} && xchesscc_wrapper ${CHESSCCWRAP2_FLAGS} -DBIT_WIDTH=8 -c $< -o ${@F}
	
build/final_${PASSTHROUGH_WIDTH}.xclbin: build/aie2_lineBased_8b_${PASSTHROUGH_WIDTH}.mlir build/passThrough.cc.o
	mkdir -p ${@D}
	cd ${@D} && aiecc.py --aie-generate-cdo --aie-generate-npu --no-compile-host --basic-alloc-scheme \
		--xclbin-name=${@F} --npu-insts-name=insts.txt $(<:%=../%)

${targetname}.exe: ${srcdir}/test.cpp
	rm -rf _build
	mkdir -p _build
	cd _build && ${powershell} cmake ${srcdir} -DTARGET_NAME=${targetname} -DPASSTHROUGH_WIDTH=${PASSTHROUGH_WIDTH} -DPASSTHROUGH_HEIGHT=${PASSTHROUGH_HEIGHT} -D CMAKE_C_COMPILER=gcc-13 -D CMAKE_CXX_COMPILER=g++-13 
	cd _build && ${powershell} cmake --build . --config Release
ifeq "${powershell}" "powershell.exe"
	cp _build/${targetname}.exe $@
else
	cp _build/${targetname} $@ 
endif

run: ${targetname}.exe build/final_${PASSTHROUGH_WIDTH}.xclbin build/insts.txt
	${powershell} ./$< -x build/final_${PASSTHROUGH_WIDTH}.xclbin -i build/insts.txt -k MLIR_AIE

clean:
	rm -rf build _build ${targetname}.exe ${targetname}Out_test.jpg
