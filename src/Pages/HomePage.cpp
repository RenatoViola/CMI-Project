#include "HomePage.h"

void HomePage::setup() {

    vector<unique_ptr<Media>> images, videos;
    ofDirectory imageDir, videoDir;

    int mediaHeight = 240;
    int horizontalMiddle = (ofGetHeight() - mediaHeight) / 2;

    // Set up the image carousel
    imageDir.allowExt("jpg");
    imageDir.listDir("images/");

    images.reserve(imageDir.size());

    for (int i = 0; i < imageDir.size(); i++) {
        auto image = make_unique<ImageMedia>();
        image->load(imageDir.getPath(i));
        images.push_back(move(image));
    }

    // Top half of the screen
    imageCarrousel.setup(move(images), horizontalMiddle - mediaHeight / 2, "IMAGES");

    // Set up the video carousel
    videoDir.allowExt("mp4");
    videoDir.listDir("videos/");

    videos.reserve(videoDir.size());

    for (int i = 0; i < videoDir.size(); i++) {
        auto video = make_unique<VideoMedia>();
        video->load(videoDir.getPath(i));
        videos.push_back(move(video));
    }

    // Bottom half of the screen
    videoCarrousel.setup(move(videos), horizontalMiddle + mediaHeight, "VIDEOS");

    // Mouse event listeners
    ofAddListener(imageCarrousel.clickedOnSelected, this, &HomePage::gotoImagePage);
    ofAddListener(videoCarrousel.clickedOnSelected, this, &HomePage::gotoVideoPage);
    
    // Buttons and their listeners
    cameraBtn.setup("cameraIcon.png", 100, 80, 50);
    lockBtn.setup("lockIcon.png", 100, ofGetWidth() - 180, 50);

    ofAddListener(cameraBtn.clickedInside, this, &HomePage::gotoCameraPage);
    ofAddListener(lockBtn.clickedInside, this, &HomePage::gotoSavescreenPage);
}

void HomePage::exit() {
    imageCarrousel.exit();
    videoCarrousel.exit();

    ofRemoveListener(imageCarrousel.clickedOnSelected, this, &HomePage::gotoImagePage);
    ofRemoveListener(videoCarrousel.clickedOnSelected, this, &HomePage::gotoVideoPage);
    ofRemoveListener(cameraBtn.clickedInside, this, &HomePage::gotoCameraPage);
    ofRemoveListener(lockBtn.clickedInside, this, &HomePage::gotoSavescreenPage);
}

void HomePage::update() {
    imageCarrousel.update();
    videoCarrousel.update();
}

void HomePage::draw() {
    imageCarrousel.draw();
    videoCarrousel.draw();
    cameraBtn.draw();
    lockBtn.draw();
}

Media* HomePage::getSelectedMedia() {
    return selectedMedia;
}

void HomePage::mousePressed(int x, int y, int button) {
    imageCarrousel.mousePressed(x, y, button);
    videoCarrousel.mousePressed(x, y, button);
}

void HomePage::mouseReleased(int x, int y, int button) {
    imageCarrousel.mouseReleased(x, y, button);
    videoCarrousel.mouseReleased(x, y, button);
    cameraBtn.mouseReleased(x, y, button);
    lockBtn.mouseReleased(x, y, button);
}

void HomePage::gotoImagePage() {
    selectedMedia = imageCarrousel.getCurrentMedia();

    int PAGE = IMAGE_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void HomePage::gotoVideoPage() {
    selectedMedia = videoCarrousel.getCurrentMedia();

    int PAGE = VIDEO_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void HomePage::gotoCameraPage() {
    selectedMedia = imageCarrousel.getCurrentMedia();

    int PAGE = CAMERA_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void HomePage::gotoSavescreenPage() {
    selectedMedia = videoCarrousel.getCurrentMedia();

    int PAGE = SCREEN_SAVER_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}