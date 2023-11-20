#pragma warning(disable:4996)

#include "opencv2/opencv.hpp"
//#include <GL/glut.h>
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"

using namespace cv;

std::string TCHARToString(const TCHAR* ptsz)
{
	int len = wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	std::string s = psz;
	delete[] psz;
	return s;
}


int main_window;
String Filename;
Mat src;
int check1=1;
int rg1 = 0;
int editint=10, listint=2;
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
void checkfunc(int id)
{
	printf("%d\n", check1);

}

void radiofunc(int id)
{
	printf("%d\n", rg1);

}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	GLUI *glui = GLUI_Master.create_glui("Menu", 0);
	main_window = glui->get_glut_window_id();
	GLUI_Master.set_glutIdleFunc(NULL);

	new GLUI_Button(glui, "Open", 0, open);
//	new GLUI_Button(glui, "Save", 0, save);
	GLUI_Rollout *rollout1 = glui->add_rollout("Output", FALSE);
	GLUI_Button *button = glui->add_button_to_panel(rollout1, "Save", 0, save);
	GLUI_Checkbox *cb1 = glui->add_checkbox_to_panel(rollout1, "Check", &check1, 0, checkfunc);
	GLUI_RadioGroup *rg = glui->add_radiogroup(&rg1, 0, radiofunc);
	glui->add_radiobutton_to_group(rg, "1st");
	glui->add_radiobutton_to_group(rg, "2nd");
	glui->add_radiobutton_to_group(rg, "3rd");

	GLUI_Spinner *edittext1 = glui->add_spinner("Int", GLUI_SPINNER_INT, &editint);
	edittext1->set_int_limits(1, 10, GLUI_LIMIT_WRAP);

	GLUI_Listbox *listbox1 = glui->add_listbox("item", &listint);
	listbox1->add_item(1, "Apple");
	listbox1->add_item(2, "Orange");
	listbox1->add_item(3, "Banana");
	new GLUI_Button(glui, "Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);
	glutMainLoop();
	return EXIT_SUCCESS;
}
