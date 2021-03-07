#include <stdint.h>

typedef uint8_t   u8;

extern void NewAddHatchedMonToParty(u8 id);

void HookAddHatchedMonToParty(u8 id)
{
    NewAddHatchedMonToParty(id);
}