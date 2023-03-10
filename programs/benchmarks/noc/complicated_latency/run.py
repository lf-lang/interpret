#!/usr/bin/env python3

max_offset = 10

sender = 3
receiver = 1

import subprocess

subprocess.check_output(["make"])
output = subprocess.run(["fp-emu"], stderr=subprocess.PIPE, stdout=subprocess.PIPE).stderr.decode("utf-8")
lines = output.splitlines()
outputs = lambda core: [int(line.split()[-1]) for line in lines if line.startswith(f"Core-{core}:")]
sender_offsets = outputs(sender)
receiver_outputs = outputs(receiver)
receiver_offsets = receiver_outputs[::2]
receiver_measurements = receiver_outputs[1::2]
print(sender_offsets)
print(receiver_offsets)
print(receiver_measurements)

table = [[0 for _ in range(max_offset)] for _ in range(max_offset)]
for sender_offset, receiver_offset, measurement in zip(sender_offsets, receiver_offsets, receiver_measurements):
    table[sender_offset][receiver_offset] = measurement
tex = " & ".join(str(i) for i in range(max_offset)) + "\\\\\n"
for i, row in enumerate(table):
    tex += f"{i} & {' & '.join([str(k) for k in row])} \\\\\n"
print(tex)
