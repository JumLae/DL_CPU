#ifndef DL_CPU_DL_CPU_H_
#define DL_CPU_DL_CPU_H_

#include <stdint.h>

#if defined(DLCPU_BUILD)
#if defined(_M_IX86)
#define DLCPU_API
#else
#define DLCPU_API __declspec(dllexport)
#endif
#else
#define DLCPU_API __declspec(dllimport)
#endif

#define DLCPU_CALL __stdcall

#ifdef __cplusplus
extern "C" {
#endif

typedef enum DLCPU_Status {
    DLCPU_OK = 0,
    DLCPU_ERR_INVALID_ARG = 1,
    DLCPU_ERR_INTERNAL = 2,
    DLCPU_ERR_BUFFER_TOO_SMALL = 3
} DLCPU_Status;

DLCPU_API int32_t DLCPU_CALL DLCPU_GetVersion(int32_t* major, int32_t* minor, int32_t* patch);

DLCPU_API int32_t DLCPU_CALL DLCPU_GetLastErrorMessage(char* buf, int32_t buf_size);

#ifdef __cplusplus
}
#endif

#endif  /* DL_CPU_DL_CPU_H_ */
