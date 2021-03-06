#include <string.h>
#include "jansson.h"
#include "pomelo-protocol/message.h"
#include "pomelo-protobuf/pb.h"
#include "log.h"

pc_buf_t pc__pb_encode(const json_t *msg, const json_t *pb_def) {
  pc_buf_t buf, json_buf;
  memset(&buf, 0, sizeof(pc_buf_t));
  memset(&json_buf, 0, sizeof(pc_buf_t));

  json_buf = pc__json_encode(msg);

  if(json_buf.len == -1) {
    LOGD( "Fail to encode json for protobuf evaluate.\n");
    goto error;
  }

  size_t eval_size = json_buf.len * PC_PB_EVAL_FACTOR;

  buf.base = malloc(eval_size);

  if(buf.base == NULL) {
    LOGD( "Fail to malloc for protobuf encode.\n");
    buf.len = -1;
    goto error;
  }

  size_t written = 0;
  if(!pc_pb_encode((uint8_t *)buf.base, eval_size, &written,
                   (json_t *)pb_def, (json_t *)msg)) {
    LOGD( "Fail to do protobuf encode.\n");
    goto error;
  }

  buf.len = written;

  free(json_buf.base);

  return buf;

error:
  if(buf.len == -1) free(buf.base);
  if(json_buf.len == -1) free(json_buf.base);

  return buf;
}

json_t *pc__pb_decode(const char *data, size_t offset, size_t len,
                      const json_t *pb_def) {

  json_t *result = json_object();
  if(result == NULL) {
    LOGD( "Fail to create json_t for protobuf decode.\n");
    goto error;
  }

  if(!pc_pb_decode((uint8_t *)(data + offset), len, (json_t *)pb_def, result)) {
    LOGD( "Fail to do protobuf decode.\n");
    goto error;
  }

  return result;

error:
  if(result) json_decref(result);

  return NULL;
}
