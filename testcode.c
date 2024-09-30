#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int x;
    int y;
} Point;

int decodeValue(char *value, int base) {
    int result = 0;
    int length = strlen(value);

    for (int i = 0; i < length; i++) {
        result = result * base + (value[i] - '0');
    }
    return result;
}

int readJSON(char *filename, Point *points, int *k) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        return -1;
    }

    char line[256];
    int index = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "\"n\":")) {
        } else if (strstr(line, "\"k\":")) {
            sscanf(line, " \"k\": %d", k);
        } else if (strstr(line, "base") && strstr(line, "value")) {
            int base, x;
            char value[256];

            sscanf(line, " \"%d\": { \"base\": \"%d\", \"value\": \"%[^\"]\" }", &x, &base, value);
            points[index].x = x;
            points[index].y = decodeValue(value, base);
            index++;
        }
    }

    fclose(file);
    return index;
}

double lagrangeInterpolation(Point *points, int k) {
    double secret = 0.0;

    for (int i = 0; i < k; i++) {
        double term = points[i].y;
        
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0 - points[j].x) / (double)(points[i].x - points[j].x);
            }
        }
        secret += term;
    }

    return secret;
}

int main() {
    char filename[] = "input.json";
    Point points[10];
    int k;

    int n = readJSON(filename, points, &k);
    if (n == -1) {
        return 1;
    }

    double secret = lagrangeInterpolation(points, k);

    printf("The constant term (secret) c is: %.0lf\n", secret);

    return 0;
}
