#include <opencv2/opencv.hpp>
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"

using namespace cv;
using namespace std;

String Filename;
VideoCapture cap;
Mat src, frame;
int main_window;

double fps;
int speed;
int frameCount;
int isPlay = -1;
int isButtonPushF = -1;
int isButtonPushB = -1;


void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	   undefined during an idle callback.  So we need to explicitly change
	   it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	while (cap.isOpened()) {
		if (isPlay == 1) {
			bool isFinish = cap.read(frame);
			if (!isFinish) {
				cout << "동영상 종료" << endl;
				isPlay = -1;
				break;
			}
			imshow("src", frame);
		}
		else if (isButtonPushB == 1) {
			int currentFrame = cap.get(CAP_PROP_POS_FRAMES);

			if (currentFrame <= 2) cap.set(CAP_PROP_POS_FRAMES, 0);
			else cap.set(CAP_PROP_POS_FRAMES, currentFrame - 2);

			cap.read(frame);
			imshow("src", frame);
		}
		if (waitKey(speed) > 0) break;
	}
}

// 뒤로감기
void backwardSpeedCallback(int id) {
	isButtonPushB *= id;
	isPlay *= id;

	if (isButtonPushB == 1) speed = (int)(speed / 4);
	else speed = (int)(1000 / fps);
}


// 뒤로 5초
void backwardCallback(int id) {
	int frames = id * fps;
	if (!cap.read(frame)) cap.set(CAP_PROP_POS_FRAMES, frameCount - frames);
	else if (cap.get(CAP_PROP_POS_MSEC) < id * 1000) cap.set(CAP_PROP_POS_FRAMES, 0);
	else cap.set(CAP_PROP_POS_FRAMES, cap.get(CAP_PROP_POS_FRAMES) - frames);
	cap.read(frame);
	imshow("src", frame);
}

// 재생/정지
void playCallback(int id) {
	if (isButtonPushB == 1) {
		isPlay = -1; isButtonPushB = -1;
	}
	else isPlay *= id;
	speed = (int)(1000 / fps);
}

// 5초 앞으로
void forwardCallback(int id) {
	if (!cap.read(frame)) return;
	else if (((frameCount * 1000 / fps) - cap.get(CAP_PROP_POS_MSEC)) < id * 1000)
		cap.set(CAP_PROP_POS_FRAMES, frameCount - 1);
	else {
		int frames = id * fps;
		cap.set(CAP_PROP_POS_FRAMES, cap.get(CAP_PROP_POS_FRAMES) + frames);
	}
	cap.read(frame);
	imshow("src", frame);
}

// 빨리감기
void forwardSpeedCallback(int id) {
	isButtonPushF *= id;
	if (isButtonPushF == 1) speed = (int)(speed / 3);
	else speed = (int)(1000 / fps);
}


// FileOpen
void open(int id)
{
	switch (id)
	{
		case 0: {
			cap.open(id);
			if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; exit; }
			for (int i = 0; i < 10; i++) cap >> frame;
			isPlay = -1;
			imshow("src", frame);
			break;
		}
		case 1: {
			cap.open(id);
			if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; exit; }
			for (int i = 0; i < 5; i++) cap >> frame;
			isPlay = -1;
			imshow("src", frame);
			break;
		}
		case 2: {
			OpenFileDialog* openFileDialog = new OpenFileDialog();
			openFileDialog->Filter = "Image Files(*.BMP;*.JPG;*.GIF)|*.BMP;*.JPG;*.GIF|All files (*.*)|*.*";
			if (openFileDialog->ShowDialog()) {
				Filename = openFileDialog->FileName;
				src = imread(Filename);
				isPlay = -1;
				imshow("image", src);
			}
			else exit;
			break;
		}
	default: {
		OpenFileDialog* openVideoDialog = new OpenFileDialog();
		if (openVideoDialog->ShowDialog()) {
			Filename = openVideoDialog->FileName;
			cap.open(Filename);	// 동영상 파일인 경우
			if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; exit; }
			cap >> frame;
			isPlay = -1;
			fps = cap.get(CAP_PROP_FPS);
			speed = (int)(1000 / fps);
			frameCount = cap.get(CAP_PROP_FRAME_COUNT);
			imshow("src", frame);
		}
		else exit;
		break;
	}
	}
}

//FileSave
void save(int id)
{
	SaveFileDialog* openFileDialog = new SaveFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = openFileDialog->FileName;
		imwrite(Filename, src);
	}
}


int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	GLUI_Master.set_glutIdleFunc(NULL);
	GLUI* glui = GLUI_Master.create_glui("Menu", 0);
	main_window = glui->get_glut_window_id();

	
	GLUI_Panel* videoPanel = glui->add_panel("Video panel", GLUI_PANEL_RAISED);
	GLUI_Rollout* buttonRollout = glui->add_rollout_to_panel(videoPanel, "CLICK HERE", FALSE);
	glui->add_button_to_panel(buttonRollout, "Open Video", -1, open);
	glui->add_button_to_panel(buttonRollout, "Save Video", -1, save);
	glui->add_separator_to_panel(buttonRollout);
	glui->add_button_to_panel(buttonRollout, "Exit", 0, exit);

	glui->add_column_to_panel(videoPanel, TRUE);

	glui->add_button_to_panel(videoPanel, "Use Camera", 0, open);
	glui->add_button_to_panel(videoPanel, "Use Smartphone", 1, open);

	// control panel
	GLUI_Panel* controlPanel = glui->add_panel("Control panel", GLUI_PANEL_NONE);
	glui->add_statictext_to_panel(controlPanel, "backward fast play");
	glui->add_button_to_panel(controlPanel, "<<", -1, backwardSpeedCallback);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "  backward play");
	glui->add_button_to_panel(controlPanel, "<", 5, backwardCallback);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "     pause/play");
	glui->add_button_to_panel(controlPanel, "||", -1, playCallback);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "   forward play");
	glui->add_button_to_panel(controlPanel, ">", 5, forwardCallback);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "forward fast play");
	glui->add_button_to_panel(controlPanel, ">>", -1, forwardSpeedCallback);

	glui->add_column(TRUE);

	GLUI_Panel* picturePanel = glui->add_panel("Picture panel", GLUI_PANEL_EMBOSSED);
	glui->add_button_to_panel(picturePanel, "Open source", 2, open);


	glui->set_main_gfx_window(main_window);

	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();
	return EXIT_SUCCESS;
}