
#ifndef PHP_EET_H
#define PHP_EET_H

extern zend_module_entry eet_module_entry;
#define phpext_eet_ptr &eet_module_entry

#ifdef PHP_WIN32
#	define PHP_EET_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_EET_API __attribute__ ((visibility("default")))
#else
#	define PHP_EET_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_EET_EET_FILE "Eet_File"

PHP_MINIT_FUNCTION(eet);
PHP_MSHUTDOWN_FUNCTION(eet);
PHP_RINIT_FUNCTION(eet);
PHP_RSHUTDOWN_FUNCTION(eet);
PHP_MINFO_FUNCTION(eet);

PHP_FUNCTION(confirm_eet_compiled);	/* For testing, remove later. */

PHP_FUNCTION(eet_open);
PHP_FUNCTION(eet_close);
PHP_FUNCTION(eet_file_get);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(eet)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(eet)
*/

/* In every utility function you add that needs to use variables 
   in php_eet_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as EET_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define EET_G(v) TSRMG(eet_globals_id, zend_eet_globals *, v)
#else
#define EET_G(v) (eet_globals.v)
#endif

#endif	/* PHP_EET_H */

