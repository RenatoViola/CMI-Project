"""Browser checks for the built demo. Requires Playwright and Microsoft Edge."""
import argparse
import json
from pathlib import Path
from playwright.sync_api import sync_playwright


def canvas_click(page, x, y):
    box = page.locator("#canvas").bounding_box()
    width, height = page.locator("#canvas").evaluate("c => [c.width, c.height]")
    page.mouse.click(box["x"] + x * box["width"] / width,
                     box["y"] + y * box["height"] / height)


def run(mode, base):
    output = Path(__file__).resolve().parents[1] / ".web-build" / "qa"
    output.mkdir(parents=True, exist_ok=True)
    logs, errors = [], []
    with sync_playwright() as p:
        browser = p.chromium.launch(channel="msedge", headless=True,
                                    args=["--enable-webgl", "--use-angle=swiftshader", "--enable-unsafe-swiftshader"])
        page = browser.new_page(viewport={"width": 1440, "height": 1000}, device_scale_factor=1)
        page.on("console", lambda msg: logs.append(f"{msg.type}: {msg.text}"))
        page.on("pageerror", lambda error: errors.append(str(error)))
        response = page.goto(f"{base}/{mode}/", wait_until="load")
        assert response.ok, response.status
        assert page.evaluate("crossOriginIsolated"), "Missing browser isolation headers"
        marker = "Image demo ready" if mode == "demo" else ("OpenCV PASS" if mode == "opencv-smoke" else "openFrameworks PASS")
        # Wait for a C++ log message, not just a loaded HTML shell.
        for _ in range(120):
            if any(marker in line for line in logs):
                break
            page.wait_for_timeout(500)
        assert any(marker in line for line in logs), json.dumps({"logs": logs[-20:], "errors": errors})
        page.wait_for_timeout(1500)
        page.screenshot(path=str(output / f"{mode}.png"), full_page=True)
        if mode == "demo":
            canvas_click(page, 960, 530)
            page.wait_for_timeout(1000)
            page.screenshot(path=str(output / "editor.png"), full_page=True)
            original = page.locator("#canvas").screenshot()
            # These coordinates are in the original desktop canvas, regardless
            # of the CSS scale used by the browser shell.
            for name, y in [("invert", 104), ("blur", 122), ("gaussian", 140),
                            ("dilate", 158), ("erode", 176), ("edge", 84), ("ascii", 65)]:
                canvas_click(page, 1710, y)
                page.wait_for_timeout(200)
                assert page.locator("#canvas").screenshot() != original, f"{name} had no visible effect"
                canvas_click(page, 1710, y)
                page.wait_for_timeout(200)
                assert page.locator("#canvas").screenshot() == original, f"{name} did not reset"
            canvas_click(page, 1710, 104)
            page.wait_for_timeout(200)
            inverted = page.locator("#canvas").screenshot()
            canvas_click(page, 1710, 238)  # SAVE control
            page.wait_for_timeout(200)
            saved = page.evaluate("""() => {
                const fs = Module.FS;
                const folder = '/data/versions/images';
                const name = fs.readdir(folder).find(n => n.endsWith('.xml'));
                return name ? fs.readFile(folder + '/' + name, {encoding: 'utf8'}) : '';
            }""")
            assert '<INVERTED_COLORS>1</INVERTED_COLORS>' in saved, saved
            canvas_click(page, 100, 250)  # Version button
            page.wait_for_timeout(200)
            page.screenshot(path=str(output / "versions.png"), full_page=True)
            canvas_click(page, 200, 280)  # First saved version
            page.wait_for_timeout(400)
            assert page.locator("#canvas").screenshot() == inverted, "Saved version did not restore filters"
            page.screenshot(path=str(output / "inverted.png"), full_page=True)
            canvas_click(page, 100, 100)  # Home
            page.wait_for_timeout(300)
            box = page.locator('#canvas').bounding_box()
            sx, sy = box['width'] / 1920, box['height'] / 1032
            page.mouse.move(box['x'] + 1500 * sx, box['y'] + 530 * sy)
            page.mouse.down()
            page.mouse.move(box['x'] + 1200 * sx, box['y'] + 530 * sy, steps=12)
            page.mouse.up()
            page.wait_for_timeout(1200)
            canvas_click(page, 960, 530)
            page.wait_for_timeout(500)
            assert page.locator("#canvas").screenshot() != original, "Carousel did not select the second image"
        assert not errors, errors
        result = {"mode": mode, "passed": True, "isolation": True, "errors": errors, "logs": logs}
        (output / f"{mode}.json").write_text(json.dumps(result, indent=2), encoding="utf-8")
        print(json.dumps(result, indent=2))
        browser.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mode", choices=["smoke", "opencv-smoke", "demo"])
    parser.add_argument("--base", default="http://localhost:8080")
    args = parser.parse_args()
    run(args.mode, args.base)
