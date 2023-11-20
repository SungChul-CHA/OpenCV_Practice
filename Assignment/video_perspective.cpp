#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int cx, cy;
int index = 0;
Point2f inputp[4], outputp[4];
Mat h;

// ���콺 �̺�Ʈ�� �߻��ϸ� ȣ��Ǵ� �ݹ� �Լ��̴�. 
void extract(int event, int x, int y, int flags, void* param)
{
	Mat& img = *(Mat*)param;
	if ((index < 4) & (event == EVENT_LBUTTONDOWN)) {
		cx = x;
		cy = y;
		inputp[index] = Point2f(cx, cy);
		circle(img, inputp[index], 7, Scalar(100, 255, 51), 3);
		imshow("image", img);
		index++;
	}
}

void setPos(int event, int x, int y, int flags, void* param)
{
	Mat& img = *(Mat*)param;
	
	if ((index < 4) & (event == EVENT_LBUTTONDOWN)) {
		cx = x;
		cy = y;
		outputp[index] = Point2f(cx, cy);
		circle(img, outputp[index], 7, Scalar(100, 255, 51), 3);
		imshow("image", img);
		index++;
		if (index == 4)
			h = getPerspectiveTransform(inputp, outputp);
	}
}

int main()
{
	Mat src = imread("lenna.jpg");
	Mat buf1, buf2;
	buf1 = src.clone();
	imshow("image", src);

	setMouseCallback("image", extract, &src);

	while (1) {
		// Ű���� �Է� ���
		int key = waitKey();
		if (key == 'c') {
			break;
		}
		else {
			index = 0;
			buf1.copyTo(src);
			imshow("image", src);
		}
	}

	VideoCapture cap(0);	// ��ķ
	//VideoCapture cap("BigBuckBunny.mp4");	// ������
	if (!cap.isOpened()) { cout << "�������� �� �� ����\n"; return -1; }

	Mat frame;
	for (int i = 0; i < 5; i++)
		cap >> frame;
	buf2 = frame.clone();
	index = 0;
	imshow("image", frame);

	setMouseCallback("image", setPos, &frame);

	while (1) {
		// Ű���� �Է� ���
		int key = waitKey();
		if (key == 'c') {
			break;
		}
		else {
			index = 0;
			buf2.copyTo(frame);
			imshow("image", frame);
		}
	}

	warpPerspective(buf1, buf2, h, frame.size());
	
	while (1) {
		int speed = 5;

		cap >> frame;
		if (frame.empty()) {
			cout << "������ ����" << endl;
			break;
		}

		frame.copyTo(buf2);
		imshow("image", buf2);
		waitKey(speed);
	}


    return 0;
}