# Module 17 — Text, Unicode & Encoding

## How Does a Computer Store Text?

A computer only understands numbers — binary 0s and 1s. So to store the letter `A`,
we need to agree on a number for it. That agreement is called a **character encoding**.

The oldest and most fundamental encoding is **ASCII**.

---

## ASCII — The Foundation

**ASCII** (American Standard Code for Information Interchange, 1963) uses **7 bits**
to represent characters. That gives us 2^7 = **128** possible values (0-127).

```
    7 bits:  0 1 1 0 0 0 1  =  97  =  'a'
             ───────────────
             (fits in one byte, with the high bit always 0)
```

### The ASCII Table (Key Ranges)

```
 Dec  Hex  Char  │  Dec  Hex  Char  │  Dec  Hex  Char
─────────────────┼──────────────────┼──────────────────
   0  00   NUL   │   32  20   (sp)  │   64  40    @
   7  07   BEL   │   33  21    !    │   65  41    A
   8  08    BS   │   34  22    "    │   66  42    B
   9  09   TAB   │   ...            │   ...
  10  0A    LF   │   48  30    0    │   90  5A    Z
  13  0D    CR   │   49  31    1    │   91  5B    [
  27  1B   ESC   │   ...            │   ...
  31  1F   (us)  │   57  39    9    │   97  61    a
                 │   ...            │   98  62    b
                 │   63  3F    ?    │   ...
                 │                  │  122  7A    z
                 │                  │  127  7F   DEL
```

### Key Ranges to Remember

| Range     | What's There        | Count |
|-----------|---------------------|-------|
| 0 - 31   | Control characters  | 32    |
| 32        | Space               | 1     |
| 33 - 47   | Punctuation `! " # $ % ...` | 15 |
| 48 - 57   | Digits `0-9`        | 10    |
| 65 - 90   | Uppercase `A-Z`     | 26    |
| 97 - 122  | Lowercase `a-z`     | 26    |
| 127       | DEL                 | 1     |

**Useful tricks in C:**
```c
char c = 'A';
int is_upper = (c >= 'A' && c <= 'Z');       /* check range */
char lower = c + ('a' - 'A');                 /* to lowercase: add 32 */
int digit_value = '7' - '0';                  /* char to int: 7 */
```

ASCII is elegant — but it only covers **English**. What about French (e), German (u),
Japanese (kanji), Arabic, Chinese, emoji?

---

## The Encoding Problem

In the 1980s, every country invented its own encoding for bytes 128-255:

```
  Byte value 0xC4:
    ├── In Latin-1 (Western Europe):   A with diaeresis (Ä)
    ├── In Windows-1251 (Russian):     Cyrillic De (Д)
    └── In Shift-JIS (Japanese):       part of a 2-byte sequence

  Same byte, completely different character!
```

This caused chaos. Send a Russian email to a French computer, and you'd see
garbage characters. This was called **mojibake** — garbled text.

The world needed ONE table that included EVERY writing system.

---

## Unicode — One Table to Rule Them All

**Unicode** (first version 1991) assigns a unique number called a **code point** to
every character in every writing system. Code points are written as `U+` followed
by a hex number:

| Code Point | Character | Name                        |
|------------|-----------|-----------------------------|
| U+0041     | A         | LATIN CAPITAL LETTER A      |
| U+00F1     | n with tilde | LATIN SMALL LETTER N WITH TILDE |
| U+4E16     | (CJK)    | CJK character meaning "world" |
| U+1F600    | (emoji)  | GRINNING FACE               |

As of 2024, Unicode defines over **149,000** characters across **161** scripts.
The maximum code point is U+10FFFF (about 1.1 million possible values).

**But Unicode is NOT an encoding!** Unicode tells you *what number* each character
gets. It doesn't tell you *how to store that number as bytes*. For that, you need
an encoding like UTF-8, UTF-16, or UTF-32.

---

## UTF-8 — The Brilliant Encoding

**UTF-8** (created by Ken Thompson and Rob Pike in 1993) is a **variable-length**
encoding that uses 1 to 4 bytes per character.

### The Bit Patterns

```
 Codepoint range          Byte 1     Byte 2     Byte 3     Byte 4    Bits
─────────────────────── ────────── ────────── ────────── ────────── ──────
 U+0000  ..  U+007F      0xxxxxxx                                    7
 U+0080  ..  U+07FF      110xxxxx   10xxxxxx                         11
 U+0800  ..  U+FFFF      1110xxxx   10xxxxxx   10xxxxxx              16
 U+10000 ..  U+10FFFF    11110xxx   10xxxxxx   10xxxxxx   10xxxxxx   21
```

**Reading the pattern:**

- **1-byte**: starts with `0` — this IS ASCII! Every ASCII string is valid UTF-8.
- **2-byte**: first byte starts with `110`, second with `10`.
- **3-byte**: first byte starts with `1110`, then two `10` continuation bytes.
- **4-byte**: first byte starts with `11110`, then three `10` continuation bytes.

The number of leading 1s in the first byte tells you how many bytes total.
Continuation bytes always start with `10`.

### Encoding Example: Step by Step

Let's encode **U+00F1** (n with tilde, the Spanish letter):

```
  Step 1: U+00F1 = 0000 0000 1111 0001 in binary
                                 (decimal 241)

  Step 2: 241 is in range U+0080..U+07FF, so we need 2 bytes.
          Pattern: 110xxxxx  10xxxxxx  (11 x-bits available)

  Step 3: Take the 11 low bits of 241:
          241 = 000 1111 0001
                ^^^^^ ^^^^^^
                 5 bits  6 bits

  Step 4: Fill the pattern:
          110 00011  10 110001
          ─── ─────  ── ──────
          tag  bits  tag  bits

  Step 5: Result: 0xC3 0xB1  (two bytes)
```

Let's decode those bytes back:

```
  Bytes: C3 B1
         1100 0011   1011 0001

  Step 1: First byte starts with 110 → 2-byte sequence.
  Step 2: Extract data bits:
          110 00011 → 00011
           10 110001 → 110001
  Step 3: Combine: 00011 110001 = 000 1111 0001 = 0xF1
  Step 4: U+00F1 = n with tilde.  Correct!
```

### Why UTF-8 is Brilliant

1. **ASCII compatible**: Any ASCII text is already valid UTF-8. No conversion needed.
2. **Self-synchronizing**: If you jump to a random byte, you can tell immediately:
   - `0xxxxxxx` → this is a complete 1-byte character
   - `10xxxxxx` → this is a continuation byte, scan backwards for the start
   - `110xxxxx` → start of a 2-byte character
   - `1110xxxx` → start of a 3-byte character
   - `11110xxx` → start of a 4-byte character
3. **No embedded NULs**: The null byte 0x00 only appears for U+0000. C string
   functions (strlen, strcpy) work without modification.
4. **Sorted order preserved**: Byte-wise sorting of UTF-8 gives the same order as
   codepoint sorting (for the same number of bytes).

### Why UTF-8 Won — And Others Didn't

UTF-8 wasn't the first Unicode encoding — UTF-16 and UTF-32 came first. So why
does almost the entire internet use UTF-8 today? Here's the story:

**The problem UTF-8 solved**: In the early 1990s, the world had hundreds of
incompatible text encodings — Shift-JIS for Japanese, KOI8-R for Russian,
ISO-8859-1 for Western European. If you opened a Japanese file with a Russian
encoding, you'd see garbage (**mojibake**). Unicode was the solution — one
universal character set — but the question was: *how do you encode it as bytes?*

```
  The three contenders:

  Encoding   English 'A'   Storage     Works with       Byte order
                           per char    existing C code?  problem?
  ─────────  ─────────────  ─────────  ────────────────  ──────────
  UTF-32     00 00 00 41    4 bytes    NO (NUL bytes     YES (BOM)
                            always     break strlen)
  UTF-16     00 41          2-4 bytes  NO (same issue)   YES (BOM)
  UTF-8      41             1-4 bytes  YES!              NO!
```

**Why UTF-32 lost**: It wastes 4 bytes for every character. An English text
file becomes 4x larger. Unacceptable for storage and bandwidth.

**Why UTF-16 lost** (for most purposes):
- It **embeds NUL bytes** (0x00) in ordinary text. For example, 'A' is encoded
  as `00 41`. C string functions like `strlen()` see the `0x00` and think the
  string ended. Every C string function would need to be rewritten.
- It has a **byte-order problem**: is 'A' stored as `00 41` (big-endian) or
  `41 00` (little-endian)? You need a special marker called a **BOM** (Byte
  Order Mark) at the start of every file to tell the reader which order to use.
- It's *still* variable-width (2 or 4 bytes), so you don't get true random access.

**Why UTF-8 won**:
- **Backward compatible**: Every existing ASCII file is already valid UTF-8.
  Billions of existing text files, configs, source code — they all just work.
  No conversion, no migration, no breakage.
- **No NUL bytes in normal text**: C code, Unix tools, network protocols —
  everything that treats `0x00` as a terminator works without modification.
- **No byte-order issues**: UTF-8 is a stream of bytes, not multi-byte integers.
  There's only one way to read it. No BOM needed.
- **Space efficient for Latin text**: English uses 1 byte per character (same
  as ASCII). Chinese/Japanese use 3 bytes. This is a great trade-off for the
  internet, which uses a lot of ASCII (HTML tags, URLs, headers).

The result: UTF-8 now accounts for over 98% of web pages. It is the default
encoding for Linux, macOS, most programming languages, JSON, and the web.

---

## UTF-16 and UTF-32

### UTF-32

The simplest encoding: every code point is stored as a **4-byte (32-bit)** integer.

```
  U+0041  →  00 00 00 41   (4 bytes for 'A' — wasteful!)
  U+1F600 →  00 01 F6 00   (4 bytes for the emoji)
```

**Pro**: Random access — character N is always at byte offset N*4.
**Con**: Wastes space. English text takes 4x the memory compared to ASCII.

### UTF-16

Uses **2 bytes** for most characters, **4 bytes** for characters above U+FFFF
(using pairs called **surrogates**).

```
  U+0041  →  00 41          (2 bytes)
  U+1F600 →  D8 3D DE 00    (4 bytes — surrogate pair)
```

Java and JavaScript use UTF-16 internally. Windows APIs use UTF-16.

---

## Endianness and BOM

With multi-byte encodings (UTF-16, UTF-32), the **byte order** matters.

### Big-Endian vs Little-Endian

```
  The number 0x1234 stored in memory:

  Big-Endian (BE):     12  34      (most significant byte first)
  Little-Endian (LE):  34  12      (least significant byte first)

  Think of it like writing a number:
    Big-Endian:     "twelve thirty-four"   (big part first)
    Little-Endian:  "thirty-four twelve"   (little part first)
```

**Example**: The character U+FEFF in UTF-16:

```
  UTF-16 BE:  FE FF
  UTF-16 LE:  FF FE
```

### Byte Order Mark (BOM)

A **BOM** is the character U+FEFF placed at the start of a file to signal which
byte order is being used:

```
  File starts with FF FE → "I'm UTF-16 Little-Endian"
  File starts with FE FF → "I'm UTF-16 Big-Endian"
  File starts with EF BB BF → "I'm UTF-8" (optional, often discouraged)
```

UTF-8 doesn't need a BOM because byte order doesn't matter — it's a stream of
single bytes. But some Windows tools add one anyway.

---

## String Handling in C with Multi-Byte Characters

Here's the critical insight: **C's `strlen()` counts bytes, not characters!**

```
  const char *s = "Hello";    /* 5 bytes, 5 characters — same */

  /* But with UTF-8: */
  /* "n~" (U+00F1) encoded as 0xC3 0xB1 — 2 bytes, 1 character */
  char s2[] = {0xC3, 0xB1, 0x00};
  strlen(s2);  /* returns 2 (bytes), but it's 1 character! */

  /* "world" in Chinese (U+4E16) — 3 bytes, 1 character */
  char s3[] = {0xE4, 0xB8, 0x96, 0x00};
  strlen(s3);  /* returns 3 (bytes), but it's 1 character! */
```

### The Problem with Standard C String Functions

| Function    | What it does          | UTF-8 safe? |
|-------------|-----------------------|-------------|
| `strlen()`  | Counts bytes          | Misleading  |
| `strcpy()`  | Copies bytes          | Safe        |
| `strcmp()`   | Compares bytes        | Works for equality, not locale-aware sorting |
| `s[i]`      | Access byte i         | Might be in the middle of a character! |
| `toupper()` | Uppercases one byte   | Only works for ASCII |

**This is why you'll build your own UTF-8 functions in the exercise!**

To properly work with UTF-8 strings in C, you need to:
1. Walk byte-by-byte, checking the bit patterns
2. Count codepoints (not bytes) for "string length"
3. Validate that byte sequences are well-formed
4. Never split a multi-byte sequence in the middle

---

## Exercises

| File           | What You'll Build                             | Tests |
|----------------|-----------------------------------------------|-------|
| utf8.c         | UTF-8 encoder, decoder, strlen, validator     | 16    |
| charcount.c    | UTF-8 character counter & navigator           | 18    |
| text_stats.c   | Word/line counting, case conversion           | 18    |

### utf8.c — UTF-8 from Scratch

You'll implement four functions:

- **`utf8_encode`**: Convert a Unicode code point to UTF-8 bytes
- **`utf8_decode`**: Read UTF-8 bytes and extract the code point
- **`utf8_strlen`**: Count the number of characters (not bytes!) in a UTF-8 string
- **`utf8_validate`**: Check whether a byte sequence is valid UTF-8

This is real-world code — every text editor, web browser, and database does
exactly this internally.

### charcount.c — UTF-8 Character Counter

C's `strlen()` counts bytes, not characters. You'll build functions that
correctly count and navigate multi-byte UTF-8 characters:

- **`utf8_char_width`**: Given a first byte, determine how many bytes the character uses (1-4)
- **`utf8_char_count`**: Count characters (not bytes) in a UTF-8 string
- **`utf8_byte_count`**: Find the byte offset of the Nth character
- **`is_ascii`**: Check whether a string is pure ASCII

### text_stats.c — Text Statistics

Build a set of text analysis functions that work on ASCII characters
(and are safe for UTF-8 strings since they leave non-ASCII bytes untouched):

- **`count_words`**: Count whitespace-separated words
- **`count_lines`**: Count lines (based on `\n` characters)
- **`to_uppercase`** / **`to_lowercase`**: Convert ASCII letters in place
- **`longest_word_length`**: Find the length of the longest word

---

[← Previous: Module 16: File I/O & the Unix Philosophy](../16-file-io-unix-philosophy/README.md) | [Next: Module 18: Date & Time →](../18-date-time/README.md)
