
#ifndef RLELIST_ASCIIARTTOOL_H
#define RLELIST_ASCIIARTTOOL_H

#include "RLEList.h"

RLEList asciiArtRead(FILE* in_stream);
RLEListResult asciiArtPrint(RLEList list, FILE *out_stream);
RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream);


#endif //RLELIST_ASCIIARTTOOL_H
