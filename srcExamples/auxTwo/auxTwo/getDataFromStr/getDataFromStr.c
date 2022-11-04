#include <getDataFromStr.h>
#include <string.h>
#include <stdlib.h>

char* getStrBetweenTwoStr(const char *srcString, const char *strStart, const char *strEnd) {
	char *target = NULL;
	char *start, *end;
	start = strstr(srcString, strStart);
	if(start) {
		start += strlen(strStart);
		end = strstr(srcString, strEnd);
		if(end) {
			int sizeData = end-start;
			if(sizeData > 0) {
				target = (char *)malloc(sizeData + 1);
				memcpy(target, start, sizeData);
				target[sizeData] = '\0';
			}
		}
	}
	return target;
}

int getIntBetweenTwoStr(const char *srcString, const char *strStart, const char *strEnd) {
	char *strNumber;
	int valueGot = 0;
	strNumber = getStrBetweenTwoStr(srcString, strStart, strEnd);
	if (strNumber) {
		valueGot = atoi(strNumber);
		free(strNumber);
	}
	return valueGot;
}