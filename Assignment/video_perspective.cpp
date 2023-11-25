#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int cx, cy;
int index = 0;
Point2f inputp[4], outputp[4];
Mat h, out, buf;
Mat buf1, buf2, buf3;
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
			buf = Mat::zeros(frame.rows, frame.cols, frame.depth());
			fillPoly(buf, points, Scalar::all(255));
			h = getPerspectiveTransform(inputp, outputp);
			warpPerspective(buf1, out, h, frame.size());
			out.copyTo(buf3, buf);
			imshow("video", buf3);
		}
	}
}

int main()
{

	//VideoCapture cap(0);	// 웹캠
	VideoCapture cap("src/BigBuckBunny.mp4");	// 동영상
	if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; return -1; }
	Mat frame;
	for (int i = 0; i < 5; i++)
		cap >> frame;
	src = imread("src/yoon2.jpg");
	buf1 = src.clone();
	imshow("image", src);

	setMouseCallback("image", extract, &src);

	while (1) {
		if (waitKey() == 'c') break;
		else {
			index = 0;
			buf1.copyTo(src);
			imshow("image", src);
		}
	}

	buf2 = frame.clone();
	buf3 = frame.clone();
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
			buf2.copyTo(frame);
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

		out.copyTo(frame, buf);
		imshow("video", frame);
		waitKey(speed);
	}


	return 0;
}