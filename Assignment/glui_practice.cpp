#pragma warning(disable:4996)


#include <opencv2/opencv.hpp>
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"


using namespace cv;

String Filename;
Mat src;
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


void open(int id)
{
	OpenFileDialog* openFileDialog = new OpenFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = TCHARToString(openFileDialog->FileName);
		src = imread(Filename);
		imshow("src", src);
	}
}


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

	

	
	GLUI_Panel* buttonPanel = glui->add_panel("", GLUI_PANEL_RAISED);
	GLUI_Rollout* buttonRollout = glui->add_rollout_to_panel(buttonPanel, "CLICK");
	glui->add_button_to_panel(buttonRollout, "Open File", 1, open);
	glui->add_button_to_panel(buttonRollout, "Save File", 1, save);
	glui->add_separator_to_panel(buttonRollout);
	glui->add_button_to_panel(buttonRollout, "Exit", 0, exit);


	// control panel
	GLUI_Panel* controlPanel = glui->add_panel("", GLUI_PANEL_NONE);
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