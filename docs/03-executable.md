# The executable

`sys/main.dol`, 4.4 MB, stripped. Entry 8000403C; text 80004000–800064E0
and 80021A20–803CA440 (961 472 words); bss 80440200 + 0xF7F90. No RELs.
r13 (SDA) 805391C0, r2 (SDA2) 8053AB80.

## What is linked

The RVL SDK of late 2007, every library `0x4199_60831`:

| Library | Build |
|---|---|
| OS, GX, NAND, WPAD | Dec 11 2007 |
| PAD | Oct 3 2007 |
| AI, AX, DSP, DVD, EXI, KPAD, SC, SI, VI | Aug 8 2007 |
| HBM (Home Button menu) | Nov 22 2007 (`0x4199_60726`) |
| NW4R G3D, NW4R SND | Apr 2 2008 |

None of these builds is shared with the other wiikit ports (Victorious's
are of 2010, Dragon Quest Swords's of 2006–07), so no function is matched
word for word from a symbolised executable.

Middleware: NintendoWare (G3D models, SND sound), CRI (ADX voices, Sofdec
`.sfd` movies, an AFS archive; the CRIWARE logo at boot). The GameCube pad
library is linked and called.

## Names

`tools/names.py` gathers, most trusted first:

| Source | Names |
|---|---|
| by hand, each with its evidence (`tools/names-manual.tsv`) | 41 |
| SDK functions that print their own name (`"WPADInit()"`) | 40 |
| signatures (`tools/sigmatch.py`), unique ones: Dolphin's `totaldb.dsy` alone | 1 311 |
| the same with Victorious's ELF too (`--elf`) | 1 449 |

1 392 in all from Dolphin's database, 1 530 with Victorious's ELF. The hand
names are what the runtime hooks and the signatures miss: the 2007 KPAD and
much of WPAD (their builds are in no signature database), aligned object by
object with Victorious's ELF, by order and size, and checked by what calls
what; `PPCHalt`, `RealMode`, `__VIRetraceHandler`, `OSFatal`, `OSRestart`,
`__OSReboot` the same way. Three hooks only Victorious's ELF named by
signature (`OSLoadContext`, `OSShutdownSystem`, `WPADSendStreamData`) are
hand names too, placed next to functions Dolphin's database names, so the
database alone names every hook the game has (46 of the runtime's 53), at
the same addresses; the recompiled code is the same but for the names of
functions no hook needs and its comments.
Two things only this SDK has: `KPADRead` is the whole reader (no
`KPADiRead`, no `KPADReadEx`), and KPAD keeps a copy of WPAD's samples that
games may read (`kpad_wpad_status`, `08-input.md`).

`tools/look.py` is `wiikit.ppc` on this DOL: discovery's functions, these
names (`--func`, `--callers`, `--xref`).

## Recompiled

    python -m wiikit.recomp build/extract/sys/main.dol --out build/recomp \
        --symbols build/names.tsv --hooks tools/arf-hooks.txt

13 633 units, no gaps, no branch to an unknown target; 319 switch tables
sized, 14 not (one is `__ptmf_scall`, a call through a member pointer); 76
words undecoded (data in text). 117 files of C++, compiled and linked at the
first try.

## Landmarks

| Address | What |
|---|---|
| 80207434 | the game's main loop (called from `__start`) |
| 8020EE78 | a channel's read: `WPADProbe`, `KPADRead(chan, buf, 16)` |
| 8020EF58 | the input module's per-channel step (6 KB): the Classic from `WPADCLStatus` |
| 8034A354 | `kpad_wpad_status` (KPAD's copy of WPAD samples) |
| 8034C830 | `KPADRead` |
| 8034CF5C | `KPADInit` |
