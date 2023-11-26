#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int cx, cy;
int index = 0;
Point2f inputp[4], outputp[4];
Mat transform_matrix, PT_out, or_mask;
Mat src_origin, frame_for_init, frame_for_trans;
Mat src;

// 마우스 이벤트가 발생하면 호출되는 콜백 함수이다. 
void extract(int event, int x, int y, int flags, void* param)
{
	Mat& src = *(Mat*)param;
	if ((index < 4) & (event == EVENT_LBUTTONDOWN)) {
		cx = x;
		cy = y;
		inputp[index] = Point2f(cx, cy);
		circle(src, inputp[index], 5, Scalar(100, 255, 51), -1);
		imshow("image", src);
		index++;
		if (index == 4) {
			vector<Point> points;
			for (int i = 0; i < 4; ++i) {
				points.push_back(Point(inputp[i].x, inputp[i].y));
			}
			polylines(src, points, true, Scalar(100, 255, 51), 3);
			imshow("image", src);
		}
	}
}

void setPos(int event, int x, int y, int flags, void* param)
{
	Mat& frame = *(Mat*)param;
	if ((index < 4) & (event == EVENT_LBUTTONDOWN)) {
		cx = x;
		cy = y;
		outputp[index] = Point2f(cx, cy);
		circle(frame, outputp[index], 7, Scalar(100, 255, 51), 3);
		imshow("video", frame);
		index++;
		if (index == 4) {
			vector<Point> points;
			for (int i = 0; i < 4; ++i) {
				points.push_back(Point(outputp[i].x, outputp[i].y));
			}
			or_mask = Mat::zeros(frame.rows, frame.cols, frame.depth());
			fillPoly(or_mask, points, Scalar::all(255));
			transform_matrix = getPerspectiveTransform(inputp, outputp);
			warpPerspective(src_origin, PT_out, transform_matrix, frame.size());
			PT_out.copyTo(frame_for_trans, or_mask);
			imshow("video", frame_for_trans);
		}
	}
}

int main()
{

	//VideoCapture cap(0);	// 웹캠
	VideoCapture cap("src/timesquare.mp4");	// 동영상
	if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; return -1; }
	Mat frame;
	for (int i = 0; i < 5; i++)
		cap >> frame;
	src = imread("src/yoon2.jpg");
	src_origin = src.clone();
	imshow("image", src);

	setMouseCallback("image", extract, &src);

	while (1) {
		if (waitKey() == 'c') break;
		else {
			index = 0;
			src_origin.copyTo(src);
			imshow("image", src);
		}
	}

	frame_for_init = frame.clone();
	frame_for_trans = frame.clone();
	index = 0;
	imshow("video", frame);

	setMouseCallback("video", setPos, &frame);

	while (1) {
		// 키보드 입력 대기
		int key = waitKey();
		if (key == 'c') {
			break;
		}
		else {
			index = 0;
			frame_for_init.copyTo(frame);
			imshow("video", frame);
		}
	}



	while (1) {
		int speed = 5;

		cap >> frame;
		if (frame.empty()) {
			cout << "동영상 종료" << endl;
			break;
		}

		PT_out.copyTo(frame, or_mask);
		imshow("video", frame);
		waitKey(speed);
	}


	return 0;
}