#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>  // Use <windows.h> and Sleep() on Windows

// Function to generate ANSI escape code for 24-bit RGB color
void setColor(float r, float g, float b) {
    printf("\x1b[38;2;%d;%d;%dm", (int)r, (int)g, (int)b);
}

int main() {
    float A = 0, B = 0;
    int width = 80, height = 22;
    float z[1760];
    char b[1760];
    printf("\x1b[2J");  // Clear the screen

    while (1) {
        memset(b, 32, 1760);
        memset(z, 0, 7040);

        // Calculate color dynamically for smooth transitions
        float t = fmod(A, 6.28) / 6.28;  // Normalized time variable (0 to 1)
        float r = 128 * (1 + sin(t * 6.28));  // Red oscillates between 0 and 255
        float g = 64 * (1 + cos(t * 6.28));   // Green oscillates less, between 0 and 128
        float b_val = 255 - r;  // Blue shifts opposite to red for a smooth blend

        for (float theta = 0; theta < 6.28; theta += 0.07) {
            for (float phi = 0; phi < 6.28; phi += 0.02) {
                float sinA = sin(A), cosA = cos(A);
                float sinB = sin(B), cosB = cos(B);
                float sinTheta = sin(theta), cosTheta = cos(theta);
                float sinPhi = sin(phi), cosPhi = cos(phi);

                float h = cosTheta + 2;
                float D = 1 / (sinPhi * h * sinA + sinTheta * cosA + 5);
                float t = sinPhi * h * cosA - sinTheta * sinA;

                int x = (int)(width / 2 + 30 * D * (cosPhi * h * cosB - t * sinB));
                int y = (int)(height / 2 + 15 * D * (cosPhi * h * sinB + t * cosB));
                int o = x + width * y;
                int N = (int)(8 * ((sinTheta * sinA - sinPhi * cosTheta * cosA) * cosB - sinPhi * cosTheta * sinA - sinTheta * cosA - cosPhi * cosTheta * sinB));

                if (height > y && y > 0 && x > 0 && width > x && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }

        printf("\x1b[H");  // Reset cursor to top-left
        for (int k = 0; k < 1760; k++) {
            if (k % width == 0) putchar('\n');  // New line at the end of each row

            // Set dynamic color and print character
            setColor(r, g, b_val);
            putchar(b[k]);
        }
        printf("\x1b[0m");  // Reset color

        A += 0.04;
        B += 0.02;
        usleep(30000);  // Adjust delay if using Windows with Sleep(30)
    }

    return 0;
}
