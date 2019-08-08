#!/bin/bash

cd /sec/rocket-chip/riscv-tools/riscv-isa-sim/build && make && make install

cd /sec/rocket-chip/rocc-template/tests && make

/sec/rocket-chip/riscv-tools/riscv-isa-sim/build/spike --extension=rsa pk /sec/rocket-chip/rocc-template/tests/rsa-rocc.rv
