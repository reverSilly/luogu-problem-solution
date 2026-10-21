#!/usr/bin/env python3
import os, random, string, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
std = '/tmp/P1827'
brute = '/tmp/P1827_brute'
sample_in = 'ABEDFCHG\nCBADEFGH\n'
sample_out = 'AEFDBHGC'


def compile_all():
    for src, out in (('P1827.cpp', std), ('P1827_brute.cpp', brute)):
        r = subprocess.run(
            ['g++', '-O2', '-std=c++17', '-Wall', '-Wextra', '-Werror',
             '-o', out, os.path.join(here, src)],
            capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            sys.stderr.write(r.stderr)
            sys.exit(1)


def run(prog, inp, timeout=10):
    r = subprocess.run([prog], input=inp, capture_output=True, text=True, timeout=timeout)
    if r.returncode != 0:
        raise RuntimeError(f'{prog} exit {r.returncode}\n{r.stderr[:400]}')
    return r.stdout.strip()


def gen_tree(rnd, letters):
    if not letters:
        return None
    root = letters[0]
    rest = letters[1:]
    cut = rnd.randrange(len(rest) + 1)
    return (root, gen_tree(rnd, rest[:cut]), gen_tree(rnd, rest[cut:]))


def inorder(t):
    if not t:
        return ''
    return inorder(t[1]) + t[0] + inorder(t[2])


def preorder(t):
    if not t:
        return ''
    return t[0] + preorder(t[1]) + preorder(t[2])


def postorder(t):
    if not t:
        return ''
    return postorder(t[1]) + postorder(t[2]) + t[0]


def chain(letters, left):
    if not letters:
        return None
    if left:
        return (letters[-1], chain(letters[:-1], True), None)
    return (letters[0], None, chain(letters[1:], False))


def main():
    compile_all()
    tests = 0
    a, b = run(std, sample_in), run(brute, sample_in)
    if a != sample_out or b != sample_out:
        print(f'sample FAIL std={a} brute={b}')
        return 1
    tests += 1
    rnd = random.Random(1827)
    t0 = time.time()
    cases = []
    for n in range(1, 27):
        letters = list(string.ascii_uppercase[:n])
        cases.append(gen_tree(rnd, letters[:]))
        cases.append(chain(letters[:], True))
        cases.append(chain(letters[:], False))
    for i in range(200):
        n = rnd.randint(1, 26)
        letters = rnd.sample(string.ascii_uppercase, n)
        cases.append(gen_tree(rnd, letters))
    for i, t in enumerate(cases, 1):
        inn, pr, expect = inorder(t), preorder(t), postorder(t)
        inp = inn + '\n' + pr + '\n'
        a, b = run(std, inp), run(brute, inp)
        if a != expect or b != expect:
            print(f'FAIL case {i}\n{inp}std={a}\nbrut={b}\nexpect={expect}')
            return 1
        tests += 1
    dt = time.time() - t0
    print(f'[随机+边界] {tests} 组一致,耗时 {dt:.2f}s')
    print(f'{tests} tests OK')
    return 0


if __name__ == '__main__':
    sys.exit(main())
