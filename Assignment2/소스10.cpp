#include "opencv2/opencv.hpp"
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"
#include <stdio.h>

using namespace cv;

int main_window;
String Filename;

int chk1=1, chk2=0, chk3=0;
GLUI_Checkbox* pchk1;
GLUI_Checkbox* pchk2;
GLUI_Checkbox* pchk3;

int open()
{
	OpenFileDialog* openFileDialog = new OpenFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = openFileDialog->FileName;
		return(1);
	}
	else
		return(-1);
}
int save()
{
	SaveFileDialog* openFileDialog = new SaveFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = openFileDialog->FileName;
		return(1);
	}
	else
		return(-1);
}

void IDLEF()
{
	static int i = 0;

	printf("%d\n", i++);
	waitKey(500);
}

void btnfunc(int id)
{
	static int toggle = 0;

	toggle = toggle ^ 1;
	if(toggle == 0)
		GLUI_Master.set_glutIdleFunc(NULL);
	else
		GLUI_Master.set_glutIdleFunc(IDLEF);
}

void checkbox(int id)
{
	printf("%d\n", id);
	switch (id) {
	case 1: chk1 = 1; chk2 = chk3 = 0; GLUI_Master.sync_live_all(); pchk1->set_int_val(1); break;
	case 2: chk2 = 1; chk1 = chk3 = 0; GLUI_Master.sync_live_all(); pchk2->set_int_val(1); break;
	case 3: chk3 = 1; chk1 = chk2 = 0; GLUI_Master.sync_live_all(); pchk3->set_int_val(1); break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	GLUI* glui = GLUI_Master.create_glui("Menu", 0);
	main_window = glui->get_glut_window_id();
	GLUI_Master.set_glutIdleFunc(NULL);

	pchk1 = glui->add_checkbox("A", &chk1, 1, checkbox);
	pchk2 = glui->add_checkbox("B", &chk2, 2, checkbox);
	pchk3 = glui->add_checkbox("C", &chk3, 3, checkbox);

	new GLUI_Button(glui, "Idle Toggle", 0, btnfunc);

	new GLUI_Button(glui, "Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);
	glutMainLoop();
	return EXIT_SUCCESS;
}