# TODO — session 2

The game plays from the boot to its first battle with a pad (session 1).
Everything runs at the game's 30 fps but battles, 27-30 since the draws are
merged (20 before).

1. **Battles to a steady 30.** The renderer's thread spends ~17 us of
   driver time per GL draw (`WIIKIT_PERF`, `WIIKIT_PROFILE=render`); about
   1 000 draws a frame remain, split by XF matrix loads between the runs of
   strips. Ideas, measured one at a time:
   * skip GL state that did not change (viewport, scissor, depth, blend,
     cull, program, textures, samplers are set for every draw);
   * upload only what changed of XF memory; or keep the position matrices
     per vertex (the skinned vertices carry their matrix index: several
     runs with their own matrices could then be one draw);
   * the GPU sits in its P5 state (360 MHz, 40 %): try "prefer maximum
     performance" in the NVIDIA control panel for `wiiboot.exe`, to tell
     driver cost from GPU clock.
2. **Saving**: play to the first save point, check the NAND files and a
   load.
3. **The keyboard for a JRPG**: wiikit's default `[Classic Controller]` keys
   are a shooter's; a layout for menus and battles (and the controls screen
   in Dolphin with the Classic, to follow the game's own layout).
4. The 14 unresolved switch tables: watch the log for unknown targets.

Build and run:

    python tools/names.py build/extract/sys/main.dol
    python -m wiikit.recomp build/extract/sys/main.dol --out build/recomp \
        --symbols build/names.tsv --hooks tools/arf-hooks.txt
    cmake -S build/recomp -B build/recomp-build -G Ninja -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE=-O1 -DWIIKIT_EXTRA=$PWD/tools/arf.cmake
    ninja -C build/recomp-build
    cd build/run1 && ../recomp-build/wiiboot ../extract --symbols ../names.tsv --nand ../nand

`build/fonts` holds the boot ROM's fonts and `dsp_coef.bin` (copied from
the other ports' builds). `WIIKIT_PERF=1` reports a frame's cost every
second; F12 writes the next frame's GX commands to the working directory.
