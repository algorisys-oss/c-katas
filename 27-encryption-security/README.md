# Module 27: Encryption & Security

## Why Encryption?

Every time you send a message, log into a website, or make a purchase online,
encryption protects your data from being read by anyone other than the intended
recipient. Without encryption, the internet would be like sending postcards —
anyone handling the mail could read your message.

Encryption transforms **plaintext** (readable data) into **ciphertext** (scrambled
data) using a **key**. Only someone with the correct key can reverse the process.

```
    Plaintext + Key  ──►  Encryption  ──►  Ciphertext
    Ciphertext + Key ──►  Decryption  ──►  Plaintext
```

---

## Classical Ciphers

### Caesar Cipher

The simplest cipher: shift every letter by a fixed number of positions in the
alphabet. Julius Caesar supposedly used a shift of 3.

```
    Shift = 3

    Plain:   A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
    Cipher:  D E F G H I J K L M N O P Q R S T U V W X Y Z A B C

    "HELLO" → "KHOOR"
```

**Breaking it**: There are only 26 possible shifts. Try them all (brute force).
Even smarter: use **frequency analysis** — in English, 'E' is the most common
letter (~13%), followed by 'T' (~9%) and 'A' (~8%). The most frequent letter in
the ciphertext is probably 'E'.

### Substitution Cipher

Instead of a fixed shift, map each letter to a *different* letter. The key is
the entire mapping (a permutation of the alphabet). This gives 26! ≈ 4×10²⁶
possible keys — far too many to brute force. But frequency analysis still
breaks it easily.

### XOR Cipher

XOR (exclusive or) is the foundation of modern encryption. It has a magical
property: applying it twice with the same key gives back the original.

```
    XOR Truth Table
    ┌───┬───┬───────┐
    │ A │ B │ A ⊕ B │
    ├───┼───┼───────┤
    │ 0 │ 0 │   0   │
    │ 0 │ 1 │   1   │
    │ 1 │ 0 │   1   │
    │ 1 │ 1 │   0   │
    └───┴───┴───────┘

    Key property:  (A ⊕ B) ⊕ B = A
    This means:    encrypt = decrypt!
```

XOR encryption works byte by byte:

```
    Plaintext:   H        E        L        L        O
    Binary:      01001000 01000101 01001100 01001100 01001111
    Key (K):     01001011 01001011 01001011 01001011 01001011
    XOR result:  00000011 00001110 00000111 00000111 00000100
                 (ciphertext — looks like garbage)

    XOR again with same key → back to original!
```

A single-byte key is trivially breakable. A key as long as the message (a
**one-time pad**) is *theoretically unbreakable* — but you need a new key for
every message, which creates a key distribution problem.

### Vigenère Cipher

A polyalphabetic cipher: use a keyword, and shift each letter by the
corresponding letter of the key (wrapping the key around).

```
    Plaintext: ATTACKATDAWN
    Key:       LEMONLEMONLE      (repeated to match length)
    Shift A by L(11), T by E(4), T by M(12), ...
    Result:    LXFOPVEFRNHR
```

Harder to break than Caesar, but still vulnerable to frequency analysis on
groups of letters spaced key-length apart (Kasiski examination).

---

## Hash Functions

A hash function takes input of any size and produces a fixed-size output
(the **digest** or **hash**). Crucially, it's a one-way function.

### Properties of Cryptographic Hashes

1. **Deterministic**: Same input always gives same output
2. **Fast**: Quick to compute for any input
3. **One-way**: Given a hash, you can't find the input (pre-image resistance)
4. **Avalanche effect**: Changing 1 bit of input changes ~50% of output bits
5. **Collision resistant**: Hard to find two inputs with the same hash

```
    SHA-256("hello")  = 2cf24dba5fb0a30e26e83b2ac5b9e29e...
    SHA-256("hellp")  = completely different 256-bit value
    SHA-256("hello")  = 2cf24dba5fb0a30e26e83b2ac5b9e29e...  (always the same)
```

### Uses

- **Password storage**: Store hash(password), not the password itself
- **File integrity**: Verify downloads haven't been tampered with
- **Digital signatures**: Sign the hash of a document, not the whole document
- **Blockchain**: Each block contains the hash of the previous block

---

## Modular Arithmetic

Encryption relies heavily on modular arithmetic — "clock arithmetic."

```
    a mod n = remainder when a is divided by n

    17 mod 5 = 2    (because 17 = 3×5 + 2)
    25 mod 7 = 4    (because 25 = 3×7 + 4)
```

Key properties:
```
    (a + b) mod n = ((a mod n) + (b mod n)) mod n
    (a × b) mod n = ((a mod n) × (b mod n)) mod n
    a^b mod n can be computed efficiently (modular exponentiation)
```

### Modular Exponentiation (Repeated Squaring)

Computing `base^exp mod m` naively would overflow. Instead, we use **repeated
squaring**:

```
    To compute 3^13 mod 7:

    13 in binary = 1101

    Start with result = 1
    Bit 1 (1): result = result² × 3 mod 7 = 1 × 3 = 3
    Bit 1 (1): result = result² × 3 mod 7 = 9 × 3 mod 7 = 27 mod 7 = 6
    Bit 0 (0): result = result² mod 7     = 36 mod 7 = 1
    Bit 1 (1): result = result² × 3 mod 7 = 1 × 3 = 3

    Answer: 3^13 mod 7 = 3
```

This runs in O(log exp) multiplications — fast even for huge exponents.

---

## Prime Numbers and RSA

### Why Primes?

The security of RSA rests on a simple asymmetry:
- **Easy**: Multiply two large primes: p × q = n
- **Hard**: Given n, find p and q (integer factorization)

For small numbers this seems trivial, but for 300-digit primes, no known
algorithm can factor their product in a reasonable time.

### RSA — From First Principles

RSA (Rivest–Shamir–Adleman, 1977) is the most famous public-key cryptosystem.

#### Key Generation

```
    ┌─────────────────────────────────────────────────────────┐
    │                  RSA Key Generation                     │
    │                                                         │
    │  1. Choose two primes:     p = 61,  q = 53             │
    │  2. Compute n = p × q:     n = 3233                    │
    │  3. Compute φ(n):          φ = (p-1)(q-1) = 3120       │
    │  4. Choose e:              e = 17  (coprime to φ)      │
    │  5. Compute d:             d = e⁻¹ mod φ = 2753        │
    │                            (17 × 2753 = 46801          │
    │                             46801 mod 3120 = 1  ✓)     │
    │                                                         │
    │  Public key:   (e=17,  n=3233)  ← share with everyone  │
    │  Private key:  (d=2753, n=3233) ← keep secret!         │
    └─────────────────────────────────────────────────────────┘
```

#### Encryption and Decryption

```
    ┌─────────────────────────────────────────────────────────┐
    │                RSA Encrypt / Decrypt                    │
    │                                                         │
    │     message = 65  (must be < n)                         │
    │                                                         │
    │  Encrypt (public key e=17, n=3233):                     │
    │     ciphertext = 65^17 mod 3233 = 2790                  │
    │                                                         │
    │  Decrypt (private key d=2753, n=3233):                  │
    │     plaintext  = 2790^2753 mod 3233 = 65  ✓             │
    │                                                         │
    │  Anyone can encrypt (public key is public).             │
    │  Only the key owner can decrypt (private key is secret).│
    └─────────────────────────────────────────────────────────┘
```

#### Why It Works (Euler's Theorem)

Euler's theorem says: if gcd(m, n) = 1, then m^φ(n) ≡ 1 (mod n).

Since e×d ≡ 1 (mod φ), we have e×d = 1 + k×φ for some integer k.

```
    (m^e)^d = m^(e×d) = m^(1 + k×φ) = m × (m^φ)^k ≡ m × 1^k = m (mod n)
```

So decrypting an encrypted message gives back the original. Math is beautiful!

---

## Diffie-Hellman Key Exchange

**Problem**: Alice and Bob want to agree on a shared secret key, but they can
only communicate over a public channel that Eve is watching.

**Solution**: The Diffie-Hellman protocol (1976).

### The Paint Mixing Analogy

```
    Alice                   Public Channel              Bob
    ─────                   ──────────────              ───

    Pick secret color:                          Pick secret color:
    [ORANGE]                                    [TEAL]
         │                                           │
         ▼                                           ▼
    Mix with public                          Mix with public
    paint [YELLOW]:                          paint [YELLOW]:
    ORANGE+YELLOW                            TEAL+YELLOW
    = [BURNT ORANGE]                         = [LIGHT GREEN]
         │                                           │
         └──────────►  Exchange mixtures  ◄──────────┘
         │           (Eve sees both, but            │
         │            can't unmix them!)             │
         ▼                                           ▼
    Add my secret:                          Add my secret:
    LIGHT GREEN                             BURNT ORANGE
    + ORANGE                                + TEAL
    = [BROWN]                               = [BROWN]
         │                                           │
         ▼                                           ▼
    ┌──────────────────────────────────────────────────┐
    │  Both have the SAME shared secret: [BROWN]       │
    │  Eve saw the mixtures but can't derive [BROWN]   │
    └──────────────────────────────────────────────────┘
```

### The Math Version

```
    Public values: prime p, generator g

    Alice                                    Bob
    ─────                                    ───
    Pick secret a                            Pick secret b
    Compute A = g^a mod p   ──────►
                            ◄──────  Compute B = g^b mod p

    Compute: B^a mod p               Compute: A^b mod p
    = (g^b)^a mod p                  = (g^a)^b mod p
    = g^(ab) mod p                   = g^(ab) mod p
              ▲                                ▲
              └── Same shared secret! ─────────┘

    Eve knows g, p, A, B — but computing a from A = g^a mod p
    is the "discrete logarithm problem" and is computationally hard.
```

---

## Digital Signatures

Digital signatures prove **who** sent a message and that it **wasn't altered**.

RSA signatures work "backwards" from encryption:
- **Sign**: signature = hash(message)^d mod n (use private key)
- **Verify**: hash' = signature^e mod n (use public key), check hash' == hash(message)

Only the private key holder can create a valid signature, but anyone with the
public key can verify it.

---

## Symmetric vs. Asymmetric Encryption

```
    ┌──────────────────────┬───────────────────────────────┐
    │    Symmetric          │    Asymmetric (Public Key)    │
    ├──────────────────────┼───────────────────────────────┤
    │ Same key encrypts    │ Public key encrypts           │
    │   and decrypts       │ Private key decrypts          │
    │                      │                               │
    │ Fast (AES, ChaCha20) │ Slow (RSA, ECC)               │
    │                      │                               │
    │ Key distribution     │ Key distribution solved       │
    │   is the problem     │   (public key is public!)     │
    │                      │                               │
    │ Used for: bulk data  │ Used for: key exchange,       │
    │                      │   signatures, small data      │
    └──────────────────────┴───────────────────────────────┘
```

In practice (TLS/HTTPS), both are used together:
1. Asymmetric crypto (RSA or Diffie-Hellman) to exchange a **session key**
2. Symmetric crypto (AES) to encrypt the actual data — much faster

---

## TLS Overview (How HTTPS Works)

When you visit `https://example.com`, this happens:

```
    Browser                                    Server
    ───────                                    ──────
    1. ClientHello ──────────────────────────►
       (supported ciphers, random bytes)

    2.                 ◄──────────────────────  ServerHello
       (chosen cipher, random bytes, certificate)

    3. Verify certificate (check CA signature)
       Extract server's public key

    4. Key exchange (Diffie-Hellman or RSA)
       Both sides now have a shared session key

    5. ══════ Encrypted tunnel established ══════
       All data encrypted with AES using session key
```

---

## Exercises

1. **ciphers.c** — Implement classical ciphers: Caesar (encrypt, decrypt,
   crack), XOR, and Vigenère. Learn about frequency analysis.

2. **rsa_mini.c** — Build a mini RSA system with small primes. Implement
   modular exponentiation, GCD, modular inverse, key generation, encrypt,
   and decrypt.

---

## Key Takeaways

- **XOR** is the building block of all modern encryption
- **One-way functions** (hashes, factoring) are the mathematical foundation
- **Public-key crypto** (RSA) solves the key distribution problem
- **Diffie-Hellman** lets two parties create a shared secret over a public channel
- **Real systems** combine asymmetric + symmetric encryption (TLS)
- **Never roll your own crypto** in production — use battle-tested libraries
  (OpenSSL, libsodium). But understanding the fundamentals is essential!
