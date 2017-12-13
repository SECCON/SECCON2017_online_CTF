#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


using namespace std;


class Clip;
class VideoClip;
class AudioClip;
class SubClip;
class Metadata;

Clip* clips[0x4000];
uint64_t clipCount;
uint64_t subtitleIdx = -1;
uint64_t metaIdx = -1;


void fuck_the_heap(){
	int i;
	uint8_t *chunks[0x200];
	int seed;
	int fd;

	fd = open("/dev/urandom", O_RDONLY);
	if(fd < 0){
		exit(1);
	}

	if(read(fd, &seed, 4)!=4){
		exit(1);
	}
	close(fd);

	srand(seed);
	
	
	for(i=0;i<0x100;i++){
		chunks[i] = new uint8_t[rand() & 0xFF];
	}

	for(i=0;i<0x100;i++){
		if(rand() % 3 == 0){
			delete[] chunks[i];
		}
		chunks[i] = NULL;
	}

	seed = 0;
}

void INIT(){
	alarm(600);
	fuck_the_heap();
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	
	cout << "Welcome to SVP ( SECCON Video Player ) !!!" << endl;


}

void menu(){
	cout << "1. Add Clip" << endl;
	cout << "2. Edit Clip" << endl;
	cout << "3. Play Clip" << endl;
	cout << "4. Remove Clip" << endl;
	cout << ">>> ";
}


class Clip{
	public:
		virtual void add(){
			return;
		}
		virtual void edit(){
			return;
		}
		virtual void remove(){
			return;
		}
		virtual void play(){
			return;
		}
};

class VideoClip: public Clip{
public:
	uint64_t resolution;
	uint32_t fps;
	uint32_t frames;
	uint8_t *data;
	uint8_t desc[0x30];

	virtual void add(){
		cout << "Video Resolution : ";
		if(read(0, &resolution, 8) <= 0){
			exit(1);
		}
		cout << "FPS : ";
		if(read(0, &fps, 4) <= 0){
			exit(1);
		}
		cout << "Number of Frames : ";
		if(read(0, &frames, 4) <= 0){
			exit(1);
		}

		if ( frames > 0x400 ){
			frames = 0x400;
		}


		data = new uint8_t[frames];
		
		if(!data){
			exit(1);
		}

		cout << "Video Data : ";
		frames = read(0, data, frames);
		if( frames <= 0 ){
			exit(1);
		}

		memset(desc, 0, 0x30);

		cout << "Add description : ";
		if(read(0, desc, 0x2f) <= 0){
			exit(1);
		}
	}

	virtual void edit(){
		cout << "Video Resolution : ";
		if(read(0, &resolution, 8) <= 0){
			exit(1);
		}
		cout << "FPS : ";
		if(read(0, &fps, 4) <= 0){
			exit(1);
		}
		cout << "Number of Frames : ";
		if(read(0, &frames, 4) <= 0){
			exit(1);
		}

		if ( frames > 0x400 ){
			frames = 0x400;
		}

		uint8_t *newData = new uint8_t[frames];
		
		if(!newData){
			exit(1);
		}
		data = newData;
		delete[] data;

		cout << "Video Data : ";
		frames = read(0, data, frames);
		if( frames <= 0 ){
			exit(1);
		}

		memset(desc, 0, 0x30);

		cout << "Edit description : ";
		if(read(0, desc, 0x2f) <= 0){
			exit(1);
		}
	}

	virtual void play(){
		uint32_t i;
		cout << "Playing video..." << endl;
		for(i=0;i<=frames;i++){
			cout << (char)(data[i] ^ 0xCC);
		}
		cout << endl;
	}

	virtual void remove(){
		delete[] data;
		data = NULL;
	}
};

class AudioClip: public Clip{
public:
	uint16_t bitrate;
	uint32_t seconds;
	uint8_t *data;
	uint8_t desc[0x30];

	virtual void add(){
		cout << "Audio Bitrate : ";
		if(read(0, &bitrate, 2) <= 0){
			exit(1);
		}

		cout << "Audio Length (seconds) : ";
		if(read(0, &seconds, 4) <= 0){
			exit(1);
		}

		if( seconds > 0x100){
			seconds = 0x100;
		}
		
		data = new uint8_t[seconds];

		if(!data){
			exit(1);
		}

		cout << "Audio Data : ";
		seconds = read(0, data, seconds);
		if( seconds <= 0){
			exit(1);
		}

		memset(desc, 0, 0x30);

		cout << "Add description : ";
		if(read(0, desc, 0x2f) <= 0){
			exit(1);
		}
	}

	virtual void edit(){
		cout << "Audio Bitrate : ";
		if(read(0, &bitrate, 2) <= 0){
			exit(1);
		}

		cout << "Audio Length (seconds) : ";
		if(read(0, &seconds, 4) <= 0){
			exit(1);
		}

		if( seconds > 0x100){
			seconds = 0x100;
		}
		
		uint8_t *newData = new uint8_t[seconds];

		if(!newData){
			exit(1);
		}
		delete[] data;
		data = newData;

		cout << "Audio Data : ";
		seconds = read(0, data, seconds);
		if( seconds <= 0){
			exit(1);
		}

		memset(desc, 0, 0x30);

		cout << "Edit description : ";
		if(read(0, desc, 0x2f) <= 0){
			exit(1);
		}
	}

	virtual void play(){
		uint32_t i;
		for(i=0;i<=seconds;i++){
			cout << (data[i] ^ 0x55);
		}
		cout << endl;
	}

	virtual void remove(){
		delete[] data;
		data = NULL;
	}
};


class SubClip: public Clip{
public:
	uint8_t language[4];
	uint32_t length;
	uint8_t *data = NULL;

	virtual void add(){
		if(!data){
			cout << "Subtitle Language : ";
			if(read(0, language, 4) <= 0){
				exit(1);
			}

			cout << "Subtitle Length : ";
			if(read(0, &length, 4) <= 0){
				exit(1);
			}

			if( length > 0x400){
				length = 0x400;
			}

			data = new uint8_t[length];
			if(!data){
				exit(1);
			}

			cout << "Add Subtitle : ";
			if(read(0, data, length) <= 0){
				exit(1);
			}
		}
		else{
			uint32_t addLength = 0;
			uint8_t *newData = NULL;

			cout << "Subtitle Length : ";
			if(read(0, &addLength, 4) <= 0){
				exit(1);
			}

			if(length + addLength > 0x400){
				addLength = 0x400 - length;
			}

			newData = new uint8_t[length + addLength];
			if(!newData){
				exit(1);
			}

			cout << "Add Subtitle : ";
			memcpy(newData, data, length);

			if(read(0, newData + length, addLength) <= 0){
				exit(1);
			}

			delete[] data;
			data = newData;
		}
	}

	virtual void edit(){
		cout << "New Language : ";
		if(read(0, language, 4) <= 4){
			exit(1);
		}

		cout << "Edit data : ";
		if(read(0, data, length) <= 0){
			exit(1);
		}
	}

	virtual void play(){
		cout << "Not Playable" << endl;
	}

	virtual void remove(){
		delete[] data;
		data = NULL;
	}
};


class Metadata: public Clip{
public:
	uint8_t date[0x20];
	uint8_t owner[0x20];

	virtual void add(){
		cout << "Date of Creation : ";
		memset(date, 0, 0x20);
		if(read(0, date, 0x1f) <= 0){
			exit(1);
		}

		cout << "Owner of video : ";
		memset(owner, 0, 0x20);
		if(read(0, owner, 0x1f) <= 0){
			exit(1);
		}
	}

	virtual void edit(){
		cout << "Date of Creation : ";
		memset(date, 0, 0x20);
		if(read(0, date, 0x1f) <= 0){
			exit(1);
		}

		cout << "Owner of video : ";
		memset(owner, 0, 0x20);
		if(read(0, owner, 0x1f) <= 0){
			exit(1);
		}
	}

	virtual void play(){
		cout << "Not Playable" << endl;
	}

	virtual void remove(){
		return;
	}
};

void addMenu(){
	cout << "Clip Adding" << endl;
	cout << "1. Video Clip" << endl;
	cout << "2. Audio Clip" << endl;
	cout << "3. Subtitle Clip" << endl;
	cout << "4. Metadata Clip" << endl;
	cout << ">>> ";
}

void addClip(){
	if(clipCount >= 0x4000){
		exit(1);
	}

	addMenu();
	
	uint64_t choice;

	cin >> choice;

	switch(choice){
		case 1:
			clips[clipCount] = new VideoClip();
			clips[clipCount++]->add();
			break;
		case 2:
			clips[clipCount] = new AudioClip();
			clips[clipCount++]->add();
			break;
		case 3:
			if(subtitleIdx == -1){
				clips[clipCount] = new SubClip();
				subtitleIdx = clipCount++;
			}
			clips[subtitleIdx]->add();
			break;
		case 4:
			if(metaIdx == -1){
				clips[clipCount] = new Metadata();
				metaIdx = clipCount++;
			}
			clips[metaIdx]->add();
			break;
		default:
			cout << "Wrong Type!!!" << endl;
			break;
	}
}


void editClip(){
	uint64_t idx;

	cout << "Enter index : ";
	cin >> idx;

	if(clips[idx] == NULL){
		exit(1);
	}

	clips[idx]->edit();
}

void playClip(){
	uint64_t idx;

	cout << "Enter index : ";
	cin >> idx;

	if(clips[idx] == NULL){
		exit(1);
	}

	clips[idx]->play();
}

void remClip(){
	uint64_t idx;

	cout << "Enter index : ";
	cin >> idx;

	if(clips[idx] == NULL){
		exit(1);
	}

	clips[idx]->remove();
	clips[idx] = NULL;

	if(idx == subtitleIdx){
		subtitleIdx = -1;
	}

	else if(idx == metaIdx){
		metaIdx = -1;
	}
}

int main(){
	INIT();
	
	uint64_t choice;
	char movieName[0x100];

	cout << "What is your movie name?" << endl;
	read(0, movieName, 0xFF);

	while(1){
		menu();
	
		choice = 0;
		cin >> choice;

		switch( choice ){
			case 1:
				addClip();
				break;
			case 2:
				editClip();
				break;
			case 3:
				playClip();
				break;
			case 4:
				remClip();
				break;
			default:
				cout << "See you next time!!" << endl;
				return 0;
		}
	}
}
