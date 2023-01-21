# #!/bin/bash

infer run --annotation-reachability --bufferoverrun --cost --immutable-cast --impurity --liveness --pulse --purity --uninit -- gcc -c test_main.c
