#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <endian.h>
#include "../utils/bt_utils.h"

#include "sdpd.h"

#ifndef __packed
#define __packed __attribute__((packed))
#endif

#define INPUT_SIZE 64
#define CHUNK_SIZE 0x26

#define SERVICE_SEARCH_RQ_PDU 0x02
#define SERVICE_ATTRIBUTE_RQ_PDU 0x04
#define SERVICE_SEARCH_ATTRIBUTE_RQ_PDU 0x06

#define PDU_SS_ID 0x03
#define PDU_SA_ID 0x05
#define PDU_SSA_ID 0x07

#define PNP_UUID 0x0012
#define L2CAP_UUID 0x0001
#define HID_UUID 0x2411



struct SDP_request_header
{
    uint8_t pdu_ID;
    uint16_t transaction_ID;
    uint16_t parameter_length;
}__packed;

struct SDP_service_search_request
{
    struct SDP_request_header header;
    uint8_t data_type;
    uint8_t data_length;
    uint8_t uuid_data_type;
    uint16_t uuid;
    uint16_t max_record_count;
    uint8_t continuation_state;
}__packed;

struct SDP_service_search_attr_request
{
    struct SDP_request_header header;
    uint8_t data_type;
    uint8_t data_length;
    uint8_t uuid_data_type;
    uint16_t uuid;
    uint16_t max_record_count;
    uint8_t att_type;
    uint8_t att_length;
    uint8_t range_type;
    uint16_t range_from;
    uint16_t range_to;
    uint8_t continuation_state;
    uint16_t continuation_state_data;
}__packed;

struct SDP_service_attr_request
{
    struct SDP_request_header header;
    uint32_t service_record_handle;
    uint16_t max_record_count;
    uint8_t att_type;
    uint8_t att_length;
    uint8_t range_type;
    uint16_t range_from;
    uint16_t range_to;
    uint8_t continuation_state;
    uint16_t continuation_state_data;
}__packed;

#define GET_PDU_ID(input) (((struct SDP_request_header*)input)->pdu_ID)

static void sendExtendedResponse(int socket,
    uint8_t header, uint16_t transaction_ID,
    const char *record, uint16_t record_size,
    uint8_t continuation_state, uint16_t continuation_state_data)
{
    uint16_t parameter_size, record_size_be;
    bool chunk = false;
    int offset = 0;
    struct iovec iov[7];


    if (continuation_state != 0)
    {
        offset = continuation_state_data;
        record = record + offset;
        record_size -= offset;
    }

    if (record_size > CHUNK_SIZE)
    {
        chunk = true;
        continuation_state_data = offset + CHUNK_SIZE;
        record_size = CHUNK_SIZE;
    }
    
    parameter_size = htobe16(
                        sizeof(record_size) +
                        record_size +
                        sizeof(continuation_state) +
                        (chunk ?sizeof(continuation_state_data) : 0));

    record_size_be = htobe16(record_size);

    continuation_state = chunk ? sizeof(continuation_state_data) : 0;

    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &transaction_ID;
    iov[1].iov_len = sizeof(transaction_ID);
    iov[2].iov_base = &parameter_size;
    iov[2].iov_len = sizeof(parameter_size);
    iov[3].iov_base = &record_size_be;
    iov[3].iov_len = sizeof(record_size_be);
    iov[4].iov_base = (void*)record;
    iov[4].iov_len = record_size;
    iov[5].iov_base = &continuation_state;
    iov[5].iov_len = sizeof(continuation_state);

    if(chunk)
    {
        iov[6].iov_base = &continuation_state_data;
        iov[6].iov_len = sizeof(continuation_state_data);
    }

    writev(socket, iov, chunk ? 7 : 6);
}

static void respondServiceSearchRequest(int socket, const char *input)
{
    struct SDP_service_search_request *request =
                (struct SDP_service_search_request*)input;
    
    const char *record;
    uint16_t record_size, record_size_be;
    uint8_t header = PDU_SS_ID;
    uint8_t continuation_state = 0;
    struct iovec iov[5];

    switch(request->uuid)
    {        
        case HID_UUID:
        {
            record = HID_SERVICE_RECORD;
            record_size = sizeof(HID_SERVICE_RECORD) - 1;
        }
        break;
        
        case PNP_UUID:
        {
            record = PNP_SERVICE_RECORD;
            record_size = sizeof(PNP_SERVICE_RECORD) - 1;
        }
        break;

        default:
        {
            record = EMPTY_SERVICE_RECORD;
            record_size = sizeof(EMPTY_SERVICE_RECORD) - 1;
        }
        break;
    }

    record_size_be = htobe16(record_size + 1);

    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &request->header.transaction_ID;
    iov[1].iov_len = sizeof(request->header.transaction_ID);
    iov[2].iov_base = &record_size_be;
    iov[2].iov_len = sizeof(record_size_be);
    iov[3].iov_base = (void*)record;
    iov[3].iov_len = record_size;
    iov[4].iov_base = &continuation_state;
    iov[4].iov_len = sizeof(continuation_state);

    writev(socket, iov, 5);
}

void respondServiceAttributeRequest(int socket, const char *input)
{
    const char *record = NULL;
    uint16_t record_size = 0;

    struct SDP_service_attr_request *request =
        (struct SDP_service_attr_request*)input;

    switch(request->service_record_handle)
    {
        case HID_SERVICE_RECORD_HANDLE_INT:
        {
            record = HID_ATT_RECORD;
            record_size = sizeof(HID_ATT_RECORD) - 1;
        }
        break;
        
        case PNP_SERVICE_RECORD_HANDLE_INT:
        {
            record = PNP_ATT_RECORD;
            record_size = sizeof(PNP_ATT_RECORD) - 1;
        }
        break;
        
        default:
            break;
    }

    sendExtendedResponse(socket,
        PDU_SA_ID, request->header.transaction_ID,
        record, record_size,
        request->continuation_state, request->continuation_state_data);
}

void respondServiceSearchAttributeRequest(int socket, const char *input)
{
    const char *record = NULL;
    uint16_t record_size = 0;

    struct SDP_service_search_attr_request *request =
                (struct SDP_service_search_attr_request*)input;

    // In case of UUID of not supported length
    if (request->uuid_data_type != 0x19)
    {
        request->uuid = 0;
        request->continuation_state = 0;
        request->continuation_state_data = 0;
    }

    switch(request->uuid)
    {
        case L2CAP_UUID:
        {
            record = L2CAP_SSA_RECORD;
            record_size = sizeof(L2CAP_SSA_RECORD) - 1;
        }
        break;
        
        case HID_UUID:
        {
            record = HID_SSA_RECORD;
            record_size = sizeof(HID_SSA_RECORD) - 1;
        }
        break;
        
        case PNP_UUID:
        {
            record = PNP_SSA_RECORD;
            record_size = sizeof(PNP_SSA_RECORD) - 1;
        }
        break;
        
        default:
        {
            record = EMPTY_RECORD;
            record_size = sizeof(EMPTY_RECORD) - 1;
        }
        break;
    }

    sendExtendedResponse(socket,
        PDU_SSA_ID, request->header.transaction_ID,
        record, record_size,
        request->continuation_state, request->continuation_state_data);
}

void bt_sdp_start()
{
	char input[INPUT_SIZE];
    int serverSocket = -1;

    while(1)
    {
        int socket = bt_wait_for_connection_on_psm(&serverSocket, 1);

        if(socket >= 0)
        {
            while(read(socket, input, INPUT_SIZE) >= 0)
            {
                switch(GET_PDU_ID(input))
                {
                    case SERVICE_SEARCH_RQ_PDU:
                    {
                        respondServiceSearchRequest(socket, input);
                    }
                    break;

                    case SERVICE_ATTRIBUTE_RQ_PDU:
                    {
                        respondServiceAttributeRequest(socket, input);
                    }
                    break;

                    case SERVICE_SEARCH_ATTRIBUTE_RQ_PDU:
                    {
                        respondServiceSearchAttributeRequest(socket, input);
                    }
                    break;

                    default:
                        break;
                }
            }

            close(socket);
        }
    }
}

int main(int argc, char *argv[])
{
	bt_sdp_start();

	return 0;
}
