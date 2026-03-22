# Undo Tree

Non-linear undo/redo engine in C++ using a tree structure.

## How it works
Instead of wiping redo history on new action, history branches into a tree.
Each node stores a Command. currentNode pointer tracks where you are.
Undo moves up to parent. New action creates a new child branch.
Redo shows all available branches and lets you choose.

## Data Structures
- Tree of Command nodes
- Each node has parent pointer and vector of children

## Complexity
- Undo/Redo: O(1) navigation
- Insert/Delete: O(n) due to position shifting

## How to run
g++ -o undoredotree undoredotree.cpp
.\undoredotree.exe

## Improvement over linear
- History is never lost
- Multiple redo paths preserved
- Any past state is navigable