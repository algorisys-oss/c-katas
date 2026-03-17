# Module 25: Parsing & the SQL Engine

## Overview

Every programming language, every configuration file, every query you type into a
database goes through the same journey: a string of characters gets broken into
meaningful chunks, those chunks get organized into a tree structure, and that tree
gets executed. This module teaches you that journey by building two things:

1. **An expression evaluator** — parse and evaluate `3 + 4 * (2 - 1)`
2. **A SQL tokenizer** — break SQL statements into tokens, the first stage of a
   database engine

By the end, you'll understand how parsers work from the ground up.

---

## The Parsing Pipeline

Every language processor follows this pipeline:

```
  Source Text         Tokens            AST              Result
 ┌───────────┐    ┌───────────┐    ┌───────────┐    ┌───────────┐
 │ "SELECT   │    │ KEYWORD   │    │  SELECT   │    │           │
 │  name     │───>│ IDENT     │───>│  ├─ cols  │───>│  Output   │
 │  FROM     │    │ KEYWORD   │    │  │  └name  │    │  Rows     │
 │  users    │    │ IDENT     │    │  └─ from  │    │           │
 │  WHERE    │    │ KEYWORD   │    │     └users │    │           │
 │  age > 5" │    │ IDENT     │    │           │    │           │
 │           │    │ OPERATOR  │    │           │    │           │
 │           │    │ NUMBER    │    │           │    │           │
 └───────────┘    └───────────┘    └───────────┘    └───────────┘
   Tokenizer         Parser          Executor
  (lexer/scanner)  (syntax analysis) (evaluation)
```

### Stage 1: Tokenizer (Lexer)

The tokenizer reads raw characters and groups them into **tokens** — the smallest
meaningful units. It handles:

- **Skipping whitespace** — spaces, tabs, newlines are separators, not meaning
- **Recognizing keywords** — `SELECT`, `FROM`, `WHERE` are special
- **Identifying numbers** — sequences of digits like `42`, `100`
- **Identifying strings** — text in quotes like `'hello'`
- **Recognizing operators** — `=`, `>`, `<`, `>=`, `<=`, `!=`
- **Single-character tokens** — `(`, `)`, `,`, `*`, `;`

```
  Input: "SELECT name, age FROM users WHERE age > 18;"

  Tokenizer output:
  ┌──────────┬──────────┐
  │ Type     │ Value    │
  ├──────────┼──────────┤
  │ KEYWORD  │ SELECT   │
  │ IDENT    │ name     │
  │ COMMA    │ ,        │
  │ IDENT    │ age      │
  │ KEYWORD  │ FROM     │
  │ IDENT    │ users    │
  │ KEYWORD  │ WHERE    │
  │ IDENT    │ age      │
  │ OPERATOR │ >        │
  │ NUMBER   │ 18       │
  │ SEMICOLON│ ;        │
  └──────────┴──────────┘
```

**How does the tokenizer decide?** It looks at one character at a time:

- Is it a letter? Read the whole word, then check if it's a keyword.
- Is it a digit? Read all digits to form a number.
- Is it a quote? Read until the closing quote.
- Is it `>`, `<`, `!`, `=`? Check the next char for two-char operators.
- Is it `(`, `)`, `,`, `*`, `;`? Emit a single-character token.
- Is it whitespace? Skip it.

### Stage 2: Parser

The parser takes the flat list of tokens and builds a **tree** that reflects the
structure (grammar) of the language. This tree is called an **Abstract Syntax Tree
(AST)**.

For arithmetic expressions, the grammar encodes **operator precedence**:

```
  Expression grammar (recursive descent):

  expr   → term (('+' | '-') term)*
  term   → factor (('*' | '/') factor)*
  factor → NUMBER | '(' expr ')'
```

This grammar ensures `*` and `/` bind tighter than `+` and `-`:

```
  Input: "3 + 4 * 2"

  Parse tree (AST):
          [+]
         /   \
       [3]   [*]
             / \
           [4] [2]

  Evaluation: 4 * 2 = 8, then 3 + 8 = 11 ✓
  (NOT: 3 + 4 = 7, then 7 * 2 = 14 ✗)
```

**Recursive descent** means each grammar rule becomes a function:
- `parse_expr()` handles `+` and `-`
- `parse_term()` handles `*` and `/`
- `parse_factor()` handles numbers and parenthesized sub-expressions

Each function calls the one below it, which naturally gives higher-precedence
operators their tighter binding.

### Stage 3: Executor

The executor walks the AST and produces results. For arithmetic, it evaluates
the tree bottom-up. For SQL, it would scan tables, filter rows, and return
matching data.

---

## SQL: A Sample AST

Here is what the AST for a `SELECT` statement looks like:

```
  SQL: "SELECT name, age FROM users WHERE age > 18"

  AST:
  ┌────────────────────────────┐
  │       SELECT (node)        │
  ├────────────────────────────┤
  │ columns:                   │
  │   ├── "name"               │
  │   └── "age"                │
  │                            │
  │ table: "users"             │
  │                            │
  │ where:                     │
  │   ┌──────────┐             │
  │   │    >     │             │
  │   │   / \    │             │
  │   │ age  18  │             │
  │   └──────────┘             │
  └────────────────────────────┘
```

This module focuses on **Stage 1 (tokenizer)** and builds a complete
**recursive descent evaluator** for arithmetic expressions (Stages 1-3 combined).

---

## The Relational Model (Brief)

SQL operates on **tables** (also called relations). Each table has:
- **Columns** (fields) — like `name`, `age`, `email`
- **Rows** (records/tuples) — each row is one entry

Core SQL statements:
- `CREATE TABLE users (name, age)` — define a table
- `INSERT INTO users VALUES ('Alice', 30)` — add a row
- `SELECT name FROM users WHERE age > 25` — query rows

---

## Exercises

| File            | What You Build                          | Tests |
|-----------------|-----------------------------------------|-------|
| `tokenizer.c`  | SQL tokenizer — break SQL into tokens   | ~12   |
| `eval.c`        | Arithmetic expression evaluator         | ~10   |

### Building

```bash
make exercises    # compile exercise stubs (will fail until you fill them in)
make solutions    # compile reference solutions
make test         # build and run solutions to verify correctness
```

---

## Key Takeaways

1. **Tokenizers** are simple state machines — look at one character, decide what
   to do. No need for regex or complex libraries.
2. **Recursive descent** is the most intuitive parsing technique — each grammar
   rule becomes a function.
3. **Operator precedence** is encoded in the grammar structure — lower-precedence
   operators are parsed at higher levels.
4. **Every language** you use (C, Python, SQL, JSON, HTML) goes through this
   same tokenizer -> parser -> AST -> executor pipeline.
