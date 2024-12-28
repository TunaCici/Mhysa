int trap(int* height, int heightSize)
{
    // assume all inputs are valid

    int total_water = 0;
    int potential_water = 0;

    // fill in: left -> right
    int left = 0;
    for (int i = left; i < heightSize; i++) {
        if (height[left] <= height[i]) {
            total_water += potential_water;
            potential_water = 0;
            left = i;
        } else {
            potential_water += (height[left] - height[i]);
        }
    }

    potential_water = 0;

    // fill in: right -> left
    int right = (heightSize - 1);
    for (int i = right; 0 <= i; i--) {
        if (height[right] < height[i]) {
            total_water += potential_water;
            potential_water = 0;
            right = i;
        } else {
            potential_water += (height[right] - height[i]);
        }
    }

    return total_water;    
}
