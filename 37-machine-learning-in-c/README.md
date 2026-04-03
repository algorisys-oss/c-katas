# Module 37: Machine Learning in C

## What is Machine Learning?

Machine learning is teaching a computer to **find patterns in data** instead of
giving it explicit rules. Instead of writing `if temperature > 30: hot`, you
show the computer thousands of examples and it *learns* the boundary itself.

At its core, ML is just **math** — linear algebra, calculus, and optimization.
That's what makes C a great language to learn it in: no magic libraries hiding
the details. You'll build everything from scratch.

**Good news**: you don't need a math degree. You need three things:
1. Vectors and matrices (fancy words for lists and grids of numbers)
2. One idea from calculus (slope of a curve — we'll build it from scratch)
3. Gradient descent (walking downhill to find the best answer)

That's it. Let's go.

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

## 2. The One Calculus Idea You Need

You know slope from grade 9 math:

```
slope = rise / run = (y2 - y1) / (x2 - x1)
```

For a straight line, the slope is the same everywhere. The line `y = 3x + 1`
has slope 3 — for every 1 you move right, you go up 3.

But what about a **curve**? The slope of `y = x²` is different at every point.
Near x=0 it's nearly flat. At x=3 it's steep. The slope *changes*.

```
  y
  ▲
  │                              ╱  steep here
  │                           ╱
  │                        ╱
  │                     ●  ← at x=3, slope = 6
  │                  ╱╱
  │              ╱╱
  │          ╱╱
  │      ●╱  ← at x=1, slope = 2
  │   ╱╱
  │ ●  ← at x=0, slope = 0 (flat!)
  └──────────────────────────────► x
```

### The Derivative = Slope at a Point

The **derivative** is just the slope of a curve at one specific point. How do
you find it? You zoom in until the curve looks like a straight line, then
measure the slope of that line.

In practice, you compute it by nudging:

```
slope at x  ≈  ( f(x + tiny) - f(x) ) / tiny
```

Let's try it. What's the slope of f(x) = x² at x = 3?

```
  f(3)       = 3²       = 9
  f(3.001)   = 3.001²   = 9.006001

  slope ≈ (9.006001 - 9) / 0.001
        = 0.006001 / 0.001
        = 6.001

  The slope of x² at x = 3 is approximately 6.
```

Try it yourself at x = 5:

```
  f(5)       = 25
  f(5.001)   = 25.010001

  slope ≈ (25.010001 - 25) / 0.001
        = 0.010001 / 0.001
        = 10.001

  The slope of x² at x = 5 is approximately 10.
```

See the pattern? The slope of x² at any point x is **2x**. At x=3 it's 6. At
x=5 it's 10. This is written as:

```
  If f(x) = x², then the derivative f'(x) = 2x
```

You don't need to prove this — just know the rule. Here are the only rules
you need for ML:

```
  Function        Derivative        Example
  ─────────────   ─────────────     ────────────────────────
  f(x) = c        f'(x) = 0        f(x) = 7 → f'(x) = 0
  f(x) = cx       f'(x) = c        f(x) = 3x → f'(x) = 3
  f(x) = x²       f'(x) = 2x       at x=4: f'(4) = 8
  f(x) = x^n      f'(x) = n·x^(n-1)
```

**That's it.** You now know enough calculus for ML.

### Partial Derivatives: One Variable at a Time

What if you have a function with TWO inputs, like f(x, y) = x² + 3y?

A **partial derivative** answers: "how does f change when I wiggle ONLY ONE
input, keeping the other fixed?"

```
  f(x, y) = x² + 3y

  ∂f/∂x = 2x        (treat y as a constant — 3y doesn't have x in it,
                      so it vanishes, just like the derivative of 7 is 0)

  ∂f/∂y = 3          (treat x as a constant — x² doesn't have y in it,
                      so it vanishes, and 3y has derivative 3)
```

The ∂ symbol (a curly d) just means "partial derivative." It tells you: focus
on one variable, ignore the others.

Let's verify with a numerical nudge. Take f(x, y) = x² + 3y at the point
(x=2, y=5):

```
  f(2, 5)       = 4 + 15           = 19
  f(2.001, 5)   = 4.004001 + 15    = 19.004001

  ∂f/∂x ≈ (19.004001 - 19) / 0.001 = 4.001 ≈ 4

  Check: 2x at x=2 = 4  ✓
```

```
  f(2, 5)       = 4 + 15           = 19
  f(2, 5.001)   = 4 + 15.003       = 19.003

  ∂f/∂y ≈ (19.003 - 19) / 0.001 = 3.0

  Check: derivative of 3y is 3  ✓
```

**Why does this matter for ML?** Your model has many parameters (weights). The
loss is a function of ALL of them. To improve weight w1, you compute ∂loss/∂w1
— how much does the loss change when you nudge w1 alone? Then you adjust w1 to
make the loss go down. That's gradient descent.

---

## 3. Gradient Descent

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
   └──────────────────────────────► Parameter value
```

### The Algorithm

1. **Start** with a random guess for your parameters (weights)
2. **Compute the loss** — how wrong your prediction is
3. **Compute the gradient** — which direction makes the loss worse
4. **Update**: `parameter = parameter - learning_rate × gradient`
5. **Repeat** until the loss is small enough

### Why Subtract?

The gradient points **uphill** (direction of steepest increase). You want to go
**downhill** (decrease the loss). So you go in the *opposite* direction — you
subtract.

If the gradient is positive (slope goes up to the right), subtracting moves
you left. If the gradient is negative (slope goes up to the left), subtracting
moves you right. Either way, you move toward lower loss.

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

### A Concrete Example

Let's minimize f(x) = x² (find the x where f is smallest — obviously x=0,
but let the algorithm discover it).

```
  Start: x = 6.0, learning_rate = 0.1
  Derivative of x² is 2x.

  Step 1: gradient = 2 × 6.0 = 12.0
           x = 6.0 - 0.1 × 12.0 = 4.8
           loss = 4.8² = 23.04

  Step 2: gradient = 2 × 4.8 = 9.6
           x = 4.8 - 0.1 × 9.6 = 3.84
           loss = 3.84² = 14.75

  Step 3: gradient = 2 × 3.84 = 7.68
           x = 3.84 - 0.1 × 7.68 = 3.072
           loss = 3.072² = 9.44

  ...after 20 steps: x ≈ 0.07, loss ≈ 0.005
  ...after 50 steps: x ≈ 0.0001, loss ≈ basically 0
```

It found the minimum by just following the slope downhill. No algebra tricks,
no solving equations — just nudge, check, nudge, check.

---

## 4. Linear Regression — Your First ML Model

Before neural networks, let's build the simplest possible ML model: a
straight line that fits data.

### The Setup

Say you have 5 students. You know how many hours each one studied and what
score they got:

```
  Hours (x)  │  Score (y)
  ───────────┼────────────
      1      │     2
      2      │     4
      3      │     5
      4      │     4
      5      │     5
```

You want to predict the score from the hours. Your model is a line:

```
  y_pred = w * x + b
```

Where **w** (weight, same as slope) and **b** (bias, same as y-intercept) are
the parameters you need to find.

```
  Score
   ▲
  5 ┤        ●           ●       ● ← actual data
    │     ●        ●
  4 ┤
    │                        ╱ ← we want to find this line
  3 ┤                     ╱
    │                  ╱
  2 ┤  ●            ╱
    │            ╱
  1 ┤         ╱
    │      ╱
  0 ┤   ╱
    └──┬──┬──┬──┬──┬──┬──► Hours
       1  2  3  4  5  6
```

### The Loss Function

How do you measure "how wrong" the line is? **Mean Squared Error (MSE)**:

```
  MSE = (1/n) × sum of (prediction - actual)² for all data points
```

Why square the error? Two reasons:
1. Squaring penalizes big mistakes MORE than small ones. An error of 10
   counts as 100, while an error of 1 counts as 1. This is what we want.
2. The squared function is smooth everywhere, so we can compute gradients.

### The Gradients

We need ∂MSE/∂w and ∂MSE/∂b — how does the loss change when we nudge w or b?

For a single data point (x, y) with prediction p = w*x + b:
- The error is (p - y)
- The loss for this point is (p - y)²
- ∂loss/∂w = 2 * (p - y) * x   (because p depends on w through w*x)
- ∂loss/∂b = 2 * (p - y) * 1   (because p depends on b directly)

For the full MSE (average over all points):

```
  ∂MSE/∂w = (2/n) × sum of (prediction - actual) × x
  ∂MSE/∂b = (2/n) × sum of (prediction - actual)
```

### Worked Example: One Step of Training

Let's train with learning rate 0.1.

**Start**: w = 0, b = 0 (random starting point — everything predicts 0)

**Compute predictions and errors for all 5 points**:

```
  x=1: pred = 0×1 + 0 = 0, actual = 2, error = 0-2 = -2
  x=2: pred = 0×2 + 0 = 0, actual = 4, error = 0-4 = -4
  x=3: pred = 0×3 + 0 = 0, actual = 5, error = 0-5 = -5
  x=4: pred = 0×4 + 0 = 0, actual = 4, error = 0-4 = -4
  x=5: pred = 0×5 + 0 = 0, actual = 5, error = 0-5 = -5
```

**Compute gradients**:

```
  ∂MSE/∂w = (2/5) × [(-2×1) + (-4×2) + (-5×3) + (-4×4) + (-5×5)]
          = (2/5) × [-2 + (-8) + (-15) + (-16) + (-25)]
          = (2/5) × (-66)
          = -26.4

  ∂MSE/∂b = (2/5) × [(-2) + (-4) + (-5) + (-4) + (-5)]
          = (2/5) × (-20)
          = -8.0
```

**Update weights**:

```
  w = 0 - 0.1 × (-26.4) = 0 + 2.64 = 2.64
  b = 0 - 0.1 × (-8.0)  = 0 + 0.80 = 0.80
```

**After one step**: w=2.64, b=0.80. The line is now y = 2.64x + 0.80. That's
already too steep (it overshoots), but subsequent steps will correct it. After
many iterations, it converges to something like w=0.7, b=1.5 — a reasonable
fit.

### The Training Loop

```
for 10000 iterations:
    total_gw = 0, total_gb = 0
    for each data point (x, y):
        prediction = w * x + b
        error = prediction - y
        total_gw += error * x
        total_gb += error
    gradient_w = (2/n) * total_gw
    gradient_b = (2/n) * total_gb
    w = w - learning_rate * gradient_w
    b = b - learning_rate * gradient_b
```

This is the COMPLETE training algorithm for linear regression. No magic. You
compute the predictions, measure how wrong they are, figure out which direction
to nudge each parameter, and nudge them. Repeat.

---

## 5. Neural Networks from First Principles

A neural network is just layers of simple math operations stacked together.
Think of it as stacking multiple linear regressions and putting "bends" between
them.

### Why Not Just Use Linear Regression?

Linear regression can only learn straight lines. Many real patterns are not
straight. A neural network can learn **curves** by combining simple pieces:

```
  Linear regression:         Neural network:
  can only do this:          can do this:

  y│     ╱                   y│       ╱╲
   │   ╱                      │     ╱    ╲
   │ ╱                        │   ╱        ╲
   │╱                         │ ╱╱           ╲
   └──────── x                └──────────────── x
```

### Architecture

```
    INPUT LAYER         HIDDEN LAYER         OUTPUT LAYER
   (2 neurons)          (2 neurons)          (1 neuron)

    ┌─────┐         ┌──────────────┐        ┌──────────────┐
    │     │─── w1 ──│  sum(w*x + b)│        │              │
    │ x1  │╲       │  sigmoid()   │── w5 ──│  sum(w*x + b)│
    │     │ ╲ w2   │   = h1       │╲       │  sigmoid()   │
    └─────┘  ╲     └──────────────┘ ╲      │   = output   │
              ╲                      ╲     └──────────────┘
               ╲   ┌──────────────┐  ╲    ╱
                ╲──│  sum(w*x + b)│── w6 ╱
    ┌─────┐   ╱   │  sigmoid()   │    ╱
    │     │──╱ w3  │   = h2       │──╱
    │ x2  │╱      └──────────────┘
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

Where e = 2.71828... (Euler's number, a mathematical constant like pi).

Sigmoid squashes any number into the range (0, 1):

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

Some reference values (verify with a calculator: `1 / (1 + e^(-x))`):

```
  Input (x)   │  sigmoid(x)  │  Meaning
  ────────────┼──────────────┼───────────────────
    -6        │   0.0025     │  Very negative → near 0
    -2        │   0.1192     │
     0        │   0.5000     │  Zero → exactly 0.5
     2        │   0.8808     │
     6        │   0.9975     │  Very positive → near 1
```

Why sigmoid? Without a non-linear function, stacking layers of linear math
just gives you... more linear math. Sigmoid lets the network learn curves
and complex boundaries.

### The Sigmoid Derivative (Slope of the Sigmoid Curve)

During backpropagation, we need to know the slope of the sigmoid curve. We can
find it the same way we found the slope of x² — by nudging.

Let's find the slope of sigmoid at x = 2.0:

```
  sigmoid(2.000) = 1/(1 + e^(-2.000)) = 0.8808
  sigmoid(2.001) = 1/(1 + e^(-2.001)) = 0.8810

  slope ≈ (0.8810 - 0.8808) / 0.001 = 0.1050
```

Now here's a beautiful shortcut. Try computing `output * (1 - output)`:

```
  0.8808 × (1 - 0.8808) = 0.8808 × 0.1192 = 0.1050  ✓
```

Same answer! Let's check another point, x = 0:

```
  sigmoid(0.000) = 0.5000
  sigmoid(0.001) = 0.5003

  slope ≈ (0.5003 - 0.5000) / 0.001 = 0.2500

  Check: 0.5000 × (1 - 0.5000) = 0.5000 × 0.5000 = 0.2500  ✓
```

And x = -1:

```
  sigmoid(-1.000) = 0.2689
  sigmoid(-0.999) = 0.2692

  slope ≈ (0.2692 - 0.2689) / 0.001 = 0.1966

  Check: 0.2689 × (1 - 0.2689) = 0.2689 × 0.7311 = 0.1966  ✓
```

The formula `out * (1 - out)` always gives the slope of sigmoid. This is a
shortcut that comes from the math (the calculus derivation works out to this
neat formula). The practical benefit: you already computed `out` during the
forward pass, so getting the slope during backpropagation is almost free — one
multiply and one subtract.

```
  sigmoid_derivative(out) = out × (1 - out)

  Note: 'out' is the ALREADY COMPUTED sigmoid output.
  You don't need the original input.
```

---

## 6. Forward Pass — With Actual Numbers

Let's walk through a complete forward pass of a neural network. We'll use the
XOR problem: inputs [1, 0] should produce output 1.

**Network setup**:
- 2 inputs, 2 hidden neurons, 1 output neuron
- We'll pick specific weights so you can follow every computation

```
  Weights (input → hidden):      Biases (hidden):
    w1 = 0.5   w2 = 0.3            b_h1 = 0.1
    w3 = 0.7   w4 = 0.2            b_h2 = 0.1

  Weights (hidden → output):     Bias (output):
    w5 = 0.6                        b_out = 0.1
    w6 = 0.4
```

```
    ┌─────┐      w1=0.5        ┌────┐
    │x1=1 │──────────────────►│ h1 │      w5=0.6    ┌─────┐
    │     │╲     w2=0.3       │    │──────────────►│     │
    └─────┘ ╲  ╱              └────┘              │ out │
             ╲╱                                    │     │
             ╱╲                                    └─────┘
    ┌─────┐╱  ╲ w4=0.2       ┌────┐      w6=0.4  ╱
    │x2=0 │──────────────────►│ h2 │──────────────╱
    │     │      w3=0.7       │    │
    └─────┘                   └────┘
```

### Step 1: Hidden neuron h1

```
  net_h1 = (x1 × w1) + (x2 × w3) + b_h1
         = (1 × 0.5)  + (0 × 0.7)  + 0.1
         = 0.5 + 0.0 + 0.1
         = 0.6

  h1 = sigmoid(0.6)
     = 1 / (1 + e^(-0.6))
     = 1 / (1 + 0.5488)
     = 1 / 1.5488
     = 0.6457
```

### Step 2: Hidden neuron h2

```
  net_h2 = (x1 × w2) + (x2 × w4) + b_h2
         = (1 × 0.3)  + (0 × 0.2)  + 0.1
         = 0.3 + 0.0 + 0.1
         = 0.4

  h2 = sigmoid(0.4)
     = 1 / (1 + e^(-0.4))
     = 1 / (1 + 0.6703)
     = 1 / 1.6703
     = 0.5987
```

### Step 3: Output neuron

```
  net_out = (h1 × w5) + (h2 × w6) + b_out
          = (0.6457 × 0.6) + (0.5987 × 0.4) + 0.1
          = 0.3874 + 0.2395 + 0.1
          = 0.7269

  output = sigmoid(0.7269)
         = 1 / (1 + e^(-0.7269))
         = 1 / (1 + 0.4834)
         = 1 / 1.4834
         = 0.6741
```

### Result

```
  Input: [1, 0]
  Expected output: 1 (XOR of 1 and 0)
  Actual output: 0.6741
  Error: 0.6741 - 1.0 = -0.3259

  Not great! But we haven't trained yet. Training adjusts the weights
  to make the output closer to 1.0.
```

---

## 7. The Chain Rule — How Backpropagation Works

Now we need to adjust the weights to reduce the error. This is where
**backpropagation** comes in. It answers: "how much did each weight contribute
to the error?"

### The Gear Analogy

Imagine three gears connected in a chain:

```
  ┌───┐     ┌───┐     ┌───┐
  │ A │────►│ B │────►│ C │
  └───┘     └───┘     └───┘
  3x speed   2x speed

  If gear A turns gear B at 3× speed,
  and gear B turns gear C at 2× speed,
  then gear A turns gear C at 3 × 2 = 6× speed.
```

That's the **chain rule**: when effects pass through a chain, you **multiply
the rates** through each link.

In a neural network, the "gears" are:

```
  weight → weighted sum → sigmoid → loss
```

To find how the loss changes when you change a weight, you multiply the rates
through the chain:

```
  (how loss changes     (how sigmoid changes   (how weighted sum
   with sigmoid output) × with weighted sum)  ×  changes with weight)
```

### Backprop Step — With Actual Numbers

Let's continue from our forward pass. We had:
- Input: [1, 0], target: 1.0
- h1 = 0.6457, h2 = 0.5987, output = 0.6741

**Loss** (we'll use squared error for one data point):

```
  loss = (output - target)²
       = (0.6741 - 1.0)²
       = (-0.3259)²
       = 0.1062
```

#### Step 1: Output layer gradients

How does the loss change with the output?

```
  ∂loss/∂output = 2 × (output - target)
                = 2 × (0.6741 - 1.0)
                = 2 × (-0.3259)
                = -0.6518
```

How does the sigmoid output change with its input (net_out)?

```
  sigmoid_derivative = output × (1 - output)
                     = 0.6741 × (1 - 0.6741)
                     = 0.6741 × 0.3259
                     = 0.2197
```

Multiply them (chain rule!) to get the "output delta":

```
  delta_out = ∂loss/∂output × sigmoid_derivative
            = -0.6518 × 0.2197
            = -0.1432
```

Now we can compute the gradient for each output weight. Remember, the weighted
sum was net_out = h1*w5 + h2*w6 + b_out, so:

```
  ∂loss/∂w5 = delta_out × h1 = -0.1432 × 0.6457 = -0.0925
  ∂loss/∂w6 = delta_out × h2 = -0.1432 × 0.5987 = -0.0857
  ∂loss/∂b_out = delta_out × 1 = -0.1432
```

#### Step 2: Hidden layer gradients

The error flows backwards through the weights. How much error reaches h1?

```
  delta_h1_raw = delta_out × w5 = -0.1432 × 0.6 = -0.0859

  Then multiply by h1's sigmoid derivative:
  sigmoid_deriv_h1 = h1 × (1 - h1) = 0.6457 × 0.3543 = 0.2288
  delta_h1 = -0.0859 × 0.2288 = -0.0197
```

Similarly for h2:

```
  delta_h2_raw = delta_out × w6 = -0.1432 × 0.4 = -0.0573
  sigmoid_deriv_h2 = h2 × (1 - h2) = 0.5987 × 0.4013 = 0.2403
  delta_h2 = -0.0573 × 0.2403 = -0.0138
```

Now compute gradients for input-to-hidden weights (inputs were x1=1, x2=0):

```
  ∂loss/∂w1 = delta_h1 × x1 = -0.0197 × 1 = -0.0197
  ∂loss/∂w3 = delta_h1 × x2 = -0.0197 × 0 = 0.0000
  ∂loss/∂w2 = delta_h2 × x1 = -0.0138 × 1 = -0.0138
  ∂loss/∂w4 = delta_h2 × x2 = -0.0138 × 0 = 0.0000
```

(The gradients for w3 and w4 are 0 because x2=0 — input 0 means that weight
had no effect on the output. Makes sense!)

#### Step 3: Update all weights

With learning rate 0.1:

```
  w1  = 0.5 - 0.1 × (-0.0197) = 0.5 + 0.00197 = 0.5020
  w2  = 0.3 - 0.1 × (-0.0138) = 0.3 + 0.00138 = 0.3014
  w3  = 0.7 - 0.1 × (0.0000)  = 0.7000
  w4  = 0.2 - 0.1 × (0.0000)  = 0.2000
  w5  = 0.6 - 0.1 × (-0.0925) = 0.6 + 0.00925 = 0.6093
  w6  = 0.4 - 0.1 × (-0.0857) = 0.4 + 0.00857 = 0.4086
  b_h1 = 0.1 - 0.1 × (-0.0197) = 0.1020
  b_h2 = 0.1 - 0.1 × (-0.0138) = 0.1014
  b_out = 0.1 - 0.1 × (-0.1432) = 0.1143
```

All the gradients were negative (meaning "increasing this weight would
DECREASE the loss"), so all weights got slightly LARGER. This makes sense —
the output was 0.6741 but should be 1.0, so we need bigger values to push the
output up.

**That's one training step.** You repeat this thousands of times (cycling
through all 4 XOR input pairs) and the network gradually learns XOR.

### The Full Backprop Summary

```
  FORWARD PASS (compute outputs):
  ─────────────────────────────────────────────────────────►

  inputs ──► hidden layer ──► output layer ──► loss = (predicted - actual)²

  ◄─────────────────────────────────────────────────────────
  BACKWARD PASS (compute gradients):

  Step 1: delta_out = ∂loss/∂output × sigmoid'(output)

  Step 2: For each output weight:
          gradient = delta_out × (input to that weight)

  Step 3: For each hidden neuron:
          delta_h = (delta_out × weight to output) × sigmoid'(hidden)

  Step 4: For each hidden weight:
          gradient = delta_h × (input to that weight)

  Step 5: Update all weights:
          weight = weight - learning_rate × gradient
```

---

## 8. The XOR Problem

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
  x2
  1 ┤  ●(0,1)=1         ●(1,1)=0
    │
    │       No single line works!
    │       Need a hidden layer.
  0 ┤  ●(0,0)=0         ●(1,0)=1
    └──┬─────────────────┬──── x1
       0                 1
```

A hidden layer solves this by TRANSFORMING the inputs into a new space where
the problem IS linearly separable. The hidden neurons learn to create new
features that make the separation possible.

The XOR truth table:

```
  x1  x2  │  expected output
  ────────┼──────────────────
   0   0  │       0
   0   1  │       1
   1   0  │       1
   1   1  │       0
```

---

## 9. Loss Curve

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

### Exercise 2: `linear_regression.c` — Linear Regression

Implement linear regression from scratch:
- Hard-code the hours/score dataset from Section 4
- Implement the MSE loss function
- Implement gradient computation for w and b
- Run gradient descent and print the loss every 1000 iterations
- Print the final line equation and predictions

### Exercise 3: `neural_net.c` — Neural Network

Build a single-hidden-layer neural network:
- Sigmoid activation function
- Forward pass
- Train on XOR data using backpropagation
- Print the loss curve and final predictions for all 4 XOR inputs

---

## Key Takeaways

1. **The derivative is just the slope of a curve** — you can always find it by nudging
2. **Gradient descent** finds the minimum of a function by following the slope downhill
3. **Linear regression** is the simplest ML model — fit a line with gradient descent
4. **Neural networks** are layers of weighted sums + non-linear activations (sigmoid)
5. **Backpropagation** uses the chain rule (multiply the rates through the chain) to compute how each weight affects the loss
6. **XOR** proves you need hidden layers — a single layer can only learn linear boundaries
7. **Everything is just arithmetic** — there's no magic. Every step can be verified with a calculator.

---

[← Previous: Module 36 — Date & Time](../36-date-time/README.md) | [Next: Module 38 — Capstone Project →](../38-capstone-project/README.md)
