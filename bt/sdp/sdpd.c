#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <endian.h>
#include "../utils/bt_utils.h"

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

#define PNP_SERVICE_RECORD "\x00\x00\x00\x00"

#define HID_SERVICE_RECORD_HANDLE "\x00\x01\x00\x00"
#define HID_SERVICE_RECORD_HANDLE_INT (0x00000100)
#define HID_SERVICE_RECORD "\x00\x01\x00\x01" HID_SERVICE_RECORD_HANDLE

#define PNP_RECORD "\x35\x00"

#define HID_RECORD "\x36\x02\x9a\x09\x00\x00\x0a\x00\x01\x00\x00\x09"\
    "\x00\x01\x35\x03\x19\x11\x24\x09\x00\x04\x35\x0d\x35\x06\x19\x01"\
    "\x00\x09\x00\x11\x35\x03\x19\x00\x11\x09\x00\x05\x35\x03\x19\x10"\
    "\x02\x09\x00\x06\x35\x09\x09\x65\x6e\x09\x00\x6a\x09\x01\x00\x09"\
    "\x00\x09\x35\x08\x35\x06\x19\x11\x24\x09\x01\x01\x09\x00\x0d\x35"\
    "\x0f\x35\x0d\x35\x06\x19\x01\x00\x09\x00\x13\x35\x03\x19\x00\x11"\
    "\x09\x01\x00\x25\x33\x4c\x6f\x67\x69\x74\x65\x63\x68\x20\x4b\x37"\
    "\x36\x30\x20\x53\x6f\x6c\x61\x72\x20\x4b\x65\x79\x62\x6f\x61\x72"\
    "\x64\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"\
    "\x20\x20\x20\x20\x20\x20\x20\x20\x09\x01\x01\x25\x12\x42\x6c\x75"\
    "\x65\x74\x6f\x6f\x74\x68\x20\x4b\x65\x79\x62\x6f\x61\x72\x64\x09"\
    "\x01\x02\x25\x10\x4c\x6f\x67\x69\x74\x65\x63\x68\x20\x20\x20\x20"\
    "\x20\x20\x20\x20\x09\x02\x01\x09\x01\x11\x09\x02\x02\x08\xc0\x09"\
    "\x02\x03\x08\x21\x09\x02\x04\x28\x01\x09\x02\x05\x28\x01\x09\x02"\
    "\x06\x36\x01\x85\x36\x01\x82\x08\x22\x26\x01\x7d\x05\x01\x09\x06"\
    "\xA1\x01\x85\x01\x75\x01\x95\x08\x05\x07\x19\xE0\x29\xE7\x15\x00"\
    "\x25\x01\x81\x02\x95\x01\x75\x08\x81\x03\x95\x05\x75\x01\x05\x08"\
    "\x19\x01\x29\x05\x91\x02\x95\x01\x75\x03\x91\x03\x95\x06\x75\x08"\
    "\x15\x00\x26\xFF\x00\x05\x07\x19\x00\x29\xFF\x81\x00\xC0\x05\x0C"\
    "\x09\x01\xA1\x01\x85\x02\x15\x00\x25\x01\x75\x01\x95\x12\x0A\x94"\
    "\x01\x0A\x92\x01\x0A\x83\x01\x0A\x23\x02\x0A\x8A\x01\x0A\x82\x01"\
    "\x0A\x21\x02\x0A\x30\x00\x0A\x25\x02\x0A\x23\x02\x0A\x27\x02\x09"\
    "\xB6\x09\xB5\x09\xB8\x09\xCD\x09\xE9\x09\xEA\x09\xE2\x81\x02\x95"\
    "\x01\x75\x06\x81\x03\xC0\x05\x0C\x09\x01\xA1\x01\x85\x03\x05\x01"\
    "\x09\x06\xA1\x02\x05\x06\x09\x20\x15\x00\x26\xFF\x00\x75\x08\x95"\
    "\x01\x81\x02\xC0\xC0\x05\x01\x09\x80\xA1\x01\x85\x04\x15\x00\x25"\
    "\x01\x75\x01\x95\x01\x09\x82\x81\x02\x95\x01\x75\x07\x81\x03\xC0"\
    "\x05\x0C\x09\x01\xA1\x01\x85\x05\x05\x01\x09\x06\xA1\x02\x06\x00"\
    "\xFF\x25\x01\x75\x01\x95\x02\x0A\x03\xFE\x0A\x04\xFE\x81\x02\x95"\
    "\x06\x81\x03\xC0\xC0\x05\x0C\x09\x01\xA1\x01\x85\xFF\x05\x06\x95"\
    "\x01\x75\x02\x19\x24\x29\x26\x81\x02\x75\x06\x81\x01\xC0\x06\x00"\
    "\xFF\x09\x01\xA1\x01\x85\x10\x75\x08\x95\x06\x15\x00\x26\xFF\x00"\
    "\x09\x01\x81\x00\x09\x01\x91\x00\xC0\x06\x00\xFF\x09\x02\xA1\x01"\
    "\x85\x11\x75\x08\x95\x13\x15\x00\x26\xFF\x00\x09\x02\x81\x00\x09"\
    "\x02\x91\x00\xC0\x05\x01\x09\x02\xA1\x01\x85\x12\x09\x01\xA1\x00"\
    "\x05\x09\x19\x01\x29\x10\x15\x00\x25\x01\x95\x10\x75\x01\x81\x02"\
    "\x05\x01\x16\x01\x80\x26\xFF\x7F\x75\x10\x95\x02\x09\x30\x09\x31"\
    "\x81\x06\x15\x81\x25\x7F\x75\x08\x95\x01\x09\x38\x81\x06\x05\x0C"\
    "\x0A\x38\x02\x95\x01\x81\x06\xC0\xC0\x09\x02\x07\x35\x08\x35\x06"\
    "\x09\x04\x09\x09\x01\x00\x09\x02\x08\x28\x00\x09\x02\x09\x28\x01"\
    "\x09\x02\x0a\x28\x01\x09\x02\x0c\x09\x26\x20\x09\x02\x0d\x28\x00"\
    "\x09\x02\x0e\x28\x01\x09\x02\x0f\x09\x0c\x60\x09\x02\x10\x09\x00"\
    "\x00"

#define L2CAP_RECORD \
    "\x36\x03\x2e\x36\x00\x8e\x09\x00\x00\x0a\x00\x00\x00\x00\x09\x00"\
    "\x01\x35\x03\x19\x10\x00\x09\x00\x04\x35\x0d\x35\x06\x19\x01\x00"\
    "\x09\x00\x01\x35\x03\x19\x00\x01\x09\x00\x05\x35\x03\x19\x10\x02"\
    "\x09\x00\x06\x35\x09\x09\x65\x6e\x09\x00\x6a\x09\x01\x00\x09\x00"\
    "\x09\x35\x08\x35\x06\x19\x01\x00\x09\x01\x00\x09\x01\x00\x25\x25"\
    "\x4c\x6f\x67\x69\x74\x65\x63\x68\x20\x4b\x37\x36\x30\x20\x53\x6f"\
    "\x6c\x61\x72\x20\x4b\x65\x79\x62\x6f\x61\x72\x64\x20\x20\x20\x20"\
    "\x20\x20\x20\x20\x20\x09\x01\x01\x25\x12\x42\x6c\x75\x65\x74\x6f"\
    "\x6f\x74\x68\x20\x4b\x65\x79\x62\x6f\x61\x72\x64\x09\x02\x00\x35"\
    "\x03\x09\x01\x00" HID_RECORD


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
        case PNP_UUID:
        {
            record = PNP_SERVICE_RECORD;
            record_size = sizeof(PNP_SERVICE_RECORD) - 1;
        }
        break;
        
        case HID_UUID:
        {
            record = HID_SERVICE_RECORD;
            record_size = sizeof(HID_SERVICE_RECORD) - 1;
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
            record = HID_RECORD;
            record_size = sizeof(HID_RECORD) - 1;
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
        case PNP_UUID:
        default:
        {
            record = PNP_RECORD;
            record_size = sizeof(PNP_RECORD) - 1;
        }
        break;
        
        case L2CAP_UUID:
        {
            record = L2CAP_RECORD;
            record_size = sizeof(L2CAP_RECORD) - 1;
        }
        break;

        case HID_UUID:
        {
            record = "\x36\x02\x9d" HID_RECORD;
            record_size = sizeof(HID_RECORD) + 3 - 1;
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
