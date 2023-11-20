#pragma warning(disable:4996)


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


std::string TCHARToString(const TCHAR* ptsz)
{
	int len = wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	std::string s = psz;
	delete[] psz;
	return s;
}

void playVideo(VideoCapture cap) {
	while (1) {
		cap >> frame;
		if (frame.empty()) {
			cout << "동영상 종료" << endl;
			break;
		}

		imshow("Video", frame);
		if (waitKey(33) >= 0) break;
	}
}


// FileOpen
void open(int id)
{
	OpenFileDialog* openFileDialog = new OpenFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = TCHARToString(openFileDialog->FileName);
		cap.open(Filename);	// 동영상 파일인 경우
		if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; exit; }
		playVideo(cap);
	}
}

//FileSave
void save(int id)
{
	SaveFileDialog* openFileDialog = new SaveFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = TCHARToString(openFileDialog->FileName);
		imwrite(Filename, src);
	}
}

int check = 1;


int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	GLUI_Master.set_glutIdleFunc(NULL);
	GLUI* glui = GLUI_Master.create_glui("Menu", 0);
	main_window = glui->get_glut_window_id();




	GLUI_Panel* buttonPanel = glui->add_panel("Button panel", GLUI_PANEL_RAISED);
	GLUI_Rollout* buttonRollout = glui->add_rollout_to_panel(buttonPanel, "CLICK HERE !!!", FALSE);
	glui->add_button_to_panel(buttonRollout, "Open Video", -1, open);
	glui->add_button_to_panel(buttonRollout, "Save Video", -1, save);
	glui->add_separator_to_panel(buttonRollout);
	glui->add_button_to_panel(buttonRollout, "Exit", 0, exit);


	// control panel
	GLUI_Panel* controlPanel = glui->add_panel("Control panel", GLUI_PANEL_NONE);
	glui->add_statictext_to_panel(controlPanel, "backward fast play");
	glui->add_button_to_panel(controlPanel, "<<", 0);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "  backward play");
	glui->add_button_to_panel(controlPanel, "<", 0);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "     pause/play");
	glui->add_button_to_panel(controlPanel, "||", 0);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "   forward play");
	glui->add_button_to_panel(controlPanel, ">", 0);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "forward fast play");
	glui->add_button_to_panel(controlPanel, ">>", 0);





	glui->set_main_gfx_window(main_window);
	glutMainLoop();
	return EXIT_SUCCESS;
}