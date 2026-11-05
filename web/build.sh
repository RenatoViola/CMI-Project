#!/usr/bin/env bash
set -euo pipefail
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
if [ -f "$PROJECT_DIR/.web-build/toolchain.env" ]; then source "$PROJECT_DIR/.web-build/toolchain.env"; fi
OF_ROOT="${OF_ROOT:-$HOME/cmi-web-toolchain/openFrameworks}"
EMSDK_DIR="${EMSDK_DIR:-$HOME/cmi-web-toolchain/emsdk}"
if [ ! -f "$EMSDK_DIR/emsdk_env.sh" ] || [ ! -f "$OF_ROOT/libs/openFrameworks/ofMain.h" ]; then
    echo 'Toolchain missing. Run: bash web/setup.sh' >&2; exit 1
fi
MODE="${1:-demo}"
case "$MODE" in demo|smoke|opencv-smoke) ;; *) echo 'Usage: bash web/build.sh [demo|smoke|opencv-smoke]' >&2; exit 2;; esac
source "$EMSDK_DIR/emsdk_env.sh"
STAGE="$PROJECT_DIR/.web-build/$MODE"
mkdir -p "$STAGE/src" "$STAGE/bin/data" "$PROJECT_DIR/dist"
cp "$OF_ROOT/scripts/templates/linux64/Makefile" "$STAGE/Makefile"
printf 'OF_ROOT = %s\nPROJECT_DEFINES = CMI_WEB_DEMO\n' "$OF_ROOT" > "$STAGE/config.make"
printf 'PROJECT_EMSCRIPTEN_TEMPLATE = %s/web/shell.html\n' "$PROJECT_DIR" >> "$STAGE/config.make"
if [ "$MODE" = smoke ]; then
    : > "$STAGE/addons.make"
    cp "$PROJECT_DIR/web/smoke/main.cpp" "$STAGE/src/main.cpp"
elif [ "$MODE" = opencv-smoke ]; then
    printf 'ofxOpenCv\n' > "$STAGE/addons.make"
    printf 'PROJECT_DEFINES += CMI_OPENCV_SMOKE\n' >> "$STAGE/config.make"
    cp "$PROJECT_DIR/web/smoke/main.cpp" "$STAGE/src/main.cpp"
else
    printf 'ofxGui\nofxOpenCv\n' > "$STAGE/addons.make"
    cp "$PROJECT_DIR/web/src/"* "$STAGE/src/"
    mkdir -p "$STAGE/src/Pages" "$STAGE/src/UIElements" "$STAGE/src/DataTypes"
    cp "$PROJECT_DIR/src/screen_names.h" "$PROJECT_DIR/src/filter_names.h" "$STAGE/src/"
    for name in Media ImageMedia; do cp "$PROJECT_DIR/src/DataTypes/$name.h" "$STAGE/src/DataTypes/"; done
    for name in ImageEditor MediaEditor; do cp "$PROJECT_DIR/src/Pages/$name.h" "$STAGE/src/Pages/"; done
    for name in Button Carrousel FilterPanel; do cp "$PROJECT_DIR/src/UIElements/$name."* "$STAGE/src/UIElements/"; done
    cp "$PROJECT_DIR/bin/data/"*.ttf "$STAGE/bin/data/"
    cp -R "$PROJECT_DIR/bin/data/icons" "$STAGE/bin/data/"
    mkdir -p "$STAGE/bin/data/images" "$STAGE/bin/data/versions/images"
    cp "$PROJECT_DIR/bin/data/images/220-Maryland-Ave-21122-1.jpg" "$STAGE/bin/data/images/"
    cp "$PROJECT_DIR/bin/data/images/SmallHouseExteriors-Twitter-card-B-LOGO.jpg" "$STAGE/bin/data/images/"
fi
emmake make -C "$STAGE" -j"${JOBS:-4}" Release
OUTPUT="$STAGE/bin/em/$MODE"
if [ ! -f "$OUTPUT/index.html" ]; then echo "Missing browser output: $OUTPUT" >&2; exit 1; fi
mkdir -p "$PROJECT_DIR/dist/$MODE"
cp -R "$OUTPUT/". "$PROJECT_DIR/dist/$MODE/"
cp "$PROJECT_DIR/web/browser.js" "$PROJECT_DIR/web/browser.css" "$PROJECT_DIR/dist/$MODE/"
cp "$PROJECT_DIR/web/_headers" "$PROJECT_DIR/dist/_headers"
cp "$PROJECT_DIR/web/index.html" "$PROJECT_DIR/dist/index.html"
printf '\nReady: %s/dist/%s/index.html\n' "$PROJECT_DIR" "$MODE"
