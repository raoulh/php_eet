
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_eet.h"

#include <Eet.h>

const zend_function_entry eet_functions[] = {
	PHP_FE(confirm_eet_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in eet_functions[] */
};

zend_module_entry eet_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"eet",
	eet_functions,
	PHP_MINIT(eet),
	PHP_MSHUTDOWN(eet),
	PHP_RINIT(eet),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(eet),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(eet),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_EET
ZEND_GET_MODULE(eet)
#endif

PHP_MINIT_FUNCTION(eet)
{
	eet_init();
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(eet)
{
	eet_shutdown();
	return SUCCESS;
}

PHP_RINIT_FUNCTION(eet)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(eet)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(eet)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "eet support", "enabled");
	php_info_print_table_end();
}

/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* proto string confirm_eet_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_eet_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "eet", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
