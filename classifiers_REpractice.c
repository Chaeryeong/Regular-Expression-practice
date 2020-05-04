#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: [%s] [input path]\n", argv[0]);
		return 0;
	}
	pcre2_code *re = NULL;
	pcre2_match_data *match_data = NULL;
	PCRE2_SIZE erroffset, *ovector;
	int errorcode;
	int rc;
	int num;
	char strTemp[255];

	FILE *fp = NULL;
	if (fopen_s(&fp, argv[1], "r") != 0)
	{
		printf("no file found\n");
		return 0;
	}

	
	for (int p = 0; p < 3; p++) {
		fscanf_s(fp, "%d", &num);
		fgetc(fp);
		PCRE2_SPTR pattern = (PCRE2_SPTR)"[0][1-3][0|1|6|9]?-\\d{3,4}-\\d{4}"; 
		//PCRE2_SPTR pattern = (PCRE2_SPTR)"[0][2][-]\\d{ 3,4 }[-]\\d{ 4 } | [0][1][0 | 6 | 9][-]\\d{ 3,4 }[-]\\d{ 4 } | [0][3][1][-]\\d{ 3,4 }[-]\\d{ 4 }";
		switch (p) 
		{
		case 0: //for #1 Phone number classifier
			pattern = (PCRE2_SPTR)"[0][1-3][0|1|6|9]?-\\d{3,4}-\\d{4}\n"; 
			break;
		case 1: //for #2 Log classfier
			pattern = (PCRE2_SPTR)"[\\w|\\W]*[[]\\d{1,2}\\/Apr\\/2018[]]\\s*[\"]GET\\s*\\/admin\\s*[\\w|\\W]*"; 
			break;							
		case 2: //for #3 Specific image file classifier
			pattern = (PCRE2_SPTR)"424D[\\d|A-F]{32}50[0]{6}46[0]{6}[\\d|A-F]{44}"; 
			break;
		}
		while (num--)
		{
			fgets(strTemp, sizeof(strTemp), fp);
			PCRE2_SPTR input = (PCRE2_SPTR)strTemp;

			re = pcre2_compile(pattern, -1, 0, &errorcode, &erroffset, NULL);

			if (re == NULL)
			{
				PCRE2_UCHAR8 buffer[120];
				(void)pcre2_get_error_message(errorcode, buffer, 120);
				/* Handle error */
				return 0;
			}

			match_data = pcre2_match_data_create(20, NULL);
			rc = pcre2_match(re, input, -1, 0, 0, match_data, NULL);

			if (rc <= 0)
				printf("N\n");
			else
			{
				ovector = pcre2_get_ovector_pointer_8(match_data);
				printf("Y\n");
				int index = (int)ovector[0];
				int n = 0;
			}
		}
	}

	fclose(fp);

	pcre2_match_data_free(match_data);
	pcre2_code_free(re);

	return 0;
	
}