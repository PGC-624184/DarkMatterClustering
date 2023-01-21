# #!/bin/bash

infer run --annotation-reachability --bufferoverrun --cost --immutable-cast --liveness --pulse --uninit -- gcc -c test_main.c
