#!/bin/bash
cd ../src

make clean

infer run --annotation-reachability --bufferoverrun --cost --immutable-cast --liveness --pulse --uninit --results-dir ../test/infer-out -- make

cd ..