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

int speed = 33;
int isPlay = -1;
int isButtonPush = -1;


void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	   undefined during an idle callback.  So we need to explicitly change
	   it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	while (isPlay == 1) {
		cap >> frame;
		if (frame.empty()) {
			cout << "동영상 종료" << endl;
			isPlay = -1;
			break;
		}

		imshow("src", frame);
		if (waitKey(speed) > 0) break;
	}
}


void playCallback(int id) {
	isPlay *= id;
}

void forwardCallback(int id) {
	for (int i = 0; i < id; i++)
		cap >> frame;
}

void forwardSpeedCallback(int id) {
	isButtonPush *= id;
	if (isButtonPush == 1) speed = 11;
	else speed = 33;
}


// FileOpen
void open(int id)
{
	switch (id)
	{
		case 0: {
			cap.open(id);
			if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; exit; }
			for (int i = 0; i < 5; i++) cap >> frame;
			isPlay = -1;
			imshow("src", frame);
			break;
		}
		case 1: {
			OpenFileDialog* openFileDialog = new OpenFileDialog();
			openFileDialog->Filter = "Image Files(*.BMP;*.JPG;*.GIF)|*.BMP;*.JPG;*.GIF|All files (*.*)|*.*";
			if (openFileDialog->ShowDialog()) {
				Filename = openFileDialog->FileName;
				src = imread(Filename);
				isPlay = -1;
				imshow("image", src);
			}
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
			imshow("src", frame);
		}
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



int check = 1;


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

	// control panel
	GLUI_Panel* controlPanel = glui->add_panel("Control panel", GLUI_PANEL_NONE);
	glui->add_statictext_to_panel(controlPanel, "backward fast play");
	glui->add_button_to_panel(controlPanel, "<<", 0);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "  backward play");
	glui->add_button_to_panel(controlPanel, "<", 0);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "     pause/play");
	glui->add_button_to_panel(controlPanel, "||", -1, playCallback);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "   forward play");
	glui->add_button_to_panel(controlPanel, ">", 151, forwardCallback);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "forward fast play");
	glui->add_button_to_panel(controlPanel, ">>", -1, forwardSpeedCallback);

	glui->add_column(TRUE);

	GLUI_Panel* picturePanel = glui->add_panel("Picture panel", GLUI_PANEL_EMBOSSED);
	glui->add_button_to_panel(picturePanel, "Open source", 1, open);


	glui->set_main_gfx_window(main_window);

	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();
	return EXIT_SUCCESS;
}