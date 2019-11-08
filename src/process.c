//
// Created by Joris GOUNAND on 11/8/19.
//

bool process_runing(void)
{
	DIR* dir;
	struct dirent* ent;
	char* endptr;
	char buf[32];

	if (!(dir = opendir("/proc"))) {
		perror("can't open /proc");
		return 1;
	}
}