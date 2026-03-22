# Linear Undo/Redo Engine

A two stack linear undo/redo engine in C++ implementing the command pattern.

## How it works
Every action is wrapped as a Command object storing the action type, character, 
and position. Undo pops from the undo stack and reverses the action. Redo pops 
from the redo stack and reapplies it. Any new action clears the redo stack.

## Data Structures
- Two stacks: undoStack and redoStack
- Vector of chars as the document

## Complexity
- Undo/Redo: O(1)
- Insert/Delete: O(n) due to position shifting

## How to run
g++ -o undoredo undoredo.cpp
.\undoredo.exe

## Limitations
- Linear only — can only undo in reverse chronological order
- New action permanently destroys redo history
- No selective undo support