#pragma once

#include <_internal/state.h>

extern void P6(state_t* state);
extern void P8(state_t* state);
extern void P12(state_t* state);

extern const uint32_t* ascon_round_constants_bi;
extern const uint64_t ascon_round_constants[];

#include "../../ascon-p/perm_api.h"