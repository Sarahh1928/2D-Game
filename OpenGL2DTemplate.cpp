#include <stdio.h>
#include <string.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <mmsystem.h>
#include <glut.h>
#include <tchar.h>
#include <Wininet.h>

bool playGoalSoundFlag = false;
bool playLostSoundFlag = false;
bool ended = false;
float playerX = 0.0f;
float playerY = 0.0f;
float playerSize = 20.0f;
float playerSpeed = 5.0f;
float playerVelocityX = 0.0f;
float playerVelocityY = 0.0f;
GLfloat scaleFactor = 27.0;
bool ScalingUp = false;
bool gameStarted = false;
const int numCollectables = 10;
float collectableX[numCollectables];
float collectableY[numCollectables];
bool  collectableActive[numCollectables];
float collectableSize = 30.0f;
float obstacleWidth = 40.0f;
float obstacleHeight = 20.0f;
bool gameOver = false;
int obstacleCount = 8; // Number of obstacles
const int numPowerUps = 3;
float powerupPositions[numPowerUps][2];
bool powerupCollected[numPowerUps] = { false };
float powerUpTwoPositions[numPowerUps][2];
bool powerupTwoCollected[numPowerUps] = { false };
bool multiplier = false;
int multiplierTimeRemaining = 0;
float collectableRotation = 0.0f;
float rotationSpeed = 2.0;
struct Obstacle {
    float x;
    float y;
};
Obstacle obstacles[8];
float rotationAngle = 45.0f;
int lives = 5;
int gameTime = 60;     // Initial game time in seconds (1 minute)
int score = 0;
int lastTime = 0;
bool timeFreeze = false;
int remainingFrozenTime = 0;
const int screenWidth = 700;
const int screenHeight = 600;
const int availScreenHeight = 550;
GLfloat leftMargin = 40.0;
GLfloat bottomMargin = 40.0;
float sceneLeft = -0.85 * scaleFactor + leftMargin;
float sceneRight = 1.0 * scaleFactor + leftMargin;
float sceneTop = 1.0 * scaleFactor + bottomMargin;
float sceneBottom = -0.85 * scaleFactor + bottomMargin;
bool movingUp = false;
bool movingDown = false;
bool movingRight = false;
bool movingLeft = false;
float animationTime = 0.0;

void playWinSound(int val) {
    const wchar_t* soundFilePath = L"win.wav";  // Update with your sound file path

    PlaySound(soundFilePath, NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
}
void playLostSound(int val) {
    const wchar_t* soundFilePath = L"lose.wav";  // Update with your sound file path

    PlaySound(soundFilePath, NULL, SND_ASYNC | SND_FILENAME);
}
void PlayBackgroundSound(int val) {
    const wchar_t* soundFilePath = L"background.wav";  // Update with your sound file path

    PlaySound(soundFilePath, NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
    
}
void PlayCollectableSound() {
    const wchar_t* soundFilePath = L"collectable.wav";  // Update with your sound file path

    PlaySound(soundFilePath, NULL, SND_ASYNC | SND_FILENAME);
    
    glutTimerFunc(1000, PlayBackgroundSound, 0);
}

void playGoalSound(int val) {
    printf("Hello");
    const wchar_t* soundFilePath = L"goal.wav";  // Update with your sound file path

    PlaySound(soundFilePath, NULL, SND_ASYNC | SND_FILENAME);
    if (val == 0) {
        glutTimerFunc(1000, playWinSound, 0);
    }
}

void drawGoal() {
    glPushMatrix();

    // Define the margin values
    GLfloat leftMargin = 40.0;
    GLfloat bottomMargin = 40.0; // Adjust this to make the scene bigger

    // Background (blue)
    glColor3f(0.0, 0.0, 1.0);  // Dark Blue
    glBegin(GL_POLYGON);
    glVertex2f(-0.85 * scaleFactor + leftMargin, -0.85 * scaleFactor + bottomMargin);
    glVertex2f(1.0 * scaleFactor + leftMargin, -0.85 * scaleFactor + bottomMargin);
    glVertex2f(1.0 * scaleFactor + leftMargin, 1.0 * scaleFactor + bottomMargin);
    glVertex2f(-0.85 * scaleFactor + leftMargin, 1.0 * scaleFactor + bottomMargin);
    glEnd();

    // House walls (red)
    glColor3f(1.0, 0.0, 0.0);  // Dark Red
    glBegin(GL_QUADS);
    glVertex2f(-0.4 * scaleFactor + leftMargin, -0.4 * scaleFactor + bottomMargin);
    glVertex2f(0.4 * scaleFactor + leftMargin, -0.4 * scaleFactor + bottomMargin);
    glVertex2f(0.4 * scaleFactor + leftMargin, 0.4 * scaleFactor + bottomMargin);
    glVertex2f(-0.4 * scaleFactor + leftMargin, 0.4 * scaleFactor + bottomMargin);
    glEnd();

    // Roof (green)
    glColor3f(0.7, 0.7, 0.7);  // Dark Green
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5 * scaleFactor + leftMargin, 0.4 * scaleFactor + bottomMargin);
    glVertex2f(0.5 * scaleFactor + leftMargin, 0.4 * scaleFactor + bottomMargin);
    glVertex2f(0.0 * scaleFactor + leftMargin, 0.7 * scaleFactor + bottomMargin);
    glEnd();

    // Door (brown)
    glColor3f(0.4, 0.2, 0.0);  // Brown
    glBegin(GL_QUADS);
    glVertex2f(-0.15 * scaleFactor + leftMargin, -0.4 * scaleFactor + bottomMargin);
    glVertex2f(0.15 * scaleFactor + leftMargin, -0.4 * scaleFactor + bottomMargin);
    glVertex2f(0.15 * scaleFactor + leftMargin, 0.1 * scaleFactor + bottomMargin);
    glVertex2f(-0.15 * scaleFactor + leftMargin, 0.1 * scaleFactor + bottomMargin);
    glEnd();

    // Windows (cyan)
    glColor3f(1.0, 1.6, 1.0);  // Light Blue
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex2f(-0.2 * scaleFactor + leftMargin, 0.2 * scaleFactor + bottomMargin);
    glVertex2f(0.2 * scaleFactor + leftMargin, 0.2 * scaleFactor + bottomMargin);
    glVertex2f(-0.2 * scaleFactor + leftMargin, -0.1 * scaleFactor + bottomMargin);
    glVertex2f(0.2 * scaleFactor + leftMargin, -0.1 * scaleFactor + bottomMargin);
    glEnd();

    // Fence (yellow)
    glColor3f(1.0, 1.0, 0.0);  // Yellow
    glBegin(GL_LINES);
    glVertex2f(-0.8 * scaleFactor + leftMargin, -0.4 * scaleFactor + bottomMargin);
    glVertex2f(0.8 * scaleFactor + leftMargin, -0.4 * scaleFactor + bottomMargin);
    glVertex2f(-0.8 * scaleFactor + leftMargin, -0.5 * scaleFactor + bottomMargin);
    glVertex2f(0.8 * scaleFactor + leftMargin, -0.5 * scaleFactor + bottomMargin);
    glEnd();

    glPopMatrix();
}

void drawTimesTwoPowerup(float x, float y) {
    float timesTwoWidth = 35.0f;
    float timesTwoHeight = 35.0f;

    glPushMatrix();

    glTranslatef(x, y, 0.0f); // Set pivot point to (0, 0)
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-x, -y, 0.0f);

    // Draw a blue square
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(x - timesTwoWidth / 2, y - timesTwoHeight / 2);
    glVertex2f(x + timesTwoWidth / 2, y - timesTwoHeight / 2);
    glVertex2f(x + timesTwoWidth / 2, y + timesTwoHeight / 2);
    glVertex2f(x - timesTwoWidth / 2, y + timesTwoHeight / 2);
    glEnd();

    // Draw a white circle in the middle
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float radian = i * (3.14159265359 / 180); // Convert degrees to radians
        float cx = x; // X coordinate of the center
        float cy = y; // Y coordinate of the center
        float radius = timesTwoWidth / 2.5; // Adjust the radius as needed
        float x_pos = cx + radius * cosf(radian);
        float y_pos = cy + radius * sinf(radian);
        glVertex2f(x_pos, y_pos);
    }
    glEnd();

    // Draw the first arrow using GL_LINES
    glColor3f(1.0f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex2f(x - timesTwoWidth /3.0, y - timesTwoHeight / 5 -3.0f);
    glVertex2f(x, y + timesTwoHeight / 18 -3.0f);
    glVertex2f(x, y + timesTwoHeight / 18 - 3.0f);
    glVertex2f(x + timesTwoWidth / 3.0, y - timesTwoHeight / 5 - 3.0f);
    glEnd();

    // Draw the second arrow with a small height difference
    glColor3f(1.0f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex2f(x - timesTwoWidth / 2.5, y - timesTwoHeight / 5 + 5.0f); // Adjust the height difference
    glVertex2f(x, y + timesTwoHeight / 18 + 5.0f); // Adjust the height difference
    glVertex2f(x, y + timesTwoHeight / 18 + 5.0f); // Adjust the height difference
    glVertex2f(x + timesTwoWidth / 2.5, y - timesTwoHeight / 5 + 5.0f); // Adjust the height difference
    glEnd();
    

    // Draw a border around the square using GL_LINE_STRIP
    glColor3f(0.0f, 0.0f, 0.0f);  // Set color to black for the border
    glLineWidth(1.0f);  // Set the line width to 1.0f
    float borderOffset = 0.0;
    // Draw the outer border
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - timesTwoWidth / 2 - borderOffset, y - timesTwoHeight / 2 - borderOffset);
    glVertex2f(x + timesTwoWidth / 2 + borderOffset, y - timesTwoHeight / 2 - borderOffset);
    glVertex2f(x + timesTwoWidth / 2 + borderOffset, y + timesTwoHeight / 2 + borderOffset);
    glVertex2f(x - timesTwoWidth / 2 - borderOffset, y + timesTwoHeight / 2 + borderOffset);
    glVertex2f(x - timesTwoWidth / 2 - borderOffset, y - timesTwoHeight / 2 - borderOffset);  // Close the loop
    glEnd();

    // Draw the inner border with a gap of 2.0f
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - timesTwoWidth / 2 - borderOffset - 3.0, y - timesTwoHeight / 2 - borderOffset - 3.0);
    glVertex2f(x + timesTwoWidth / 2 + borderOffset + 3.0, y - timesTwoHeight / 2 - borderOffset - 3.0);
    glVertex2f(x + timesTwoWidth / 2 + borderOffset + 3.0, y + timesTwoHeight / 2 + borderOffset + 3.0);
    glVertex2f(x - timesTwoWidth / 2 - borderOffset - 3.0, y + timesTwoHeight / 2 + borderOffset + 3.0);
    glVertex2f(x - timesTwoWidth / 2 - borderOffset - 3.0, y - timesTwoHeight / 2 - borderOffset - 3.0);  // Close the loop
    glEnd();
    glPopMatrix();
    rotationAngle += 0.2;
    // Draw "x2" text within the circle
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x - timesTwoWidth / 4, y - timesTwoHeight / 5); // Adjust position for text
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '2');
}

void drawPowerups() {
    for (int i = 0; i < numPowerUps; i++) {
        if (!powerupTwoCollected[i]) {
            drawTimesTwoPowerup(powerUpTwoPositions[i][0], powerUpTwoPositions[i][1]);
        }
    }
}

void drawTimeFreezePowerup() {
    for (int i = 0; i < numPowerUps; i++) {
        if (!powerupCollected[i]) {
            glPushMatrix();
            glColor3f(0.0, 1.0, 0.0);
            glTranslatef(powerupPositions[i][0], powerupPositions[i][1], 0.0);
            glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
            // Draw a smaller square in blue
            glColor3f(0.0f, 0.0f, 1.0f); // Blue color
            glBegin(GL_QUADS);
            glVertex2f(-15.0f, -15.0f);
            glVertex2f(15.0f, -15.0f);
            glVertex2f(15.0f, 15.0f);
            glVertex2f(-15.0f, 15.0f);
            glEnd();

            // Draw a smaller circle (light blue) within the square
            glColor3f(0.6f, 0.6f, 1.0f); // Light blue color
            glBegin(GL_POLYGON);
            for (int i = 0; i < 360; i++) {
                float angle = i * 3.14 / 180.0;
                float x = 10.0f * cos(angle); // Smaller circle radius = 10.0
                float y = 10.0f * sin(angle);
                glVertex2f(x, y);
            }
            glEnd();

            // Draw the clock hands (hour and minute hands) within the circle
            glColor3f(1.0f, 0.0f, 0.0f); // Red color

            // Hour hand
            glPushMatrix();
            glRotatef(-60.0f, 0, 0, 1); // Rotate by 60 degrees for 1:20
            glBegin(GL_LINES);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, 6.0f); // Smaller hour hand length = 6.0
            glEnd();
            glPopMatrix();

            // Minute hand
            glPushMatrix();
            glRotatef(0, 0, 0, 1); // Rotate by 120 degrees for 1:20
            glBegin(GL_LINES);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, 8.0f); // Smaller minute hand length = 8.0
            glEnd();
            glPopMatrix();

            // Draw a smaller point at the center (representing the clock center)
            glColor3f(0.0f, 0.0f, 0.0f); // Black color
            glPointSize(2.0f); // Smaller point size
            glBegin(GL_POINTS);
            glVertex2f(0.0f, 0.0f);
            glEnd();
            glPopMatrix();
            rotationAngle += 0.2;
        }
    }

}

float randomFloat(float min, float max) {
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}
void generateRandomTimeFreezePowerups() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    for (int i = 0; i < 3; i++) {
        if (!powerupCollected[i]) {
            bool overlap = true;
            float powerupX, powerupY;

            while (overlap) {
                powerupX = randomFloat(80, screenWidth - 80);
                powerupY = randomFloat(80, availScreenHeight - 80);

                // Check that the generated position is not too close to the player
                float dx = powerupX - playerX;
                float dy = powerupY - playerY;
                float distanceToPlayer = sqrt(dx * dx + dy * dy);

                overlap = false; // Assume no overlap

                // Check for overlap with collectables
                for (int j = 0; j < numCollectables; j++) {
                    float dx = powerupX - collectableX[j];
                    float dy = powerupY - collectableY[j];
                    float distance = sqrt(dx * dx + dy * dy);

                    if (distance < (60 + collectableSize) / 2) {
                        overlap = true;
                        break; // Overlapping with a collectable, regenerate position
                    }
                }

                // Check for overlap with obstacles
                for (int j = 0; j < obstacleCount; j++) {
                    float dx = powerupX - obstacles[j].x;
                    float dy = powerupY - obstacles[j].y;
                    float distance = sqrt(dx * dx + dy * dy);

                    if (distance < (60 + obstacleWidth) / 2) {
                        overlap = true;
                        break; // Overlapping with an obstacle, regenerate position
                    }
                }

                // Check for overlap with powerUpTwoPositions
                for (int j = 0; j < numPowerUps; j++) {
                    float dx = powerupX - powerUpTwoPositions[j][0];
                    float dy = powerupY - powerUpTwoPositions[j][1];
                    float distance = sqrt(dx * dx + dy * dy);

                    if (distance < (60 + 60) / 2) {
                        overlap = true;
                        break; // Overlapping with a powerUpTwoPosition, regenerate position
                    }
                }

                // Check for overlap with other powerupPositions
                for (int j = 0; j < numPowerUps; j++) {
                    float dx = powerupX - powerupPositions[j][0];
                    float dy = powerupY - powerupPositions[j][1];
                    float distance = sqrt(dx * dx + dy * dy);

                    if (distance < (60 + 60) / 2) {
                        overlap = true;
                        break; // Overlapping with another powerup, regenerate position
                    }
                }

                // Check that the generated position is not too close to the player
                if (distanceToPlayer < (60 + playerSize) / 2) {
                    overlap = true;
                }
            }

            // Assign the position once no overlap is found
            powerupPositions[i][0] = powerupX;
            powerupPositions[i][1] = powerupY;
        }
    }
}

void generateRandomObstacles() {
    for (int i = 0; i < obstacleCount; i++) {
        bool overlap = true;
        while (overlap) {
            // Generate random coordinates within the specified boundaries
            obstacles[i].x = static_cast<float>(rand() % (551) + 100); // Random x between 100 and 650
            obstacles[i].y = static_cast<float>(rand() % (401) + 100); // Random y between 100 and 500

            // Check that the generated position is not too close to the player
            float dx = obstacles[i].x - playerX;
            float dy = obstacles[i].y - playerY;
            float distanceToPlayer = sqrt(dx * dx + dy * dy);

            overlap = false; // Assume no overlap

            // Check for overlap with other obstacles
            for (int j = 0; j < i; j++) {
                float dx = obstacles[i].x - obstacles[j].x;
                float dy = obstacles[i].y - obstacles[j].y;
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < (obstacleWidth + obstacleHeight) / 2) {
                    overlap = true;
                    break; // Overlapping, regenerate position
                }
            }

            // Check for overlap with collectables
            for (int j = 0; j < numCollectables; j++) {
                float dx = obstacles[i].x - collectableX[j];
                float dy = obstacles[i].y - collectableY[j];
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < (obstacleWidth + collectableSize) / 2) {
                    overlap = true;
                    break; // Overlapping with a collectable, regenerate position
                }
            }

            // Check for overlap with powerUpTwoPositions
            for (int j = 0; j < numPowerUps; j++) {
                float dx = obstacles[i].x - powerUpTwoPositions[j][0];
                float dy = obstacles[i].y - powerUpTwoPositions[j][1];
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < (obstacleWidth + 60) / 2) {
                    overlap = true;
                    break; // Overlapping with a powerUpTwoPosition, regenerate position
                }
            }

            // Check for overlap with powerupPositions
            for (int j = 0; j < numPowerUps; j++) {
                float dx = obstacles[i].x - powerupPositions[j][0];
                float dy = obstacles[i].y - powerupPositions[j][1];
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < (obstacleWidth + 60) / 2) {
                    overlap = true;
                    break; // Overlapping with a powerup, regenerate position
                }
            }

            // Check for overlap with scene boundaries
            if (obstacles[i].x > sceneLeft && obstacles[i].x < sceneRight &&
                obstacles[i].y > sceneBottom && obstacles[i].y < sceneTop) {
                overlap = true;
            }

            // Check that the generated position is not too close to the player
            if (distanceToPlayer < (obstacleWidth + playerSize) / 2) {
                overlap = true;
            }
        }
    }
}

void drawObstacle(float x, float y) {
    // Draw a rectangular obstacle
    glColor3f(0.5f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glVertex2f(x - obstacleWidth / 2, y - obstacleHeight / 2);
    glVertex2f(x + obstacleWidth / 2, y - obstacleHeight / 2);
    glVertex2f(x + obstacleWidth / 2, y + obstacleHeight / 2);
    glVertex2f(x - obstacleWidth / 2, y + obstacleHeight / 2);
    glEnd();

    // Draw a cross on top of the obstacle
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINES);
    // Horizontal line
    glVertex2f(x - obstacleWidth / 2, y);
    glVertex2f(x + obstacleWidth / 2, y);

    // Vertical line
    glVertex2f(x, y - obstacleHeight / 2);
    glVertex2f(x, y + obstacleHeight / 2);
    glEnd();
}

void drawObstacles() {
    for (int i = 0; i < obstacleCount; i++) {
        drawObstacle(obstacles[i].x, obstacles[i].y);
    }
}

void drawCollectable(float x, float y) {
    glPushMatrix();
    // Translate to the position of the collectable
    glTranslatef(x, y, 0.0f);

    // Draw a star-shaped collectable
    glColor3f(1.0f, 0.5f, 0.0f);  // Use an orange color

    // Apply the rotation around the center of the collectable
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);

    // Draw a surrounding polygon
    glBegin(GL_POLYGON);
    for (int i = 0; i < 5; i++) {
        float angle = 2.0f * 3.14 * i / 5;
        float radius = collectableSize / 2;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    // Draw a center quad
    glBegin(GL_QUADS);
    glVertex2f(-collectableSize / 6, -collectableSize / 6);
    glVertex2f(collectableSize / 6, -collectableSize / 6);
    glVertex2f(collectableSize / 6, collectableSize / 6);
    glVertex2f(-collectableSize / 6, collectableSize / 6);
    glEnd();

    // Draw a star shape around the center quad
    glBegin(GL_LINES);
    glVertex2f(-collectableSize / 6, 0);
    glVertex2f(collectableSize / 6, 0);

    glVertex2f(0, -collectableSize / 6);
    glVertex2f(0, collectableSize / 6);

    glVertex2f(-10.0f, 0);
    glVertex2f(10.0f, 0);

    glVertex2f(0, -10.0f);
    glVertex2f(0, 10.0f);
    glEnd();

    glPopMatrix();
    rotationAngle += 0.2;
}

void initializeCollectables() {
    for (int i = 0; i < numCollectables; i++) {
        bool overlap = true;
        while (overlap) {
            // Generate random coordinates within the specified boundaries
            collectableX[i] = static_cast<float>(rand() % (screenWidth - 50 - static_cast<int>(2 * collectableSize)) + collectableSize);
            collectableY[i] = static_cast<float>(rand() % (availScreenHeight - 50 - static_cast<int>(2 * collectableSize)) + collectableSize);
            collectableActive[i] = true;
            overlap = false; // Assume no overlap

            // Check for overlap with other collectables
            for (int j = 0; j < i; j++) {
                float dx = collectableX[i] - collectableX[j];
                float dy = collectableY[i] - collectableY[j];
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < collectableSize) {
                    overlap = true;
                    break; // Overlapping with another collectable, regenerate position
                }
            }

            // Check for overlap with obstacles
            for (int j = 0; j < obstacleCount; j++) {
                float dx = collectableX[i] - obstacles[j].x;
                float dy = collectableY[i] - obstacles[j].y;
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < (collectableSize + obstacleWidth) / 2) {
                    overlap = true;
                    break; // Overlapping with an obstacle, regenerate position
                }
            }

            // Check for overlap with powerups (powerUpPositions and powerUpTwoPositions)
            for (int j = 0; j < numPowerUps; j++) {
                float dx = collectableX[i] - powerupPositions[j][0];
                float dy = collectableY[i] - powerupPositions[j][1];
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < (collectableSize + 60) / 2) {
                    overlap = true;
                    break; // Overlapping with a powerup, regenerate position
                }

                dx = collectableX[i] - powerUpTwoPositions[j][0];
                dy = collectableY[i] - powerUpTwoPositions[j][1];
                distance = sqrt(dx * dx + dy * dy);

                if (distance < (collectableSize + 60) / 2) {
                    overlap = true;
                    break; // Overlapping with a powerup, regenerate position
                }
            }

            // Check for overlap with scene boundaries
            if (collectableX[i] > sceneLeft && collectableX[i] < sceneRight &&
                collectableY[i] > sceneBottom && collectableY[i] < sceneTop) {
                overlap = true;
            }

            // Check that the generated position is not too close to the player
            float dx = collectableX[i] - playerX;
            float dy = collectableY[i] - playerY;
            float distanceToPlayer = sqrt(dx * dx + dy * dy);

            if (distanceToPlayer < (collectableSize + playerSize) / 2) {
                overlap = true;
            }
        }
    }
}

void drawTopBar() {
    // Background of the top bar
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, screenHeight);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(screenWidth, screenHeight - 50.0);
    glVertex2f(0.0f, screenHeight - 50.0);
    glEnd();

    // Health bar on the left (in green)
    glColor3f(0.0, 1.0, 0.0);
    float healthBarWidth = 200;
    if (lives == 5) {
         healthBarWidth = 250;
    }
    else if(lives==4) {
         healthBarWidth = 160;
    }
    else if (lives == 3) {
        healthBarWidth = 120;
    }
    else if (lives == 2) {
        healthBarWidth = 80;
    }
    else if (lives == 1) {
        healthBarWidth = 40;
    }
    else {
        healthBarWidth = 0;
    }
    
    glBegin(GL_QUADS);
    glVertex2f(10.0f, screenHeight-10);
    glVertex2f(healthBarWidth, screenHeight-10);
    glVertex2f(healthBarWidth, screenHeight - 40.0);
    glVertex2f(10.0f, screenHeight - 40.0);
    glEnd();

    // Score display in the center (white)
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(screenWidth / 2 - 50, screenHeight - 35);
    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
    for (int i = 0; scoreText[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
    }

    // Time display on the right (white)
    int minutes = gameTime / 60;
    int seconds = gameTime % 60;
    char timeString[16];
    snprintf(timeString, sizeof(timeString), "Time: %02d:%02d", minutes, seconds);
    glRasterPos2f(screenWidth - 130, screenHeight - 35);
    for (int i = 0; timeString[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timeString[i]);
    }
}

void drawTopBoundary() {
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, availScreenHeight);
    glVertex2f(screenWidth, availScreenHeight);
    glVertex2f(screenWidth, availScreenHeight - 15.0);
    glVertex2f(0.0f, availScreenHeight - 15.0);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    for (float x = 10.0; x < screenWidth-10; x += 20.0) {
        glVertex2f(x, availScreenHeight -7.5);
        glVertex2f(x + 10.0, availScreenHeight - 7.5);
    }
    glEnd();
}

void drawRightBoundary() {
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(screenWidth, availScreenHeight);
    glVertex2f(screenWidth - 15.0, availScreenHeight);
    glVertex2f(screenWidth - 15.0, 0.0);
    glVertex2f(screenWidth, 0.0);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    for (float y = 0.0; y < availScreenHeight; y += 20.0) {
        glVertex2f(screenWidth - 7.5, y);
        glVertex2f(screenWidth - 7.5, y + 10.0);
    }
    glEnd();
}

void drawLeftBoundary() {
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(0.0, availScreenHeight);
    glVertex2f(15.0, availScreenHeight);
    glVertex2f(15.0, 0.0);
    glVertex2f(0.0, 0.0);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    for (float y = 0.0; y < availScreenHeight; y += 20.0) {
        glVertex2f(7.5, y);
        glVertex2f(7.5, y + 10.0);
    }
    glEnd();
}

void drawBottomBoundary() {
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(0.0, 0.0);
    glVertex2f(screenWidth, 0.0);
    glVertex2f(screenWidth, 15.0);
    glVertex2f(0.0, 15.0);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    for (float x = 10.0; x < screenWidth-10; x += 20.0) {
        glVertex2f(x, 7.5);
        glVertex2f(x + 10.0, 7.5);
    }
    glEnd();
}

void drawPlayer() {
    glPushMatrix();
    glTranslatef(playerX, playerY, 0.0f);

    if (movingDown) {
        glScalef(-1.0f, -1.0f, 1.0f);
    }
    else if (movingRight) {
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    }
    else if (movingLeft) {
        glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
    }
    // Draw the player using various primitives
    // Here, we'll use a combination of lines, triangles, and quads
    glColor3f(1.0f, 0.0f, 0.0f);

    // Draw the body of the player using a quad
    glBegin(GL_QUADS);
    glVertex2f(-playerSize / 2, -playerSize / 2);
    glVertex2f(playerSize / 2, -playerSize / 2);
    glVertex2f(playerSize / 2, playerSize / 2);
    glVertex2f(-playerSize / 2, playerSize / 2);
    glEnd();

    // Draw the head of the player using a triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, playerSize);
    glVertex2f(-playerSize / 2, playerSize / 2);
    glVertex2f(playerSize / 2, playerSize / 2);
    glEnd();

    // Draw the arms of the player using lines
    glBegin(GL_LINES);
    glVertex2f(-playerSize / 2, playerSize / 4);
    glVertex2f(-playerSize / 2 - playerSize / 4, playerSize / 4);
    glVertex2f(playerSize / 2, playerSize / 4);
    glVertex2f(playerSize / 2 + playerSize / 4, playerSize / 4);
    glEnd();

    // Draw the first leg of the player using line strips
    glBegin(GL_LINE_STRIP);
    glVertex2f(-playerSize / 4, -playerSize / 2);
    glVertex2f(-playerSize / 4, -playerSize);
    glVertex2f(playerSize / 4-7, -playerSize);
    glVertex2f(playerSize / 4-7, -playerSize / 2);
    glEnd();

    // Draw the second leg of the player using line strips
    glBegin(GL_LINE_STRIP);
    glVertex2f(-playerSize / 4+7, -playerSize / 2);
    glVertex2f(-playerSize / 4+7, -playerSize);
    glVertex2f(playerSize / 4, -playerSize);
    glVertex2f(playerSize / 4, -playerSize / 2);
    glEnd();

    glPopMatrix();
}

void drawStarIcon() {
    glPushMatrix();

    // Calculate the center of the screen
    int centerX = screenWidth / 2;
    int centerY = availScreenHeight / 2;

    // Define the scale factor for the star (make it smaller)
    float scale = 400; // Adjust as needed

    glTranslatef(centerX, centerY, 0.0); // Translate to the center
    glRotatef(rotationAngle, 0.0, 0.0, 1.0); // Apply rotation around the Z-axis
    glTranslatef(-centerX, -centerY, 0.0);
    // Draw the star icon
    glBegin(GL_TRIANGLES);

    glColor3f(1.0, 0.9, 0.9); // Red color

    for (int i = 0; i < 5; i++) {
        float angle1 = i * 72.0 * 3.14 / 180.0;
        float angle2 = (i * 72.0 + 36.0) * 3.14 / 180.0;

        // Outer points of the star (scaled down)
        glVertex2f(centerX + scale * 0.5 * cos(angle1), centerY + scale * 0.5 * sin(angle1));
        glVertex2f(centerX + scale * 0.2 * cos(angle2), centerY + scale * 0.2 * sin(angle2));

        // Inner point of the star (scaled down)
        glVertex2f(centerX, centerY);
    }

    glEnd();

    glPopMatrix();
    rotationAngle += 0.2;
}


void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (gameOver&&lives!=0){
        glColor3f(1.0f, 0.0f, 0.0f); // Red color
        glRasterPos2f(screenWidth / 2 - 100, screenHeight / 2);
        const char* gameOverText = "You Won!!";
        for (int i = 0; gameOverText[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameOverText[i]);
        }
        playerSpeed = 0;
        playerVelocityX = 0;
        playerVelocityY = 0;
        if (!playGoalSoundFlag) {
            playGoalSound(0);
            playGoalSoundFlag = true;
        }
    }
    else if (lives==0||gameTime==0) { 
        glColor3f(1.0f, 0.0f, 0.0f); // Red color
        glRasterPos2f(screenWidth / 2 - 100, screenHeight / 2);
        const char* gameOverText = "You Lost.";
        for (int i = 0; gameOverText[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameOverText[i]);
        }
        if (!playLostSoundFlag) {
            playLostSound(0);
            playLostSoundFlag = true;
        }
    }
    
    else { 
        drawStarIcon();
        drawGoal();
        drawPowerups();
        drawObstacles();
        drawTimeFreezePowerup();
        for (int i = 0; i < numCollectables; i++) {
            if (collectableActive[i]) {
                drawCollectable(collectableX[i], collectableY[i]);
            }
        }
        drawPlayer();
        drawTopBar();
        drawRightBoundary();
        drawLeftBoundary();
        drawTopBoundary();
        drawBottomBoundary();
       
        
    }
    glutSwapBuffers();

}

void checkCollision() {
    // Define the boundary offsets
    float verticalBoundaryOffset = 25.0;
    float horizontalBoundaryOffset = 21.0;

    bool collided = false; // Flag to check if a collision occurred

    for (int i = 0; i < numCollectables; i++) {
        if (collectableActive[i] &&
            playerX - playerSize / 2 < collectableX[i] + collectableSize / 2 &&
            playerX + playerSize / 2 > collectableX[i] - collectableSize / 2 &&
            playerY - playerSize / 2 < collectableY[i] + collectableSize / 2 &&
            playerY + playerSize / 2 > collectableY[i] - collectableSize / 2) {
            // Player has collected the item, you can add scoring logic here
            collectableActive[i] = false;  // Deactivate the collectable
            PlayCollectableSound();
            if (multiplier) {
                score += 200;
            }
            else {
                score += 100;
            }  
        }
    }

    for (int i = 0; i < numPowerUps;i++) {
        if (!powerupCollected[i] && playerX - playerSize / 2 < powerupPositions[i][0]+15 &&
            playerX + playerSize / 2 > powerupPositions[i][0]-15 &&
            playerY - playerSize / 2 < powerupPositions[i][1]+15 &&
            playerY + playerSize / 2 > powerupPositions[i][1]-15) {
            powerupCollected[i] = true;
            PlayCollectableSound();
            timeFreeze = true;
            remainingFrozenTime += 10;
        }
    }
    for (int i = 0; i < numPowerUps;i++) {
        if (!powerupTwoCollected[i] && playerX - playerSize / 2 < powerUpTwoPositions[i][0] + 15 &&
            playerX + playerSize / 2 > powerUpTwoPositions[i][0] - 15 &&
            playerY - playerSize / 2 < powerUpTwoPositions[i][1] + 15 &&
            playerY + playerSize / 2 > powerUpTwoPositions[i][1] - 15) {
            powerupTwoCollected[i] = true;
            PlayCollectableSound();
            multiplier = true;
            multiplierTimeRemaining += 10;
        }
    }

    // Check for collisions with obstacles
    const float playerSize = 20.0f; // Adjust the player size

    for (int i = 0; i < obstacleCount; i++) {
        float dx = playerX - obstacles[i].x;
        float dy = playerY - obstacles[i].y;
        float distance = sqrt(dx * dx + dy * dy);
        float overlap = ((21 + obstacleWidth) / 2) - distance;

        if (overlap > 0) {
            float angle = atan2(dy, dx);
            // Check if the collision is more horizontal or vertical
            if (abs(dx) > abs(dy)) {
                // Horizontal collision, adjust both horizontal and vertical directions
                playerX += (overlap+2) * cos(angle);
            }
            else {
                // Vertical collision, adjust only in the vertical direction
                playerY += (overlap + 2) * sin(angle);
            }

            collided = true;
        }
    }

    // Boundary checks to prevent player from crossing boundaries
    if (playerX - playerSize / 2 < horizontalBoundaryOffset) {
        playerX = horizontalBoundaryOffset + playerSize / 2;
        collided = true;
    }
    if (playerX + playerSize / 2 > screenWidth - horizontalBoundaryOffset) {
        playerX = screenWidth - horizontalBoundaryOffset - playerSize / 2;
        collided = true;
    }
    if (playerY - playerSize / 2 < verticalBoundaryOffset) {
        playerY = verticalBoundaryOffset + playerSize / 2;
        collided = true;
    }
    if (playerY + playerSize / 2 > availScreenHeight - verticalBoundaryOffset) {
        playerY = availScreenHeight - verticalBoundaryOffset - playerSize / 2;
        collided = true;
    }

    // If a collision occurred, set player velocity to 0
    if (collided) {
        playerVelocityX = 0.0f;
        playerVelocityY = 0.0f;

        // Decrement lives by one
        if (lives > 0) {
            lives--;
            if (lives == 0 || gameTime==0) {
                gameOver = true;
            }
        }  
    }
    // Check if playerX and playerY are outside the scene boundaries
    if ((playerX > sceneLeft && playerX < sceneRight) && (playerY > sceneBottom && playerY < sceneTop)) {
        gameOver = true;
    }

}

void updateCollectableRotation() {
    // Update the rotation angle for the animation
    collectableRotation += rotationSpeed;
    if (collectableRotation >= 360.0f) {
        collectableRotation -= 360.0f;
    }
}

void update(int value) {
    // Update player position based on velocity
    playerX += playerVelocityX;
    playerY += playerVelocityY;

    // Call collision check
    checkCollision();

    int currentTime = time(0);
    
    if (multiplier && currentTime > lastTime) {
        multiplierTimeRemaining--;
        if (multiplierTimeRemaining == 0) {
            multiplier = false;
        }
    }
    if (timeFreeze&&currentTime > lastTime) {
        lastTime = currentTime;
        remainingFrozenTime--;
        if (remainingFrozenTime == 0) {
            timeFreeze = false;
        }
    }
    if (!timeFreeze&&gameTime!=0&&currentTime > lastTime&&gameStarted) {
        lastTime = currentTime;
        gameTime--;
        // You can add game over logic here
        if (gameTime <= 0) {
            // Game Over
        }
    }

    updateCollectableRotation();
    if (scaleFactor >= 29)
       ScalingUp = false;
    else if (scaleFactor <= 27)
        ScalingUp = true;

    if (ScalingUp)
        scaleFactor += 0.2;
    else
        scaleFactor -= 0.2;

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (!gameStarted) {
        gameStarted = true;
        lastTime = time(0);
    }
    switch (key) {
    case 'w':
        playerVelocityX = 0.0f;
        playerVelocityY = playerSpeed;
        movingDown = false;
        movingUp = true;
        movingRight = false;
        movingLeft = false;
        break;
    case 's':
        playerVelocityX = 0.0f;
        playerVelocityY = -playerSpeed;
        movingDown = true;
        movingUp = false;
        movingRight = false;
        movingLeft = false;
        break;
    case 'a':
        playerVelocityX = playerSpeed;
        playerVelocityY = 0.0f;
        movingDown = false;
        movingUp = false;
        movingRight = false;
        movingLeft = true;
        break;
    case 'd':
        playerVelocityX = playerSpeed;
        playerVelocityY = 0.0f;
        movingDown = false;
        movingUp = false;
        movingRight = true;
        movingLeft = false;
        break;
    }
}

void specialKeys(int key, int x, int y) {
    if (!gameStarted) {
        gameStarted = true;
        lastTime = time(0);
    }
    switch (key) {
    case GLUT_KEY_UP:
        playerVelocityX = 0.0f;
        playerVelocityY = playerSpeed;
        movingUp = true;
        movingDown = false;
        movingRight = false;
        movingLeft = false;
        break;
    case GLUT_KEY_DOWN:
        playerVelocityX = 0.0f;
        playerVelocityY = -playerSpeed;
        movingDown = true;
        movingUp = false;
        movingRight = false;
        movingLeft = false;
        break;
    case GLUT_KEY_LEFT:
        playerVelocityX = -playerSpeed;
        playerVelocityY = 0.0f;
        movingDown = false;
        movingUp = false;
        movingRight = false;
        movingLeft = true;
        break;
    case GLUT_KEY_RIGHT:
        playerVelocityX = playerSpeed;
        playerVelocityY = 0.0f;
        movingDown = false;
        movingUp = false;
        movingRight = true;
        movingLeft = false;
        break;
    }
}

void initPowerups() {
    for (int i = 0; i < numPowerUps; i++) {
        bool overlap = true;
        while (overlap) {
            powerUpTwoPositions[i][0] = static_cast<float>(std::rand() % 351 + 150); // Range: 150 to 500
            powerUpTwoPositions[i][1] = static_cast<float>(std::rand() % 251 + 150); // Range: 150 to 400

            overlap = false; // Assume no overlap

            // Check for overlap with player position
            float dx = powerUpTwoPositions[i][0] - playerX;
            float dy = powerUpTwoPositions[i][1] - playerY;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < (50 + playerSize) / 2) {
                overlap = true;
                continue; // Overlapping with the player, regenerate position
            }

            // Check for overlap with collectables
            for (int j = 0; j < numCollectables; j++) {
                float dx = powerUpTwoPositions[i][0] - collectableX[j];
                float dy = powerUpTwoPositions[i][1] - collectableY[j];
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < (50 + collectableSize) / 2) {
                    overlap = true;
                    break; // Overlapping with a collectable, regenerate position
                }
            }

            // Check for overlap with obstacles
            for (int j = 0; j < obstacleCount; j++) {
                float dx = powerUpTwoPositions[i][0] - obstacles[j].x;
                float dy = powerUpTwoPositions[i][1] - obstacles[j].y;
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < (50 + obstacleWidth + 20) / 2) {
                    overlap = true;
                    break; // Overlapping with an obstacle, regenerate position
                }
            }

            // Check for overlap with other powerUpTwoPositions
            for (int j = 0; j < i; j++) {
                float dx = powerUpTwoPositions[i][0] - powerUpTwoPositions[j][0];
                float dy = powerUpTwoPositions[i][1] - powerUpTwoPositions[j][1];
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < (50 + 50) / 2) {
                    overlap = true;
                    break; // Overlapping with another powerUpTwoPosition, regenerate position
                }
            }

            // Check for overlap with powerupPositions
            for (int j = 0; j < numPowerUps; j++) {
                if (j != i) { // Skip checking against itself
                    float dx = powerUpTwoPositions[i][0] - powerupPositions[j][0];
                    float dy = powerUpTwoPositions[i][1] - powerupPositions[j][1];
                    float distance = sqrt(dx * dx + dy * dy);

                    if (distance < (50 + 50) / 2) {
                        overlap = true;
                        break; // Overlapping with a powerUp, regenerate position
                    }
                }
            }
        }
    }
}



int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(0)));
    initializeCollectables();
    generateRandomTimeFreezePowerups();
    initPowerups();
    generateRandomObstacles();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Game");
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, screenWidth, 0.0f, screenHeight, 0.0f, 1.0f);

    // Initialize player position at the center of the screen
    playerX = screenWidth / 2;
    playerY = availScreenHeight / 2;

    glutDisplayFunc(display);
    glutTimerFunc(1000 / 60, update, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    PlayBackgroundSound(0);

    glutMainLoop();
    
    return 0;
}