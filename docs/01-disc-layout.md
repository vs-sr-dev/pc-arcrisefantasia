# The disc

**RPJE7U**, "ARC RISE FANTASIA", the North American release (Ignition
Entertainment, 2010; imageepoch, first released in Japan by Marvelous in
2009). The work is done on an RVZ image, which `wiikit.disc` reads as it is:

    python -m wiikit.disc "Arc Rise Fantasia (USA).rvz" --info
    python -m wiikit.disc "Arc Rise Fantasia (USA).rvz" --extract build/extract

An update partition (201 MB) and the game's DATA partition (4 425 MB):
9 428 files, 4.1 GB extracted. No RELs; `sys/main.dol` is the whole program
(`03-executable.md`).

| Directory | Size | Files | What |
|---|---|---|---|
| `Movie/` | 1 350 MB | 21 | CRI Sofdec movies (`.sfd`): the opening (`RAY_OP_01`, `RAY_OP_02`), the ending (`RAY_ED`, `RAY_ED_ROLL`), event movies (`EV…`), the imageepoch and Marvelous logos |
| `Event/` | 871 MB | 7 516 | events; 7 042 ADX voice files (`Event/voice/…`, "face chat" lines) |
| `Btl/` | 727 MB | 27 | battles: `.vld` banks (enemies, magic, maps, party, weapons, the tutorial, victory), `.vol` archives (shared textures, the auto demo, game over), battle voices in an AFS |
| `Map/` | 690 MB | 328 | the field maps |
| `interface/` | 146 MB | 758 | menus and 2D (`.cxd`) |
| `world/` | 72 MB | 551 | the world map; its sound as an `.SWD`/`.SED` pair |
| `Char/` | 66 MB | 151 | characters |
| `Sys/` | 13 MB | 20 | |
| `minigame/` | 9 MB | 36 | |
| `Title/` | 6 MB | 2 | the title screen |
| `Hbm/` | 4 MB | 13 | the Home Button menu, six languages |
| `_viewer/`, `_sasaki/` | 2 MB | 4 | developers' leftovers (`04-curiosities.md`) |

By extension: 7 042 `.adx`, 982 `.cxd`, 759 `.vol`, 548 `.bin`, 51 `.wtm`,
21 `.sfd`, 9 `.arc` (U8, the Home Button menu), 7 `.vld`. The game's own
formats are not studied yet: the port runs the game's code on them.
