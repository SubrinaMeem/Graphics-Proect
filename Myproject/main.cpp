#include <windows.h>
#include <GL/glut.h>
#include <math.h>

// Camera variables
float camX = 45, camY = 30, camZ = 100;
float pitch = 5, yaw = -90;
float moveSpeed = 2.0f, rotSpeed = 2.0f;

// Animation variables
float bladeAngle1 = 0.0f;
float bladeAngle2 = 0.0f;
float bladeAngle3 = 0.0f;
bool animateBlades1 = true;
bool animateBlades2 = true;
bool animateBlades3 = true;
float rotationSpeed = 2.0f;

// Third windmill control variables
float windmill3RotX = 0.0f;
float windmill3RotY = 0.0f;
float windmill3RotZ = 0.0f;
float windmill3Scale = 1.0f;
float windmill3TransX = 72.0f;
float windmill3TransY = 0.0f;
float windmill3TransZ = -20.0f;

// Cloud animation
float cloudOffset1 = 0.0f;
float cloudOffset2 = 0.0f;
float cloudOffset3 = 0.0f;
float cloudSpeed = 0.2f;
const float CLOUD_LIMIT = 88.0f;

// Day/Night mode
bool isDay = true; // Start in day mode

void updateCamera() {
    float radPitch = pitch * 3.1416 / 180.0;
    float radYaw = yaw * 3.1416 / 180.0;
    float dirX = cos(radPitch) * cos(radYaw);
    float dirY = sin(radPitch);
    float dirZ = cos(radPitch) * sin(radYaw);

    gluLookAt(camX, camY, camZ,
              camX + dirX, camY + dirY, camZ + dirZ,
              0, 1, 0);
}

void drawCircle(float cx, float cy, float cz, float r, int segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * 3.1416f * i / segments;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex3f(x + cx, y + cy, cz);
    }
    glEnd();
}

void updateClouds(int value) {
    cloudOffset1 += cloudSpeed;
    cloudOffset2 += cloudSpeed * 0.8f;
    cloudOffset3 += cloudSpeed * 1.2f;

    if (cloudOffset1 > CLOUD_LIMIT) cloudOffset1 = 0;
    if (cloudOffset2 > CLOUD_LIMIT) cloudOffset2 = 0;
    if (cloudOffset3 > CLOUD_LIMIT) cloudOffset3 = 0;

    glutPostRedisplay();
    glutTimerFunc(30, updateClouds, 0);
}

void drawCloud(float baseX, float y, float offset) {
    float frontZ = -10;
    float backZ = -12;
    float x = baseX + offset;

    if (x > CLOUD_LIMIT) {
        x = 0 + (x - CLOUD_LIMIT);
    }

    float floatOffset = sin(offset) * 0.5f;

    glColor3f(1, 1, 1);
    for (int i = 0; i < 3; i++) {
        float cx = x + i * 4;
        float cy = (i == 1) ? y + 2 + floatOffset : y + floatOffset;
        drawCircle(cx, cy, frontZ, 5, 20);
        drawCircle(cx, cy, backZ, 5, 20);

        int segments = 20;
        float angleStep = 2.0f * M_PI / segments;
        for (int j = 0; j < segments; j++) {
            float a1 = j * angleStep;
            float a2 = (j + 1) * angleStep;
            float x1 = cx + cos(a1) * 5;
            float y1 = cy + sin(a1) * 5;
            float x2 = cx + cos(a2) * 5;
            float y2 = cy + sin(a2) * 5;

            glBegin(GL_QUADS);
                glVertex3f(x1, y1, frontZ);
                glVertex3f(x1, y1, backZ);
                glVertex3f(x2, y2, backZ);
                glVertex3f(x2, y2, frontZ);
            glEnd();
        }
    }
}

void drawSun() {
    float cx = 84;
    float cy = 80;
    float frontZ = -13;
    float backZ = -16;

    if(isDay) {
        glColor3f(1, 1, 0); // Yellow sun
    } else {
        glColor3f(0.9, 0.9, 0.95); // White moon
        // Moon craters
        glColor3f(0.7, 0.7, 0.75);
        drawCircle(cx-3, cy+2, frontZ+0.1, 1.5, 10);
        drawCircle(cx+2, cy-1, frontZ+0.1, 2.0, 10);
        glColor3f(0.9, 0.9, 0.95); // Reset to moon color
    }

    drawCircle(cx, cy, frontZ, 7, 30);
    drawCircle(cx, cy, backZ, 7, 30);

    int segments = 30;
    float angleStep = 2.0f * M_PI / segments;
    for (int i = 0; i < segments; i++) {
        float a1 = i * angleStep;
        float a2 = (i + 1) * angleStep;
        float x1 = cx + cos(a1) * 7;
        float y1 = cy + sin(a1) * 7;
        float x2 = cx + cos(a2) * 7;
        float y2 = cy + sin(a2) * 7;

        glBegin(GL_QUADS);
            glVertex3f(x1, y1, frontZ);
            glVertex3f(x1, y1, backZ);
            glVertex3f(x2, y2, backZ);
            glVertex3f(x2, y2, frontZ);
        glEnd();
    }
}

void drawGround() {
    glColor3f(0.4, 0.7, 0.4);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 10);
        glVertex3f(100, 0, 0);
        glVertex3f(100, 0, -50);
        glVertex3f(0, 0, -50);
    glEnd();
}

void drawGrass(float x, float z) {
    float y = 0.0f;
    z -= -2.5f;

    glColor3f(0.0, 0.6, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(x, y, z);
        glVertex3f(x - 0.5, y + 2.5, z);
        glVertex3f(x - 1.0, y, z);

        glVertex3f(x, y, z);
        glVertex3f(x + 0.5, y + 2.5, z);
        glVertex3f(x + 1.0, y, z);

        glVertex3f(x - 0.5, y, z);
        glVertex3f(x, y + 2.0, z);
        glVertex3f(x + 0.5, y, z);
    glEnd();
}

void drawMountain() {
    glColor3f(0.5, 0.5, 0.2);
    glBegin(GL_TRIANGLES);
        glVertex3f(10, 0, -15);
        glVertex3f(25, 25, -15);
        glVertex3f(40, 0, -15);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex3f(10, 0, -20);
        glVertex3f(25, 25, -20);
        glVertex3f(40, 0, -20);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(10, 0, -15);
        glVertex3f(10, 0, -20);
        glVertex3f(25, 25, -20);
        glVertex3f(25, 25, -15);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(40, 0, -15);
        glVertex3f(40, 0, -20);
        glVertex3f(25, 25, -20);
        glVertex3f(25, 25, -15);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex3f(30, 0, -20);
        glVertex3f(50, 30, -20);
        glVertex3f(70, 0, -20);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex3f(30, 0, -25);
        glVertex3f(50, 30, -25);
        glVertex3f(70, 0, -25);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(30, 0, -20);
        glVertex3f(30, 0, -25);
        glVertex3f(50, 30, -25);
        glVertex3f(50, 30, -20);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(70, 0, -20);
        glVertex3f(70, 0, -25);
        glVertex3f(50, 30, -25);
        glVertex3f(50, 30, -20);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex3f(60, 0, -25);
        glVertex3f(80, 25, -25);
        glVertex3f(100, 0, -25);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex3f(60, 0, -30);
        glVertex3f(80, 25, -30);
        glVertex3f(100, 0, -30);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(60, 0, -25);
        glVertex3f(60, 0, -30);
        glVertex3f(80, 25, -30);
        glVertex3f(80, 25, -25);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(100, 0, -25);
        glVertex3f(100, 0, -30);
        glVertex3f(80, 25, -30);
        glVertex3f(80, 25, -25);
    glEnd();
}

void drawHouse() {
    float frontZ = -10;
    float backZ = -14;

    glColor3f(0.8, 0.4, 0.1);
    glBegin(GL_QUADS);
        glVertex3f(10, 0, frontZ);
        glVertex3f(20, 0, frontZ);
        glVertex3f(20, 10, frontZ);
        glVertex3f(10, 10, frontZ);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(10, 0, backZ);
        glVertex3f(20, 0, backZ);
        glVertex3f(20, 10, backZ);
        glVertex3f(10, 10, backZ);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(10, 0, frontZ);
        glVertex3f(10, 0, backZ);
        glVertex3f(10, 10, backZ);
        glVertex3f(10, 10, frontZ);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(20, 0, frontZ);
        glVertex3f(20, 0, backZ);
        glVertex3f(20, 10, backZ);
        glVertex3f(20, 10, frontZ);
    glEnd();

    glColor3f(0.7, 0.1, 0.1);
    glBegin(GL_TRIANGLES);
        glVertex3f(9, 10, frontZ);
        glVertex3f(21, 10, frontZ);
        glVertex3f(15, 17, frontZ);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex3f(9, 10, backZ);
        glVertex3f(21, 10, backZ);
        glVertex3f(15, 17, backZ);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(9, 10, frontZ);
        glVertex3f(9, 10, backZ);
        glVertex3f(15, 17, backZ);
        glVertex3f(15, 17, frontZ);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(21, 10, frontZ);
        glVertex3f(21, 10, backZ);
        glVertex3f(15, 17, backZ);
        glVertex3f(15, 17, frontZ);
    glEnd();

    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_QUADS);
        glVertex3f(13, 0, frontZ + 0.01f);
        glVertex3f(17, 0, frontZ + 0.01f);
        glVertex3f(17, 7, frontZ + 0.01f);
        glVertex3f(13, 7, frontZ + 0.01f);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
        glVertex3f(15, 0, frontZ + 0.02f);
        glVertex3f(15, 7, frontZ + 0.02f);
    glEnd();

    glColor3f(1, 1, 0);
    drawCircle(15, 3.5, frontZ + 0.03f, 0.5, 20);
}

void drawFence(float startX, float endX) {
    for (float i = startX; i <= endX; i += 1.5) {
        glColor3f(0.6, 0.3, 0);
        glBegin(GL_QUADS);
            glVertex3f(i, 0, -5);
            glVertex3f(i + 0.5, 0, -5);
            glVertex3f(i + 0.5, 3, -5);
            glVertex3f(i, 3, -5);
        glEnd();
    }
}

void drawWindmillBase(float x, float z, float s) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glScalef(s, s, s);

    glColor3f(0.6, 0.6, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(-1.5, 0, -1);
        glVertex3f(1.5, 0, -1);
        glVertex3f(1.5, 12, -1);
        glVertex3f(-1.5, 12, -1);

        glVertex3f(-1.5, 0, 1);
        glVertex3f(1.5, 0, 1);
        glVertex3f(1.5, 12, 1);
        glVertex3f(-1.5, 12, 1);

        glVertex3f(-1.5, 0, -1);
        glVertex3f(-1.5, 0, 1);
        glVertex3f(-1.5, 12, 1);
        glVertex3f(-1.5, 12, -1);

        glVertex3f(1.5, 0, -1);
        glVertex3f(1.5, 0, 1);
        glVertex3f(1.5, 12, 1);
        glVertex3f(1.5, 12, -1);
    glEnd();

    glPopMatrix();
}

void drawWindmillBlades(float x, float z, float s, float rotationAngle,
                       float rotX = 0, float rotY = 0, float rotZ = 0,
                       float scale = 1.0f,
                       float transX = 0, float transY = 0, float transZ = 0) {
    glPushMatrix();
    glTranslatef(x, 12.5 * s, z);
    glScalef(s, s, s);

    // Apply blade-specific transformations
    glTranslatef(transX, transY, transZ);
    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);
    glRotatef(rotZ, 0, 0, 1);
    glScalef(scale, scale, scale);

    // Rotate all blades together
    glRotatef(rotationAngle, 0, 0, 1);

    glColor3f(0.2, 0.2, 0.2);
    float angles[] = {45, 135, 225, 315};
    float bladeWidth = 1.5;
    float bladeGap = 2.0;
    float bladeOffset = 1.0;

    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glRotatef(angles[i], 0, 0, 1);

        glBegin(GL_TRIANGLES);
            glVertex3f(0, 0, -1 + bladeOffset);
            glVertex3f(bladeWidth, 6, -1 + bladeOffset);
            glVertex3f(-bladeWidth, 6, -1 + bladeOffset);
        glEnd();

        glBegin(GL_TRIANGLES);
            glVertex3f(0, 0, -1 + bladeGap + bladeOffset);
            glVertex3f(bladeWidth, 6, -1 + bladeGap + bladeOffset);
            glVertex3f(-bladeWidth, 6, -1 + bladeGap + bladeOffset);
        glEnd();

        glBegin(GL_QUADS);
            glVertex3f(-bladeWidth, 6, -1 + bladeOffset);
            glVertex3f(-bladeWidth, 6, -1 + bladeGap + bladeOffset);
            glVertex3f(0, 0, -1 + bladeGap + bladeOffset);
            glVertex3f(0, 0, -1 + bladeOffset);
        glEnd();

        glBegin(GL_QUADS);
            glVertex3f(bladeWidth, 6, -1 + bladeOffset);
            glVertex3f(bladeWidth, 6, -1 + bladeGap + bladeOffset);
            glVertex3f(0, 0, -1 + bladeGap + bladeOffset);
            glVertex3f(0, 0, -1 + bladeOffset);
        glEnd();

        glPopMatrix();
    }

    glColor3f(0.3, 0.3, 0.3);
    drawCircle(0, 0, 0.01, 1.2, 20);

    glPopMatrix();
}

void drawWindmill(float x, float z, float s, float rotationAngle = 0.0f) {
    drawWindmillBase(x, z, s);
    drawWindmillBlades(x, z, s, rotationAngle);
}

void drawControllableWindmill() {
    glPushMatrix();
    // Apply translation and scaling to entire windmill
    glTranslatef(windmill3TransX, windmill3TransY, windmill3TransZ);
    glScalef(windmill3Scale, windmill3Scale, windmill3Scale);

    // Draw tower
    drawWindmillBase(0, 0, 0.6f);

    // Apply blade-specific transformations
    glPushMatrix();
    glTranslatef(0, 12.5 * 0.6f, 0); // Position at top of tower
    glScalef(0.6f, 0.6f, 0.6f);      // Base windmill scale
    glRotatef(windmill3RotX, 1, 0, 0);
    glRotatef(windmill3RotY, 0, 1, 0);
    glRotatef(windmill3RotZ, 0, 0, 1);
    glRotatef(bladeAngle3, 0, 0, 1);  // Animation rotation

    // Draw blades with transformations
    glColor3f(0.2, 0.2, 0.2);
    float angles[] = {45, 135, 225, 315};
    float bladeWidth = 1.5;
    float bladeGap = 2.0;
    float bladeOffset = 1.0;

    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glRotatef(angles[i], 0, 0, 1);

        // Front face
        glBegin(GL_TRIANGLES);
            glVertex3f(0, 0, -1 + bladeOffset);
            glVertex3f(bladeWidth, 6, -1 + bladeOffset);
            glVertex3f(-bladeWidth, 6, -1 + bladeOffset);
        glEnd();

        // Back face
        glBegin(GL_TRIANGLES);
            glVertex3f(0, 0, -1 + bladeGap + bladeOffset);
            glVertex3f(bladeWidth, 6, -1 + bladeGap + bladeOffset);
            glVertex3f(-bladeWidth, 6, -1 + bladeGap + bladeOffset);
        glEnd();

        // Side faces
        glBegin(GL_QUADS);
            glVertex3f(-bladeWidth, 6, -1 + bladeOffset);
            glVertex3f(-bladeWidth, 6, -1 + bladeGap + bladeOffset);
            glVertex3f(0, 0, -1 + bladeGap + bladeOffset);
            glVertex3f(0, 0, -1 + bladeOffset);
        glEnd();

        glBegin(GL_QUADS);
            glVertex3f(bladeWidth, 6, -1 + bladeOffset);
            glVertex3f(bladeWidth, 6, -1 + bladeGap + bladeOffset);
            glVertex3f(0, 0, -1 + bladeGap + bladeOffset);
            glVertex3f(0, 0, -1 + bladeOffset);
        glEnd();

        glPopMatrix();
    }

    // Hub center
    glColor3f(0.3, 0.3, 0.3);
    drawCircle(0, 0, 0.01, 1.2, 20);

    glPopMatrix();
    glPopMatrix();
}
// User-defined transformation functions
void rotateBladeX(float angle) {
    windmill3RotX += angle;
}

void rotateBladeY(float angle) {
    windmill3RotY += angle;
}

void rotateBladeZ(float angle) {
    windmill3RotZ += angle;
}

void translateWindmill(float x, float y, float z) {
    windmill3TransX += x;
    windmill3TransY += y;
    windmill3TransZ += z;
}

void scaleWindmill(float factor) {
    windmill3Scale *= factor;
}


void animate(int value) {
    if (animateBlades1) {
        bladeAngle1 += rotationSpeed;
        if (bladeAngle1 > 360) bladeAngle1 -= 360;
    }

    if (animateBlades2) {
        bladeAngle2 += rotationSpeed * 0.8f;
        if (bladeAngle2 > 360) bladeAngle2 -= 360;
    }

    if (animateBlades3) {
        bladeAngle3 += rotationSpeed * 1.2f;
        if (bladeAngle3 > 360) bladeAngle3 -= 360;
    }

    glutPostRedisplay();
    glutTimerFunc(16, animate, 0);
}

void display() {
    // Set sky color based on day/night
    if(isDay) {
        glClearColor(0.5, 0.8, 1.0, 1.0); // Day blue
    } else {
        glClearColor(0.0, 0.0, 0.2, 1.0); // Night dark blue
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    updateCamera();

    drawSun();
    drawCloud(0.0f, 80, cloudOffset1);
    drawCloud(15.0f, 90, cloudOffset2);
    drawCloud(30.0f, 70, cloudOffset3);
    drawGround();
    drawMountain();

    for (float x = 5; x <= 90; x += 5)
        drawGrass(x, -3);

    drawHouse();
    drawFence(0, 100);
    drawWindmill(30, -5, 1.2, bladeAngle1);
    drawWindmill(50, -15, 0.9, bladeAngle2);
    drawControllableWindmill();

    glutSwapBuffers();
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)w / h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    float radYaw = yaw * 3.1416 / 180.0;
    float radPitch = pitch * 3.1416 / 180.0;
    float frontX = cos(radPitch) * cos(radYaw);
    float frontY = sin(radPitch);
    float frontZ = cos(radPitch) * sin(radYaw);
    float rightX = sin(radYaw);
    float rightZ = -cos(radYaw);

    switch (key) {
        // Camera controls
        case 'w': camX += frontX * moveSpeed; camY += frontY * moveSpeed; camZ += frontZ * moveSpeed; break;
        case 's': camX -= frontX * moveSpeed; camY -= frontY * moveSpeed; camZ -= frontZ * moveSpeed; break;
        case 'a': camX -= rightX * moveSpeed; camZ -= rightZ * moveSpeed; break;
        case 'd': camX += rightX * moveSpeed; camZ += rightZ * moveSpeed; break;
        case 'q': camY += moveSpeed; break;
        case 'e': camY -= moveSpeed; break;

        // Animation toggles
        case '1': animateBlades1 = !animateBlades1; break;
        case '2': animateBlades2 = !animateBlades2; break;
        case '3': animateBlades3 = !animateBlades3; break;

        // Day/Night toggle
        case '0': isDay = !isDay; break;

        // Windmill controls
        case 'i': rotateBladeX(5); break;
        case 'k': rotateBladeX(-5); break;
        case 'j': rotateBladeY(5); break;
        case 'l': rotateBladeY(-5); break;
        case 'u': rotateBladeZ(5); break;
        case 'o': rotateBladeZ(-5); break;
        case 't': translateWindmill(0, 0.1, 0); break;
        case 'g': translateWindmill(0, -0.1, 0); break;
        case 'f': translateWindmill(-0.1, 0, 0); break;
        case 'h': translateWindmill(0.1, 0, 0); break;
        case 'r': translateWindmill(0, 0, 0.1); break;
        case 'y': translateWindmill(0, 0, -0.1); break;
        case 'n': scaleWindmill(1.1); break;
        case 'm': scaleWindmill(0.9); break;
    }
    glutPostRedisplay();
}


void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT: yaw -= rotSpeed; break;
        case GLUT_KEY_RIGHT: yaw += rotSpeed; break;
        case GLUT_KEY_UP: pitch += rotSpeed; break;
        case GLUT_KEY_DOWN: pitch -= rotSpeed; break;
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.5, 0.8, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutCreateWindow("Windmills");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, animate, 0);
    glutTimerFunc(0, updateClouds, 0);

    glutMainLoop();
    return 0;
}
