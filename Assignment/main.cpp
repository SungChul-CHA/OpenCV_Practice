#include <opencv2/opencv.hpp>
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"

using namespace cv;
using namespace std;

String Filename;
VideoCapture cap;
Mat pic, frame;
int main_window;


int cx, cy;
int index_p, index_v = 0;
Point2f inputp[4], outputp[4];
Mat PT_out, or_mask;
Mat src_origin, frame_for_trans, frame_for_init;


GLUI_Button* button_forward = NULL;
GLUI_Button* button_forwardS = NULL;

double fps = 100;
int speed;
int frameCount;
int isPlay = -1;
int isButtonPushF = -1;
int isButtonPushB = -1;
int isEdit = 0;
int isCam = 0;
int isSave = 0;


int bb = 0;

void clear(int id) {
	if (id == 1) {
		if (pic.empty()) {
			cout << "Source Image has not opend yet!" << endl;
			return;
		}
		index_p = 0;
		src_origin.copyTo(pic);
		imshow("image", pic);
	}
	else if (id == 2) {
		if (!cap.isOpened()) {
			cout << "Video has not opend yet!" << endl;
			return;
		}
		index_v = 0;
		frame_for_init.copyTo(frame);
		imshow("video", frame);
	}
}

void capture(int id) {
	if (PT_out.empty() | or_mask.empty()) {
		cout << "You need to select more areas for combine" << endl;
		return;
	}
	isEdit = 1;
	isPlay = 1;
}

void savebutton(int id) {
	if (!cap.isOpened()) {
		cout << "Cam has not opend yet!" << endl;
		return;
	}
	isSave = 1;
}

// 마우스 이벤트가 발생하면 호출되는 콜백 함수이다. 
void extract(int event, int x, int y, int flags, void* param)
{
	Mat& psrc = *(Mat*)param;
	if ((index_p < 4) & (event == EVENT_LBUTTONDOWN)) {
		cx = x;
		cy = y;
		inputp[index_p] = Point2f(cx, cy);
		circle(psrc, inputp[index_p], 5, Scalar(100, 255, 51), -1);
		imshow("image", psrc);
		index_p++;
		if (index_p == 4) {
			vector<Point> points;
			for (int i = 0; i < 4; ++i) {
				points.push_back(Point(inputp[i].x, inputp[i].y));
			}
			polylines(psrc, points, true, Scalar(100, 255, 51), 3);
			imshow("image", psrc);
		}
	}
}

void setPos(int event, int x, int y, int flags, void* param)
{
	
	Mat& pframe = *(Mat*)param;
	if ((index_v < 4) & (event == EVENT_LBUTTONDOWN)) {
		if (pic.empty()) {
			cout << "Open source image first!" << endl; return;
		}
		isPlay = -1;
		cx = x;
		cy = y;
		outputp[index_v] = Point2f(cx, cy);
		circle(pframe, outputp[index_v], 5, Scalar(100, 255, 51), -1);
		imshow("video", pframe);
		index_v++;
		if (index_v == 4) {
			vector<Point> points;
			for (int i = 0; i < 4; ++i) {
				points.push_back(Point(outputp[i].x, outputp[i].y));
			}
			or_mask = Mat::zeros(pframe.rows, pframe.cols, pframe.depth());
			fillPoly(or_mask, points, Scalar::all(255));
			Mat transform_matrix = getPerspectiveTransform(inputp, outputp);
			if (index_p != 4) { cout << "Please click source image first" << endl; return; }
			warpPerspective(src_origin, PT_out, transform_matrix, pframe.size());
			PT_out.copyTo(frame_for_trans, or_mask);
			imshow("video", frame_for_trans);
		}
	}
}


void myGlutIdle(void)
{
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	while (cap.isOpened()) {
		if (bb != 0) {
			int currentFrame = cap.get(CAP_PROP_POS_FRAMES);

			if (currentFrame <= 11) cap.set(CAP_PROP_POS_FRAMES, 0);
			else cap.set(CAP_PROP_POS_FRAMES, currentFrame + bb * 11);

			cap.read(frame);
			imshow("video", frame);
		}
		if (isPlay == 1) {
			bool isFinish = cap.read(frame);
			frame_for_init = frame.clone();
			frame_for_trans = frame.clone();
			if (!isFinish) {
				cout << "동영상 종료" << endl;
				isPlay = -1;
				break;
			}
			if (isEdit == 1) {
				PT_out.copyTo(frame, or_mask);
				imshow("video", frame);
			}
			else imshow("video", frame);
		}
		else if (isButtonPushB == 1) {
			int currentFrame = cap.get(CAP_PROP_POS_FRAMES);

			if (currentFrame <= 2) cap.set(CAP_PROP_POS_FRAMES, 0);
			else cap.set(CAP_PROP_POS_FRAMES, currentFrame - 2);

			cap.read(frame);
			imshow("video", frame);
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
	if (!cap.isOpened()) return;
	int frames = id * fps;
	if (!cap.read(frame)) cap.set(CAP_PROP_POS_FRAMES, frameCount - frames);
	else if (cap.get(CAP_PROP_POS_MSEC) < id * 1000) cap.set(CAP_PROP_POS_FRAMES, 0);
	else cap.set(CAP_PROP_POS_FRAMES, cap.get(CAP_PROP_POS_FRAMES) - frames);
	cap.read(frame);
	imshow("video", frame);
}

// 재생/정지
void playCallback(int id) {
	if (isButtonPushB == 1) {
		isPlay = -1; isButtonPushB = -1;
	}
	else isPlay *= id;
	
	if (isCam != 1) speed = (int)(1000 / fps);
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
	imshow("video", frame);
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
			if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; return; }
			for (int i = 0; i < 10; i++) cap >> frame;
			isPlay = -1;
			isEdit = 0;
			isCam = 1;
			speed = 10;
			button_forward->disable();
			button_forwardS->disable();
			imshow("video", frame);
			index_v = 0;
			setMouseCallback("video", setPos, &frame);
			break;
		}
		case 1: {
			cap.open(id);
			if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; return; }
			for (int i = 0; i < 5; i++) cap >> frame;
			isPlay = -1;
			isEdit = 0;
			isCam = 1;
			speed = 10;
			button_forward->disable();
			button_forwardS->disable();
			imshow("video", frame);
			index_v = 0;
			setMouseCallback("video", setPos, &frame);
			break;
		}
		case 2: {
			OpenFileDialog* openFileDialog = new OpenFileDialog();
			openFileDialog->Filter = "Image Files(*.BMP;*.JPG;*.GIF)|*.BMP;*.JPG;*.GIF|All files (*.*)|*.*";
			if (openFileDialog->ShowDialog()) {
				Filename = openFileDialog->FileName;
				pic = imread(Filename);
				if (pic.empty()) {
					cout << "please select an image" << endl;
					return;
				}
				src_origin = pic.clone();
				isPlay = -1;
				imshow("image", pic);
				index_p = 0;
				setMouseCallback("image", extract, &pic);
			}
			else exit;
			break;
		}
	default: {
		OpenFileDialog* openVideoDialog = new OpenFileDialog();
		if (openVideoDialog->ShowDialog()) {
			Filename = openVideoDialog->FileName;
			cap.open(Filename);	// 동영상 파일인 경우
			if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; return; }
			cap >> frame;
			isPlay = -1;
			isEdit = 0;
			fps = cap.get(CAP_PROP_FPS);
			speed = (int)(1000 / fps);
			frameCount = cap.get(CAP_PROP_FRAME_COUNT);
			imshow("video", frame);
			frame_for_init = frame.clone();
			frame_for_trans = frame.clone();
			index_v = 0;
			setMouseCallback("video", setPos, &frame);
		}
		else exit;
		break;
	}
	}
}

//FileSave
void save(int id)
{
	if (!cap.isOpened()) {
		cout << "Video has not opend yet!!" << endl;
		return;
	}
	SaveFileDialog* openFileDialog = new SaveFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = openFileDialog->FileName;

		VideoWriter videoWriter(Filename, VideoWriter::fourcc('D', 'I', 'V', 'X'), fps, frame.size());
		if (!videoWriter.isOpened()) {
			cout << "Error opening video file for writing" << endl;
			return;
		}
		if (isCam == 0) {
			cap.set(CAP_PROP_POS_FRAMES, 0);
			for (int i = 0; i < frameCount; ++i) {
				cap.read(frame);
				if (isEdit == 1) PT_out.copyTo(frame, or_mask);
				videoWriter.write(frame);
			}
		}
		while (isCam == 1) {
			cap.read(frame);
			if (isEdit == 1) PT_out.copyTo(frame, or_mask);
			videoWriter.write(frame);
			imshow("video", frame);
			if (isSave == 1) break;
			if (waitKey(speed) > 0) break;
		}
		
		videoWriter.release();
	}
}


void a(int id) {
	if (!cap.isOpened()) {
		cout << "Video has not opened yet!" << endl;
		return;
	}
	else {
		bb = -1;
		isButtonPushB = -1;
		isPlay = -1;
	}
}

void b(int id) {
	if (!cap.isOpened()) {
		cout << "Video has not opened yet!" << endl;
		return;
	}
	else {
		isButtonPushB = 1;
		isPlay = -1;
		bb = 0;
	}
}

void c(int id) {
	if (!cap.isOpened()) {
		cout << "Video has not opened yet!" << endl;
		return;
	}
	else {
		isPlay = -1;
		bb = 0;
		isButtonPushB = -1;
	}
}
void d(int id) {
	if (!cap.isOpened()) {
		cout << "Video has not opened yet!" << endl;
		return;
	}
	else {
		isPlay = 1;
		bb = 0;
		isButtonPushB = -1;
	}
}
void e(int id) {
	if (!cap.isOpened()) {
		cout << "Video has not opened yet!" << endl;
		return;
	}
	else {
		isPlay = 1;
		bb = 1;
		isButtonPushB = -1;
	}
}


int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	GLUI_Master.set_glutIdleFunc(NULL);
	GLUI* glui = GLUI_Master.create_glui("Menu", 0);
	main_window = glui->get_glut_window_id();

	
	// control panel
	GLUI_Panel* profPanel = glui->add_panel("Prof panel", GLUI_PANEL_EMBOSSED);
	GLUI_Button* button_profbackS = glui->add_button_to_panel(profPanel, "<<", -1, a);
	button_profbackS->set_w(30);
	GLUI_Button* button_profback = glui->add_button_to_panel(profPanel, "<", 5, b);
	button_profback->set_w(30);
	GLUI_Button* button_profpause = glui->add_button_to_panel(profPanel, "||", -1, c);
	button_profpause->set_w(30);
	GLUI_Button* button_profforward = glui->add_button_to_panel(profPanel, ">", 5, d);
	button_profforward->set_w(30);
	GLUI_Button* button_profforwardS = button_profforwardS = glui->add_button_to_panel(profPanel, ">>", -1, e);
	button_profforwardS->set_w(30);

	glui->add_column(TRUE);




	GLUI_Panel* videoPanel = glui->add_panel("Video panel", GLUI_PANEL_RAISED);
	GLUI_Rollout* buttonRollout = glui->add_rollout_to_panel(videoPanel, "CLICK HERE", FALSE);
	glui->add_button_to_panel(buttonRollout, "Open Video", -1, open);
	glui->add_button_to_panel(buttonRollout, "Save Video", -1, save);
	glui->add_button_to_panel(buttonRollout, "Save Cam", -1, savebutton);
	glui->add_separator_to_panel(buttonRollout);
	glui->add_button_to_panel(buttonRollout, "Exit", 0, exit);

	glui->add_column_to_panel(videoPanel, TRUE);

	glui->add_button_to_panel(videoPanel, "Use Camera", 0, open);
	glui->add_button_to_panel(videoPanel, "Use Smartphone", 1, open);

	int buttonSize = 80;
	// control panel
	GLUI_Panel* controlPanel = glui->add_panel("Control panel", GLUI_PANEL_NONE);
	glui->add_statictext_to_panel(controlPanel, "backward fast play");
	GLUI_Button* button_backS = glui->add_button_to_panel(controlPanel, "<<", -1, backwardSpeedCallback);
	button_backS->set_w(buttonSize);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "  backward play");
	GLUI_Button* button_back = glui->add_button_to_panel(controlPanel, "<", 5, backwardCallback);
	button_back->set_w(buttonSize);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "     pause/play");
	GLUI_Button* button_pause = glui->add_button_to_panel(controlPanel, "||", -1, playCallback);
	button_pause->set_w(buttonSize);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "   forward play");
	button_forward = glui->add_button_to_panel(controlPanel, ">", 5, forwardCallback);
	button_forward->set_w(buttonSize);
	glui->add_column_to_panel(controlPanel, FALSE);
	glui->add_statictext_to_panel(controlPanel, "forward fast play");
	button_forwardS = glui->add_button_to_panel(controlPanel, ">>", -1, forwardSpeedCallback);
	button_forwardS->set_w(buttonSize);

	glui->add_column(TRUE);

	GLUI_Panel* picturePanel = glui->add_panel("Picture panel", GLUI_PANEL_EMBOSSED);
	glui->add_button_to_panel(picturePanel, "Open source", 2, open);

	glui->add_button_to_panel(picturePanel, "Clear Source", 1, clear);
	glui->add_button_to_panel(picturePanel, "Clear Video", 2, clear);
	glui->add_button_to_panel(picturePanel, "Edit Video!", -1, capture);

	

	glui->set_main_gfx_window(main_window);

	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();
	return EXIT_SUCCESS;
}