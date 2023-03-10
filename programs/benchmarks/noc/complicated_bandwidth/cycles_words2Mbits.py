def mbits(cycles, words):
    return words / cycles * (32/1_000_000) * (50_000_000)
