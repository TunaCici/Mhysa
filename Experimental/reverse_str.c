void reverseString(char* s, int sSize)
{
        if (!s || sSize == 0) {
                return;
        }

        int left = 0;
        int right = (sSize - 1);

        while (left < right) {
                char tmp = s[right];
                s[right] = s[left];
                s[left] = tmp;

                left++;
                right--;
        }
}