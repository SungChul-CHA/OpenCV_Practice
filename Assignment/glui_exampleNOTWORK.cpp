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
        // OpenGL 창에 동영상 프레임 표시
        glDrawPixels(frame.cols, frame.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, frame.data);
    }

    glutSwapBuffers();
}

void update(int value)
{
    glutPostRedisplay(); // display 함수 호출
    glutTimerFunc(33, update, 0); // 30fps로 설정
}

void playButtonCallback(int state)
{
    isPlaying = (state == GL_TRUE);
}

int main(int argc, char** argv)
{
    // 동영상 파일 경로
    const char* videoPath = "src/gizmo.mp4";

    // 동영상 파일 열기
    cap.open(videoPath);

    if (!cap.isOpened()) {
        std::cerr << "Error: Couldn't open the video file." << std::endl;
        return -1;
    }

    // OpenGL 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("GLUI + OpenGL Video Player");

    // GLUI 초기화
    GLUI_Master.set_glutDisplayFunc(display);
    glui = GLUI_Master.create_glui("Controls");

    // GLUI 'Play' 버튼 추가
    glui->add_button("Play", 1, playButtonCallback);

    // OpenGL 타이머 함수 등록
    glutTimerFunc(0, update, 0);

    // OpenGL 루프 시작
    glutMainLoop();

    return 0;
}
