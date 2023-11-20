#include <opencv2/opencv.hpp>
#include <GL/glut.h>
#include <GL/glui.h>

using namespace cv;

VideoCapture cap;
GLUI* glui;
int isPlaying = true;

void display()
{
    Mat frame;

    if (isPlaying && cap.read(frame)) {
        // OpenGL â�� ������ ������ ǥ��
        glDrawPixels(frame.cols, frame.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, frame.data);
    }

    glutSwapBuffers();
}

void update(int value)
{
    glutPostRedisplay(); // display �Լ� ȣ��
    glutTimerFunc(33, update, 0); // 30fps�� ����
}

void playButtonCallback(int state)
{
    isPlaying = (state == GL_TRUE);
}

int main(int argc, char** argv)
{
    // ������ ���� ���
    const char* videoPath = "src/gizmo.mp4";

    // ������ ���� ����
    cap.open(videoPath);

    if (!cap.isOpened()) {
        std::cerr << "Error: Couldn't open the video file." << std::endl;
        return -1;
    }

    // OpenGL �ʱ�ȭ
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("GLUI + OpenGL Video Player");

    // GLUI �ʱ�ȭ
    GLUI_Master.set_glutDisplayFunc(display);
    glui = GLUI_Master.create_glui("Controls");

    // GLUI 'Play' ��ư �߰�
    glui->add_button("Play", 1, playButtonCallback);

    // OpenGL Ÿ�̸� �Լ� ���
    glutTimerFunc(0, update, 0);

    // OpenGL ���� ����
    glutMainLoop();

    return 0;
}
