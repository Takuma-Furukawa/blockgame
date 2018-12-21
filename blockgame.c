// ブロックゲーム
#include <stdio.h>
#include <stdlib.h>
#include <glut/glut.h>

void init(void);
int width, height;
int left_down;
int i = 0;
int start = 0;

/* ブロックの情報を管理する構造体 */
typedef struct {
    double X;
    double Y;                   //ブロックの座標は、長方形の左下の頂点の座標で定義する
    int count;                  //何回ボールがブロックに当たったか
} BLOCK;

/* ボールの情報を管理する構造体 */
typedef struct {
    double X;
    double Y;                   //ボールの座標
    double dx;
    double dy;                  //ボールの速度
} BALL;

/* ボードの情報を管理する構造体 */
typedef struct {
    double X;
    double Y;
} BOARD;

BALL ball;
BLOCK block[18];                 //ブロックごとに情報を管理する
BOARD board;

/* 描画 */
void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    /* プレイヤーが動かすボード */
    glColor3d(0.0,1.0,0.0);
    glRectd(board.X, board.Y, board.X + 160, board.Y + 20);
    
    /* 飛び回るボールの描画 */
    glColor3d(1.0,1.0,1.0);
    glPointSize(10.0);
    glBegin(GL_POINTS);
    glVertex2d(ball.X, ball.Y);
    glEnd();
    
    /* ブロック群 */
    for(i=0;i<=17;i++)
    {
        if(block[i].count == 0){
            glColor3d(0.0,0.0,0.8);
            glRectd(block[i].X, block[i].Y, block[i].X + 70, block[i].Y + 20);
        }
        if(block[i].count == 1){
            glColor3d(0.5,0.5,0.5);
            glRectd(block[i].X, block[i].Y, block[i].X + 70, block[i].Y + 20);
        }
    }
    glutSwapBuffers();
}

/* 再描画関数 */
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
    width = w;
    height = h;
}

/* ボードを動かすためのマウス位置入力 */
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_DOWN) {
            board.X = x;
            left_down = 1;
        } else if (state == GLUT_UP){
            left_down = 0;
        }
    }
    if (button == GLUT_RIGHT_BUTTON){
        if(state == GLUT_DOWN){
            start = 1;
        }
    }
}

/* ボードを動かすための関数 */
void motion(int x, int y)
{
    if (left_down) {
        board.X = x;
        glutPostRedisplay();
    }
}

/* ボールを動かすための関数 => 基本的な動的操作は全てここで行う */
void idle(void)
{
    
    /* ボールの移動 */
    /* ボールの発射まで */
    if(start == 0){
        /* ball.Xが5の倍数になるように調整する */
        ball.X = ((int)(board.X+0.9))/5*5.0;
        ball.Y = board.Y + 30;
    }
    /* ボールの発射後 */
    else{
        ball.X = ball.X + ball.dx;
        ball.Y = ball.Y + ball.dy;
    }
    
    /* 衝突判定と速度反転 */
    if (ball.X == 0 || ball.X == 640){
        ball.dx = - ball.dx;
    }
    if (ball.Y == 480){
        ball.dy = - ball.dy;
    }
    /* ブロックへの衝突と速度反転 */
    for (i=0;i<=17;i++){
        /* ブロックの縦面との衝突。x方向の速度が反転する */
        if((ball.X == block[i].X || ball.X == block[i].X + 70) && (block[i].Y <= ball.Y && ball.Y <= block[i].Y + 20))
        {
            if(block[i].count <= 1){
                ball.dx = - ball.dx;
                block[i].count = block[i].count + 1;
            }
        }
        /* ブロックの横面との衝突。y方向の速度が反転する */
        if((ball.Y == block[i].Y || ball.Y == block[i].Y + 20) && (block[i].X <= ball.X && ball.X <= block[i].X + 70))
        {
            if(block[i].count <= 1){
                ball.dy = - ball.dy;
                block[i].count = block[i].count + 1;
            }
        }
    }
    
    /* ボードへの衝突と速度反転　*/
    if (ball.Y == board.Y + 20 && (board.X <= ball.X && ball.X <= board.X + 160))
    {
        ball.dy = - ball.dy;
    }
    
    /* ゲームオーバーの場合はウィンドウが閉じる */
    if (ball.Y == 0)
    {
        exit(0);
    }
    glutPostRedisplay();
}

/* 初期化関数 */
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    left_down = 0;
}

/* ボールやブロックの初期座標設定を行う */
void initiate_position(void)
{
    ball.dx = -5.0;                 //ボールの初期速度
    ball.dy = 5.0;                  //ボールの速度は基本的に反転するのみ
    
    /* ブロック群（今回は12個）の座標を定義する */
    for(i=0;i<=5;i++)
    {
        block[i].X = 40 + 100 * i;
        block[i].Y = 400;
        block[i].count = 0;
    }
    for(i=6;i<=11;i++)
    {
        block[i].X = 40 + 100 * (i-6);
        block[i].Y = 370;
        block[i].count = 0;
    }
    for(i=12;i<=17;i++)
    {
        block[i].X = 40 + 100 * (i-12);
        block[i].Y = 340;
        block[i].count = 0;
    }
    /* ボードの初期位置設定 */
    board.X = 240;
    board.Y = 30;
}

/* メイン関数 */
main(int argc, char **argv)
{
    initiate_position();
    glutInitWindowSize(640, 480);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    init();
    glutMainLoop();
    return 0;
}
