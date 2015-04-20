/**
Atomix project, eth.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef ETH_H
#define ETH_H


#define VITA_HDR_LEN 4
#define VITA_TRL_LEN 4
#define ETH_HDR_LEN 14
#define IP_HDR_LEN 20
#define UDP_HDR_LEN 8
#define TOTAL_HDR_LEN (ETH_HDR_LEN+IP_HDR_LEN+UDP_HDR_LEN)
#define ETH_CRC_LEN 4

#define VITA_HDR_WRD 1
#define VITA_TRL_WRD 1

#define ETH_DEST_IDX 0
#define UDP_DEST_IDX 36
#define IP_LEN_IDX 16
#define UDP_LEN_IDX 38
#define VITA_LEN_IDX 44
#define VITA_SEQNUM_IDX 43


typedef struct eth_hdr_
{
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t type_length;
} eth_hdr;

int32_t eth_init(uint8_t* host_mac, uint8_t* amc_mac, uint8_t* wire_mac);
int32_t eth_send(uint8_t* payload, uint32_t payload_len);
int32_t eth_recv(uint8_t** payload, uint32_t* payload_len, uint8_t blocking);

//begin from https://www.cs.utah.edu/~swalton/listings/sockets/programs/part1/chap3/snooper.c

#define IP_SIZE		4
#define ETH_SIZE	6

typedef enum { eETH_ADDR, eIP_ADDR } EAddress;

typedef unsigned int uint;
typedef unsigned char uchar;

/*--------------------------------------------------------------------*/
/* Ethernet Frame                                                     */
/*                                                                    */
/* This structure defines the fields within the ethernet frame. Since */
/* this programs gets the lowest-level packet, fragmented packets are */
/* not reassembled.  The first few fields contain the MAC addresses   */
/* of the source and destination. Note that this structure is set for */
/* little-endian format.                                              */
/*--------------------------------------------------------------------*/
struct ip_packet {
	struct {
		uchar dst_eth[ETH_SIZE];
		uchar src_eth[ETH_SIZE];
		uchar __unknwn[2];
	} hw_header;             /* hardware header */
    uint header_len:4;       /* header length in words in 32bit words */
    uint version:4;          /* 4-bit version */
    uint serve_type:8;       /* how to service packet */
    uint packet_len:16;      /* total size of packet in bytes */
    uint ID:16;              /* fragment ID */
    uint frag_offset:13;     /* to help reassembly */
    uint more_frags:1;       /* flag for "more frags to follow" */
    uint dont_frag:1;        /* flag to permit fragmentation */
    uint __reserved:1;       /* always zero */
    uint time_to_live:8;     /* maximum router hop count */
    uint protocol:8;         /* ICMP, UDP, TCP */
    uint hdr_chksum:16;      /* ones-comp. checksum of header */
    uchar IPv4_src[IP_SIZE]; /* IP address of originator */
    uchar IPv4_dst[IP_SIZE]; /* IP address of destination */
    uchar options[0];        /* up to 40 bytes */
    uchar data[0];           /* message data up to 64KB */
};

//end from https://www.cs.utah.edu/~swalton/listings/sockets/programs/part1/chap3/snooper.c

typedef struct ip_packet NET_ip_packet;

typedef Uint16 u_int16_t;

struct udphdr {
  u_int16_t source; 
  u_int16_t dest; 
  u_int16_t len; 
  u_int16_t check; 
};

typedef struct udphdr NET_udphdr;

typedef struct {
  char bytes[1600];
} NET_t_MSS;

//void eth_getMACAddress(uint8_t* macAddress);
void eth_printMAC(uint8_t * mac);
void eth_printUDPPayloadChars(NET_ip_packet * ip_pkt, Uint32 nChars);

#endif
