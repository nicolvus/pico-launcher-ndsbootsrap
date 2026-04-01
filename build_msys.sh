#!/usr/bin/env bash
set -euo pipefail
eval "$(/opt/wonderful/bin/wf-config env generate)"
export BLOCKSDS BLOCKSDSEXT WONDERFUL_TOOLCHAIN
export PATH="$WONDERFUL_TOOLCHAIN/toolchain/gcc-arm-none-eabi/bin:$PATH"

echo "BLOCKSDS=$BLOCKSDS"
command -v arm-none-eabi-gcc && arm-none-eabi-gcc --version | head -1

cd "$(dirname "$0")"
# Override Makefile default if needed
export BLOCKSDS

if ! command -v make >/dev/null; then
  echo "Installing make via pacman..."
  pacman -S --needed --noconfirm make
fi

make -j"$(nproc 2>/dev/null || echo 4)" "$@"
