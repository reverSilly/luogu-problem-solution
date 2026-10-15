# Quick trace to verify understanding of P1139
# Target: "cba" (n=3)
# Initial A (bottom to top): a, b, c   (0,1,2)
# Top of A = c (rightmost / index 2)
# rev_target = reverse("cba") = "abc"

target = "cba"
rev = target[::-1]  # "abc"
print(f"target={target}, rev_target={rev}")

# Train indices: 0='a', 1='b', 2='c'
# Output order needed: a(0), b(1), c(2)

def simulate():
    # State
    remain = 3  # trains in A
    B, C = [], []
    out = 0
    steps = []
    
    # Step 1: A->B, train 2 ('c')
    t = remain - 1  # 2
    remain -= 1
    B.append(t)
    steps.append(('c', 'A', 'B'))
    
    # Step 2: A->B, train 1 ('b')
    t = remain - 1  # 1
    remain -= 1
    B.append(t)
    steps.append(('b', 'A', 'B'))
    
    # Step 3: A->D, train 0 ('a') == rev[0]='a' ✓
    t = remain - 1  # 0
    remain -= 1
    out += 1
    steps.append(('a', 'A', 'D'))
    
    # Step 4: B->D, top of B = 1 ('b') == rev[1]='b' ✓
    t = B.pop()
    out += 1
    steps.append(('b', 'B', 'D'))
    
    # Step 5: B->D, top of B = 2 ('c') == rev[2]='c' ✓
    t = B.pop()
    out += 1
    steps.append(('c', 'B', 'D'))
    
    print("Simulation result:")
    for h, l, r in steps:
        print(f"{h} {l} {r}")
    print(f"Goal reached: remain={remain}, B empty={len(B)==0}, C empty={len(C)==0}, out={out}")

simulate()
