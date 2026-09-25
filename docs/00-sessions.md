# Session log

## Session 1 — from the disc to the first battle, with a pad

Goal: set the port up on wiikit, answer whether the game reads the Classic
Controller through KPAD (wiikit had it) or WPAD (to be added), boot. It went
past that: the game plays, with an Xbox One pad, into its first battle.

Results:

* **The disc** (`01-disc-layout.md`): RPJE7U, the RVZ read by `wiikit.disc`
  directly, 9 428 files, 4.1 GB: CRI Sofdec movies, 7 042 ADX voice files,
  battles, maps, events. Two `.bat` files left in `Event/voice/` name the
  project: `project_ray` (`04-curiosities.md`).
* **The executable** (`03-executable.md`): stripped; the RVL SDK of late
  2007 and NW4R of April 2008, builds no other port shares. 1 530 names:
  1 452 by signature, 40 from the SDK's own strings, 38 by hand (the 2007
  KPAD and WPAD aligned object by object with Victorious's ELF). Recompiled
  at the first try: 13 633 units, no gaps; compiled and linked at the first
  try.
* **The controls** (`08-input.md`): **both**. The game polls `WPADProbe`,
  calls `KPADRead` for the Remote's buttons, and reads the Classic from WPAD's
  raw `WPADCLStatus`, through a 2007 KPAD function that copies WPAD's
  samples (unnamed anywhere: `kpad_wpad_status`) and
  `WPADGetLatestIndexInBuf`. The first boot crashed there as soon as the
  pad was announced. wiikit now answers at WPAD's level too.
* **The first boot**: a black window for minutes, then nothing: wiikit
  opened each of the disc's 9 428 files at start to measure it, and the
  antivirus scanned each one (26 ms a file, for any newly built
  executable). Sizes now come from the FST.
* **It plays.** The strap screen, Ignition, Marvelous, imageepoch, CRIWARE,
  the title (60 fps), the System screen ("Controller: Classic Controller",
  chosen by the game), a new game, the prologue, the first battle, with
  sound, played by the user with the Xbox One pad. Everything at a steady
  30 fps (the game's rate), the 3D field included, except battles: 20.
* **Battles** (`10-wiikit.md`): the renderer, not the game (the game's
  thread idles 70 % of the time). An F12 trace: 23 000 draws a frame,
  NW4R's skinned models as strips of 4 to 10 vertices, one GL call each. At
  internal resolution x1 no better. wiikit now merges draws in a row into one
  `glMultiDrawArrays`: 1.8 M GX draws → 46 000 GL calls over the boot;
  battles 27-30 fps. The rest is ~17 us of driver time per remaining draw,
  with the GPU in its P5 state (360 MHz, 40 %). A uniform ring was tried
  and dropped: slower, and its first version overran and made models vanish
  (seen by the user).
* wiikit checked on the three other ports (`10-wiikit.md`).

Left: battles to a steady 30; a keyboard layout for a JRPG; saving (no save
written yet: the prologue had not reached a save point).
