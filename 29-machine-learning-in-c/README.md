# Module 29: Machine Learning in C

## What is Machine Learning?

Machine learning is teaching a computer to **find patterns in data** instead of
giving it explicit rules. Instead of writing `if temperature > 30: hot`, you
show the computer thousands of examples and it *learns* the boundary itself.

At its core, ML is just **math** — linear algebra, calculus, and optimization.
That's what makes C a great language to learn it in: no magic libraries hiding
the details. You'll build everything from scratch.

---

## 1. Linear Algebra Basics

All of ML is built on vectors and matrices. Let's start there.

**Why do vectors and matrices matter in ML?** In ML, data is represented as
vectors. A single data point (e.g., a house) has features like
[square_feet, num_bedrooms, price] — that's a 3D vector. A dataset of 1000
houses is a matrix with 1000 rows and 3 columns. Neural networks are
essentially matrix multiplications followed by simple functions. Each layer
multiplies the input vector by a weight matrix. That's why linear algebra is
the language of ML.

### Vectors

A **vector** is just an ordered list of numbers:

```
v = [3.0, 1.5, 2.0]
```

Think of it as a point in space, or a direction with magnitude.

### Dot Product

The **dot product** of two vectors multiplies matching elements and sums them:

```
a = [1, 2, 3]
b = [4, 5, 6]

a · b = (1×4) + (2×5) + (3×6) = 4 + 10 + 18 = 32
```

This single number tells you how "aligned" two vectors are. It's the most
important operation in neural networks — every neuron computes a dot product.

### Matrices

A **matrix** is a 2D grid of numbers. We store them in row-major order:

```
Matrix A (2×3):             In memory (row-major):
┌              ┐
│  1   2   3   │            [1, 2, 3, 4, 5, 6]
│  4   5   6   │             ───────  ───────
└              ┘             row 0    row 1

Element at row r, col c:
  index = r * cols + c
  A[1][2] = data[1*3 + 2] = data[5] = 6
```

### Matrix Multiplication

To multiply matrix A (m×n) by matrix B (n×p), the result is C (m×p).
Each element C[i][j] is the **dot product** of row i from A and column j from B:

```
A (2×3)           B (3×2)           C (2×2)
┌          ┐     ┌       ┐     ┌             ┐
│ 1  2  3  │     │ 7  8  │     │  58    64   │
│ 4  5  6  │  ×  │ 9  10 │  =  │ 139   154   │
└          ┘     │ 11 12 │     └             ┘
                 └       ┘

C[0][0] = 1×7 + 2×9 + 3×11 = 7 + 18 + 33 = 58
C[0][1] = 1×8 + 2×10 + 3×12 = 8 + 20 + 36 = 64
C[1][0] = 4×7 + 5×9 + 6×11 = 28 + 45 + 66 = 139
C[1][1] = 4×8 + 5×10 + 6×12 = 32 + 50 + 72 = 154
```

**Key rule**: inner dimensions must match. (2×**3**) × (**3**×2) = (2×2).

Think of matrix multiplication as computing dot products row-by-column.
Row i of the result = dot product of row i of the first matrix with each
column of the second matrix.

### Transpose

Flipping a matrix over its diagonal — rows become columns:

```
A (2×3):              A^T (3×2):
┌          ┐          ┌       ┐
│ 1  2  3  │          │ 1  4  │
│ 4  5  6  │    →     │ 2  5  │
└          ┘          │ 3  6  │
                      └       ┘
```

---

## 2. Gradient Descent

Gradient descent is the engine behind all ML training. The idea is simple:

> You're standing on a hilly landscape in thick fog. You can't see the bottom
> of the valley, but you can feel the slope under your feet. So you take a
> step downhill. Repeat until you reach the lowest point.

```
  Loss
   ▲
   │
   │  ╲
   │   ╲       ● ← You are here
   │    ╲     ╱
   │     ╲   ╱    Step downhill
   │      ╲ ╱     (subtract gradient × learning rate)
   │       ●
   │      ╱ ╲
   │     ╱   ╲        ● ← Getting closer
   │    ╱     ╲      ╱
   │   ╱       ╲    ╱
   │  ╱         ╲  ╱
   │ ╱           ╲╱ ← Minimum! (goal)
   │╱
   └──────────────────────────► Parameter value
```

### The Algorithm

1. **Start** with a random guess for your parameters (weights)
2. **Compute the loss** — how wrong your prediction is
3. **Compute the gradient** — which direction makes the loss worse
4. **Update**: `parameter = parameter - learning_rate × gradient`
5. **Repeat** until the loss is small enough

### Learning Rate

The **learning rate** controls step size:

```
  Too large (lr = 1.0):         Too small (lr = 0.0001):    Just right (lr = 0.01):
  ●                              ●                           ●
   ╲                              ╲                           ╲
    ╲    ● overshoots!             ● tiny step                 ╲
     ╲  ╱ ╲                         ● tiny step                 ●
      ╲╱   ● diverges!              ● ...still going             ╲
       ●                             ● ...forever                 ●  ← converged!
```

---

## 3. Linear Regression

The simplest ML model: fit a line to data.

**Model**: `y = mx + b` (slope `m`, intercept `b`)

**Loss function** (Mean Squared Error):

```
MSE = (1/n) × Σ (predicted - actual)²
```

Why square the error instead of using absolute value? Two reasons:
(1) Squaring penalizes large errors MORE than small ones — an error of 10 is
100x worse than an error of 1, which is exactly what we want.
(2) The squared function is smooth and differentiable everywhere, which means
we can compute gradients for gradient descent. Absolute value has a sharp
corner at zero where the gradient is undefined.

**Gradients** (how to update m and b):

```
∂MSE/∂m = (2/n) × Σ (predicted - actual) × x
∂MSE/∂b = (2/n) × Σ (predicted - actual)
```

**Training loop**:
```
for each iteration:
    prediction = m * x + b
    error = prediction - y
    m = m - learning_rate * gradient_m
    b = b - learning_rate * gradient_b
```

---

## 4. Neural Networks from First Principles

A neural network is just layers of simple math operations stacked together.

### Architecture

```
    INPUT LAYER         HIDDEN LAYER         OUTPUT LAYER
   (2 neurons)          (2 neurons)          (1 neuron)

    ┌─────┐         ┌──────────────┐        ┌──────────────┐
    │     │─── w1 ──│  Σ(w·x + b) │        │              │
    │ x₁  │╲       │  sigmoid()   │── w5 ──│  Σ(w·x + b) │
    │     │ ╲ w2   │   = h₁       │╲       │  sigmoid()   │
    └─────┘  ╲     └──────────────┘ ╲      │   = output   │
              ╲                      ╲     └──────────────┘
               ╲   ┌──────────────┐  ╲    ╱
                ╲──│  Σ(w·x + b) │── w6 ╱
    ┌─────┐   ╱   │  sigmoid()   │    ╱
    │     │──╱ w3  │   = h₂       │──╱
    │ x₂  │╱      └──────────────┘
    │     │── w4 ──╱
    └─────┘
```

Each neuron does three things:
1. **Weighted sum**: multiply each input by a weight, add them up, add a bias
2. **Activation**: pass the sum through a non-linear function (sigmoid)
3. **Output**: send the result to the next layer

### The Sigmoid Activation Function

```
sigmoid(x) = 1 / (1 + e^(-x))
```

It squashes any number into the range (0, 1):

```
  Output
  1.0 ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ────────
                                              ╱
                                           ╱
  0.5 ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ●╱─ ─ ─ ─ ─ ─ ─
                                     ╱
                                  ╱
  0.0 ──────── ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─
      -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6
                                Input (x)
```

Why sigmoid? Without a non-linear function, stacking layers of linear math
just gives you... more linear math. Sigmoid lets the network learn curves
and complex boundaries.

### Forward Pass

The forward pass computes the output from inputs:

```
For each neuron in hidden layer:
    h[j] = sigmoid( Σ(inputs[i] × weights_ih[i][j]) + bias_h[j] )

For each neuron in output layer:
    out[k] = sigmoid( Σ(hidden[j] × weights_ho[j][k]) + bias_o[k] )
```

### Backpropagation (The Chain Rule)

Training means adjusting weights to reduce error. **Backpropagation** does this
by flowing the error **backwards** through the network.

Here's the intuition: imagine a factory assembly line with three workers. The
final product has a defect. How much is each worker responsible? You trace
backwards: if the last worker made the product 20% worse, and the middle
worker's contribution was amplified by 3x through the last worker, then the
middle worker is 3 x 20% = 60% responsible. That's the chain rule — you
multiply the "responsibility" (gradient) backwards through each stage.

Backpropagation is just the chain rule applied systematically from the output
layer back to the input layer. Each layer computes: "how much did MY weights
contribute to the final error?" Then it adjusts its weights to reduce that
contribution.

```
  FORWARD PASS (compute outputs):
  ─────────────────────────────────────────────────────────►

  inputs ──► hidden layer ──► output layer ──► loss = (predicted - actual)²

  ◄─────────────────────────────────────────────────────────
  BACKWARD PASS (compute gradients):

  ∂loss/∂output  →  ∂loss/∂weights_ho  →  ∂loss/∂hidden  →  ∂loss/∂weights_ih
       │                    │                    │                    │
       ▼                    ▼                    ▼                    ▼
  output error     update output weights   hidden error    update hidden weights
```

The **chain rule** from calculus makes this work:

```
∂loss        ∂loss     ∂output     ∂net_output
────────── = ────── × ────────── × ───────────
∂weight_ho   ∂output   ∂net_output  ∂weight_ho

Where:
  ∂loss/∂output      = 2 × (predicted - actual)     ← how loss changes with output
  ∂output/∂net       = sigmoid'(net) = out×(1-out)   ← sigmoid derivative
                        (This elegant formula comes from calculus — the
                         derivative of 1/(1+e^(-x)). The beautiful part: you
                         don't need the original input x to compute the
                         derivative — you only need the OUTPUT. Since you
                         already computed the output during the forward pass,
                         computing the gradient during backpropagation is
                         almost free.)
  ∂net/∂weight       = hidden_value                   ← input to this weight
```

### The XOR Problem

XOR is the classic neural network test case because it's **not linearly
separable** — you can't draw a single straight line to separate 0s from 1s.

Compare XOR with AND to see why:

```
  AND (linearly separable):     XOR (NOT linearly separable):

    1 | - | +                     1 | + | -
    0 | - | -                     0 | - | +
      0   1                         0   1

  A single line can separate     No single line can separate
  the + from the - cases.        + from - cases!
```

Here's XOR plotted with actual values:

```
  x₂
  1 ┤  ●(0,1)=1         ●(1,1)=0
    │
    │       No single line works!
    │       Need a hidden layer.
  0 ┤  ●(0,0)=0         ●(1,0)=1
    └──┬─────────────────┬──── x₁
       0                 1
```

A hidden layer solves this by TRANSFORMING the inputs into a new space where
the problem IS linearly separable. The hidden neurons learn to create new
features that make the separation possible.

---

## 5. Loss Curve

During training, you plot the loss over iterations. A healthy training run
looks like this:

```
  Loss
   ▲
  1.0│●
     │ ●
     │  ●
  0.8│   ●
     │    ●
     │     ●
  0.6│      ●
     │       ●
     │        ●●
  0.4│          ●●
     │            ●●
     │              ●●●
  0.2│                 ●●●●
     │                     ●●●●●●
  0.1│                            ●●●●●●●●●●●●●●
     │
  0.0└─────────────────────────────────────────────►
     0    1000   2000   3000   4000   5000   6000
                    Training iterations
```

If the loss **goes up** or **oscillates wildly**, your learning rate is too high.
If it barely moves, your learning rate is too low.

**Important caveat**: gradient descent is guaranteed to find the minimum only
for CONVEX functions (bowl-shaped — one lowest point). Neural network loss
landscapes are NOT convex — they have many hills and valleys (local minima).
In practice, gradient descent works surprisingly well for neural networks
despite this, partly because most local minima in high-dimensional spaces are
"good enough." But this is why training the same network twice can give
different results — you might end up in different valleys.

---

## Exercises

### Exercise 1: `matrix.c` — Matrix Math Library

Build a matrix library with:
- Create/destroy matrices (heap-allocated)
- Get/set elements
- Matrix multiply, transpose, add, scale

This is the foundation — neural networks are just matrix operations.

### Exercise 2: `neural_net.c` — Neural Network

Build a single-hidden-layer neural network:
- Sigmoid activation function
- Forward pass
- Train on XOR data using backpropagation

---

## Key Takeaways

1. **ML is math** — linear algebra + calculus + optimization
2. **Gradient descent** finds the minimum of a function by following the slope downhill
3. **Neural networks** are layers of weighted sums + non-linear activations
4. **Backpropagation** uses the chain rule to compute how each weight affects the loss
5. **XOR** proves you need hidden layers — a single layer can only learn linear boundaries

---

[← Previous: Module 28c — Ray Tracing & 3D Graphics](../28c-ray-tracing-3d-graphics/README.md) | [Next: Module 30 — Capstone Project →](../30-capstone-project/README.md)
