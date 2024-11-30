#include <stdlib.h>
#include <string.h>

char* longest_common_prefix(char** strs, int strsSize) {
    if (!strs || strsSize == 0) {
        return "";
    }

    // take the 1st str as ref.
    char *ref = strs[0];
    int ref_size = strlen(ref);

    char *lcp = NULL;
    int lcp_size = 0;

    for (int i = 0; i < ref_size; i++) {
        int j = 1;
        for (; j < strsSize; j++) {
            if ((strlen(strs[j]) < i) || (ref[i] != strs[j][i])) {
                break;
            }
        }

        if (j == strsSize) {
            lcp_size++;
        } else {
            break;
        }
    }

    if (lcp_size) {
        lcp = (char*) malloc((lcp_size + 1) * sizeof(char));
        memcpy(lcp, ref, lcp_size);
        lcp[lcp_size] = '\0';

        return lcp;
    }

    return "";
}
