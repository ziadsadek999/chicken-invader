
#include <stdio.h>
#include <math.h>
#include <random>
#include <glut.h>
#include <queue>
#include <iostream>
#include <chrono>
#include <string>
using namespace std;

void drawRect(int x, int y, int w, int h, double r, double g, double b);
void drawPlayer();
void drawEnemy();
void Key(unsigned char key, int x, int y);
void Loop();
void drawPlayerLives();
void generatePowerUp(int value);
void enemyFire(int value);
void Display();
bool intersectsEnemy(int x, int y, int r);
bool intersectsPlayer(int x, int y, int r);
void drawCircle(int R, int x, int y, double r, double g, double b);
void drawTriangle(int x, int y, int w, int h,double r,double g,double b);
float perpDistance(int x, int y, float a, float b, float c);
float prepDistance(int x, int y, int a, int b);

int gameStatus = 0;
int windowWidth = 800;
int windowHeight = 800;
int playerPositionX = windowWidth / 2;
int playerPositionY = windowHeight / 2;
int startX = 150;
int startY = 150;
int playerHealth = 3;
int enemyHealth = windowWidth-125;
float enemyPos = windowWidth / 2;
float enemyDir = 0.05;
int powerUpX = -1;
int powerUpY = -1;
bool playerHasPowerUp = false;
int playerStep = 10;
auto powerUpTime =  std::chrono::system_clock::now();
queue<double> bulletsX;
queue<double> bulletsY;
queue<double> enemyBulletsX;
queue<double> enemyBulletsY;


void main(int argc, char** argr) {
    glutInit(&argc, argr);

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(startX, startY);
    glutCreateWindow("Robotic Arm");
    glutDisplayFunc(Display);
    glutKeyboardFunc(Key);
    glutIdleFunc(Loop);
    glutTimerFunc(0, enemyFire, 0);
    glutTimerFunc(0, generatePowerUp, 0);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 800, 0, 800);

    glutMainLoop();
}

void drawRect(int x, int y, int w, int h, double r,double g, double b) {
    glBegin(GL_POLYGON);
    glColor3f(r, g, b);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawPlayer() {
    float r = 0;
    float g = 0;
    float b = 0;
    if (playerHasPowerUp) {
        r = 0.9;
        g = 1;
        b = 0;
    }
    glPushMatrix();
    glTranslatef(playerPositionX, playerPositionY, 0);
    drawRect(-50, -10, 100, 20,r,g,b);
    drawCircle(15, 35, -10, r, g, b);
    drawCircle(15, -35, -10, r, g, b);
    drawTriangle(0, 10, 20, 20,r,g,b);
    glPopMatrix();
    int n = bulletsX.size();
    while (n--) {
        double x = bulletsX.front();
        double y = bulletsY.front();
        bulletsX.pop();
        bulletsY.pop();
        drawCircle(5, x, y, 0, 0, 0);
        bulletsX.push(x);
        bulletsY.push(y);
    }
}

void drawEnemy() {
    glPushMatrix();
    glTranslatef(enemyPos, windowHeight-60, 0);
    drawRect(-150, -20, 300, 40, 0, 0, 0);
    drawCircle(20, 130, 20, 0, 0, 0);
    drawCircle(20, -130, 20, 0, 0, 0);
    drawTriangle(0, -20, 30, -30,0,0,0);
    glPopMatrix();
    int n = enemyBulletsX.size();
    while (n--) {
        double x = enemyBulletsX.front();
        double y = enemyBulletsY.front();
        enemyBulletsX.pop();
        enemyBulletsY.pop();
        drawCircle(5, x, y, 1, 0, 0);
        enemyBulletsX.push(x);
        enemyBulletsY.push(y);
    }
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRasterPos2f(5, 5);
    int len, i;
    char* string = "Enemy health:";
    len = (int)strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
    }
    drawRect(125, 0, enemyHealth, 20, 1, 0, 0);
    glPopMatrix();
}
void drawCircle(int R, int x, int y,double r,double g,double b) {
    glColor3f(r, g, b);
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric* quadObj = gluNewQuadric();
    gluDisk(quadObj, 0, R, 50, 50);
    glPopMatrix();
}
void drawTriangle(int x, int y, int w, int h,double r,double g,double b) {
    glBegin(GL_POLYGON);
    glColor3f(r, g, b);
    glVertex2f(x-w, y);
    glVertex2f(x+w, y);
    glVertex2f(x, y+h);
    glEnd();
}

bool intersectsEnemy(int x, int y, int r) {
    int enemyPosY = windowHeight - 60;
    if (x-r > enemyPos + 150 || x+r < enemyPos - 150) {
        return false;
    }
    if (y+r<enemyPosY - 50 || y-r>enemyPosY + 40) {
        return false;
    }
    if (x+r<enemyPos - 30 && y+r<enemyPosY - 20) {
        return false;
    }
    if (x-r>enemyPos + 30 && y+r<enemyPosY - 20) {
        return false;
    }
    if (y + r < enemyPosY - 20) {
        if (perpDistance(x, y, -1, 1, playerPositionX - playerPositionY - 50) > r) {
            if (perpDistance(x, y, 1, 1, -playerPositionX - playerPositionY - 30) > r) {
                return false;
            }
        }
    }
    return true;
}

bool intersectsPlayer(int x, int y, int r) {
    if (x-r > playerPositionX + 50 || x+r < playerPositionX - 50) {
        return false;
    }
    if (y+r<playerPositionY - 25 || y-r>playerPositionY + 30) {
        return false;
    }
    if (x+r < playerPositionX - 20 && y-r> playerPositionY+10) {
        return false;
    }
    if (x - r > playerPositionX + 20 && y - r> playerPositionY + 10) {
        return false;
    }
    if (x + r < playerPositionX + 20 && y + r< playerPositionY - 10&&x-r>playerPositionX-20) {
        return false;
    }
    if (y - r > playerPositionY + 10) {
        if (perpDistance(x, y, -1, 1,playerPositionX - playerPositionY - 30) > r) {
            if (perpDistance(x, y, 1, 1, -playerPositionX - playerPositionY - 30) > r) {
                return false;
            }
        }
    }
    if (y + r < playerPositionY - 10) {
        if (prepDistance(x, y, playerPositionX+35,playerPositionY-10)>r+15) {
            if (prepDistance(x, y, playerPositionX - 35, playerPositionY - 10) > r + 15) {
                return false;
            }
        }
    }
    return true;
}

void Key(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        if (playerPositionX - playerStep - 50 < 0) {
            playerPositionX = 50;
        }
        else {
            playerPositionX -= playerStep;
        }
      
        break;
    case 'd':
        if (playerPositionX + playerStep + 50 > windowWidth) {
            playerPositionX = windowWidth-50;
        }
        else {
            playerPositionX += playerStep;
        }
        break;
    case 'w':
        if (playerPositionY + playerStep + 30 > windowHeight-110) {
            playerPositionY = windowHeight - 30-110;
        }
        else {
            playerPositionY += playerStep;
        }
        break;
    case 's':
        if (playerPositionY - playerStep - 30 < 40) {
            playerPositionY = 70;
        }
        else {
            playerPositionY -= playerStep;
        }
        break;
    case 'c':
            bulletsX.push(playerPositionX);
            bulletsY.push(playerPositionY+35);
        break;
    case 'f':
        playerStep = playerStep==1?10:1;
        break;
    case ' ':
        if (gameStatus != 0) {
            gameStatus = 0;
            windowWidth = 800;
            windowHeight = 800;
            playerPositionX = windowWidth / 2;
            playerPositionY = windowHeight / 2;
            startX = 150;
            startY = 150;
            playerHealth = 3;
            enemyHealth = windowWidth - 125;
            enemyPos = windowWidth / 2;
            enemyDir = 0.05;
            powerUpX = -1;
            powerUpY = -1;
            playerHasPowerUp = false;
            playerStep = 10;
            powerUpTime = std::chrono::system_clock::now();
            std::queue<double> empty;
            std::swap(bulletsX, empty);
            std::queue<double> empty2;
            std::swap(bulletsY, empty2);
            std::queue<double> empty3;
            std::swap(enemyBulletsX, empty3);
            std::queue<double> empty4;
            std::swap(enemyBulletsY, empty4);
            playerHasPowerUp = false;
            glutPostRedisplay();
       }
        break;
    }
    if (powerUpX != -1) {
        if (intersectsPlayer(powerUpX, powerUpY, 10)) {
            powerUpX = -1;
            powerUpY = -1;
            playerHasPowerUp = true;
            powerUpTime = std::chrono::system_clock::now();
        }
        glutPostRedisplay();
    }
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (gameStatus == 0) {
        drawEnemy();
        drawPlayer();
        drawPlayerLives();
        if (powerUpX != -1) {
            drawCircle(10, powerUpX, powerUpY, 0, 1, 0);
        }
    }
    else if (gameStatus == 1) {
        glColor3f(1, 0, 0);
        glRasterPos2f(windowWidth/2-160, windowHeight/2);
        int len, i;
        char* string = "Player lost, press space to restart";
        len = (int)strlen(string);
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
        }
    }
    else if (gameStatus == 2) {
        glColor3f(0, 1, 0);
        glRasterPos2f(windowWidth / 2-160, windowHeight / 2);
        int len, i;
        char* string = "Player won, press space to restart";
        len = (int)strlen(string);
        for (i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
        }
    }
    glFlush();
    
}

void drawPlayerLives() {
    glPushMatrix();
    glTranslatef(0, windowHeight-15, 0);
    glColor3f(0, 0, 0);
    glRasterPos2f(0, 0);
    int len, i;
    string s = "Player lives: "+to_string(playerHealth);
    len = s.length();
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s[i]);
    }
    glPopMatrix();
}

void enemyFire(int value) {
    enemyBulletsX.push(enemyPos);
    enemyBulletsY.push(windowHeight - 125);
    glutTimerFunc(5 * 1000, enemyFire, 0);
}

void generatePowerUp(int value) {
    if (powerUpX == -1) {
        powerUpX = rand() % (windowWidth - 20);
        powerUpY = rand() % (windowHeight - 170);
        powerUpY += 20;

        if (intersectsPlayer(powerUpX, powerUpY,10)) {
            powerUpX = -1;
            powerUpY = -1;
        }
    }
    glutPostRedisplay();

    glutTimerFunc(1 * 1000, generatePowerUp, 0);
}
void Loop() {
    if (gameStatus == 0) {
        auto now = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(
            now.time_since_epoch()).count() - std::chrono::duration_cast<std::chrono::seconds>(
                powerUpTime.time_since_epoch()).count() > 3) {
            playerHasPowerUp = false;
        }
        if (enemyPos + enemyDir - 150 <= 0 || enemyPos + enemyDir + 150 >= windowWidth) {
            enemyDir *= -1;
        }
        enemyPos += enemyDir;
        int n = bulletsX.size();
        if (intersectsPlayer(powerUpX, powerUpY, 10)) {
            powerUpX = -1;
            powerUpY = -1;
        }
        while (n--) {
            double x = (double)bulletsX.front();
            bulletsX.pop();
            double y = (double)bulletsY.front();
            bulletsY.pop();
            y += 5;
            if (intersectsEnemy(x,y,5)) {
                enemyHealth -= 10;
            }
            else {
                if (y+5 < windowHeight -20) {
                    bulletsX.push(x);
                    bulletsY.push(y);
                }
            }

        }
        n = enemyBulletsX.size();
        while (n--) {
            double x = (double)enemyBulletsX.front();
            enemyBulletsX.pop();
            double y = (double)enemyBulletsY.front();
            enemyBulletsY.pop();
            y -= 0.5;
            if (intersectsPlayer(x,y,5)) {
                if(!playerHasPowerUp)
                    playerHealth--;
            }
            else {
                if (y-15 > 40) {
                    enemyBulletsX.push(x);
                    enemyBulletsY.push(y);
                }
            }

        }
        if (playerHealth <= 0) {
            gameStatus = 1;
        }
        else if (enemyHealth <= 0) {
            gameStatus = 2;
        }
    }
    glutPostRedisplay();
}

float perpDistance(int x, int y, float a, float b,float c)
{
    return fabs((a * x + b * y + c)) / (sqrt(a * a + b * b));
}
float prepDistance(int x, int y, int a, int b)
{
    return (sqrt((x-a)*(x-a) + (y-b)*(y-b)));
}
