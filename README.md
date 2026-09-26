# pc-arcrisefantasia

Toward a native PC port of **Arc Rise Fantasia** (Wii, imageepoch, 2009;
Ignition Entertainment in North America, 2010), the JRPG. It was a Wii
exclusive and never re-released. The goal is the game running natively on
PC, played with a gamepad as the Classic Controller, or the keyboard.

The route is static recompilation: the game's own PowerPC code, translated
to C++ and built for the PC, running on a replacement for the Wii's
hardware and system software. Nothing is emulated at the instruction
level, and nothing of the game is rewritten.

This repository documents the disc and its code, and holds the port's own
tools and layer. It is the fourth port built on
**[wiikit](https://github.com/vs-sr-dev/wiikit)**, the game-agnostic Wii
toolkit that grew with [pc-victorious](https://github.com/vs-sr-dev/pc-victorious),
[pc-dragonquestswords](https://github.com/vs-sr-dev/pc-dragonquestswords)
and [pc-conduit2](https://github.com/vs-sr-dev/pc-conduit2), taken here as
a submodule at `wiikit/` (clone with `--recursive`, or
`git submodule update --init`). Where this game needs something every Wii
game would need (WPAD's own samples, thousands of tiny draws merged, disc
files sized from the FST), it goes into wiikit, not here.

## Where it stands

After one session the game **boots, draws, sounds and plays to its first
battle**: the strap screen, the Ignition, Marvelous, imageepoch and
CRIWARE logos, the title, the System menu, a new game, the prologue and the
first battle, at 16:9. It is played with **a gamepad as the Classic
Controller** (an Xbox One pad here), or the keyboard. Everything runs at
the game's own 30 frames a second but battles, which run at 27-30: the
renderer still spends too long in the driver per draw. **It is not yet
played through**: nothing past the first battle has been tried, and **no
save has been written yet** (the prologue had not reached a save point).
The keyboard has wiikit's default Classic keys, a shooter's layout, not yet
one made for a JRPG. See [Status](#status) and
[docs/07-next-session.md](docs/07-next-session.md).

## BYOA — Bring Your Own Assets

This repository contains **documentation and tools only**. No game data, no
executables, no assets. You need your own original disc. The work is done on
the North American release, RPJE7U; the addresses in `tools/` and `docs/`
are that executable's.

## Layout

    docs/     disc, code and input analysis, the plan, the session log
    tools/    Arc Rise Fantasia-specific tools, the port's layer (arf.cpp)
    wiikit/   game-agnostic Wii toolkit (submodule: github.com/vs-sr-dev/wiikit)
    build/    (not in git) the disc, everything derived from it, the build

## Tools

The Python tools need only Python 3.8+ and no dependencies (RVZ images
compressed with zstd, as this game's usually is, need Python 3.14;
pycryptodome, if installed, speeds up disc decryption; `dolphin.py` needs
Pillow for its screenshots). Building the recompiled code needs CMake,
Ninja, a C++20 compiler (clang from MSYS2 is what is used here) and SDL3;
running it needs OpenGL 4.5. Run from the repository root.

```sh
# the disc, straight from the RVZ (or .iso, .wbfs)
python -m wiikit.disc GAME.rvz --info
python -m wiikit.disc GAME.rvz --extract build/extract

# the executable: stripped, so its names are found, not read
python -m wiikit.dol build/extract/sys/main.dol --info
python tools/sigmatch.py build/extract/sys/main.dol \
    --dsy <Dolphin>/Sys/totaldb.dsy --out build/sig_guess.tsv
python tools/names.py build/extract/sys/main.dol      # -> build/names.tsv
```

The names come from Dolphin's signature database, `totaldb.dsy`, which
every Dolphin install carries in its `Sys` folder (next to `Dolphin.exe`
on Windows: `Dolphin-x64/Sys/totaldb.dsy`). `sigmatch.py` hashes the DOL's
functions as Dolphin does and looks them up there (1 704 found);
`names.py` keeps the unique ones, adds the SDK functions that print their
own name, and the hand names of `tools/names-manual.tsv`, each with its
evidence: 1 392 names, among them every function the runtime hooks that
the game has. With `--elf`, `sigmatch.py` also takes signatures from any
symbolised executable (Victorious's `Oscar_wii_final_versioned.elf` adds
138 names); the build does not need them. See
[docs/03-executable.md](docs/03-executable.md).

```sh
# recompile, build: the port's hooks, and its layer through WIIKIT_EXTRA
python -m wiikit.recomp build/extract/sys/main.dol --out build/recomp \
    --symbols build/names.tsv --hooks tools/arf-hooks.txt
cmake -S build/recomp -B build/recomp-build -G Ninja -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE=-O1 \
    -DWIIKIT_EXTRA=$PWD/tools/arf.cmake
ninja -C build/recomp-build

# boot the game: the NAND (saves, SYSCONF) in build/nand; the boot ROM's
# fonts and the DSP ROM's resampling table in build/fonts (font_western.bin,
# font_japanese.bin, dsp_coef.bin: Dolphin's Sys/GC has free ones)
build/recomp-build/wiiboot build/extract --symbols build/names.tsv
build/recomp-build/wiiboot build/extract --window 1920x1080
build/recomp-build/wiiboot build/extract --input pad      # the pad alone on channel 1

# where the time goes: a frame's cost every second; a sampling profiler of
# the game's thread (1) or the renderer's (render) (see wiikit)
WIIKIT_PERF=1 build/recomp-build/wiiboot build/extract 2> run.err
WIIKIT_PROFILE=render build/recomp-build/wiiboot build/extract 2> run.err
```

`tools/look.py` is `wiikit.ppc` on the stripped DOL: discovery's
functions, these names (`--func KPADRead`, `--callers`, `--xref`).
`tools/dolphin.py` runs the disc in Dolphin as the reference, with
screenshots and key presses on a timeline (`--dolphin PATH`, or `DOLPHIN`
in the environment).

### Playing

The game plays with a Classic Controller (its System menu chooses it by
itself), which the port makes out of any gamepad and the keyboard. Channel
1 merges the keyboard, the mouse buttons and the first pad: buttons from
any of them, each stick from whichever is pushed further. Every other pad
plugged in is the next channel. `--input pad` or `--input keyboard` keeps
one source on channel 1. The mouse's motion does nothing: there is no
pointer in this game.

| Keys | Gamepad | Classic Controller |
|---|---|---|
| W A S D | left stick | left stick |
| | right stick | right stick |
| Enter, Space | right face button | A |
| Backspace, C | bottom face button | B |
| R | top face button | X |
| F | left face button | Y |
| Left Shift, E | shoulders | L, R |
| right mouse button | left trigger | ZL |
| left mouse button | right trigger | ZR |
| Tab, Q | Start, Back | +, - |
| arrows | d-pad | d-pad |
| H | guide | Home |
| Esc, F11 / Alt+Enter, F12 | | pause box, fullscreen, trace a frame's GX commands |

The pad's face buttons count by position, as on a Classic Controller (the
right one is A); `Face Buttons = Label` in the key file counts them by
their labels instead. The keys are in `build/keys.txt`, written with
wiikit's defaults on the first run; its `[Classic Controller]` section
changes them (the right stick has no keys by default). F12 writes the
trace to the working directory.

## Status

Session 1: **the game plays, with a pad.** The disc read from its RVZ
(9 428 files), the stripped executable named and recompiled at the first
try (13 633 units, no gaps); the game boots through its logos to the
title, a new game, the prologue and the first battle, with sound, played
with an Xbox One pad as the Classic Controller. The game reads the Classic
from WPAD's raw samples, through a copy of them only the 2007 KPAD keeps,
so wiikit learnt to answer there. A steady 30 frames a second, the game's
rate, but in battles: 27-30 since wiikit merges the thousands of tiny
draws of skinned models into single calls (20 before). The first boot took
minutes: wiikit opened every file of the disc to size it, and the
antivirus scanned each one; sizes now come from the FST. See
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
