import subprocess, time

def compile_all():
    subprocess.run(["g++", "-O2", "-std=c++17", "P1549.cpp", "-o", "P1549"], check=True)
    subprocess.run(["g++", "-O2", "-std=c++17", "P1549_brute.cpp", "-o", "P1549_brute"], check=True)

def run(bin_name, inp):
    p = subprocess.run([f"./{bin_name}"], input=inp, text=True, capture_output=True, check=True)
    return p.stdout.strip()

def is_prime(x):
    if x < 2:
        return False
    for i in range(2, int(x**0.5) + 1):
        if x % i == 0:
            return False
    return True

def verify_board(n, out_str):
    lines = out_str.splitlines()
    assert len(lines) == n
    mat = [list(map(int, line.split())) for line in lines]
    for row in mat:
        assert len(row) == n
    assert mat[0][0] == 1
    vals = [x for row in mat for x in row]
    assert sorted(vals) == list(range(1, n * n + 1))
    for i in range(n):
        for j in range(n):
            if i + 1 < n:
                assert is_prime(mat[i][j] + mat[i + 1][j])
            if j + 1 < n:
                assert is_prime(mat[i][j] + mat[i][j + 1])
    rc_sum = sum(mat[0]) + sum(mat[i][0] for i in range(1, n))
    return rc_sum

def main():
    compile_all()
    # N = 1, 3 无解
    for n in [1, 3]:
        assert run("P1549", f"{n}\n") == "NO"
        assert run("P1549_brute", f"{n}\n") == "NO"

    # N = 2, 4 与暴力全搜索核对最小首行首列和
    for n in [2, 4]:
        out_std = run("P1549", f"{n}\n")
        out_bf = run("P1549_brute", f"{n}\n")
        s_std = verify_board(n, out_std)
        s_bf = verify_board(n, out_bf)
        assert s_std == s_bf, f"N={n}: s_std={s_std}, s_bf={s_bf}"

    # N = 5, 6 验证合法性与最优性耗时
    t0 = time.perf_counter()
    s5 = verify_board(5, run("P1549", "5\n"))
    s6 = verify_board(6, run("P1549", "6\n"))
    dt = time.perf_counter() - t0
    print(f"P1549 check passed! N=1..6 verified (s5={s5}, s6={s6}) in {dt:.4f}s")

if __name__ == "__main__":
    main()
