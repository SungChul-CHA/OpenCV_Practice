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
		// ���콺 Ŭ�� ��ǥ
		cx = x;
		cy = y;
	}
	else if (event == EVENT_MOUSEMOVE) {
		if (isDrawing) {
			// buf�� �ִ� �̹����� img1����
			img = buf.clone();
			rx = cx;
			ry = cy;
			if (x < cx) rx = x;
			if (y < cy) ry = y;
			// ���� ��ǥ�� Ŭ�� ��ǥ �� ���� ������ img1�� �ʷ� �簢�� �׸���
			rectangle(img, Rect(rx, ry, abs(x - cx), abs(y - cy)), Scalar(0, 255, 0), 2);
			// �簢�� �׷��� img1 ������
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
		// ��Ŭ�� ���� �� ��ǥ�� Ŭ�� ��ǥ �� ���� ������ img1�� �ʷ� �簢�� �׸���
		rectangle(img, Rect(rx, ry, abs(x - cx), abs(y - cy)), Scalar(0, 255, 0), 2);
		// �簢�� �׷��� img1 ������
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
	// ���� �� �̹��� �ҷ�����
	Mat img1 = imread("src/lenna.jpg");
	// �ʷ� �簢���� ���� img1�� buf�� ����
	buf = img1.clone();
	// �ʱ� img1
	imshow("image", img1);
	setMouseCallback("image", drawing, &img1);
	while (1) {
		// Ű���� �Է� ���
		int key = waitKey();
		if (key == 'c') {
			rx = cx;
			ry = cy;
			if (x2 < cx) rx = x2;
			if (y2 < cy) ry = y2;
			// roi1�� �簢�� ũ���� buf(�ʷ� �簢�� ���� img1)
			roi1 = buf(Rect(rx, ry, abs(x2 - cx), abs(y2 - cy)));
			break;
		}
	}


	VideoCapture cap(0);	// ��ķ
	//VideoCapture cap("BigBuckBunny.mp4");	// ������
	if (!cap.isOpened()) { cout << "�������� �� �� ����\n"; return -1; }

	// image â���� Ŭ���� ��ǥ�� �簢���� ������ �ǵ��� x, y, width, height ��ȯ
	setMouseCallback("image", click);



	while (1) {
		Mat frame;
		cap >> frame;	// 1 frame ����
		if (frame.empty()) {
			cout << "������ ����" << endl;
			break;
		}
		int speed = 1;

		// roi2�� �簢�� ũ���� frame
		Mat roi2 = frame(Rect(rx, ry, w, h));

		// frame���� Ŭ���� ��ġ�� �簢�� ũ�⸸ŭ 0����
		roi2 = Scalar::all(0);

		// 0�̶� roi1 ������ buf�� �簢���� bitwise_or �ϰ�ʹ�..
		roi1.copyTo(roi2);

		// ������ ���
		imshow("image", frame);

		// ������ �ӵ�
		if (waitKey(speed) >= 0) break;
	}
	return 0;
}