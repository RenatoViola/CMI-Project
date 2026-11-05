#include "ofMain.h"
#include "BrowserApp.h"

int main() {
    ofGLESWindowSettings settings;
    settings.setGLESVersion(3);
    settings.setSize(1920, 1032);
    auto window = ofCreateWindow(settings);
    ofRunApp(window, std::make_shared<BrowserApp>());
    ofRunMainLoop();
}
