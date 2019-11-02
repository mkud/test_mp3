#include <stdio.h>
#include <lame/lame.h>
#include <dirent.h>
#include "CProtectedList.h"
#include "CThreadCounter.h"

using namespace std;

#ifdef _WIN32
#include <windows.h>.
#define PATH_SEPARATOR "\\"
#else
#include <unistd.h>
#define PATH_SEPARATOR "/"
#endif

int get_count_of_cores() {
#ifdef WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#else
	return sysconf(_SC_NPROCESSORS_ONLN);
#endif

}

string szWavExtension = ".wav";

CThreadCounter cntr;

void GetListOfWavFiles(char *szDir, CProtectedList &list_Files) {
	struct dirent *ent;
	DIR *dir;
	if ((dir = opendir(szDir)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			//Checking if this is *.wav file
			string curName = ent->d_name;
			for (string::reverse_iterator szEnd = szWavExtension.rbegin(), szEnd2 = curName.rbegin();
					szEnd2 != curName.rend(); szEnd++, szEnd2++) {
				if (szEnd == szWavExtension.rend()) {
					list_Files.AppendValueUnsafe(curName);
					break;
				}
				if (*szEnd != tolower(*szEnd2))
					break;
			}
		}
		closedir(dir);
	} else {
		/* could not open directory */
		perror("");
		return;
	}
}

static void* ToMP3Worker(void *listOfFiles) {
	int read, write;
	CProtectedList *list_Files = (CProtectedList*) listOfFiles;
	string sz1;

	while (list_Files->GetNextValueSafe(sz1)) {

//From this I used a standart way to encoding
//Very simply, without error check. TODO - by the way.

		FILE *wav = fopen((list_Files->GetDirectory() + PATH_SEPARATOR + sz1).c_str(), "rb");
		FILE *mp3 = fopen(
				(list_Files->GetDirectory() + PATH_SEPARATOR + sz1.substr(0, sz1.length() - 4) + ".mp3").c_str(), "wb");

		const int WAV_SIZE = 8192;
		const int MP3_SIZE = 8192;

		short int wav_buffer[WAV_SIZE * 2];
		unsigned char mp3_buffer[MP3_SIZE];

// https://fossies.org/linux/misc/lame-3.100.tar.gz:a/lame-3.100/HACKING
		/*		   66 THREADSAFE:
		 67 ===========
		 68
		 69 Lame should now be thread safe and re-entrant. The only problem seems to
		 70 be some OS's allocate small stacks (< 128K) to threads launched by
		 71 applications, and this is not enough for LAME.  Fix is to increase the
		 72 stack space, or move some of our automatic variables onto the heap with
		 73 by using bug-proof malloc()'s and free(). */

		lame_t lame = lame_init();
		lame_set_in_samplerate(lame, 44100);
		lame_set_VBR(lame, vbr_default);
		lame_set_quality(lame, 5);
		lame_init_params(lame);

		do {
			read = fread(wav_buffer, 2 * sizeof(short int), WAV_SIZE, wav);
			if (read == 0)
				write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
			else
				write = lame_encode_buffer_interleaved(lame, wav_buffer, read, mp3_buffer, MP3_SIZE);
			fwrite(mp3_buffer, write, 1, mp3);
		} while (read != 0);

		lame_close(lame);
		fclose(mp3);
		fclose(wav);
	}
	cntr.DecrementCount();
	return 0;

}

int main(int argc, char *argv[]) {
	CProtectedList list_Files(argv[1]);
	GetListOfWavFiles(argv[1], list_Files);

	int cnt = get_count_of_cores();
	for (int cnt = get_count_of_cores(); cnt; cnt--){
		cntr.IncrementCount();
		pthread_t t1;
		pthread_create(&t1, NULL, ToMP3Worker, &list_Files);
	}

	// Here can use pthread_join()
	// But condition variables is more funny)))
	cntr.WaitFinish();
}
