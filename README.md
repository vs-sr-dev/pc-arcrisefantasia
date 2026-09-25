# pc-arcrisefantasia

Toward a native PC port of **Arc Rise Fantasia** (Wii, imageepoch, 2009;
Ignition Entertainment in North America, 2010), the JRPG. It was a Wii
exclusive and never re-released. The goal is the game running natively on
PC, played with a gamepad as the Classic Controller, or the keyboard.

This repository documents the disc and its code, and grows the tooling for
the port. It is the fourth port built on
**[wiikit](https://github.com/vs-sr-dev/wiikit)**, the game-agnostic Wii
toolkit, taken here as a submodule at `wiikit/` (clone with `--recursive`,
or `git submodule update --init`). Where this game needs something every
Wii game would need, it goes into wiikit, not here.

## BYOA — Bring Your Own Assets

This repository contains **documentation and tools only**. No game data, no
executables, no assets. You need your own original disc. The work is done on
the North American release, RPJE7U.

## Layout

    docs/     disc, code and input analysis, the plan
    tools/    Arc Rise Fantasia-specific tools and the port's layer
    wiikit/   game-agnostic Wii toolkit (submodule: github.com/vs-sr-dev/wiikit)

## Tools

Python 3.8+ (Pillow for `dolphin.py`'s screenshots; pycryptodome, if
installed, speeds up disc decryption). Run from the repository root. The
build as in `docs/07-next-session.md`: clang, Ninja and SDL3 from MSYS2.

```sh
python -m wiikit.disc GAME.rvz --extract build/extract
python tools/sigmatch.py build/extract/sys/main.dol --dsy <Dolphin>/Sys/totaldb.dsy \
    --elf <a symbolised Wii ELF> --out build/sig_guess.tsv
python tools/names.py build/extract/sys/main.dol            # -> build/names.tsv
python tools/look.py --func KPADRead                        # wiikit.ppc on the stripped DOL
python -m wiikit.recomp build/extract/sys/main.dol --out build/recomp \
    --symbols build/names.tsv --hooks tools/arf-hooks.txt
```

## Status

Session 1: **the game plays, with a pad.** The disc read from its RVZ, the
stripped executable named (1 530 functions) and recompiled at the first
try; the game boots through its logos to the title, a new game, the
prologue and the first battle, with sound, played with an Xbox One pad as
the Classic Controller, which the game reads from WPAD's raw samples
(wiikit learnt to answer there). A steady 30 frames a second, the game's
rate, but in battles: 27-30 since wiikit merges the thousands of tiny draws
of skinned models (20 before). See
[docs/00-sessions.md](docs/00-sessions.md).

## Documentation

    00-sessions.md            progress log
    01-disc-layout.md         what is on the disc
    03-executable.md          the stripped DOL: what is linked, how it is named, landmarks
    04-curiosities.md         what the disc reveals
    07-next-session.md        the plan for the next session
    08-input.md               the Classic Controller: how the game reads it
    10-wiikit.md              how this port uses and grows wiikit

## Licence

MIT. This covers the documentation and tools in this repository only. It
says nothing about Arc Rise Fantasia, which remains the property of its
rights holders.
