// Arc Rise Fantasia — the port's own layer over the wiikit runtime.
//
// Linked into wiiboot by arf.cmake. What belongs here is what only this game
// needs.
#include "rt.h"

namespace {

// The game plays with the Classic Controller (docs/08-input.md), read through
// the 2007 SDK's KPAD: KPADRead's status is 0x84 bytes.
void install() {
    wpad_set_kpad_status_size(0x84);
    wpad_set_classic(true);
}

RtGameLayer layer("Arc Rise Fantasia", install);

}  // namespace
