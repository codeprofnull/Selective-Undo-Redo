# Selective Undo v2

Improved regional selective undo with active node tracking.

## Improvements over v1
- Each TreeNode has an active flag
- Nodes set to inactive on undo, active on redo
- selectRange only returns active nodes in range
- Handles stale branch nodes correctly
- Handles multiple undos without redo correctly

## How to run
g++ -o regionalundoredov2 regionalundoredov2.cpp
.\regionalundoredov2.exe

## Open Problems
- Position invalidation when selective undo reinserts characters
- Collaborative selective undo remains unsolved in research
- Semantic conflict detection not yet implemented