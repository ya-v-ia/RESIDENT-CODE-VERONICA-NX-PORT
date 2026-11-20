#!/bin/bash
# Nintendo Switch 2 build (homebrew NRO) with ounce-devkit (libounce + OunceVK) and devkitA64.
#   scripts/build-switch2.sh [extra cmake -D args / ninja args after --]
# Output: build-switch2/pc/recvx.elf and build-switch2/pc/recvx.nro.
set -eu
root=$(cd "$(dirname "$0")/.." && pwd)
dkp=${DEVKITPRO:-/opt/devkitpro}
export DEVKITPRO=$dkp
if [ ! -f "$dkp/libounce/cmake/Switch2.cmake" ]; then
  echo "error: ounce-devkit not found at $dkp/libounce/cmake/Switch2.cmake" >&2
  echo "Install it via https://github.com/ouncebrew/ounce-devkit (sh install.sh)" >&2
  exit 1
fi
dir=${BUILD_DIR:-$root/build-switch2}
cargs=(); nargs=()
while [ $# -gt 0 ]; do
  if [ "$1" = -- ]; then shift; nargs=("$@"); break; fi
  cargs+=("$1"); shift
done
cmake -S "$root" -B "$dir" -G Ninja -DCMAKE_TOOLCHAIN_FILE="$dkp/libounce/cmake/Switch2.cmake" \
      -DCMAKE_BUILD_TYPE="${BUILD_TYPE:-RelWithDebInfo}" -DRECVX_64=ON -DRECVX_PIE=ON "${cargs[@]}"
ninja -C "$dir" "${nargs[@]:-recvx_nro}"
