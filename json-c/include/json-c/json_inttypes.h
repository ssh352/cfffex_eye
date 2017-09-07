
#ifndef _json_inttypes_h_
#define _json_inttypes_h_

#ifndef _MSC_VER
#include "json_config.h"
#endif

#if defined(_MSC_VER)

#include <stdint.h>
#define PRId64 "I64d"
#define SCNd64 "I64d"

#define INFINITY INT64_MAX
#define NAN INT64_MAX

#define HAVE_DECL__ISNAN 1
#define HAVE_DECL__FINITE 1
#define HAVE_DECL_NAN 0
#define HAVE_DECL_INFINITY 0

#define InterlockedCompareExchange _InterlockedCompareExchange

#else

#ifdef JSON_C_HAVE_INTTYPES_H
#include <inttypes.h>
#endif
/* inttypes.h includes stdint.h */

#endif

#endif
