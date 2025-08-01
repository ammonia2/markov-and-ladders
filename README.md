# markov-and-ladders
Modeling Snake &amp; Ladder using Markov chain first order sequence

## How to Run

### Requirements
- Have any Python version > 2.7 installed
- **C++ Compliler**: g++11 or higher
- **Python Packages**:
  ```bash
  pip install matplotlib numpy
  ```

### Command
- `g++ main.cpp -std=c++17 -o main -I/usr/include/python3.12 -I/usr/lib/python3/dist-packages/numpy/core/include -lpython3.12 && ./main` (the paths for matplotlib and numpy python are according to linux, change the version and path based on your setup)


## About the project

For creating graphs, [Matplotlib-cpp](https://github.com/lava/matplotlib-cpp/tree/master) is used.

### Features
- A Random Snake & Ladder board generation code
- Implemented a custom matrix class `Matrix.hpp` to support operations with transition matrix
- Analysis of game dynamics through transition matrix, fundamental matrix and probability distributions
- Graph of expected moves to win after every block
- Graph of winning probability after steps (0-100)

### Board Generation
- More ladders near start, more snakes near end
- No snakes/ladders in adjacent positions
- Maximum snake length ≤ 90% of board

### Markov Chain Model
The game is modeled as an absorbing Markov chain where:
- **States**: Board positions (0-99 for a 10×10 board)
- **Transient States**: Positions 0-98 (can transition to other states)
- **Absorbing State**: Position 99 (winning position)
- **Transition Probabilities**: Based on dice rolls (1/6 each) and board elements

### Matrices used in calculations
- **P**: Full transition matrix (100×100)
- **Q**: Transient-to-transient transitions (99×99)
- **R**: Transient-to-absorbing transitions (99×1)
- **N**: Fundamental matrix `N = (I - Q)^(-1)`

### Expected total moves to win
Multiply the fundamental matrix `N` by a column vector of ones:
```
Expected moves = N × [1, 1, ..., 1]^T
```

### Winning probability after each step

```
π(k) = P^k × π(0)
```

Where:
- `π(k)` = probability distribution after k steps
- `P` = full transition matrix (100×100)
- `π(0)` = initial distribution (all probability on starting position)
- `P^k` = transition matrix raised to the k-th power

**Initial Setup:**
```
π(0) = [1, 0, 0, ..., 0]^T
```
Always starts with 100% probability at position 0 (starting square).

After each step k, the winning probability is:
```
P(win after k steps) = π(k)[99]
```
This represents the probability mass accumulated in the absorbing state (position 99).
