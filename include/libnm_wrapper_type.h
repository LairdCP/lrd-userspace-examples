#ifndef __LIBNM_WRAPPER_TYPE_H__
#define __LIBNM_WRAPPER_TYPE_H__

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBNM_WRAPPER_MAX_UUID_LEN 64
#define LIBNM_WRAPPER_MAX_NAME_LEN 64
#define LIBNM_WRAPPER_MAX_PATH_LEN 256
#define LIBNM_WRAPPER_MAX_FREQUENCY_LIST_LEN 256
#define LIBNM_WRAPPER_MAX_MAC_ADDR_LEN	6
#define LIBNM_WRAPPER_MAX_ADDR_NUM	3


typedef enum _LIBNM_WRAPPER_ERR {
	LIBNM_WRAPPER_ERR_SUCCESS = 0,
	LIBNM_WRAPPER_ERR_FAIL,
	LIBNM_WRAPPER_ERR_INVALID_NAME,
	LIBNM_WRAPPER_ERR_INVALID_CONFIG,
	LIBNM_WRAPPER_ERR_INVALID_DELETE,
	LIBNM_WRAPPER_ERR_POWERCYCLE_REQUIRED,
	LIBNM_WRAPPER_ERR_INVALID_PARAMETER,
	LIBNM_WRAPPER_ERR_INVALID_EAP_TYPE,
	LIBNM_WRAPPER_ERR_INVALID_WEP_TYPE,
	LIBNM_WRAPPER_ERR_INVALID_FILE,
	LIBNM_WRAPPER_ERR_INSUFFICIENT_MEMORY,
	LIBNM_WRAPPER_ERR_NOT_IMPLEMENTED,
	LIBNM_WRAPPER_ERR_NO_HARDWARE,
	LIBNM_WRAPPER_ERR_INVALID_VALUE
} LIBNM_WRAPPER_ERR;

typedef struct _LIBNM_WRAPPER_STATE_MONITOR_CALLBACK_ST
{
	int (*callback)(int state, int reason);
	void *arg;
}LIBNM_WRAPPER_STATE_MONITOR_CALLBACK_ST;

static inline void safe_strncpy(char *dest, const char *src, int n)
{
	size_t i, m = src ? strlen(src) : 0;

	m = m > n ? n : m;

	for (i = 0; i < m; i++)
		dest[i] = src[i];

	///A null byte will be added
	if(i < n) dest[i] = '\0';
	else  dest[n-1] = '\0';
}

#ifdef __cplusplus
}
#endif

#endif //__LIBNM_WRAPPER_TYPE_H__
