#include "HomePage.h"

void HomePage::setup() {

    vector<unique_ptr<Media>> images, videos;
    ofDirectory imageDir, videoDir;

    int mediaHeight = 240;
    int horizontalMiddle = (ofGetHeight() - mediaHeight) / 2;

    // Set up the image carousel
    imageDir.allowExt("jpg");
    imageDir.allowExt("png");
    imageDir.listDir("images/");

    images.reserve(imageDir.size());

    for (int i = 0; i < imageDir.size(); i++) {
        auto image = make_unique<ImageMedia>();
        image->load(imageDir.getPath(i));
        images.push_back(move(image));
    }

    // Top half of the screen
    imageCarrousel.setup(move(images), horizontalMiddle - mediaHeight, "IMAGES");

    // Set up the video carousel
    videoDir.allowExt("mp4");
    videoDir.allowExt("avi");
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
    ofAddListener(imageCarrousel.clickedOnSelected, this, &HomePage::clickOnImage);
    ofAddListener(videoCarrousel.clickedOnSelected, this, &HomePage::clickOnVideo);
}

void HomePage::exit() {
    imageCarrousel.exit();
    videoCarrousel.exit();

    ofRemoveListener(imageCarrousel.clickedOnSelected, this, &HomePage::clickOnImage);
    ofRemoveListener(videoCarrousel.clickedOnSelected, this, &HomePage::clickOnVideo);
}

void HomePage::update() {
    imageCarrousel.update();
    videoCarrousel.update();
}

void HomePage::draw() {
    imageCarrousel.draw();
    videoCarrousel.draw();
}

void HomePage::mousePressed(int x, int y, int button) {
    imageCarrousel.mousePressed(x, y, button);
    videoCarrousel.mousePressed(x, y, button);
}

void HomePage::mouseReleased(int x, int y, int button) {
    imageCarrousel.mouseReleased(x, y, button);
    videoCarrousel.mouseReleased(x, y, button);
}

void HomePage::clickOnImage() {
    selectedMedia = imageCarrousel.getCurrentMedia();

    int PAGE = IMAGE_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void HomePage::clickOnVideo() {
    selectedMedia = videoCarrousel.getCurrentMedia();

    int PAGE = VIDEO_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

Media* HomePage::getSelectedMedia() {
    return selectedMedia;
}