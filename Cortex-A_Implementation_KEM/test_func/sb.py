def parse(string):
    s = string.strip().split(',')[:-1]
    s = map(int, s)
    return list(s)


with open('test_schoolbook_a.txt', 'r') as f:
    a = f.readlines()
    a = parse(a[0])

with open('test_schoolbook_b.txt', 'r') as f:
    b = f.readlines()
    b = parse(b[0])

with open('test_schoolbook_c.txt', 'r') as f:
    c = f.readlines()
    c = parse(c[0])

new_a = a[::16]
new_b = b[::16]


def multiply(A, B, m, n):

    prod = [0] * (m + n - 1)

    # Multiply two polynomials term by term

    # Take ever term of first polynomial
    for i in range(m):

        # Multiply the current term of first
        # polynomial with every term of
        # second polynomial.
        for j in range(n):
            prod[i + j] += A[i] * B[j]

    for i in range(len(prod)):
        prod[i] = prod[i] & 0xffff

    return prod


z = multiply(new_a, new_b, 16, 16)
print("THIS: ", z)
new_c = c[::16]
print("THAT: ", new_c)

for i in range(len(z)):
    if z[i] != new_c[i]:
        print(z[i], new_c[i], i)
