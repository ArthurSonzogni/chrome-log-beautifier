#ifndef _diffusion_protocol_dpt_h_
#define _diffusion_protocol_dpt_h_ 1

#include "buf.h"
#include "types/session_types.h"
#include "types/topic_types.h"
#include "types/transport_types.h"
#include "internal/error_int.h"

int dpt_validate_protocol(int protocol, DIFFUSION_ERROR_T *error);
int dpt_validate_version(const TRANSPORT_T *transport, int version, DIFFUSION_ERROR_T *error);
int dpt_validate_response_code(int response_code, DIFFUSION_ERROR_T *error);

int is_recoverable_connection_response(const CONNECTION_RESPONSE_CODE_T code);
const char *connection_response_code_str(const CONNECTION_RESPONSE_CODE_T code);
ERROR_CODE_T connection_response_code_to_diffusion_error(const CONNECTION_RESPONSE_CODE_T code);

int dpt_parse_session_identity(char *data, SESSION_ID_T **session_id);

MESSAGE_T *dpt_message_create(const MESSAGE_TYPE_T type);
void dpt_message_free(MESSAGE_T *message);

LIST_T *dpt_read_headers(const char *data, char **hdr_end_ptr);
char *dpt_read_payload(const char *data, const char *end_ptr, const uint8_t encoding, size_t *payload_len);

size_t dpt_decode_length(const char *data, const unsigned int msg_length_size);
void dpt_encode_length(char *dst, const size_t len, const unsigned int msg_length_size);
void dpt_populate_msg_length_size(char *ptr, const unsigned int msg_length_size, const size_t len);

char *dpt_encode_credentials(const CREDENTIALS_T *creds);

int dpt_close_msg_in_buf(const BUF_T *buf, const unsigned int msg_prefix_len);

#endif
