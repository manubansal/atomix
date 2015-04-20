#include <stdio.h>
#include <osl/inc/swpform.h>
#include <osl/vitdec/inc/vitdec/vitdec_profile.h>
#include <ti/csl/csl_tsc.h>

int main() {

//void vitdec_wifiGee_setViterbiDecodingMap(
//	IN  Uint32 numInfoBits,
//	IN  Uint32 numDataBitsPerSymbol,
//	IN  Uint32 frameLength, 
//	IN  Uint32 convergenceLength,
//	OUT Vitdec_ViterbiDecodingMap *vitdecMap
//	){

  Vitdec_ViterbiDecodingMap vitdecMap;

  //Uint32 datalen_bytes = 200;		//number of data bytes
  Uint32 datalen_bytes = 2;		//number of data bytes

  Uint32 nibits = (datalen_bytes + 4) * 8;	//number of info bits, 4 bytes is for CRC
  Uint32 ndbps = 216;
  Uint32 f = 54;
  Uint32 c = 36;

  vitdec_wifiGee_setViterbiDecodingMap(nibits, ndbps, f, c, &vitdecMap);

  return 0;
}
