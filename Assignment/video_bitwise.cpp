#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int cx, cy, x2, y2, rx, ry, w, h;
bool isDrawing = false;
Mat buf, roi1, result;

void drawing(int event, int x, int y, int, void* param) {
	// img = img1
	Mat& img = *(Mat*)param;
	if (event == EVENT_LBUTTONDOWN) {
		isDrawing = true;
		// 마우스 클릭 좌표
		cx = x;
		cy = y;
	}
	else if (event == EVENT_MOUSEMOVE) {
		if (isDrawing) {
			// buf에 있는 이미지를 img1으로
			img = buf.clone();
			rx = cx;
			ry = cy;
			if (x < cx) rx = x;
			if (y < cy) ry = y;
			// 현재 좌표와 클릭 좌표 중 작은 값으로 img1에 초록 사각형 그리기
			rectangle(img, Rect(rx, ry, abs(x - cx), abs(y - cy)), Scalar(0, 255, 0), 2);
			// 사각형 그려진 img1 보여줌
			imshow("image", img);
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
		// 좌클릭 땠을 때 좌표와 클릭 좌표 중 작은 값으로 img1에 초록 사각형 그리기
		rectangle(img, Rect(rx, ry, abs(x - cx), abs(y - cy)), Scalar(0, 255, 0), 2);
		// 사각형 그려진 img1 보여줌
		imshow("image", img);
	}
}

void click(int event, int x, int y, int, void*) {
	if (event == EVENT_LBUTTONDOWN) {
		w = abs(x2 - cx);
		h = abs(y2 - cy);
		rx = x - (abs(x2 - cx) / 2);
		ry = y - abs(y2 - cy) / 2;
	}
}

int main() {
	// 편집 할 이미지 불러오기
	Mat img1 = imread("src/lenna.jpg");
	// 초록 사각형이 없는 img1을 buf에 저장
	buf = img1.clone();
	// 초기 img1
	imshow("image", img1);
	setMouseCallback("image", drawing, &img1);
	while (1) {
		// 키보드 입력 대기
		int key = waitKey();
		if (key == 'c') {
			rx = cx;
			ry = cy;
			if (x2 < cx) rx = x2;
			if (y2 < cy) ry = y2;
			// roi1은 사각형 크기의 buf(초록 사각형 없는 img1)
			roi1 = buf(Rect(rx, ry, abs(x2 - cx), abs(y2 - cy)));
			break;
		}
	}


	VideoCapture cap(0);	// 웹캠
	//VideoCapture cap("BigBuckBunny.mp4");	// 동영상
	if (!cap.isOpened()) { cout << "동영상을 열 수 없음\n"; return -1; }

	// image 창에서 클릭한 좌표가 사각형의 중점이 되도록 x, y, width, height 반환
	setMouseCallback("image", click);



	while (1) {
		Mat frame;
		cap >> frame;	// 1 frame 추출
		if (frame.empty()) {
			cout << "동영상 종료" << endl;
			break;
		}
		int speed = 1;

		// roi2는 사각형 크기의 frame
		Mat roi2 = frame(Rect(rx, ry, w, h));

		// frame에서 클릭한 위치의 사각형 크기만큼 0으로
		roi2 = Scalar::all(0);

		// 0이랑 roi1 편집한 buf의 사각형과 bitwise_or 하고싶다..
		roi1.copyTo(roi2);

		// 동영상 출력
		imshow("image", frame);

		// 동영상 속도
		if (waitKey(speed) >= 0) break;
	}
	return 0;
}