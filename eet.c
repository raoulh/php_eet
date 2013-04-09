
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_eet.h"

#include <Eet.h>

int le_eet_file;

const zend_function_entry eet_functions[] = {
        PHP_FE(eet_open, NULL)
        PHP_FE(eet_close, NULL)
        PHP_FE(eet_file_get, NULL)
        PHP_FE(eet_list, NULL)
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


static void php_eet_file_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
        Eet_File *ef = (Eet_File *)rsrc->ptr;

        if (ef)
                eet_close(ef);
}


PHP_MINIT_FUNCTION(eet)
{
        le_eet_file = zend_register_list_destructors_ex(php_eet_file_dtor, NULL,
                                        PHP_EET_EET_FILE, module_number);

        //For eet_open
        REGISTER_LONG_CONSTANT("EET_FILE_MODE_READ", EET_FILE_MODE_READ, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_FILE_MODE_WRITE", EET_FILE_MODE_WRITE, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_FILE_MODE_READ_WRITE", EET_FILE_MODE_READ_WRITE, CONST_CS|CONST_PERSISTENT);

        //EET data type
        REGISTER_LONG_CONSTANT("EET_T_CHAR", EET_T_CHAR, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_SHORT", EET_T_SHORT, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_INT", EET_T_INT, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_LONG_LONG", EET_T_LONG_LONG, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_FLOAT", EET_T_FLOAT, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_DOUBLE", EET_T_DOUBLE, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_UCHAR", EET_T_UCHAR, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_USHORT", EET_T_USHORT, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_UINT", EET_T_UINT, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_ULONG_LONG", EET_T_ULONG_LONG, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_STRING", EET_T_STRING, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_T_INLINED_STRING", EET_T_INLINED_STRING, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_G_ARRAY", EET_G_ARRAY, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_G_VAR_ARRAY", EET_G_VAR_ARRAY, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_G_LIST", EET_G_LIST, CONST_CS|CONST_PERSISTENT);
        REGISTER_LONG_CONSTANT("EET_G_HASH", EET_G_HASH, CONST_CS|CONST_PERSISTENT);

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


PHP_FUNCTION(eet_open)
{
        Eet_File *ef = NULL;
        char *fname;
        int fname_len;
        long mode;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &fname, &fname_len, &mode) == FAILURE)
        {
                RETURN_FALSE;
        }

        if (fname_len < 1)
        {
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "No filename given!");
                RETURN_FALSE;
        }

        ef = eet_open(fname, mode);

        if (!ef)
        {
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "eet_open(%s, %ld) failed!", fname, mode);
                RETURN_FALSE;
        }

        ZEND_REGISTER_RESOURCE(return_value, ef, le_eet_file);
}


PHP_FUNCTION(eet_close)
{
        Eet_File *ef = NULL;
        zval *zef;
        Eet_Error ret;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zef) == FAILURE)
        {
                RETURN_FALSE;
        }

        ZEND_FETCH_RESOURCE(ef, Eet_File *, &zef, -1, PHP_EET_EET_FILE, le_eet_file);

        ret = eet_close(ef);

        if (ret == EET_ERROR_NONE)
        {
                RETURN_TRUE;
        }

        RETURN_LONG((long)ret);
}

PHP_FUNCTION(eet_file_get)
{
        Eet_File *ef = NULL;
        zval *zef;
        Eet_Error ret;
        const char *fname;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zef) == FAILURE)
        {
                RETURN_FALSE;
        }

        ZEND_FETCH_RESOURCE(ef, Eet_File *, &zef, -1, PHP_EET_EET_FILE, le_eet_file);

        fname = eet_file_get(ef);

        if (!fname)
        {
                RETURN_FALSE;
        }

        RETURN_STRING(fname, 1);
}

PHP_FUNCTION(eet_list)
{
        Eet_File *ef = NULL;
        zval *zef;
        char *glob;
        int len;
        char **list;
        int i;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &zef, &glob, &len) == FAILURE)
        {
                RETURN_FALSE;
        }

        ZEND_FETCH_RESOURCE(ef, Eet_File *, &zef, -1, PHP_EET_EET_FILE, le_eet_file);

        array_init(return_value);

        list = eet_list(ef, glob, &len);
        if (!list)
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "eet_list() failed!");
        if (list)
        {
                for (i = 0; i < len; i++)
                        add_next_index_string(return_value, list[i], 1);
                free(list);
        }
}



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
