void explore(char **grid, int gridSize, int* gridColSize,
                         int row, int col)
{
        char up = (0 < row) ? grid[row - 1][col] : '0';
        char down = (row < (gridSize - 1)) ? grid[row + 1][col] : '0';
        char left = (0 < col) ? grid[row][col - 1] : '0';
        char right = (col < (*gridColSize - 1)) ? grid[row][col + 1] : '0';

        // mark
        grid[row][col] = '2';

        // explore further
        if (up == '1') {
                explore(grid, gridSize, gridColSize, row - 1, col);
        }
        
        if (down == '1') {
                explore(grid, gridSize, gridColSize, row + 1, col);
        }
        
        if (left == '1') {
                explore(grid, gridSize, gridColSize, row, col - 1);
        } 
        
        if (right == '1') {
                explore(grid, gridSize, gridColSize, row, col + 1);
        }
}

int numIslands(char** grid, int gridSize, int* gridColSize)
{
        // assume all inputs all valid

        int no_of_islands = 0;

        // O(m * n)
        for (int i = 0; i < gridSize; i++) {
                for (int j = 0; j < *gridColSize; j++) {
                        if (grid[i][j] == '1') {
                                no_of_islands++;
                                explore(grid, gridSize, gridColSize, i, j);
                        }
                }
        }

        return no_of_islands;
}