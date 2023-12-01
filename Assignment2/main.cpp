#include <opencv2/opencv.hpp>
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"

using namespace cv;
using namespace std;

#define BINARY_ID 100
#define FILTER_ID 200


int main_window;
String Filename;
Mat src, result, gray;

GLUI* glui;
GLUI_Checkbox* binary; GLUI_Checkbox* filter; GLUI_Checkbox* histEqual;
GLUI_Checkbox* lut; GLUI_Checkbox* warping; GLUI_Checkbox* featureExtract;
GLUI_Checkbox* detectObj; GLUI_Checkbox* brush; GLUI_Checkbox* photoshop; 
GLUI_Checkbox* numDetect;

int binaryCheck, filterCheck, histCheck, lutCheck;
int warpingCheck, featureExtCheck, detectingCheck;
int brushCheck, photoshopCheck, numDetectCheck;

void shuffleDFT(Mat& src);
Mat getFilter(Size size, float D);

// error handler
void errorMsg(string s) {
	cout << s << " Fail" << endl;
}

// file open
void open(int id)
{
	OpenFileDialog* openFileDialog = new OpenFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = openFileDialog->FileName;
		src = imread(Filename);
		if (src.empty()) errorMsg("Image Open");
		else imshow("src", src);
	}
	else {
		errorMsg("Open File Dialog");
		return;
	}
}

// file save
void save(int id)
{
	SaveFileDialog* openFileDialog = new SaveFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = openFileDialog->FileName;
		bool isSave = imwrite(Filename, src);
		if (!isSave) { errorMsg("File Save"); return; }
	}
	else {
		errorMsg("Save File Dialog");
		return;
	}
}


// checkbox synchronize
int isClick[10] = { 0 };
void initCheckbox(int exception) {
	switch (exception - 1)
	{
	case 0:
		for (int i = 0; i < 10; i++) if(i != exception - 1) isClick[i] = 0;
		isClick[0] = isClick[0] ^ 1;
		binaryCheck = 1; filterCheck = 0; histCheck = 0; lutCheck = 0;
		warpingCheck = 0; featureExtCheck = 0; detectingCheck = 0;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		binary->set_int_val(isClick[0]);
		break;
	case 1:
		for (int i = 0; i < 10; i++) if (i != exception - 1) isClick[i] = 0;
		isClick[1] = isClick[1] ^ 1;
		binaryCheck = 0; filterCheck = 1; histCheck = 0; lutCheck = 0;
		warpingCheck = 0; featureExtCheck = 0; detectingCheck = 0;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		filter->set_int_val(isClick[1]);
		break;
	case 2:
		for (int i = 0; i < 10; i++) if (i != exception - 1) isClick[i] = 0;
		isClick[2] = isClick[2] ^ 1;
		binaryCheck = 0; filterCheck = 0; histCheck = 1; lutCheck = 0;
		warpingCheck = 0; featureExtCheck = 0; detectingCheck = 0;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		histEqual->set_int_val(isClick[2]);
		break;
	case 3:
		for (int i = 0; i < 10; i++) if (i != exception - 1) isClick[i] = 0;
		isClick[3] = isClick[3] ^ 1;
		binaryCheck = 0; filterCheck = 0; histCheck = 0; lutCheck = 1;
		warpingCheck = 0; featureExtCheck = 0; detectingCheck = 0;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		lut->set_int_val(isClick[3]);
		break;
	case 4:
		for (int i = 0; i < 10; i++) if (i != exception - 1) isClick[i] = 0;
		isClick[4] = isClick[4] ^ 1;
		binaryCheck = 0; filterCheck = 0; histCheck = 0; lutCheck = 0;
		warpingCheck = 1; featureExtCheck = 0; detectingCheck = 0;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		warping->set_int_val(isClick[4]);
		break;
	case 5:
		for (int i = 0; i < 10; i++) if (i != exception - 1) isClick[i] = 0;
		isClick[5] = isClick[5] ^ 1;
		binaryCheck = 0; filterCheck = 0; histCheck = 0; lutCheck = 0;
		warpingCheck = 0; featureExtCheck = 1; detectingCheck = 0;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		featureExtract->set_int_val(isClick[5]);
		break;
	case 6:
		for (int i = 0; i < 10; i++) if (i != exception - 1) isClick[i] = 0;
		isClick[6] = isClick[6] ^ 1;
		binaryCheck = 0; filterCheck = 0; histCheck = 0; lutCheck = 0;
		warpingCheck = 0; featureExtCheck = 0; detectingCheck = 1;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		detectObj->set_int_val(isClick[6]);
		break;
	case 7:
		for (int i = 0; i < 10; i++) if (i != exception - 1) isClick[i] = 0;
		isClick[7] = isClick[7] ^ 1;
		binaryCheck = 0; filterCheck = 0; histCheck = 0; lutCheck = 0;
		warpingCheck = 0; featureExtCheck = 0; detectingCheck = 0;
		brushCheck = 1; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		brush->set_int_val(isClick[7]);
		break;
	case 8:
		for (int i = 0; i < 10; i++) if (i != exception - 1) isClick[i] = 0;
		isClick[8] = isClick[8] ^ 1;
		binaryCheck = 0; filterCheck = 0; histCheck = 0; lutCheck = 0;
		warpingCheck = 0; featureExtCheck = 0; detectingCheck = 0;
		brushCheck = 0; photoshopCheck = 1; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		photoshop->set_int_val(isClick[8]);
		break;
	case 9:
		for (int i = 0; i < 10; i++) if (i != exception - 1) isClick[i] = 0;
		isClick[9] = isClick[9] ^ 1;
		binaryCheck = 0; filterCheck = 0; histCheck = 0; lutCheck = 0;
		warpingCheck = 0; featureExtCheck = 0; detectingCheck = 0;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 1;
		GLUI_Master.sync_live_all();
		numDetect->set_int_val(isClick[9]);
		break;
	case 10:
		fill(begin(isClick) + 2, end(isClick), 0);
		for (int i = 0; i < 2; i++) isClick[i] = isClick[i] ^ 1;
		binaryCheck = 1; filterCheck = 1; histCheck = 0; lutCheck = 0;
		warpingCheck = 0; featureExtCheck = 0; detectingCheck = 0;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		binary->set_int_val(isClick[0]);
		filter->set_int_val(isClick[1]);
		break;
	case 11:
		fill(begin(isClick), begin(isClick) + 2, 0);
		fill(begin(isClick) + 7, end(isClick), 0);
		for (int i = 2; i < 7; i++) isClick[i] = isClick[i] ^ 1;
		binaryCheck = 0; filterCheck = 0; histCheck = 1; lutCheck = 1;
		warpingCheck = 1; featureExtCheck = 1; detectingCheck = 1;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		histEqual->set_int_val(isClick[2]);
		lut->set_int_val(isClick[3]);
		warping->set_int_val(isClick[4]);
		featureExtract->set_int_val(isClick[5]);
		detectObj->set_int_val(isClick[6]);
		break;
	default:
		fill(begin(isClick), end(isClick), 0);
		binaryCheck = 0; filterCheck = 0; histCheck = 0; lutCheck = 0;
		warpingCheck = 0; featureExtCheck = 0; detectingCheck = 0;
		brushCheck = 0; photoshopCheck = 0; numDetectCheck = 0;
		GLUI_Master.sync_live_all();
		binary->set_int_val(0);
		filter->set_int_val(0);
		histEqual->set_int_val(0);
		lut->set_int_val(0);
		warping->set_int_val(0);
		featureExtract->set_int_val(0);
		detectObj->set_int_val(0);
		brush->set_int_val(0);
		photoshop->set_int_val(0);
		numDetect->set_int_val(0);
		break;
	}
}


// function 1 binary
GLUI* binary_window;
int binary_radio = 0;
int thresh_value, thresh_type, thresh_maxvalue;

void binaryTypeCallback(int id) {
	threshold(gray, result, thresh_value, thresh_maxvalue, binary_radio);
	imshow("src", result);
}

void on_trackbar(int, void*) {
	threshold(gray, result, thresh_value, thresh_maxvalue, binary_radio);
	imshow("src", result);
}


// function 2 filter
GLUI* filter_window;
GLUI_Spinner* blurSpinner;
GLUI_Spinner* medianBlurSpinner;
GLUI_Spinner* gaussianBlurSpinner;
GLUI_Spinner* butterWorthBlurSpinner;

int filter_radio = 0;
int blurSize = 10;
int medianBlurSize = 5;
int gaussianBlurSize = 1;
float butterWorthD = 50.0;

void filterTypeCallback(int id) {
	blurSpinner->disable();
	medianBlurSpinner->disable();
	gaussianBlurSpinner->disable();
	butterWorthBlurSpinner->disable();
	switch (filter_radio)
	{
	case 0: {
		blurSpinner->enable();
		blur(gray, result, Size(blurSize, blurSize));
		break;
	}
	case 1: {
		medianBlurSpinner->enable();
		if (medianBlurSize % 2 == 0) medianBlurSize++;
		medianBlur(gray, result, medianBlurSize);
		break;
	}
	case 2: {
		gaussianBlurSpinner->enable();
		if (gaussianBlurSize % 2 == 0) gaussianBlurSize++;
		GaussianBlur(gray, result, Size(gaussianBlurSize, gaussianBlurSize), 0, 0);
		break;
	}
	case 3: {
		butterWorthBlurSpinner->enable();
		Mat gray_float, dft_image, buf;
		gray.convertTo(gray_float, CV_32FC1, 1.0 / 255.0);
		dft(gray_float, dft_image, DFT_COMPLEX_OUTPUT);
		shuffleDFT(dft_image);
		Mat lowpass = getFilter(dft_image.size(), butterWorthD);
		multiply(dft_image, lowpass, buf);
		shuffleDFT(buf);
		idft(buf, result, DFT_SCALE | DFT_REAL_OUTPUT);
		break;
	}
	default:
		errorMsg("FILTER ERROR Unknown");
		return;
		break;
	}
	imshow("src", result);
}

void spinnerCallback(int id) {
	switch (id)
	{
	case 0:
		blur(gray, result, Size(blurSize, blurSize));
		break;
	case 1:
		if (medianBlurSize % 2 == 0) medianBlurSize++;
		medianBlur(gray, result, medianBlurSize);
		break;
	case 2:
		if (gaussianBlurSize % 2 == 0) gaussianBlurSize++;
		GaussianBlur(gray, result, Size(gaussianBlurSize, gaussianBlurSize), 0, 0);
		break;
	case 3: {
		Mat gray_float, dft_image, buf;
		gray.convertTo(gray_float, CV_32FC1, 1.0 / 255.0);
		dft(gray_float, dft_image, DFT_COMPLEX_OUTPUT);
		shuffleDFT(dft_image);
		Mat lowpass = getFilter(dft_image.size(), butterWorthD);
		multiply(dft_image, lowpass, buf);
		shuffleDFT(buf);
		idft(buf, result, DFT_SCALE | DFT_REAL_OUTPUT);
		break;
	}
	default:
		errorMsg("SPINNER ERROR Unknown");
		return;
		break;
	}
	imshow("src", result);
}


// function 3 histEqual











void closeCallback(int id) {
	switch (id)
	{
	case BINARY_ID: 
		binary_window->close();
		binary->enable();
		isClick[0] = 0;
		binaryCheck = 0;
		binary->set_int_val(0);
		if (getWindowProperty("src", WND_PROP_VISIBLE) < 1) return;
		else destroyWindow("src");
		break;
	case FILTER_ID:
		filter_window->close();
		filter->enable();
		isClick[1] = 0;
		filterCheck = 0;
		filter->set_int_val(0);
		if (getWindowProperty("src", WND_PROP_VISIBLE) < 1) return;
		else destroyWindow("src");
		break;
	default:
		break;
	} 
}

void checkCallback(int id) {
	initCheckbox(id);
	switch (id)
	{
	case 1: {
		if (src.empty()) { errorMsg("Open Image"); initCheckbox(id);  return; }
		cvtColor(src, gray, COLOR_BGR2GRAY);
		imshow("src", gray);
		binary->disable();
		binary_window = GLUI_Master.create_glui("Binary", BINARY_ID, 700, 200);
		GLUI_Panel* binaryRadioGroupPanel = binary_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* binaryRadioGroup = binary_window->add_radiogroup_to_panel(binaryRadioGroupPanel, &binary_radio, -1, binaryTypeCallback);
		binary_window->add_radiobutton_to_group(binaryRadioGroup, "Thresh Binary");
		binary_window->add_radiobutton_to_group(binaryRadioGroup, "Thresh Binary INV");
		binary_window->add_radiobutton_to_group(binaryRadioGroup, "Thresh Trunc");
		binary_window->add_radiobutton_to_group(binaryRadioGroup, "Thresh to Zero INV");
		binary_window->add_radiobutton_to_group(binaryRadioGroup, "Thresh to Zero");

		binary_window->add_button("Close", BINARY_ID, closeCallback);

		createTrackbar("Level", "src", &thresh_value, 255, on_trackbar);
		createTrackbar("Maxval", "src", &thresh_maxvalue, 255, on_trackbar);
		break;
	}
	case 2: {
		if (src.empty()) { errorMsg("Open Image"); initCheckbox(id);  return; }
		cvtColor(src, gray, COLOR_BGR2GRAY);
		imshow("src", gray);
		filter->disable();
		filter_window = GLUI_Master.create_glui("Binary", FILTER_ID, 700, 400);
		GLUI_Panel* filterRadioGroupPanel = filter_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* filterRadioGroup = filter_window->add_radiogroup_to_panel(filterRadioGroupPanel, &filter_radio, -1, filterTypeCallback);
		filter_window->add_radiobutton_to_group(filterRadioGroup, "Blur");
		filter_window->add_radiobutton_to_group(filterRadioGroup, "Median Blur");
		filter_window->add_radiobutton_to_group(filterRadioGroup, "Gaussian");
		filter_window->add_radiobutton_to_group(filterRadioGroup, "Butterworth");

		filter_window->add_button("Close", FILTER_ID, closeCallback);

		filter_window->add_column(FALSE);
		blurSpinner = filter_window->add_spinner("Blur filter size", GLUI_SPINNER_INT, &blurSize, 0, spinnerCallback);
		blurSpinner->set_speed(0.3);
		blurSpinner->set_int_limits(1, (int)(src.rows / 2), GLUI_LIMIT_CLAMP);
		medianBlurSpinner = filter_window->add_spinner("MedianBlur filter size", GLUI_SPINNER_INT, &medianBlurSize, 1, spinnerCallback);
		medianBlurSpinner->set_speed(0.3);
		medianBlurSpinner->set_int_limits(1, (int)(src.rows / 2), GLUI_LIMIT_CLAMP);
		gaussianBlurSpinner = filter_window->add_spinner("GaussianBlur filter size", GLUI_SPINNER_INT, &gaussianBlurSize, 2, spinnerCallback);
		gaussianBlurSpinner->set_speed(0.3);
		gaussianBlurSpinner->set_int_limits(1, (int)(src.rows / 2), GLUI_LIMIT_CLAMP);
		butterWorthBlurSpinner = filter_window->add_spinner("ButterWorth filter D size", GLUI_SPINNER_FLOAT, &butterWorthD, 3, spinnerCallback);
		butterWorthBlurSpinner->set_speed(0.8);
		butterWorthBlurSpinner->set_float_limits(1.0, 200.0, GLUI_LIMIT_CLAMP);
		break;
	}
	case 3: {
		if (src.empty()) { errorMsg("Open Image"); initCheckbox(id);  return; }
		Mat RGB[3], RGBO[3];
		split(src, RGB);
		for(int i = 0; i < 3; i++) equalizeHist(RGB[i], RGBO[i]);
		merge(RGBO, 3, result);
		imshow("src", result);
		break;
	}
	case 11: {
		if (src.empty()) { errorMsg("Open Image"); initCheckbox(id);  return; }
		cvtColor(src, gray, COLOR_BGR2GRAY);
		imshow("src", gray);
		binary->disable();
		filter->disable();
		binary_window = GLUI_Master.create_glui("Binary", BINARY_ID, 700, 200);
		filter_window = GLUI_Master.create_glui("Binary", FILTER_ID, 700, 400);
		GLUI_Panel* binaryRadioGroupPanel = binary_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_Panel* filterRadioGroupPanel = filter_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* binaryRadioGroup = binary_window->add_radiogroup_to_panel(binaryRadioGroupPanel, &binary_radio, -1, binaryTypeCallback);
		GLUI_RadioGroup* filterRadioGroup = filter_window->add_radiogroup_to_panel(filterRadioGroupPanel, &filter_radio, -1, filterTypeCallback);
		binary_window->add_radiobutton_to_group(binaryRadioGroup, "Thresh Binary");
		binary_window->add_radiobutton_to_group(binaryRadioGroup, "Thresh Binary INV");
		binary_window->add_radiobutton_to_group(binaryRadioGroup, "Thresh Trunc");
		binary_window->add_radiobutton_to_group(binaryRadioGroup, "Thresh to Zero INV");
		binary_window->add_radiobutton_to_group(binaryRadioGroup, "Thresh to Zero");
		filter_window->add_radiobutton_to_group(filterRadioGroup, "Blur");
		filter_window->add_radiobutton_to_group(filterRadioGroup, "Median Blur");
		filter_window->add_radiobutton_to_group(filterRadioGroup, "Gaussian");
		filter_window->add_radiobutton_to_group(filterRadioGroup, "Butterworth");

		binary_window->add_button("Close", BINARY_ID, closeCallback);
		filter_window->add_button("Close", FILTER_ID, closeCallback);

		filter_window->add_column(FALSE);
		blurSpinner = filter_window->add_spinner("Blur filter size", GLUI_SPINNER_INT, &blurSize, 0, spinnerCallback);
		blurSpinner->set_speed(0.3);
		blurSpinner->set_int_limits(1, (int)(src.rows / 2), GLUI_LIMIT_CLAMP);
		medianBlurSpinner = filter_window->add_spinner("MedianBlur filter size", GLUI_SPINNER_INT, &medianBlurSize, 1, spinnerCallback);
		medianBlurSpinner->set_speed(0.3);
		medianBlurSpinner->set_int_limits(1, (int)(src.rows / 2), GLUI_LIMIT_CLAMP);
		gaussianBlurSpinner = filter_window->add_spinner("GaussianBlur filter size", GLUI_SPINNER_INT, &gaussianBlurSize, 2, spinnerCallback);
		gaussianBlurSpinner->set_speed(0.3);
		gaussianBlurSpinner->set_int_limits(1, (int)(src.rows / 2), GLUI_LIMIT_CLAMP);
		butterWorthBlurSpinner = filter_window->add_spinner("ButterWorth filter D size", GLUI_SPINNER_FLOAT, &butterWorthD, 3, spinnerCallback);
		butterWorthBlurSpinner->set_speed(0.8);
		butterWorthBlurSpinner->set_float_limits(1.0, 200.0, GLUI_LIMIT_CLAMP);

		createTrackbar("Level", "src", &thresh_value, 255, on_trackbar);
		createTrackbar("Maxval", "src", &thresh_maxvalue, 255, on_trackbar);
		break;
	}

	default:
		break;
	}
	
	
}



int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	GLUI_Master.set_glutIdleFunc(NULL);
	glui = GLUI_Master.create_glui("Menu", 0, 400, 200);
	main_window = glui->get_glut_window_id();


	GLUI_Panel* fileManagePanel = glui->add_panel("File Panel", GLUI_PANEL_RAISED);
	glui->add_button_to_panel(fileManagePanel, "Open Image", -1, open);
	glui->add_button_to_panel(fileManagePanel, "Save Image", -1, save);
	glui->add_separator_to_panel(fileManagePanel);

	glui->add_button_to_panel(fileManagePanel, "Clear All", 0, checkCallback);
	glui->add_button_to_panel(fileManagePanel, "Check Gray", 11, checkCallback);
	glui->add_button_to_panel(fileManagePanel, "Check Color", 12, checkCallback);
	glui->add_separator_to_panel(fileManagePanel);
	
	glui->add_button_to_panel(fileManagePanel, "Exit", 0, exit);

	glui->add_column(true);


	GLUI_Panel* functionPanel = glui->add_panel("Function Panel", GLUI_PANEL_EMBOSSED);
	// 컬러 불가
	binary = glui->add_checkbox_to_panel(functionPanel, "Binary", &binaryCheck, 1, checkCallback);
	filter = glui->add_checkbox_to_panel(functionPanel, "Filtering", &filterCheck, 2, checkCallback);
	glui->add_separator_to_panel(functionPanel);
	// 영상 처리
	histEqual = glui->add_checkbox_to_panel(functionPanel, "Histogram Equalize", &histCheck, 3, checkCallback);
	lut = glui->add_checkbox_to_panel(functionPanel, "Color Map", &lutCheck, 4, checkCallback);
	warping = glui->add_checkbox_to_panel(functionPanel, "Image Warping", &warpingCheck, 5, checkCallback);
	featureExtract = glui->add_checkbox_to_panel(functionPanel, "Feature Extract", &featureExtCheck, 6, checkCallback);
	detectObj = glui->add_checkbox_to_panel(functionPanel, "Detect Object", &detectingCheck, 7, checkCallback);
	glui->add_separator_to_panel(functionPanel);
	// 영상 직접 처리 x
	brush = glui->add_checkbox_to_panel(functionPanel, "Paint Brush", &brushCheck, 8, checkCallback);
	photoshop = glui->add_checkbox_to_panel(functionPanel, "Image Paste", &photoshopCheck, 9, checkCallback);
	numDetect = glui->add_checkbox_to_panel(functionPanel, "Drawing Number", &numDetectCheck, 10, checkCallback);


	glui->set_main_gfx_window(main_window);
	glutMainLoop();
	return EXIT_SUCCESS;
}


// 버터워쓰 필터를 만든다. 
Mat getFilter(Size size, float D)
{
	Mat tmp = Mat(size, CV_32F);
	Point center = Point(tmp.cols / 2, tmp.rows / 2);
	double radius, n = 2;

	for (int i = 0; i < tmp.rows; i++) {
		for (int j = 0; j < tmp.cols; j++) {
			radius = (double)sqrt(pow((j - center.x), 2.0) + pow((double)(i - center.y), 2.0));
			tmp.at<float>(i, j) = (float)(1 / (1 + pow((double)(radius / D), (double)(2 * n))));
		}
	}
	Mat toMerge[] = { tmp, tmp };
	Mat filter;
	merge(toMerge, 2, filter);
	return filter;
}

void shuffleDFT(Mat& src)
{
	int cX = src.cols / 2;
	int cY = src.rows / 2;

	Mat q1(src, Rect(0, 0, cX, cY));      Mat q2(src, Rect(cX, 0, cX, cY));
	Mat q3(src, Rect(0, cY, cX, cY));    Mat q4(src, Rect(cX, cY, cX, cY));
	Mat tmp;
	// q1과 q4를 교환
	q1.copyTo(tmp);
	q4.copyTo(q1);
	tmp.copyTo(q4);
	// q2와 q3을 교환
	q2.copyTo(tmp);
	q3.copyTo(q2);
	tmp.copyTo(q3);
}













bool drawing = false;
int r = 1;
void drawCircle(int event, int x, int y, int, void* param) {
	if (event == EVENT_LBUTTONDOWN)
		drawing = true;
	else if (event == EVENT_MOUSEMOVE) {
		if (drawing == true)
			circle(src, Point(x, y), r, Scalar(0, 0, 255), -1);
	}
	else if (event == EVENT_LBUTTONUP)
		drawing = false;
	imshow("src", src);
}
