/*
 * COP 3502H | Spring 2026 | Section 0206
 * Name: Sahil Narsinghani
 * UCF ID: 5753629
*/

#include <stdio.h>

int floodfillRec(char grid[][100], int x, int y, int M, int N);

int main() {

    int M;
    int N;

    char grid[100][100];

    scanf("%d %d", &M, &N);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            scanf(" %c", &grid[i][j]);
        }
    }

    /*
        count off here tmrw.
    */

    return 0;
}

int floodfillRec(char grid[][100], int x, int y, int M, int N) {
    if (x < 0 || y < 0) {
        if (x >=M || y >= N) {
            return 0;
        }
    }

    if (grid[x][y] != 'F' && grid[x][y] != 'S') {
        return 0;
    }

    grid[x][y] = '.';

    int size = 1;

    size += floodfillRec(grid, x + 1, y, M, N);
    size += floodfillRec(grid, x - 1, y, M, N);
    size += floodfillRec(grid, x, y + 1, M, N);
    size += floodfillRec(grid, x, y - 1, M, N);

    return size;
}