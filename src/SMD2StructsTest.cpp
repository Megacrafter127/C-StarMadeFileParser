/*
 * SMD2Structs.cpp
 *
 *  Created on: 08.02.2016
 *      Author: Megacrafter127
 */

#include "SMD2StructStreaming.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace smd2;

int parse(char*);

int main(int count,char** arg) {
	if(count==2) {
		return parse(arg[1]);
	}
	char buf[1024];
	cin.getline(buf,sizeof(buf));
	while(strlen(buf)>1) {
		int l=parse(buf);
		if(l) cout<<"Error: "<<l<<endl;
		cin.getline(buf,sizeof(buf));
	}
	return 0;
}

int parse(char* file) {
	ifstream in(file,ios::binary);
	struct smd2Head head;
	readHead(&head,in);
	struct segmentV0 *segments[4096];
	size_t len;
	for(len=0;len<4096 && !in.eof();len++) {
		segments[len]=(struct segmentV0*)malloc(sizeof(struct segmentV0));
		readSegmentV0(segments[len],in);
	} for(;len<4096;len++) {
		segments[len]=NULL;
	}
	struct smd2Head::segmentIndex *current;
	struct segmentV0 *cs;
	for(size_t i=0;i<16;i++) for(size_t j=0;j<16;j++) for(size_t k=0;k<16;k++) {
		current=&(head.indizes[i][j][k]);
		if(current->segmentID==-1) continue;
		if(current->segmentID<0||current->segmentID>4095) {
			cout<<"Illegal index"<<endl;
		} else {
			cs=segments[current->segmentID];
			cout<<"Length difference 1: "<<current->segmentLen-cs->head.inlen<<endl;
			cout<<"Length difference 2: "<<current->segmentLen-((struct segmentV1*)cs)->head.inlen<<endl;
		}
	}
	for(size_t i=0;i<16;i++) for(size_t j=0;j<16;j++) for(size_t k=0;k<16;k++) {
		current=&(head.indizes[i][j][k]);
		if(current->segmentID==-1) continue;
		if(current->segmentID<0||current->segmentID>4095) {
			cout<<"Illegal index"<<endl;
		} else {
			cs=segments[current->segmentID];
			cout<<"Timestamp difference 1: "<<head.timestamps[i][j][k]-cs->head.timestamp<<endl;
			cout<<"Timestamp difference 2: "<<head.timestamps[i][j][k]-((struct segmentV1*)cs)->head.timestamp<<endl;
		}
	}
	for(len=0;len<4096;len++) {
		if(segments[len]) {
			free(segments[len]);
			segments[len]=NULL;
		}
	}
	return 0;
}

