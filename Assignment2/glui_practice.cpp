#pragma warning(disable:4996)


#include <opencv2/opencv.hpp>
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"


using namespace cv;

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


void open(int id)
{
	OpenFileDialog* openFileDialog = new OpenFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = TCHARToString(openFileDialog->FileName);
		src = imread(Filename);
		imshow("src", src);
	}
}


int check = 1;


// checkbox�� radioó�� ���� ���ؼ�
// sync_live, sync_live_all ���
// �ش� ������ glui_manual p.17�� ����
// glui�� ���������� ����ؾ� callback �Լ������� ��� ����
// üũ�ڽ��� ���� �ٲ� üũ ������ �ȵǱ⿡ sync_live ���

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	GLUI_Master.set_glutIdleFunc(NULL);
	GLUI* glui = GLUI_Master.create_glui("Menu", 0);
	main_window = glui->get_glut_window_id();

	//GLUI button �߰� 2���� ���
	new GLUI_Button(glui, "Open File", 1, open);
	glui->add_button("Exit", 0, exit);

	// Cehckbox
	glui->add_checkbox("Name", &check, 2);


	glui->add_column(FALSE);

	// panel �߰�
	GLUI_Panel* panel1 = glui->add_panel("Panel1", GLUI_PANEL_EMBOSSED);

	// column ������ �Ŀ� ����͵��� ������ ���� �̵�
	glui->add_column(TRUE);

	glui->add_panel("Panel2", GLUI_PANEL_RAISED);
	//raised�� ���� �Ⱥ���
	glui->add_panel_to_panel(panel1, "Panel2pPanel", GLUI_PANEL_RAISED);
	glui->add_column_to_panel(panel1, TRUE);

	// Rollout �߰�
	GLUI_Rollout* rollout1 = glui->add_rollout("False_rollout", FALSE);
	glui->add_rollout_to_panel(panel1, "True_rollout", TRUE);
	glui->add_button_to_panel(rollout1, "<<", -1);
	glui->add_checkbox_to_panel(rollout1, "checkboxtoroll", &check, 3);

	int radio;
	glui->add_column(TRUE);
	GLUI_Panel* panelForRadio = glui->add_panel("Radio Group", GLUI_PANEL_NONE);
	//Radio button
	GLUI_RadioGroup* radioGroup = glui->add_radiogroup_to_panel(panelForRadio, &radio, 1);
	glui->add_radiobutton_to_group(radioGroup, "Choice1");
	glui->add_radiobutton_to_group(radioGroup, "Choice2");
	glui->add_radiobutton_to_group(radioGroup, "Choice3");

	// static text
	glui->add_statictext("name");


	// text box
	String* s = NULL;
	int* i = NULL;
	float* f = NULL;
	glui->add_edittext("box name", GLUI_EDITTEXT_TEXT, s);
	GLUI_EditText* intText = glui->add_edittext("box name", GLUI_EDITTEXT_INT, i);
	GLUI_EditText* floatText = glui->add_edittext("box name", GLUI_EDITTEXT_FLOAT, f);
	intText->set_int_limits(0, 99, GLUI_LIMIT_CLAMP);
	floatText->set_float_limits(-5, 99.9, GLUI_LIMIT_WRAP);

	//spinner
	//shift ������ ������, ctrl ������ ������ ������
	GLUI_Spinner* spinner = glui->add_spinner("Int", GLUI_SPINNER_INT, i);
	spinner->set_speed(0.5);

	glui->set_main_gfx_window(main_window);
	glutMainLoop();
	return EXIT_SUCCESS;
}