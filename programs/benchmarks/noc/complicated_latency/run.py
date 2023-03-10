#!/usr/bin/env python3

max_offset = 10
tdm_max_offset = 20

sender = 0
receiver = 1

import subprocess
import matplotlib
import matplotlib.pyplot as plt

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

table = [[[] for _ in range(max_offset)] for _ in range(tdm_max_offset)]
for sender_offset, receiver_offset, measurement in zip(sender_offsets, receiver_offsets, receiver_measurements):
    table[sender_offset][receiver_offset] += [measurement]
tex = " & ".join(str(i) for i in range(tdm_max_offset)) + "\\\\\n"
for i, row in enumerate(table):
    tex += f"{i} & {' & '.join([f'{k}' for k in row])} \\\\\n"
print(tex)

x = [i for i in range(max_offset)]
y0 = [min(s[i] for s in table) for i in x]
y1 = [max(s[i] for s in table) for i in x]

matplotlib.rcParams['font.family'] = 'Nimbus Roman'
plt.plot(x, y1, linestyle='--', linewidth=1, marker='o', color='firebrick', label='maximum latency')
plt.plot(x, y0, linestyle=':' , linewidth=1, marker='+', color='brown', label='minimum latency')
plt.legend()
plt.title("NoC Latency")
plt.xlabel("Offset of Receiving Busy-Wait Loop (Cycles)")
plt.ylabel("Measured Latency (Cycles)")
plt.savefig("latencies.pdf", format="pdf")
plt.show()

plt.hist(sum([x for s in table for x in s], start=[]))
plt.show()
