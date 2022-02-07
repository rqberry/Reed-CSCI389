# benchmark.py is a simple script for culling large amounts of outputs
# from update_locations.c and update_locations.py

import subprocess # for subprocess

# Flip between testing python and testing C
PYTHON = False

# Run the selected test on given inputs a specified number of times
if (PYTHON):
    for i in range (21):
        size = 2**i
        iters = 2097152 // size
        for j in range(3):
            subprocess.call(["python3", "update_locations.py", str(size), str(iters)])
else:
    for i in range (21):
        size = 2**i
        iters = 2097152 // size
        for j in range(3):
            subprocess.call(["./update_locations", str(size), str(iters)])
