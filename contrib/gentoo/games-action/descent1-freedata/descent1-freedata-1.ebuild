# Distributed under the terms of the GNU General Public License v2

EAPI=7

DXX_ENGINE="${PN:7:1}"
DESCRIPTION="Free content for games-action/d${DXX_ENGINE}x-rebirth"
HOMEPAGE='https://github.com/dxx-rebirth/dxx-rebirth/'
IUSE="l10n_de opl3-musicpack sc55-musicpack"
SRC_URI="
	l10n_de? ( https://raw.githubusercontent.com/pudlez/dxx-addons/8a624db1abdc8a87bd63719065e668e4be809628/d${DXX_ENGINE}xr-briefings-ger/d${DXX_ENGINE}xr-briefings-ger.dxa )
"

if [[ "${PN}" = descent1-freedata ]]; then
	inherit unpacker

	IUSE+=" +textures"
	# pudlez split the music archives into a 7z multipart archive.
	# Retrieve each part separately, and reconstruct the underlying dxa
	# at src_install time.
	SRC_URI+="
	opl3-musicpack? (
		https://raw.githubusercontent.com/pudlez/dxx-addons/8a624db1abdc8a87bd63719065e668e4be809628/d1xr-opl3-music/d1xr-opl3-music.7z.001
		https://raw.githubusercontent.com/pudlez/dxx-addons/8a624db1abdc8a87bd63719065e668e4be809628/d1xr-opl3-music/d1xr-opl3-music.7z.002
		)
	sc55-musicpack? (
		https://raw.githubusercontent.com/pudlez/dxx-addons/8a624db1abdc8a87bd63719065e668e4be809628/d1xr-sc55-music/d1xr-sc55-music.7z.001
		https://raw.githubusercontent.com/pudlez/dxx-addons/8a624db1abdc8a87bd63719065e668e4be809628/d1xr-sc55-music/d1xr-sc55-music.7z.002
		)
	textures? ( https://raw.githubusercontent.com/JeodC/RHH-Ports/e445f826136f26ab9f2898fc5b545b195b960db6/ports/released/descent-1-2/addons/descent/other/d1xr-hires.dxa )
"
	# Pass a placeholder filename to unpacker_src_uri_depends.  It needs
	# to see a `.7z` suffix so that it picks the right archive format,
	# but it will not access the named file.
	MY_BDEPEND_7z="$(unpacker_src_uri_depends music.7z)"
	# If any split files are in SRC_URI, then BDEPEND on a 7z package
	# list provided by unpacker.
	BDEPEND="
	opl3-musicpack? ( ${MY_BDEPEND_7z} )
	sc55-musicpack? ( ${MY_BDEPEND_7z} )
"
	unset MY_BDEPEND_7z
	_descent_freedata_doins_music() {
		unpack_7z "${DISTDIR}/$1.7z.001"
		doins "$1.dxa"
	}
else
	# D2X music is not split, so it can be downloaded normally and does
	# not need post-processing.
	SRC_URI+="
	opl3-musicpack? (
		https://raw.githubusercontent.com/pudlez/dxx-addons/8a624db1abdc8a87bd63719065e668e4be809628/d2xr-opl3-music/d2xr-opl3-music.dxa
		)
	sc55-musicpack? (
		https://raw.githubusercontent.com/pudlez/dxx-addons/8a624db1abdc8a87bd63719065e668e4be809628/d2xr-sc55-music/d2xr-sc55-music.dxa
		)
"
	_descent_freedata_doins_music() {
		doins "${DISTDIR}/$1.dxa"
	}
fi

LICENSE="public-domain"
SLOT="0"
KEYWORDS="~amd64 ~x86"
S="$WORKDIR"

# If all USE flags are unset, this ebuild installs zero files.  Require
# at least one to be set.
REQUIRED_USE="|| ( ${IUSE//+/} )"

RDEPEND="
	!<games-action/dxx-rebirth-0.60
	!games-action/d${DXX_ENGINE}x-rebirth
	"

unset DXX_ENGINE

src_unpack() {
	:
}

src_install() {
	local DXX_ENGINE="${PN:7:1}"
	insinto /usr/share/games/d${DXX_ENGINE}x
	# Music files may be split.  Delegate to a package-name-specific
	# function to handle that.
	use opl3-musicpack && _descent_freedata_doins_music d${DXX_ENGINE}xr-opl3-music
	use sc55-musicpack && _descent_freedata_doins_music d${DXX_ENGINE}xr-sc55-music
	# German briefings are never split.
	use l10n_de && doins "${DISTDIR}"/d${DXX_ENGINE}xr-briefings-ger.dxa

	# This ebuild is used for both Descent 1 free data and Descent 2
	# free data.  Only Descent 1 provides alternate textures.
	if [[ "${PN}" = descent1-freedata ]] && use textures; then
		doins "${DISTDIR}"/d1xr-hires.dxa
	fi
}
