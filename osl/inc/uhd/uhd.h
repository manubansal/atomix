/**
Atomix project, uhd.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef UHD_H
#define UHD_H

#define VITA49IF_NUM_FIELDS 6

#define VITA49IF_PACKET_TYPE_IFDATA_SI 1

#define VITA49IF_DATA_TSI_NONE   0
#define VITA49IF_DATA_TSI_UTC    1
#define VITA49IF_DATA_TSI_GPS    2
#define VITA49IF_DATA_TSI_OTHER  3

#define VITA49IF_DATA_TSF_NONE         0
#define VITA49IF_DATA_TSF_SAMPLECOUNT  1
#define VITA49IF_DATA_TSF_REALTIME     2
#define VITA49IF_DATA_TSF_FREERUNNING  3

typedef struct vita49if_data_hdr_
{
	uint8_t  packet_type  :  4;
	uint8_t  c            :  1;   // packet has classid?
	uint8_t  t            :  1;   // packet has trailer?
	uint8_t  reserved     :  2;   // reserved
	uint8_t  tsi          :  2;   // timestamp integer type
	uint8_t  tsf          :  2;   // timestamp fractional type
	uint8_t  packet_count :  4;   // sequence number
	uint16_t packet_size  : 16;   // # of 32 bit words in the packet
} vita49if_data_hdr;

typedef struct vita49if_data_classid_
{
	uint8_t  reserved   :  8;
	uint32_t oui        : 24;
	uint16_t info_class : 16;
	uint16_t pkt_class  : 16;
} vita49if_data_classid;

typedef struct vita49if_data_payload_
{
	uint8_t* buff;
	uint32_t len;
} vita49if_data_payload;

typedef struct vita49if_data_
{
	vita49if_data_hdr hdr;
	uint32_t streamid;
	vita49if_data_classid classid;
	uint32_t time_s;
	uint64_t time_frac_s;
	vita49if_data_payload payload;
	uint32_t trailer;
} vita49if_data;

typedef struct sample16_
{
	int16_t i;
	int16_t q;
} sample16;

int32_t vita49if_data_parse(vita49if_data* data, uint8_t* buff, uint32_t len);

#endif
