#pragma once
#include "ofMain.h"
#include "UIElements/Carrousel.h"
#include "Pages/ImageEditor.h"

// Browser entry point for the image milestone. Reuses the desktop carousel,
// image editor, filters and XML version format without starting video decoders.
class BrowserApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void exit() override;

private:
    enum class Page { Gallery, Editor, Versions };
    struct Version { int id; std::string date; };
    void selectImage();
    void redirect(int& destination);
    void openEditor(int version);
    void readVersions();
    Page page = Page::Gallery;
    int requestedPage = -1;
    int requestedVersion = 0;
    std::string filePath;
    std::vector<Version> versions;
    Carrousel carousel;
    ImageEditor editor;
};
