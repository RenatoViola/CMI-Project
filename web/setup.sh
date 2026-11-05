#!/usr/bin/env bash
set -euo pipefail
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TOOLCHAIN_DIR="${TOOLCHAIN_DIR:-$HOME/cmi-web-toolchain}"
OF_REVISION=9917dd14b52f78b689e0b82fcb070e267789989d
EMSCRIPTEN_VERSION=6.0.13
LIBS_SHA256=1bee005bb6453d572357ddc687dcdbb7bbbcb09bbddf48b9c590436a077da0da
for tool in git make g++ curl python3 tar bzip2 pkg-config; do
    command -v "$tool" >/dev/null || { echo "Missing $tool. Install build-essential curl python3 bzip2 pkg-config first." >&2; exit 1; }
done
mkdir -p "$TOOLCHAIN_DIR" "$PROJECT_DIR/.web-build"
OF_ROOT="$TOOLCHAIN_DIR/openFrameworks"
EMSDK_DIR="$TOOLCHAIN_DIR/emsdk"
if [ ! -d "$OF_ROOT" ]; then
    mkdir -p "$OF_ROOT"
    curl -fLsS "https://codeload.github.com/openframeworks/openFrameworks/tar.gz/$OF_REVISION" -o "$TOOLCHAIN_DIR/of-source.tar.gz"
    tar -xzf "$TOOLCHAIN_DIR/of-source.tar.gz" -C "$OF_ROOT" --strip-components=1
    printf '%s\n' "$OF_REVISION" > "$OF_ROOT/.cmi-revision"
elif [ -f "$OF_ROOT/.cmi-revision" ]; then
    test "$(cat "$OF_ROOT/.cmi-revision")" = "$OF_REVISION"
elif [ -d "$OF_ROOT/.git" ]; then
    test "$(git -C "$OF_ROOT" rev-parse HEAD)" = "$OF_REVISION"
else
    echo "Cannot verify the existing openFrameworks source in $OF_ROOT" >&2; exit 1
fi
if [ ! -f "$EMSDK_DIR/emsdk.py" ]; then git clone --depth 1 https://github.com/emscripten-core/emsdk.git "$EMSDK_DIR"; fi
python3 "$EMSDK_DIR/emsdk.py" install "$EMSCRIPTEN_VERSION"
python3 "$EMSDK_DIR/emsdk.py" activate "$EMSCRIPTEN_VERSION"
if [ ! -f "$OF_ROOT/.cmi-libs-verified" ]; then
    (cd "$OF_ROOT" && bash scripts/emscripten/download_libs.sh -s)
    printf '%s  %s\n' "$LIBS_SHA256" "$OF_ROOT/libs/download/openFrameworksLibs_latest_emscripten_32.tar.bz2" | sha256sum -c -
    printf '%s\n' "$LIBS_SHA256" > "$OF_ROOT/.cmi-libs-verified"
fi
printf 'OF_ROOT=${OF_ROOT:-%q}\nEMSDK_DIR=${EMSDK_DIR:-%q}\n' "$OF_ROOT" "$EMSDK_DIR" > "$PROJECT_DIR/.web-build/toolchain.env"
echo 'Toolchain ready. Run: bash web/build.sh smoke'
