#ifndef POMELO_LOG_H
#define POMELO_LOG_H
#ifdef __cplusplus
extern "C" {
#endif

#include <android/log.h>
#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif /* POMELO_CLIENT_H */
