#include "ofMain.h"
#ifdef CMI_OPENCV_SMOKE
#include "ofxOpenCv.h"
#endif

class SmokeApp : public ofBaseApp {
public:
    void setup() override {
#ifdef CMI_OPENCV_SMOKE
        ofPixels pixels;
        pixels.allocate(128, 128, OF_PIXELS_RGB);
        pixels.setColor(ofColor(25, 140, 220));
        image.allocate(128, 128);
        image.setFromPixels(pixels);
        image.invert();
        const auto result = image.getPixels().getColor(0, 0);
        passed = result.r == 230 && result.g == 115 && result.b == 35;
        ofLogNotice("CMI_SMOKE") << (passed ? "OpenCV PASS" : "OpenCV FAIL");
#else
        ofLogNotice("CMI_SMOKE") << "openFrameworks PASS";
#endif
    }
    void draw() override {
        ofBackground(245);
        ofSetColor(20);
        ofDrawBitmapString("CMI browser smoke test", 30, 40);
#ifdef CMI_OPENCV_SMOKE
        ofDrawBitmapString(passed ? "OpenCV PASS" : "OpenCV FAIL", 30, 70);
        ofSetColor(255);
        image.draw(30, 100);
#else
        ofDrawBitmapString("openFrameworks PASS", 30, 70);
        ofSetColor(25, 140, 220);
        ofDrawCircle(100, 150, 45);
#endif
    }
#ifdef CMI_OPENCV_SMOKE
    ofxCvColorImage image;
    bool passed = false;
#endif
};

int main() {
    ofGLESWindowSettings settings;
    settings.setGLESVersion(3);
    settings.setSize(640, 360);
    auto window = ofCreateWindow(settings);
    ofRunApp(window, std::make_shared<SmokeApp>());
    ofRunMainLoop();
}
