/* aeron extension for PHP (c) 2022 nomnoms12 */

#ifndef PHP_AERON_H
# define PHP_AERON_H

extern zend_module_entry aeron_module_entry;
# define phpext_aeron_ptr &aeron_module_entry

# define PHP_AERON_VERSION "0.2.1"

# if defined(ZTS) && defined(COMPILE_DL_AERON)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_AERON_H */
