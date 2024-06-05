#include "VersionControlPage.h"

void VersionControlPage::setup(string filePath)
{
	// Test object recognition
	ofPixels pixels;
	if (Media::isImage(filePath))
	{
		ofImage temp;
		temp.load(filePath);
		pixels = temp.getPixels();
	}
	else
	{
		ofVideoPlayer temp;
		temp.load(filePath);
		pixels = VideoMedia::extractFirstFrame(temp);
	}

	ofDirectory imgDir;
	imgDir.allowExt("jpg");
	imgDir.listDir("images/");

	ofDirectory vidDir;
	vidDir.allowExt("mp4");
	vidDir.listDir("videos/");

	vector<string> img_paths, vid_paths;
	img_paths.reserve(imgDir.size());
	vid_paths.reserve(vidDir.size());

	for (int i = 0; i < imgDir.size(); i++) {
		img_paths.push_back(imgDir.getPath(i));
	}

	for (int i = 0; i < vidDir.size(); i++) {
		vid_paths.push_back(vidDir.getPath(i));
	}

	vector<string> matching_paths = Metadata::filesWithObject(pixels, img_paths, vid_paths);
	
	img.allocate(pixels.getWidth(), pixels.getHeight());
	img.setFromPixels(pixels);

	homeBtn.setup("homeIcon.png", 100, 50, 50);
	mediaCir.setup(matching_paths);

	ofAddListener(homeBtn.clickedInside, this, &VersionControlPage::gotoHomePage);
	ofAddListener(mediaCir.clickedOnItem, this, &VersionControlPage::gotoFilePage);
}

void VersionControlPage::draw()
{
	float displaywidth, displayheight, xpos, ypos;
	Media::setFullScreenSizeAndPos(FILE_WIDTH, FILE_HEIGHT, &displaywidth, &displayheight, &xpos, &ypos);

	img.draw(xpos, ypos, displaywidth, displayheight);

	mediaCir.draw();
	homeBtn.draw();
}

void VersionControlPage::update()
{
	mediaCir.update();
}

void VersionControlPage::gotoHomePage()
{
	int PAGE = MAIN_PAGE;
	ofNotifyEvent(redirectEvent, PAGE, this);
}

void VersionControlPage::gotoFilePage()
{
	selectedFilePath = mediaCir.getCurrentFilePath();

	int PAGE;
	if (Media::isImage(selectedFilePath))
	{
		PAGE = IMAGE_PAGE;
	}
	else
	{
		PAGE = VIDEO_PAGE;
	}
	 
	ofNotifyEvent(redirectEvent, PAGE, this);
}


void VersionControlPage::mouseReleased(int x, int y, int button) {
	homeBtn.mouseReleased(x, y, button);
	mediaCir.mouseReleased(x, y, button);
}


string VersionControlPage::getCurrentFilePath() {
	return selectedFilePath;
}


void VersionControlPage::exit() {
	mediaCir.exit();
	img.clear();

	ofRemoveListener(homeBtn.clickedInside, this, &VersionControlPage::gotoHomePage);
	ofRemoveListener(mediaCir.clickedOnItem, this, &VersionControlPage::gotoFilePage);
}