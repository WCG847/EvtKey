#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include "EKey.h"
#include "Motion.h"
#pragma pack(push, 1)

typedef struct tagEKEY_TABLE {
	uint32_t FieldCount :16;
} EKEY_TABLE;

typedef struct tagEKEY_TABLE_FIELDS {
	uint32_t Act;
	uint32_t Loc;
	uint32_t Frames;
	uint32_t Resolved;
} EKEY_TABLE_FIELDS;

typedef struct tagEKEY {
	uint8_t Data[256];
	struct TABLE {
		EKEY_TABLE Head;
		EKEY_TABLE_FIELDS Tracks[];
	} Table;
} EKEY;
#pragma pack(pop)

static void EKey_AnalyseTracks(const EKEY_TABLE_FIELDS *Tracks, size_t Count) {
	EKEY_TABLE_FIELDS Track;
	[[maybe_unused]] void *Data = (unsigned char *)Tracks + (sizeof(EKEY_TABLE_FIELDS) * Count);
	FILE *Trk = fopen("Tracks.trk", "w");
	CreateDirectoryW(L"Acts", nullptr);
	char Buf[160];
	fprintf(Trk, "[\n");
	for (size_t Index = 0; Index < Count; Index++) {
		Track = Tracks[Index];
		Track.Act &= 0x3fffffff; // yukes
		uint16_t Asset = Track.Act >> 16;
		uint8_t Category = Track.Act >> 8;
		uint8_t Entry = Track.Act & 0xFF;
		printf("[%llu]\n \n{\n Asset: [%hu],\n Category: [%hhu],\n Entry: [%hhu],\n Location: [%#x],\n Frames: [%u]\n}\n", Index, Asset, Category,
			   Entry, Track.Loc, Track.Frames);
		fprintf(Trk, "\t{\n Asset: [%hu],\n Category: [%hhu],\n Entry: "
			   "[%hhu],\n Location: [%#x],\n Frames: [%u]\n\t}\n\n",
			   Asset, Category, Entry, Track.Loc, Track.Frames);
		sprintf(Buf, "Acts\\%hu_%hhu_%hhu.act", Asset, Category, Entry);
		FILE *Action = fopen(Buf, "w");
		// Motion_Analyse(Action, (unsigned char *)Data + Track.Loc, Track.Frames);

		fflush(Trk);
		fclose(Action);

		
	}
	fprintf(Trk, "]");
	fclose(Trk);

}

static void EKey_Analyse(const EKEY *Cnk) {
	printf("EKEY STREAM\n\n");
	for (int i = 0; i < 256; i++) {
		printf("%02x ", Cnk->Data[i]);
	}
	printf("\n\nTracks: \n");
	printf("Tracks found: [%u]\n", Cnk->Table.Head.FieldCount);
	EKey_AnalyseTracks(Cnk->Table.Tracks, Cnk->Table.Head.FieldCount);
}

void EKey_Start(void *Buffer) {
	EKey_Analyse(Buffer);
}
