#include "Motion.h"

static void Motion_AnalyseFrame([[maybe_unused]] FILE *ActFile, size_t Size, uint8_t *Data) {
	while (Data < Data + Size) {
		
	}

}

void Motion_Analyse(FILE *ActFile, uint8_t *Data, size_t Frames) {
	for (size_t i = 0; i < Frames; i++) {
		if (Data[0] & 0x80) {
			printf("Taking the 0x80 path\n");
			Motion_AnalyseFrame(ActFile, Data[0] & 0x7F, Data + 1);
		} else {
			printf("Unimplemented.\n");
		}
	}
}
