# Mountain Climbing Algorithm

A C-based optimisation project focused on finding the peak of procedurally generated mountains while minimising the number of expensive view-generation calls.

The repository contains two versions of the algorithm:

- **Assignment version** — prioritises robustness and reliable peak detection.
- **Competition version** — aggressively optimised to reduce the number of calls to `generate_view()`, accepting more risk in exchange for a lower average score.

## How the problem works

The algorithm does not have access to the full mountain.

Instead, it can call:

- `generate_view()` — returns a limited view of the terrain around a given coordinate.
- `declare_peak()` — checks whether a coordinate is the actual peak.

Each call contributes to the score, so the objective is to find the correct peak while using as few calls as possible.

Returning an incorrect peak results in a very large penalty, so the problem involves a trade-off between:

- reliability
- exploration
- optimisation
- risk

## Assignment Version

The assignment implementation was designed to be more reliable.

It:

- Searches the visible area for the highest point.
- Tracks the previous direction of movement.
- Uses progressively smaller jumps when approaching higher terrain.
- Detects possible peaks before checking them and ultimately returning them if correctly identified.
- Uses momentum-based plateau escape, resorting to randomised escape when that fails.
- Validates candidate peaks before returning them.

This version prioritises correct peak detection over achieving the lowest possible score.

## Competition Version

The competition version was modified specifically to minimise the number of API calls.

Changes included:

- Larger initial jumps.
- More aggressive jump decay.
- Directional bias based on previous movement.
- Plateau escape using visible non-equal terrain points.
- Reduced use of `declare_peak()`.
- Returning likely peaks based on known mountain properties rather than always explicitly verifying them.

These optimisations reduced the average number of calls, but also introduced a greater risk of returning an incorrect peak.

The competition version finished **2nd overall** in the module competition.

## Technologies

- C
- Algorithm Design
- Optimisation
- Heuristics
- Targeted Search
- Randomised Search

## Repository Structure

```text
assignment/
  mountain_climbing_assignment.c
competition/
  mountain_climbing_competition.c
