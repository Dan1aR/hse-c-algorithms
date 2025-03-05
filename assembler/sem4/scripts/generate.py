# generate stuff
import numpy as np

def main():
    n = 2048
    t = 2048
    d = 5

    m = 1000

    with open("stress_test.txt", "w") as fout:
        fout.write(f"{n}\n{t} {d}\n")

        # gen t * d rows
        for _ in range(t * d):
            _size = np.random.randint(n)
            random_nums_1 = np.random.choice(np.arange(n), size=_size, replace=False)
            random_nums_2 = np.random.choice(np.arange(n), size=_size, replace=False)

            _out = " ".join([f"{a}:{b}" for a, b  in zip(random_nums_1, random_nums_2)])
            fout.write(_out + "\n")

        fout.write(f"{m}\n")

        for _ in range(m):
            _size = np.random.randint(n)
            random_nums_1 = np.random.choice(np.arange(n), size=_size, replace=False)
            random_nums_2 = np.random.choice(np.arange(n), size=_size, replace=False)

            _out = " ".join([f"{a}:{b}" for a, b  in zip(random_nums_1, random_nums_2)])
            fout.write(_out + "\n")


main()


