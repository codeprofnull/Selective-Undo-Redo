# Selective Undo v3

Full redesign using unique character IDs instead of positions.
Solves the position invalidation problem present in v1 and v2.

## Core Idea
Every character that is ever typed gets a unique immutable ID.
Position is never stored — it is always derived by counting alive characters.
This means selective undo never invalidates any other character's position.

## New Data Structures

### CharNode
Each character ever typed is stored as a CharNode with:
- id: unique integer, never changes
- value: the character
- alive: true if visible in document, false if deleted

### Document
Stores all CharNodes ever created, alive or dead.
Visible document is derived by filtering alive characters only.

### Command
Stores action type and charId only — no position.

## How Operations Work
- Insert: creates new CharNode, stores its ID in command
- Delete: sets CharNode alive = false, stores its ID in command
- Undo insert: sets CharNode alive = false
- Undo delete: sets CharNode alive = true
- Selective undo: flips alive flag for all matching IDs in range

## Improvements over v2
- Position invalidation fully solved
- Selective undo followed by new insert works correctly
- Redo after selective undo works correctly
- No position shifting — all positions derived dynamically

## How to run
g++ -o regionalundoredov3 regionalundoredov3.cpp
.\regionalundoredov3.exe

## Known Limitations (v4 scope)
- No collaborative editing support
- No semantic conflict detection
- Single user only
- Performance degrades with very long document history
  as all CharNodes including dead ones are kept in memory

## Research Reference
This approach is inspired by CRDT (Conflict-free Replicated Data Types)
used in collaborative editors like Google Docs.
Full paper: ICSE 2015 - Supporting Selective Undo in a Code Editor
Carnegie Mellon University - Yoon and Myers