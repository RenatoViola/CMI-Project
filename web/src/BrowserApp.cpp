#include "BrowserApp.h"

void BrowserApp::setup() {
    ofSetVerticalSync(true);
    carousel.setup("images/", 410, true);
    ofAddListener(carousel.clickedOnSelected, this, &BrowserApp::selectImage);
    ofAddListener(editor.redirectEvent, this, &BrowserApp::redirect);
    ofLogNotice("CMI_WEB") << "Image demo ready";
}

void BrowserApp::selectImage() {
    filePath = carousel.getCurrentFilePath();
    if (!filePath.empty()) {
        requestedVersion = 0;
        requestedPage = IMAGE_PAGE;
    }
}

void BrowserApp::redirect(int& destination) {
    requestedPage = destination;
}

void BrowserApp::openEditor(int version) {
    editor.setup(filePath, version);
    page = Page::Editor;
}

void BrowserApp::readVersions() {
    versions.clear();
    ofXml xml;
    if (xml.load("versions/" + filePath + ".xml")) {
        for (auto node : xml.find("//METADATA/VERSIONS/VERSION")) {
            versions.push_back({node.getAttribute("ID").getIntValue(), node.getChild("DATE").getValue()});
        }
    }
}

void BrowserApp::update() {
    // Page changes run after event handlers return, before rendering the next frame.
    if (requestedPage != -1) {
        const int destination = requestedPage;
        requestedPage = -1;
        if (page == Page::Editor) editor.exit();
        if (destination == IMAGE_PAGE) {
            openEditor(requestedVersion);
        } else if (destination == CONTROL_VERSION_PAGE) {
            readVersions();
            page = Page::Versions;
        } else {
            page = Page::Gallery;
        }
    }
    if (page == Page::Gallery) carousel.update();
}

void BrowserApp::draw() {
    ofBackground(255);
    if (page == Page::Gallery) {
        carousel.draw();
        ofSetColor(30);
        ofDrawBitmapString("CMI PROJECT / BROWSER IMAGE DEMO", 60, 70);
        ofDrawBitmapString("Drag the image row left or right. Click the centre image to edit.", 60, 105);
        ofDrawBitmapString("Image filters and saved versions are available. Saves last until you reload this page.", 60, 900);
    } else if (page == Page::Editor) {
        editor.draw();
    } else {
        ofSetColor(30);
        ofDrawBitmapString("SAVED FILTER VERSIONS", 100, 100);
        ofDrawBitmapString("Click a version to open it. Saves last until this page is reloaded.", 100, 135);
        ofDrawBitmapString("[ Return to gallery ]", 100, 180);
        ofDrawBitmapString("[ Open original ]", 100, 225);
        if (versions.empty()) ofDrawBitmapString("No versions yet. Use SAVE in the image editor.", 100, 280);
        for (std::size_t i = 0; i < versions.size(); ++i) {
            ofDrawBitmapString("Version " + ofToString(versions[i].id) + " / " + versions[i].date,
                100, 280 + static_cast<int>(i) * 35);
        }
    }
}

void BrowserApp::mousePressed(int x, int y, int button) {
    if (page == Page::Gallery) carousel.mousePressed(x, y, button);
}

void BrowserApp::mouseReleased(int x, int y, int button) {
    if (page == Page::Gallery) carousel.mouseReleased(x, y, button);
    else if (page == Page::Editor) editor.mouseReleased(x, y, button);
    else if (x >= 90 && x <= 850) {
        if (y >= 155 && y <= 195) requestedPage = MAIN_PAGE;
        else if (y >= 205 && y <= 240) {
            requestedVersion = 0;
            requestedPage = IMAGE_PAGE;
        } else if (y >= 260) {
            const auto index = static_cast<std::size_t>((y - 260) / 35);
            if (index < versions.size()) {
                requestedVersion = versions[index].id;
                requestedPage = IMAGE_PAGE;
            }
        }
    }
}

void BrowserApp::exit() {
    if (page == Page::Editor) editor.exit();
    ofRemoveListener(carousel.clickedOnSelected, this, &BrowserApp::selectImage);
    ofRemoveListener(editor.redirectEvent, this, &BrowserApp::redirect);
}
