# Input: the Classic Controller

## What the game accepts

Arc Rise Fantasia plays with the Remote alone, the Remote and Nunchuk, the
Classic Controller, or a GameCube pad (the SDK's PAD library is linked, and
the input module calls `PADReset`). Its System menu has a **Controller**
line; on the port's first run it read "Classic Controller" by itself.

The PC scheme is the Classic Controller: a gamepad (the user's: an Xbox One
pad) or the keyboard, both from wiikit (`wpad_set_classic`).

## How the game reads it (session 1)

The input module sits at 8020EBF0–80210D00. Each frame, for each channel:

1. `WPADProbe(chan, &type)`: no connect callbacks, a poll. A channel that
   answers "no controller" is `WPADDisconnect`ed (8020EE78).
2. `KPADRead(chan, buf + chan * 0x840 + 0x1D8, 16)`: sixteen `KPADStatus`
   of 0x84 bytes (the 2007 SDK's size). The game takes **only the Remote's
   buttons** from it (`hold`).
3. By device type, one of three KPAD functions no symbol source names
   (8034A324, 8034A334, 8034A344: `kpad_wpad_status_core/fs/cl` in
   `tools/names-manual.tsv`), all tail calls into 8034A354
   (`kpad_wpad_status(chan, buf, dev)`): KPAD's copy of the WPAD samples it
   keeps (16 × 0x38 per channel in its state at 804FA518) of that device
   into a static ring of 16, the newest at `WPADGetLatestIndexInBuf(chan)`.
4. From the newest **`WPADCLStatus`** (0x36 bytes) the game reads the
   Classic entirely: `clButton` (0x2A), the sticks (0x2C/0x2E, 0x30/0x32,
   s16), the triggers (0x34/0x35, u8). With the Nunchuk it reads the stick
   from KPAD's `ex_status.fs` instead.

The game's own scaling of the raw values (8020F168 on): a stick is
`raw / 416 * 127`, clamped to ±72 (left) and ±64 (right), zero under 16;
a direction bit past 60 (left) or 52 (right); a trigger past 0x80 is
pressed (0x90 for the other devices).

The 2007 KPAD's own ranges (its parameters at r13-0x7354…): a Classic stick
travels 60..308 along the radius (`clamp_stick_circle`), a trigger 30..180;
the Nunchuk's stick 15..71.

## What wiikit had to learn

wiikit's KPAD HLE fills `KPADStatus.ex_status.cl`; the game never looks
there. With a pad on channel 1 the first boot crashed in
`WPADGetLatestIndexInBuf`, which read the control block of a WPAD never
started. So wiikit now also answers at WPAD's level (`wpad.cpp`, game-agnostic):

* `WPADRead(chan, status)`: the channel's sample, a `WPADStatus`,
  `WPADFSStatus` or `WPADCLStatus` by device;
* `WPADSetAutoSamplingBuf` kept, `WPADGetLatestIndexInBuf` answering 0 and
  writing the newest sample there;
* `kpad_wpad_status(chan, buf, dev)`: sixteen samples of the device, if it
  is `dev`, returned as `buf`.

The sticks are written where the 2007 KPAD would read the host's
deflection back (60 + 248·r along the radius), the triggers 30 + 150·t.

Played by the user at the first try with the Xbox One pad: the menus, the
System screen, a new game, the prologue, the first battle.

## Next

* The keyboard's layout for a JRPG (wiikit's defaults are Conduit 2's
  shooter keys): confirm, cancel, menu, camera.
* The GameCube pad path (`PADRead`): unused, left as it is.
