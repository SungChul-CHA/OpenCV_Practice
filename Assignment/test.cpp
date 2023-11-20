#include <opencv2/opencv.hpp>
#include <GL/glut.h>
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"

using namespace cv;

VideoCapture cap;
GLUI* glui;
int playMode = 0;
bool isPaused = false;
String videoPath;
int previousPlayMode = 1; // ���� ��� ��带 ������ ���� �߰�

std::string TCHARToString(const TCHAR* ptsz)
{
    int len = wcslen((wchar_t*)ptsz);
    char* psz = new char[2 * len + 1];
    wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
    std::string s = psz;
    delete[] psz;
    return s;
}

void display()
{
    Mat frame;
    int currentFrame;

    if (!isPaused) {
        switch (playMode) {
        case 1:
            currentFrame = cap.get(CAP_PROP_POS_FRAMES);
            if (!cap.read(frame)) {
                cap.set(CAP_PROP_POS_FRAMES, 0);
                cap.read(frame);
            }
            break;
        case 2:
            currentFrame = cap.get(CAP_PROP_POS_FRAMES);
            if (currentFrame == 0) {
                cap.set(CAP_PROP_POS_FRAMES, cap.get(CAP_PROP_FRAME_COUNT) - 1);
            }
            cap.read(frame);
            break;
        case 3:
            for (int i = 0; i < 5; ++i) {
                if (!cap.read(frame)) {
                    cap.set(CAP_PROP_POS_FRAMES, 0);
                    cap.read(frame);
                }
            }
            break;
        case 4:
            for (int i = 0; i < 5; ++i) {
                currentFrame = cap.get(CAP_PROP_POS_FRAMES);
                if (currentFrame == 0) {
                    cap.set(CAP_PROP_POS_FRAMES, cap.get(CAP_PROP_FRAME_COUNT) - 1);
                }
                cap.read(frame);
            }
            break;
        }
    }

    // ���⿡�� frame�� ������ �߰����� ���� ó���� ������ �� �ֽ��ϴ�.

    // OpenGL â�� ������ ������ ǥ��
    glDrawPixels(frame.cols, frame.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, frame.data);

    glutSwapBuffers();
}

void update(int value)
{
    glutPostRedisplay();
    glutTimerFunc(33, update, 0);
}

void playButtonCallback(int state)
{
    if (state == 1) {
        isPaused = !isPaused;
    }
}

void modeRadioCallback(int mode)
{
    // ��� ���� �� ��带 �����ϸ� ���� ��� ��带 �����ϰ� �Ͻ� ���� ���·� ��ȯ
    if (!isPaused) {
        previousPlayMode = playMode;
        isPaused = true;
    }
    playMode = mode;
}

void openVideoButtonCallback(int state)
{
    OpenFileDialog openFileDialog;
    openFileDialog.Filter = _T("Video Files\0*.mp4;*.avi;*.mkv\0All Files\0*.*\0");
    openFileDialog.Title = _T("Open Video File");

    if (openFileDialog.ShowDialog()) {
        videoPath = TCHARToString(openFileDialog.FileName);
        cap.open(videoPath);

        if (!cap.isOpened()) {
            std::cerr << "Error: Couldn't open the video file." << std::endl;
        }
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("GLUI + OpenCV + OpenGL Video Player");

    glui = GLUI_Master.create_glui("Controls");

    glui->add_button("Open Video", 1, openVideoButtonCallback);
    glui->add_button("Play/Pause", 2, playButtonCallback);

    GLUI_RadioGroup* modeRadioGroup = glui->add_radiogroup(&playMode);
    GLUI_RadioButton* forwardButton = glui->add_radiobutton_to_group(modeRadioGroup, "Forward");
    GLUI_RadioButton* backwardButton = glui->add_radiobutton_to_group(modeRadioGroup, "Backward");
    GLUI_RadioButton* forwardFastButton = glui->add_radiobutton_to_group(modeRadioGroup, "Forward Fast");
    GLUI_RadioButton* backwardFastButton = glui->add_radiobutton_to_group(modeRadioGroup, "Backward Fast");

    // �� ���� ��ư�� �ݹ��� �����մϴ�.
    forwardButton->set_int_val(1);
    backwardButton->set_int_val(2);
    forwardFastButton->set_int_val(3);
    backwardFastButton->set_int_val(4);

    // �߰��� ��ư��
    glui->add_button("Pause", 5, playButtonCallback);
    glui->add_button("Resume", 6, playButtonCallback);

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}
