#include <stdint.h>
#include <string.h>

#include <uhd/uhd.h>

int32_t vita49if_data_parse(vita49if_data* data, uint8_t* buff, uint32_t len)
{
	uint32_t size, i;

	memset(data, 0, sizeof(vita49if_data));

	// Fill in the header
	size = sizeof(vita49if_data_hdr);
	if (len >= size)
	{
		memcpy(&data->hdr, buff, size);
		buff += size;
		len -= size;
	}
	else
		return -1;

	// Arrays used to fill in the optional fields
	uint8_t fields_present[] = 
	{
		(data->hdr.packet_type == VITA49IF_PACKET_TYPE_IFDATA_SI),
		data->hdr.c,
		(data->hdr.tsi > 0),
		(data->hdr.tsf > 0),
		1,                    // assumes there is always a payload
		data->hdr.t
	};

	uint8_t* fields[] = 
	{ 
		(uint8_t*)&data->streamid,
		(uint8_t*)&data->classid,
		(uint8_t*)&data->time_s,
		(uint8_t*)&data->time_frac_s,
		(uint8_t*)&(data->payload.buff),
		(uint8_t*)&data->trailer,
	};

	uint32_t field_lengths[] = 
	{ 
		sizeof(uint32_t),
		sizeof(vita49if_data_classid),
		sizeof(uint32_t),
		sizeof(uint64_t),
		(data->hdr.packet_size * 4),            // maximum payload length, headers still must be removed
		sizeof(uint32_t)
	};

	// Remove headers from payload length
	field_lengths[4] -= sizeof(vita49if_data_hdr);
	for (i = 0; i < VITA49IF_NUM_FIELDS; i++)
	{
		if (i != 4 && fields_present[i])
			field_lengths[4] -= field_lengths[i];
	}
	data->payload.len = field_lengths[4];

	// Fill in the optional fields
	for (i = 0; i < VITA49IF_NUM_FIELDS; i++)
	{
		if (fields_present[i])
		{
			size = field_lengths[i];
			if (len >= size)
			{
				// payload
				if (i == 4)
				{
					*((uint8_t**)fields[i]) = buff;
					buff += size;
					len -= size;
				}
				// other
				else
				{
					memcpy(fields[i], buff, size);
					buff += size;
					len -= size;
				}
			}
			else
				return -2;
		}
	}

	return 0;
}
