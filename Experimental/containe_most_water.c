
inline int my_min(int x, int y)
{
    return ((x < y) ? x : y);
}

inline int my_max(int x, int y)
{
    return ((x < y) ? y : x); 
}

int max_area(int* height, int heightSize) {
    int max_area = 0;

    if (!height) {
        return 0;
    }

    int left = 0;
    int right = heightSize - 1;

    while (left < right) {
        unsigned int w = right - left;
        unsigned int h = my_min(height[left], height[right]);

        max_area = my_max(max_area, w * h);

        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }
    }

    return max_area;
}
