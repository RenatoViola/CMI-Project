# CMI browser prototype

This is the image milestone of the Emscripten port. The desktop entry point remains
in `src/`; the browser entry point lives in `web/src/`. It reuses the original
carousel, ImageEditor, ASCII renderer, OpenCV filters, buttons and XML filter format.

## Build on Windows using Ubuntu / WSL

Run the following from this repository in an Ubuntu shell. The setup needs git,
Python 3, build-essential, curl, bzip2 and pkg-config. It downloads the toolchain
into `~/cmi-web-toolchain`, where compilation is much faster than on `/mnt/c`.
Keep the openFrameworks path outside hidden directories: its makefiles exclude
include paths containing hidden directory names.

```bash
bash web/setup.sh
bash web/build.sh smoke
bash web/build.sh opencv-smoke
bash web/build.sh demo
```

The tools are pinned to openFrameworks commit
`9917dd14b52f78b689e0b82fcb070e267789989d` and Emscripten `6.0.13`.
Setup checks the OpenFrameworks library archive against its recorded SHA-256.
The upstream `latest` library URL is mutable: if its contents change, setup will
fail the checksum check. Review and update the dependency lock before using a new
archive; do not bypass the check. No global shell profile changes are required.

`TOOLCHAIN_DIR` can override the setup location. `OF_ROOT`, `EMSDK_DIR` and `JOBS`
can override the paths or build parallelism. The selected tool paths are recorded
locally in `.web-build/toolchain.env`, which is excluded from git.

## Run locally

From Windows PowerShell, in this repository:

```powershell
python web/serve.py
```

Open <http://localhost:8080/demo/>. The basic tests live at `/smoke/` and
`/opencv-smoke/`. The server adds the isolation headers required by WebAssembly
threads. Opening `index.html` directly or using a plain server without these
headers will not work reliably.

Drag the gallery left/right and click the centre image. Use the filter panel on
the right. SAVE stores the filter settings for this browser session; the version
button on the left opens the saved settings. Reloading clears these saves.

## Browser checks

Install the Python `playwright` package into a local virtual environment. The
checks use the installed Microsoft Edge in headless mode; there is no browser
download step. With the local server running:

```powershell
python web/check_browser.py smoke
python web/check_browser.py opencv-smoke
python web/check_browser.py demo
```

These wait for a message from the compiled C++, verify cross-origin isolation,
capture browser errors, and save screenshots under `.web-build/qa/`. The OpenCV
smoke test checks the numeric result of a colour inversion, in addition to drawing.
The demo check also toggles and resets all seven filters, verifies the saved XML,
reopens a saved version and selects the other gallery image.

Verified on November 5, 2026 in headless Microsoft Edge: the graphics smoke test,
OpenCV numeric test, gallery, seven filters and session version round trip passed
without JavaScript exceptions. Software rendering emitted driver performance
warnings; hardware rendering and other browsers still need checking.

## Hosting

Upload the contents of `dist/` to a static host supporting custom isolation
headers. The included `_headers` file is suitable for Cloudflare Pages. The root
page opens `/demo/`. Validate the deployed app in a browser after upload.

Only two bundled sample images are packaged initially. Each deployed file must
remain below the selected host's file-size limit (25 MiB for Cloudflare Pages).
No server-side C++ process is required.

## Remaining work

- Camera capture, face detection, SIFT matching and gesture navigation.
- Asynchronous video loading / frame extraction; desktop busy-wait loops cannot
  be carried over unchanged.
- Persistent version storage (IndexedDB / IDBFS) and image import/export.
- Responsive native layout and touch interaction. The existing desktop canvas is
  scaled to fit the page for this milestone.
- Desktop build regression verification, which requires the original Windows
  openFrameworks / Visual Studio environment.

The browser build deliberately does not compile or start those camera/video and
metadata paths yet. The native source files remain available for later milestones.
