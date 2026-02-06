/*
 * COP 3502H | Spring 2026 | Section 0206
 * Name: Sahil Narsinghani
 * UCF ID: 5753629
*/

#include <stdio.h>
#define SIZE 100

// print the contents in the grid
void display_grid(char grid[][SIZE], int r, int c) {
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      printf("%c", grid[i][j]);
    }
    printf("\n");
  }
}

// recursive function
void flood_fill_rec(char grid[][SIZE], int x, int y, char new_ch, char old_ch) {
  if (x < 0 || y < 0 || x >= SIZE || y >= SIZE) // if it is out of the matrix
    return;
  if (grid[x][y] != old_ch) // if it is not the old char, no need to go to other direction for that cell
    return;

  grid[x][y] = new_ch;
  flood_fill_rec(grid, x + 1, y, new_ch, old_ch);
  flood_fill_rec(grid, x - 1, y, new_ch, old_ch);
  flood_fill_rec(grid, x, y + 1, new_ch, old_ch);
  flood_fill_rec(grid, x, y - 1, new_ch, old_ch);
}

// wrapper function
void flood_fill(char grid[][SIZE], int x, int y, char ch) {
  char old_char = grid[x][y];
  flood_fill_rec(grid, x, y, ch, old_char);
}

int main(void) {
  int M, N;
  char grid[SIZE][SIZE];

  scanf("%d %d", &M, &N);

  for (int r = 0; r < M; ++r) {
    for (int c = 0; c < N; ++c) {
      scanf(" %c", &grid[r][c]);
    }
  }

  int regionCount = 0;
  int maxSize = 0;

  for (int r = 0; r < M; ++r) {
    for (int c = 0; c < N; ++c) {
      if (grid[r][c] == 'F' || grid[r][c] == 'S') {
        regionCount++;

        flood_fill(grid, r, c, '.');

        int size = 0;
        for (int x = 0; x < M; ++x) {
          for (int y = 0; y < N; ++y) {
            if (grid[x][y] == '.') {
              size++;
            }
          }
        }

        if (size > maxSize) {
          maxSize = size;
        }

        for (int r = 0; r < M; ++r) {
          for (int c = 0; c < N; ++c) {
            if (grid[r][c] == '.') {
              grid[r][c] = 'X'; // Mark as visited permanently
            }
          }
        }
      }
    }
  }

  printf("Frozen Regions: %d\n", regionCount);
  printf("Largest Frozen Region Size: %d\n", maxSize);

  return 0;
}