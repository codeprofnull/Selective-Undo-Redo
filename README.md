# Selective Undo v1

Regional selective undo engine in C++ inspired by AZURITE (CMU 2015).

## How it works
Each character edit is stored as a Command with position and timestamp.
selectRange queries all commands within a position range.
selectiveUndo sorts them highest position first and reverses each one.

## Data Structures
- Undo tree as base
- Vector of TreeNode pointers for range queries

## Complexity
- selectRange: O(n) walk from currentNode to root
- selectiveUndo: O(s log s) where s is selection size

## How to run
g++ -o regionalundoredov1 regionalundoredov1.cpp
.\regionalundoredov1.exe

## Known Limitations (fixed in v2)
- Does not track which nodes are currently active
- Stale nodes from undone branches can appear in selection
- Multiple undos without redo causes incorrect range queries