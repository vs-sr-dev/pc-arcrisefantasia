# This port and wiikit

This port takes [wiikit](https://github.com/vs-sr-dev/wiikit) as a
submodule at `wiikit/`, like pc-victorious, pc-dragonquestswords and
pc-conduit2. It is its fourth user and the third with a stripped
executable. wiikit is public and does not name this port; changes made for
it are described by what they do, and checked on every port before they go
in.

## What this port used as it is (session 1)

`wiikit.disc` (the RVZ read directly: 9 428 files, 4.1 GB extracted), `dol`,
`ppc`, the recompiler with discovery (13 633 units, 0 gaps), and the whole
runtime: from `__start` to the menus, a new game, the prologue's 3D scenes
at 30 frames a second, the first battle, the sound, the Classic Controller's
announcement on channel 1.

## What it gave wiikit

| Commit | Change | Layer | Why it is not game knowledge |
|---|---|---|---|
| `a5bb0d3` | **the Classic in WPAD's own samples**: `WPADRead`, `WPADSetAutoSamplingBuf` / `WPADGetLatestIndexInBuf` (the newest sample at index 0, written when asked), and `kpad_wpad_status`, the 2007 KPAD's copy of WPAD samples of one device (a name ports give it: no symbol source has one); `WPADStatus`, `WPADFSStatus`, `WPADCLStatus` by device, sticks and triggers where the 2007 KPAD reads the host's deflection back | 5 | any game reading WPAD below KPAD, or the 2007 KPAD's copy; this one crashed in `WPADGetLatestIndexInBuf` without it |
| `6bab55a` | **draws in a row become one**: consecutive draws of one primitive with nothing written between them are pieces of one record entry, drawn with `glMultiDrawArrays` (`glMultiDrawElementsBaseVertex` for quads) | 5 | NW4R's skinned models send thousands of tiny strips a frame: this game's battles 23 000 GX draws a frame, ~1 000 GL calls now; 20 → 27-30 fps |
| `309832b` | the disc's files sized from the FST, opened when first read | 5 | opening 9 428 files at boot took minutes while the antivirus scanned each one for a new executable |
| `309832b` | `WIIKIT_PROFILE=render`: the sampling profiler on the renderer's thread | 5 | any port's renderer |

Tried and dropped: the uniform blocks (XF memory, the pixel stage's
constants) streamed through a persistently mapped ring instead of
`glNamedBufferSubData`. Measured slower in battle (23-26 fps against
27-30): copying 16 KB of XF memory per matrix change costs more than the
driver's own versioning. Its first try also overran its ring (17 MB of XF a
frame in a 16 MB ring, and a quarter index of 4 at the exact end): models
and textures vanished mid-battle.

Checked on every port before they went in: Victorious self-test 15/15 and
to its Auditions episode; the stripped 2007 PAL game to its Adventure Logs;
Conduit 2 to its title; the same screens as before, frames equal where the
timing matches. Their submodules moved on (Victorious `bfd5872`, DQS
`0071b4f`, Conduit 2 `ba601c1`; local commits).

## What it will give wiikit

| Addition | Layer | Why |
|---|---|---|
| the renderer's cost per draw (~17 us of driver time a call in battle, the GPU at 360 MHz and 40 %) | 5 | every port with many draws |
| `tools/look.py` (`wiikit.ppc` on a stripped DOL through discovery) | 3 | any stripped executable |
| object alignment against a symbolised ELF by order and size (done by hand here) | 3 | SDK builds no signature database has |
