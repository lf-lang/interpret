#!/usr/bin/env python3

import subprocess
import pandas as pd

NCORES = 4

configs = {
    4: (5, 32),
    9: (10, 32),
    16: (19, 8)
}

TDM_N_CYCLES, TX_RX_N_UNROLL = configs[NCORES]
MAX_NUM_TEST_NOPS = TDM_N_CYCLES + 1

nums_test_nops = list(range(MAX_NUM_TEST_NOPS))
results = {
    20: [],
    100: [],
    500: []
}

for n in results:
    for num_test_nops in nums_test_nops:
        output = subprocess.check_output(["make", f"NUM_TEST_NOPS={num_test_nops}", f"TDM_N_CYCLES={TDM_N_CYCLES}", f"TX_RX_N_UNROLL={TX_RX_N_UNROLL}", f"BENCHMARK_N={n}"]).decode()
        print(output)
        output = subprocess.run(["fp-emu"], capture_output=True).stderr.decode()
        print("output:\n", output)
        at_end = False
        for line in output.split("\n"):
            if "77777" in line:
                at_end = True
            elif at_end:
                results[n] += [int(line.split("    ")[-1])]
                print(results)
                at_end = False

pd.DataFrame(results).to_csv(f"results{NCORES}.csv")
