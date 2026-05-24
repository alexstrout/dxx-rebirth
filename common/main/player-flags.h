/*
 * This file is part of the DXX-Rebirth project <https://www.dxx-rebirth.com/>.
 * It is copyright by its individual contributors, as recorded in the
 * project's Git history.  See COPYING.txt at the top level for license
 * terms and a link to the Git history.
 */

#pragma once

#ifdef DXX_BUILD_DESCENT
#include "d_bit_enum.h"

namespace dcx {

// Values for special flags
/* Only the low 16 bits are used, so this could be switched to `uint16_t`.
 * However, that generates worse code due to passing 16-bit values wherever
 * `player_flag` is used as an argument to a function.
 *
 * Descent 1 uses few enough distinct values that it could be reduced to
 * `uint8_t`.  However, some of its values are currently above UINT8_MAX, so
 * they would need to be renumbered.  That would require special code in the
 * save/load paths to map between the historical values written to disk and the
 * renumbered values used in memory.  The savings is not sufficient to justify
 * the complexity.
 */
enum class player_flag : uint32_t
{
	None = 0,
	invulnerable = 1,	// Player is invincible
	blue_key = 2,		// Player has blue key
	red_key = 4,		// Player has red key
	gold_key = 8,		// Player has gold key
	map_all = 64,		// Player can see unvisited areas on map
	quad_lasers = 1024,	// Player shoots 4 at once
	cloaked = 2048,		// Player is cloaked for awhile
	/* DXX_BUILD_DESCENT == 2 */
	has_team_flag = 16,			// Player has his team's flag
	ammo_rack = 128,	// Player has ammo rack
	converter = 256,	// Player has energy->shield converter
	afterburner = 4096,	// Player's afterburner is engaged
	headlight = 8192,	// Player has headlight boost
	headlight_on = 16384,	// is headlight on or off?
	headlight_present_and_on = headlight | headlight_on,	// required for thief
	/* endif */
};

constexpr player_flag operator~(const player_flag a)
{
	return static_cast<player_flag>(~static_cast<uint32_t>(a));
}

template <>
inline constexpr bool enable_bit_enum_bitnot<player_flag>{true};

template <>
inline constexpr bool enable_bit_enum_or<player_flag>{true};

enum class player_flags : uint32_t
{
};

template <>
inline constexpr bool enable_bit_enum_bitnot<player_flags>{true};

template <>
inline constexpr bool enable_bit_enum_boolnot<player_flags>{true};

template <>
inline constexpr bool enable_bit_enum_and<player_flags, player_flag>{true};

template <>
inline constexpr bool enable_bit_enum_and<player_flags, player_flags>{true};

template <>
inline constexpr bool enable_bit_enum_or<player_flags, player_flag>{true};

template <>
inline constexpr bool enable_bit_enum_or<player_flags>{true};

template <>
inline constexpr bool enable_bit_enum_xor<player_flags, player_flag>{true};

}
#endif
