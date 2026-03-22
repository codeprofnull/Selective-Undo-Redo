# Selective Undo v2

Improved regional selective undo with active node tracking.

## Improvements over v1
- Each TreeNode has an active flag
- Nodes set inactive on undo, active on redo
- New branch creation marks stale children inactive
- selectRange only returns active nodes in range
- Handles stale branch nodes correctly
- Handles multiple undos without redo correctly

## How to run
g++ -o regionalundoredov2 regionalundoredov2.cpp
.\regionalundoredov2.exe

## Known Limitations (v3 scope)
- Position invalidation after selective undo
- Characters use mutable positions not unique IDs
- Redo after selective undo produces incorrect positions
- Fix requires CRDT style unique character IDs