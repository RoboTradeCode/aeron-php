/* aeron extension for PHP */

#ifndef PHP_AERON_H
# define PHP_AERON_H

#include "aeronc.h"

extern zend_module_entry aeron_module_entry;
# define phpext_aeron_ptr &aeron_module_entry

# define PHP_AERON_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_AERON)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif  /* PHP_AERON_H */
