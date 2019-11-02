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

// this should be cross-platform
int get_count_of_cpu_cores() {
#ifdef WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#else
	return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

// files with this extension should be converted. Should be lowercase.
string g_szFileConvertExtension = ".wav";

CThreadCounter g_cntrThread;

int GetListOfFilesForConvert(char *szDir, CProtectedList &list_Files) {
	struct dirent *ent;
	DIR *dir;
	if ((dir = opendir(szDir)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			//Checking if this is *.wav file
			string curName = ent->d_name;
			for (string::reverse_iterator szEnd = g_szFileConvertExtension.rbegin(), szEnd2 = curName.rbegin();
					szEnd2 != curName.rend(); szEnd++, szEnd2++) {
				if (szEnd == g_szFileConvertExtension.rend()) {
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
		return 0;
	}
	return 1;
}

static void* ToMP3Worker(void *listOfFiles) {
	int read, write;
	CProtectedList *list_Files = (CProtectedList*) listOfFiles;
	string szCurrentFileName;

	while (list_Files->GetNextValueSafe(szCurrentFileName)) {

//From this I used a standard way to encoding
//Very simply, without error check.

		FILE *wav = fopen((list_Files->GetDirectory() + PATH_SEPARATOR + szCurrentFileName).c_str(), "rb");
		FILE *mp3 = fopen(
				(list_Files->GetDirectory() + PATH_SEPARATOR
						+ szCurrentFileName.substr(0, szCurrentFileName.length() - 4) + ".mp3").c_str(), "wb");

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
	g_cntrThread.DecrementCount();
	return 0;

}

int main(int argc, char *argv[]) {

	//Get all *.wav files from DIR
	CProtectedList listFilesForConvert(argv[1]);
	if (!GetListOfFilesForConvert(argv[1], listFilesForConvert))
		return 1;

	// starts the number of threads = the number of CPU
	for (int iCntOfCPU = get_count_of_cpu_cores(); iCntOfCPU; iCntOfCPU--) {
		g_cntrThread.IncrementCount();
		pthread_t tmpThreadID;
		pthread_create(&tmpThreadID, NULL, ToMP3Worker, &listFilesForConvert);
	}

	// Here can use pthread_join()
	// But condition variables is more funny)))
	g_cntrThread.WaitFinish();
	return 0;
}
