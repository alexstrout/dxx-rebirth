/*
 * This file is part of the DXX-Rebirth project <https://www.dxx-rebirth.com/>.
 * It is copyright by its individual contributors, as recorded in the
 * project's Git history.  See COPYING.txt at the top level for license
 * terms and a link to the Git history.
 */

#pragma once

#include <cstdint>

#ifdef DXX_BUILD_DESCENT
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
	/* Renamed from CLOAKED due to name conflict with AI define
	 * `CLOAKED`.
	 */
	player_cloaked = 2048,		// Player is cloaked for awhile
#if DXX_BUILD_DESCENT == 2
	has_team_flag = 16,			// Player has his team's flag
	ammo_rack = 128,	// Player has ammo rack
	converter = 256,	// Player has energy->shield converter
	afterburner = 4096,	// Player's afterburner is engaged
	headlight = 8192,	// Player has headlight boost
	headlight_on = 16384,	// is headlight on or off?
	headlight_present_and_on = headlight | headlight_on,	// required for thief
#endif
};

constexpr player_flag operator~(const player_flag a)
{
	return static_cast<player_flag>(~static_cast<uint32_t>(a));
}

constexpr player_flag operator|(const player_flag a, const player_flag b)
{
	return static_cast<player_flag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

#define PLAYER_FLAGS_INVULNERABLE	player_flag::invulnerable
#define PLAYER_FLAGS_BLUE_KEY	player_flag::blue_key
#define PLAYER_FLAGS_RED_KEY	player_flag::red_key
#define PLAYER_FLAGS_GOLD_KEY	player_flag::gold_key
#define PLAYER_FLAGS_MAP_ALL	player_flag::map_all
#define PLAYER_FLAGS_QUAD_LASERS	player_flag::quad_lasers
#define PLAYER_FLAGS_CLOAKED	player_flag::player_cloaked
#if DXX_BUILD_DESCENT == 2
#define PLAYER_FLAGS_FLAG	player_flag::has_team_flag
#define PLAYER_FLAGS_AMMO_RACK	player_flag::ammo_rack
#define PLAYER_FLAGS_CONVERTER	player_flag::converter
#define PLAYER_FLAGS_AFTERBURNER	player_flag::afterburner
#define PLAYER_FLAGS_HEADLIGHT	player_flag::headlight
#define PLAYER_FLAGS_HEADLIGHT_ON	player_flag::headlight_on
#endif

class player_flags
{
	uint32_t flags;
public:
	void operator&() const = delete;
	player_flags() = default;
	explicit player_flags(const uint32_t &f) : flags(f)
	{
	}
	explicit player_flags(player_flag f) : flags(static_cast<uint32_t>(f))
	{
	}
	uint32_t get_player_flags() const
	{
		return flags;
	}
	player_flags &operator&=(const player_flags &rhs)
	{
		flags &= rhs.flags;
		return *this;
	}
	player_flags &operator|=(const player_flags &rhs)
	{
		flags |= rhs.flags;
		return *this;
	}
	player_flags &operator|=(const uint32_t &rhs)
	{
		flags |= rhs;
		return *this;
	}
	player_flags operator|(const player_flags &rhs) const
	{
		return player_flags(flags | rhs.flags);
	}
	player_flags operator~() const
	{
		return player_flags(~flags);
	}
	uint32_t operator&(const player_flag value) const
	{
		return flags & static_cast<uint32_t>(value);
	}
	player_flags &operator^=(const player_flag value)
	{
		flags ^= static_cast<uint32_t>(value);
		return *this;
	}
	player_flags &operator&=(const player_flag value)
	{
		flags &= static_cast<uint32_t>(value);
		return *this;
	}
	player_flags &operator|=(const player_flag value)
	{
		flags |= static_cast<uint32_t>(value);
		return *this;
	}
};
#endif
