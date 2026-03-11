# OverRide

General workspace for the 42 `OverRide` project.

## About

This repository stores notes, code, and exploit artifacts for each level.
Every level folder typically contains:

- `README.md` - analysis and write-up
- `todo.md` - task notes and checkpoints
- `levelXX.c` / scripts - extracted or reconstructed logic
- `flag` - captured token or result for the level

## Repository Structure

- `level00/` ... `level09/` - per-level work directories
- Root `README.md` - global setup and workflow reference

## Quick Start

### 1) Run the ISO in QEMU

```bash
sudo qemu-system-x86_64 \
  -smp 2 \
  -cdrom /Users/arevikmkrtchyan/Downloads/OverRide.iso \
  -boot d \
  -vga std \
  -m 2G \
  -nic user,hostfwd=tcp::4242-:4242 \
  -display default,show-cursor=on
```

### 2) Connect through SSH

```bash
ssh -p 4242 level00@127.0.0.1
```

## Recommended Workflow

1. Start from `level00/` and move forward sequentially.
2. Keep short notes in each `todo.md`.
3. Document findings and final exploit path in each level `README.md`.
4. Save reproducible scripts (`.py`, helper `.c`) alongside the level.
5. Keep the root README focused on environment and global instructions.

## Notes

- Use this repository only in legal CTF/training environments.
- If your ISO path differs, update the `-cdrom` argument accordingly.