#include <opencv2/opencv.hpp>
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"

using namespace cv;
using namespace std;


#define BINARY_ID 100
#define FILTER_ID 200
#define LUT_ID 300
#define WARPING_ID 400
#define EXTRACT_ID 500
#define HAUGH_ID 600
#define DETECT_ID 700
#define PHOTOSHOP_ID 800
#define KNN_ID 900

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
		bool isSave = imwrite(Filename, result);
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


// function 4 histEqual
GLUI* lut_window;
int lut_radio = 0;

void lutTypeCallback(int id) {
	switch (lut_radio)
	{
	case 0: {
		applyColorMap(src, result, COLORMAP_SPRING);
		break;
	}
	case 1: {
		applyColorMap(src, result, COLORMAP_SUMMER);
		break;
	}
	case 2: {
		applyColorMap(src, result, COLORMAP_AUTUMN);
		break;
	}
	case 3: {
		applyColorMap(src, result, COLORMAP_WINTER);
		break;
	}
	case 4: {
		applyColorMap(src, result, COLORMAP_RAINBOW);
		break;
	}
	case 5: {
		applyColorMap(src, result, COLORMAP_HSV);
		break;
	}
	case 6: {
		Mat tony(1, 256, CV_8UC3, Scalar::all(0));
		Vec3b* p = (Vec3b*)tony.ptr();

		for (int i = 0; i < 256; i++) {
			p[i][0] = (i / 3); p[i][1] = (255 - i) - rand() % 20; p[i][2] = i;
		}
		LUT(src, tony, result);
		break;
	}
	default:
		errorMsg("ERROR LUT Unknown");
		break;
	}
	imshow("src", result);
}


// function 5 warping
GLUI* warping_window;
GLUI_Spinner* warpingSpinner;
int warping_radio = 0;
float warping_offset = 15.0;

void warpingAxis(int id) {
	if (warping_radio == 0) {
		int cols = src.cols;
		result = src.clone();
		for (int y = 0; y < src.rows; y++)
			for (int x = 0; x < cols; x++)
				for (int c = 0; c < 3; c++) {
					int offset = (int)(warping_offset * sin(2 * 3.14 * x / 90));
					if (y + offset >= 0){
						result.at<Vec3b>(y, x)[c] = src.at<Vec3b>((y + offset) % src.rows, (x))[c];
					}
					else
						result.at<Vec3b>(y, x)[c] = 0;
				}
	}
	else {
		int cols = src.cols;
		result = src.clone();
		for (int y = 0; y < src.rows; y++)
			for (int x = 0; x < cols; x++)
				for (int c = 0; c < 3; c++) {
					int offset = (int)(-warping_offset * sin(2 * 3.14 * y / 180));
					if (x + offset >= 0) {
						result.at<Vec3b>(y, x)[c] = src.at<Vec3b>((y), (x + offset) % src.cols)[c];
					}
					else
						result.at<Vec3b>(y, x)[c] = 0;
				}
	}
	imshow("src", result);
}

void warpingOffsetSpinner(int id) {
	if (warping_radio == 0) {
		int cols = src.cols;
		result = src.clone();
		for (int y = 0; y < src.rows; y++)
			for (int x = 0; x < cols; x++)
				for (int c = 0; c < 3; c++) {
					int offset = (int)(warping_offset * sin(2 * 3.14 * x / 90));
					if (y + offset >= 0)
						result.at<Vec3b>(y, x)[c] = src.at<Vec3b>((y + offset) % src.rows, (x))[c];
					else
						result.at<Vec3b>(y, x)[c] = 0;
				}
	}
	else {
		int cols = src.cols;
		result = src.clone();
		for (int y = 0; y < src.rows; y++)
			for (int x = 0; x < cols; x++)
				for (int c = 0; c < 3; c++) {
					int offset = (int)(-warping_offset * sin(2 * 3.14 * y / 180));
					if (x + offset >= 0)
						result.at<Vec3b>(y, x)[c] = src.at<Vec3b>((y), (x + offset) % src.cols)[c];
					else
						result.at<Vec3b>(y, x)[c] = 0;
				}
	}
	imshow("src", result);
}



// function 6 extract
GLUI* extract_window;
GLUI_Spinner* extractSpinner;
int extract_radio = 0;
int haugh_mim = 40;


void haughShape(int id) {
	if (extract_radio == 0) {
		Mat dst;
		Canny(gray, dst, 100, 200);
		cvtColor(dst, result, COLOR_GRAY2BGR);
		vector<Vec4i> lines;  // 검출된 직선의 양끝점 좌표를 저장하기 위한 버퍼
		HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 100, 20);
		for (size_t i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];
			line(result, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
		}
		imshow("src", result);
	}
	else {
		GaussianBlur(gray, gray, Size(9, 9), 2, 2);
		vector<Vec3f> circles;
		HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 8, 200, 50);
		// 원을 영상 위에 그린다. 
		for (size_t i = 0; i < circles.size(); i++) {
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			circle(result, center, 3, Scalar(0, 255, 0), -1, 8, 0); // 원의 중심을 그린다. 
			circle(result, center, radius, Scalar(0, 0, 255), 3, 8, 0); // 원을 그린다.
		}
		imshow("src", result);
	}
}

void haughSensitivity(int id) {
	if (extract_radio == 0) {
		Mat dst;
		Canny(gray, dst, 100, 200);
		cvtColor(dst, result, COLOR_GRAY2BGR);
		vector<Vec4i> lines;  // 검출된 직선의 양끝점 좌표를 저장하기 위한 버퍼
		HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 100, 20);
		for (size_t i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];
			line(result, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
		}
		imshow("src", result);
	}
	else if (extract_radio == 1) {
		GaussianBlur(gray, gray, Size(9, 9), 2, 2);
		vector<Vec3f> circles;
		HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 8, 200, 50);
		// 원을 영상 위에 그린다. 
		for (size_t i = 0; i < circles.size(); i++) {
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			circle(result, center, 3, Scalar(0, 255, 0), -1, 8, 0); // 원의 중심을 그린다. 
			circle(result, center, radius, Scalar(0, 0, 255), 3, 8, 0); // 원을 그린다.
		}
		imshow("src", result);
	}
	else errorMsg("Haugh Transform");
}


// function 7 extract
GLUI* dectect_window;
int detect_radio = 0;

void detectObject(int id) {
	if (detect_radio == 0) {
		Mat blur;
		Size size = Size(5, 5);

		GaussianBlur(gray, blur, size, 0);
		threshold(blur, result, 0, 255, THRESH_BINARY | THRESH_OTSU);
		imshow("src", result);
	}
	else {
		Mat thimg, labels, centroids, stats;

		threshold(gray, thimg, 128, 255, THRESH_BINARY | THRESH_OTSU);

		//물체 갯수
		int n = connectedComponentsWithStats(thimg, labels, stats, centroids);

		vector<Vec3b> colors(n + 1);  // 컬러 버퍼
		colors[0] = Vec3b(0, 0, 0);  // 배경은 검은색
		for (int i = 1; i <= n; i++)  // 각 label에 랜덤컬러 생성
			colors[i] = Vec3b(rand() % 256, rand() % 256, rand() % 256);
		result = cv::Mat::zeros(gray.size(), CV_8UC3);
		// label된 각 화소에 컬러값 설정
		for (int y = 0; y < result.rows; y++)
			for (int x = 0; x < result.cols; x++) {
				int label = labels.at<int>(y, x);
				result.at<cv::Vec3b>(y, x) = colors[label];
			}
		imshow("src", result);
	}
}


// function 8 brush
int red, green, blue, thickness;
bool drawing = false;

void drawCircle(int event, int x, int y, int, void* param) {
	if (event == EVENT_LBUTTONDOWN)
		drawing = true;
	else if (event == EVENT_MOUSEMOVE) {
		if (drawing == true)
			circle(result, Point(x, y), thickness + 1, Scalar(blue, green, red), -1);
	}
	else if (event == EVENT_LBUTTONUP)
		drawing = false;
	imshow("src", result);
}


// function 9 photoshop
GLUI* photoshop_window;
Mat src_buf, roi1;

bool isDrawing = false;
int cx, cy, x2, y2, rx, ry = 0;

void photoshopCB(int event, int x, int y, int, void* param) {
	Mat& img = *(Mat*)param;
	if (event == EVENT_LBUTTONDOWN) {
		isDrawing = true;
		cx = x;
		cy = y;
	}
	else if (event == EVENT_MOUSEMOVE) {
		if (isDrawing) {
			img = src_buf.clone();
			rx = cx;
			ry = cy;
			if (x < cx) rx = x;
			if (y < cy) ry = y;
			rectangle(img, Rect(rx, ry, abs(x - cx), abs(y - cy)), Scalar(0, 255, 0), 2);
			cv::imshow("src", img);
		}
	}
	else if (event == EVENT_LBUTTONUP) {
		isDrawing = false;
		x2 = x;
		y2 = y;
		rx = cx;
		ry = cy;
		if (x < cx) rx = x;
		if (y < cy) ry = y;
		rectangle(img, Rect(rx, ry, abs(x - cx), abs(y - cy)), Scalar(0, 255, 0), 2);
		cv::imshow("src", img);
	}
}

void click(int event, int x, int y, int, void* param) {
	if (event == EVENT_LBUTTONDOWN) {
		Mat& img = *(Mat*)(param);
		img = src_buf.clone();

		Mat roi2 = img(Rect(x - (abs(x2 - cx) / 2), y - abs(y2 - cy) / 2, abs(x2 - cx), abs(y2 - cy)));
		roi2 = Scalar::all(0);
		bitwise_or(roi2, roi1, roi2);
		imshow("result", img);
	}
}

void captureCB(int id) {
	if (ry == 0) { errorMsg("Photoshop"); return; }
	rx = cx;
	ry = cy;
	if (x2 < cx) rx = x2;
	if (y2 < cy) ry = y2;

	roi1 = src_buf(Rect(rx, ry, abs(x2 - cx), abs(y2 - cy)));

	OpenFileDialog* openFileDialogp = new OpenFileDialog();
	if (openFileDialogp->ShowDialog()) {
		Filename = openFileDialogp->FileName;
		result = imread(Filename);
		if (result.empty()) errorMsg("Image Open");
		else imshow("result", result);
	}
	else {
		errorMsg("Open File Dialog");
		return;
	}
	src_buf = result.clone();
	setMouseCallback("result", click, &result);
}


// function 10 kNN
GLUI* kNN_window;
Ptr<ml::KNearest> knn = ml::KNearest::create();

void drawDigit(int event, int x, int y, int, void* param) {
	if (event == EVENT_LBUTTONDOWN)
		drawing = true;
	else if (event == EVENT_MOUSEMOVE) {
		if (drawing == true)
			circle(gray, Point(x, y), thickness + 1, Scalar::all(255), -1);
	}
	else if (event == EVENT_LBUTTONUP)
		drawing = false;
	imshow("Test", gray);
}

void testCB (int id) {
	if (id == 0) {
		Mat img_resize, img_float, img_flatten, res;
		resize(gray, img_resize, Size(20, 20), 0, 0, INTER_AREA);
		img_resize.convertTo(img_float, CV_32F);
		img_flatten = img_float.reshape(1, 1);
		knn->findNearest(img_flatten, 3, res);
		float prediction = res.at<float>(0, 0);
		cout << "Predicted Digit = " << prediction << '\n';
	}
	else {
		gray = Mat::zeros(400, 400, CV_8U);
		imshow("Test", gray);
	}
}



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
		imshow("src", src);
		break;
	case FILTER_ID:
		filter_window->close();
		filter->enable();
		isClick[1] = 0;
		filterCheck = 0;
		filter->set_int_val(0);
		imshow("src", src);
		break;
	case LUT_ID:
		lut_window->close();
		lut->enable();
		isClick[3] = 0;
		lutCheck = 0;
		lut->set_int_val(0);
		imshow("src", src);
		break;
	case WARPING_ID:
		warping_window->close();
		warping->enable();
		isClick[4] = 0;
		warpingCheck = 0;
		warping->set_int_val(0);
		imshow("src", src);
		break;
	case HAUGH_ID:
		extract_window->close();
		featureExtract->enable();
		isClick[5] = 0;
		featureExtCheck = 0;
		featureExtract->set_int_val(0);
		imshow("src", src);
		break;
	case DETECT_ID:
		dectect_window->close();
		detectObj->enable();
		isClick[6] = 0;
		detectingCheck = 0;
		detectObj->set_int_val(0);
		imshow("src", src);
		break;
	case PHOTOSHOP_ID:
		photoshop_window->close();
		destroyWindow("result");
		photoshop->enable();
		isClick[8] = 0;
		photoshopCheck = 0;
		photoshop->set_int_val(0);
		imshow("src", src);
		break;
	case KNN_ID:
		kNN_window->close();
		destroyWindow("Test");
		numDetect->enable();
		isClick[9] = 0;
		numDetectCheck = 0;
		numDetect->set_int_val(0);
		break;
	default:
		errorMsg("Close Button");
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
		filter_window = GLUI_Master.create_glui("Filter", FILTER_ID, 700, 400);
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
		if (histEqual->get_int_val() == 1) imshow("src", result);
		else imshow("src", src);
		break;
	}
	case 4: {
		if (src.empty()) { errorMsg("Open Image"); initCheckbox(id);  return; }
		lut->disable();
		lut_window = GLUI_Master.create_glui("LUT", LUT_ID, 700, 200);
		GLUI_Panel* lutRadioGroupPanel = lut_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* lutRadioGroup = lut_window->add_radiogroup_to_panel(lutRadioGroupPanel, &lut_radio, -1, lutTypeCallback);
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Spring");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Summer");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Autumn");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Winter");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Rainbow");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "HSV");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Tony");

		lut_window->add_button("Close", LUT_ID, closeCallback);
		break;
	}
	case 5: {
		if (src.empty()) { errorMsg("Open Image"); initCheckbox(id);  return; }
		warping->disable();
		warping_window = GLUI_Master.create_glui("Warping", WARPING_ID, 700, 200);
		GLUI_Panel* warpingRadioGroupPanel = warping_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* warpingRadioGroup = warping_window->add_radiogroup_to_panel(warpingRadioGroupPanel, &warping_radio, -1, warpingAxis);
		warping_window->add_radiobutton_to_group(warpingRadioGroup, "X");
		warping_window->add_radiobutton_to_group(warpingRadioGroup, "Y");

		warping_window->add_button("Close", WARPING_ID, closeCallback);

		warping_window->add_column(FALSE);
		warpingSpinner = warping_window->add_spinner("Warping Offset", GLUI_SPINNER_FLOAT, &warping_offset, 0, warpingOffsetSpinner);
		warpingSpinner->set_speed(5);
		warpingSpinner->set_float_limits(-30.0, 30.0, GLUI_LIMIT_CLAMP);
		break;
	}
	case 6: {
		if (src.empty()) { errorMsg("Open Image"); initCheckbox(id);  return; }
		cvtColor(src, gray, COLOR_BGR2GRAY);
		result = src.clone();
		featureExtract->disable();
		extract_window = GLUI_Master.create_glui("Haugh Trans", HAUGH_ID, 700, 400);
		GLUI_Panel* extractRadioGroupPanel = extract_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* extractRadioGroup = extract_window->add_radiogroup_to_panel(extractRadioGroupPanel, &extract_radio, -1, haughShape);
		extract_window->add_radiobutton_to_group(extractRadioGroup, "Line");
		extract_window->add_radiobutton_to_group(extractRadioGroup, "Circle");

		extract_window->add_button("Close", HAUGH_ID, closeCallback);

		extract_window->add_column(FALSE);
		extractSpinner = extract_window->add_spinner("Haugh min", GLUI_SPINNER_INT, &haugh_mim, 0, haughSensitivity);
		extractSpinner->set_speed(0.5);
		extractSpinner->set_int_limits(1, 40, GLUI_LIMIT_CLAMP);
		break;
	}
	case 7: {
		if (src.empty()) { errorMsg("Open Image"); initCheckbox(id);  return; }
		cvtColor(src, gray, COLOR_BGR2GRAY);
		detectObj->disable();
		dectect_window = GLUI_Master.create_glui("Detect Object", DETECT_ID, 700, 600);
		GLUI_Panel* detectRadioGroupPanel = dectect_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* detectRadioGroup = dectect_window->add_radiogroup_to_panel(detectRadioGroupPanel, &detect_radio, -1, detectObject);
		dectect_window->add_radiobutton_to_group(detectRadioGroup, "Otsu");
		dectect_window->add_radiobutton_to_group(detectRadioGroup, "Labeling");

		dectect_window->add_button("Close", DETECT_ID, closeCallback);
		break;
	}
	case 8: {
		if (src.empty()) src = Mat(400, 600, CV_8UC3, Scalar(0, 0, 0));
		result = src.clone();
		if (brush->get_int_val() == 1) imshow("src", result);
		else {
			destroyWindow("src");
			imshow("src", src);
			return;
		}
		
		setMouseCallback("src", drawCircle);

		createTrackbar("R", "src", &red, 255);
		createTrackbar("G", "src", &green, 255);
		createTrackbar("B", "src", &blue, 255);
		createTrackbar("Thickness", "src", &thickness, 10);
		break;
	}
	case 9: {
		if (src.empty()) { errorMsg("Open Image"); initCheckbox(id);  return; }
		gray = src.clone();
		photoshop->disable();
		src_buf = gray.clone();
		setMouseCallback("src", photoshopCB, &gray);

		photoshop_window = GLUI_Master.create_glui("Photoshop", PHOTOSHOP_ID, 700, 200);
		GLUI_Panel* photoshopPanel = photoshop_window->add_panel("", GLUI_PANEL_EMBOSSED);
		photoshop_window->add_button_to_panel(photoshopPanel, "Capture Source", 0, captureCB);

		photoshop_window->add_button("Close", PHOTOSHOP_ID, closeCallback);

		break;
	}
	case 10: {
		src = imread("src/digits.png", IMREAD_GRAYSCALE);
		numDetect->disable();

		Mat train_features;  // 50,000개 데이터, 각 특징은 20x20
		Mat labels;
		// 각 숫자 영상을 행 벡터로 만들어서 train_feature에 저장한다. 
		for (int r = 0; r < 50; r++) {  // 각 숫자별 5줄씩 10개(0~9) 
			for (int c = 0; c < 100; c++) {  // 1줄에 100씩 
				Mat roi, roi_float, roi_flatten;
				roi = src(Rect(c * 20, r * 20, 20, 20));
				roi.convertTo(roi_float, CV_32F);
				roi_flatten = roi_float.reshape(1, 1);
				train_features.push_back(roi_flatten);
				labels.push_back(r / 5);
			}
		}

		// 학습 과정
		knn->train(train_features, ml::SampleTypes::ROW_SAMPLE, labels);

		// 테스트 과정
		gray = Mat::zeros(400, 400, CV_8U);

		kNN_window = GLUI_Master.create_glui("Digit Training", KNN_ID, 700, 200);
		GLUI_Panel* kNNPanel = kNN_window->add_panel("", GLUI_PANEL_EMBOSSED);
		kNN_window->add_button_to_panel(kNNPanel, "Test!", 0, testCB);
		kNN_window->add_button_to_panel(kNNPanel, "Clear", 1, testCB);

		kNN_window->add_button("Close", KNN_ID, closeCallback);

		imshow("Test", gray);
		setMouseCallback("Test", drawDigit);
		createTrackbar("Thickness", "Test", &thickness, 10);

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
	case 12: {
		if (src.empty()) { errorMsg("Open Image"); initCheckbox(id);  return; }
		Mat RGB[3], RGBO[3];
		split(src, RGB);
		for (int i = 0; i < 3; i++) equalizeHist(RGB[i], RGBO[i]);
		merge(RGBO, 3, result);
		if (histEqual->get_int_val() == 1) imshow("src", result);
		else imshow("src", src);

		lut->disable();
		lut_window = GLUI_Master.create_glui("LUT", LUT_ID, 700, 200);
		GLUI_Panel* lutRadioGroupPanel = lut_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* lutRadioGroup = lut_window->add_radiogroup_to_panel(lutRadioGroupPanel, &lut_radio, -1, lutTypeCallback);
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Spring");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Summer");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Autumn");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Winter");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Rainbow");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "HSV");
		lut_window->add_radiobutton_to_group(lutRadioGroup, "Tony");

		lut_window->add_button("Close", LUT_ID, closeCallback);

		cvtColor(src, gray, COLOR_BGR2GRAY);
		result = src.clone();
		featureExtract->disable();
		extract_window = GLUI_Master.create_glui("Haugh Trans", HAUGH_ID, 700, 400);
		GLUI_Panel* extractRadioGroupPanel = extract_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* extractRadioGroup = extract_window->add_radiogroup_to_panel(extractRadioGroupPanel, &extract_radio, -1, haughShape);
		extract_window->add_radiobutton_to_group(extractRadioGroup, "Line");
		extract_window->add_radiobutton_to_group(extractRadioGroup, "Circle");

		extract_window->add_button("Close", HAUGH_ID, closeCallback);

		extract_window->add_column(FALSE);
		extractSpinner = extract_window->add_spinner("Haugh min", GLUI_SPINNER_INT, &haugh_mim, 0, haughSensitivity);
		extractSpinner->set_speed(0.5);
		extractSpinner->set_int_limits(1, 40, GLUI_LIMIT_CLAMP);

		detectObj->disable();
		dectect_window = GLUI_Master.create_glui("Detect Object", DETECT_ID, 700, 550);
		GLUI_Panel* detectRadioGroupPanel = dectect_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* detectRadioGroup = dectect_window->add_radiogroup_to_panel(detectRadioGroupPanel, &detect_radio, -1, detectObject);
		dectect_window->add_radiobutton_to_group(detectRadioGroup, "Otsu");
		dectect_window->add_radiobutton_to_group(detectRadioGroup, "Labeling");

		dectect_window->add_button("Close", DETECT_ID, closeCallback);

		warping->disable();
		warping_window = GLUI_Master.create_glui("Warping", WARPING_ID, 850, 200);
		GLUI_Panel* warpingRadioGroupPanel = warping_window->add_panel("", GLUI_PANEL_EMBOSSED);
		GLUI_RadioGroup* warpingRadioGroup = warping_window->add_radiogroup_to_panel(warpingRadioGroupPanel, &warping_radio, -1, warpingAxis);
		warping_window->add_radiobutton_to_group(warpingRadioGroup, "X");
		warping_window->add_radiobutton_to_group(warpingRadioGroup, "Y");

		warping_window->add_button("Close", WARPING_ID, closeCallback);

		warping_window->add_column(FALSE);
		warpingSpinner = warping_window->add_spinner("Warping Offset", GLUI_SPINNER_FLOAT, &warping_offset, 0, warpingOffsetSpinner);
		warpingSpinner->set_speed(5);
		warpingSpinner->set_float_limits(-30.0, 30.0, GLUI_LIMIT_CLAMP);
		break;
	}
	default:
		if (getWindowProperty("src", WND_PROP_VISIBLE) >= 1) { destroyWindow("src"); imshow("src", src); }		
		if (getWindowProperty("Binary", WND_PROP_VISIBLE) >= 1) destroyWindow("Binary");
		if (getWindowProperty("Filter", WND_PROP_VISIBLE) >= 1) destroyWindow("Filter");
		if (getWindowProperty("LUT", WND_PROP_VISIBLE) >= 1) destroyWindow("LUT");
		if (getWindowProperty("Warping", WND_PROP_VISIBLE) >= 1) destroyWindow("Warping");
		if (getWindowProperty("Haugh Trans", WND_PROP_VISIBLE) >= 1) destroyWindow("Haugh Trans");
		if (getWindowProperty("Detect Object", WND_PROP_VISIBLE) >= 1) destroyWindow("Detect Object");
		if (getWindowProperty("Photoshop", WND_PROP_VISIBLE) >= 1) destroyWindow("Photoshop");
		if (getWindowProperty("Digit Training", WND_PROP_VISIBLE) >= 1) destroyWindow("Digit Training");
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
