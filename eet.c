
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
        PHP_FE(eet_data_write, NULL)
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
                RETURN_NULL();
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
                RETURN_NULL();
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
                RETURN_NULL();
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
                RETURN_NULL();
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

static Eet_Node *_php_simple_to_eet_node(int type, char *name, zval *zvalue)
{
        char *str = NULL;
        double dbl = 0.0;
        int num = 0;

        switch (type)
        {
        case EET_T_CHAR:
        case EET_T_SHORT:
        case EET_T_INT:
        case EET_T_LONG_LONG:
        case EET_T_UCHAR:
        case EET_T_USHORT:
        case EET_T_UINT:
        case EET_T_ULONG_LONG:
                if (Z_TYPE_PP(zvalue) != IS_LONG)
                {
                        php_error_docref(NULL TSRMLS_CC, E_ERROR, "_php_simple_to_eet_node(): wrong data type, int required", key);
                        return NULL;
                }
                num = Z_LVAL_P(zvalue);
                break;
        case EET_T_FLOAT:
        case EET_T_DOUBLE:
                if (Z_TYPE_PP(zvalue) != IS_DOUBLE)
                {
                        php_error_docref(NULL TSRMLS_CC, E_ERROR, "_php_simple_to_eet_node(): wrong data type, double required", key);
                        return NULL;
                }
                dbl = Z_DVAL_P(zvalue);
                break;
        case EET_T_STRING:
        case EET_T_INLINED_STRING:
                if (Z_TYPE_PP(zvalue) != IS_STRING)
                {
                        php_error_docref(NULL TSRMLS_CC, E_ERROR, "_php_simple_to_eet_node(): wrong data type, string required", key);
                        return NULL;
                }
                str = Z_STRVAL_P(zvalue);
                break;
        case EET_T_NULL:
                return eet_node_null_new(name);
        }

        switch (type)
        {
        case EET_T_CHAR:
                return eet_node_char_new(name, (char) num);
        case EET_T_SHORT:
                return eet_node_short_new(name, (short) num);
        case EET_T_INT:
                return eet_node_int_new(name, (int) num);
        case EET_T_LONG_LONG:
                return eet_node_long_long_new(name, (int) num);
        case EET_T_UCHAR:
                return eet_node_unsigned_char_new(name, (unsigned char) num);
        case EET_T_USHORT:
                return eet_node_unsigned_short_new(name, (unsigned short) num);
        case EET_T_UINT:
                return eet_node_unsigned_int_new(name, (unsigned int) num);
        case EET_T_ULONG_LONG:
                return eet_node_unsigned_long_long_new(name, (unsigned long long) num);

        case EET_T_FLOAT:
                return eet_node_float_new(name, (float) dbl);
        case EET_T_DOUBLE:
                return eet_node_double_new(name, dbl);

        case EET_T_STRING:
                return eet_node_string_new(name, str);
        case EET_T_INLINED_STRING:
                return eet_node_inlined_string_new(name, str);
        }

        return NULL;
}

static Eet_Node *_php_group_to_eet_node(HashTable *hash_edd, HashTable *hash_data, int type, char *name)
{
        HashPosition pointer;
        zval **data;

        Eet_Node *ret_node, *child;
        Eina_List *subpart = NULL;

        for(zend_hash_internal_pointer_reset_ex(hash_data, &pointer);
            zend_hash_get_current_data_ex(hash_data, (void**) &data, &pointer) == SUCCESS;
            zend_hash_move_forward_ex(hash_data, &pointer))
        {
                char *key = NULL;
                int key_len = 0;
                long index;
                zval **zvalue;
                long data_type;

                if (zend_hash_get_current_key_ex(hash_data, &key, &key_len, &index, 0, &pointer) != HASH_KEY_IS_STRING)
                {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "eet_data_write(): key in data array is not a string !");
                        continue;
                }

                if (zend_hash_find(hash_edd, key, key_len, (void**)&zvalue) == FAILURE)
                {
                        php_error_docref(NULL TSRMLS_CC, E_NOTICE, "eet_data_write(): key \"%s\" is not found in the Eet Data Descriptor array, it will not be saved.", key);
                        continue;
                }

                if (Z_TYPE_PP(zvalue) != IS_LONG)
                {
                        php_error_docref(NULL TSRMLS_CC, E_NOTICE, "eet_data_write(): key \"%s\" is not a valid Eet Data Descriptor type, it will not be saved.", key);
                        continue;
                }

                data_type = Z_LVAL_P(zvalue);

                //some checks
                switch (data_type)
                {
                case EET_T_CHAR:
                case EET_T_SHORT:
                case EET_T_INT:
                case EET_T_LONG_LONG:
                case EET_T_FLOAT:
                case EET_T_DOUBLE:
                case EET_T_UCHAR:
                case EET_T_USHORT:
                case EET_T_UINT:
                case EET_T_ULONG_LONG:
                case EET_T_STRING:
                case EET_T_INLINED_STRING:
                {
                        child = _php_simple_to_eet_node(data_type, key, *data);
                        break;
                }
                case EET_G_ARRAY:
                case EET_G_VAR_ARRAY:
                case EET_G_LIST:
                case EET_G_HASH:
                {
                        TODO: read the __type thing

                        child = _php_group_to_eet_node();
                        break;
                }
                default:
                        php_error_docref(NULL TSRMLS_CC, E_NOTICE, "eet_data_write(): key \"%s\" is not a valid Eet Data Descriptor type, it will not be saved.", key);
                        continue;
                }

                if (child)
                        subpart = eina_list_append(subpart, child);
        }

        if (!subpart) return NULL;

        switch(type)
        {
        case EET_G_HASH: ret_node = eet_node_struct_new(name, subpart); break;
        case EET_G_ARRAY: ret_node = eet_node_array_new(name, arr_count, subpart); break;
        case EET_G_VAR_ARRAY: ret_node = eet_node_var_array_new(name, subpart); break;
        case EET_G_LIST: ret_node = eet_node_list_new(name, subpart); break;
        default: ret_node = NULL;
        }

        return ret_node;
}

PHP_FUNCTION(eet_data_write)
{
        Eet_File *ef = NULL;
        zval *zef;
        zval *zedd;
        zval *zdata;
        zend_bool compress;
        char *entry, *sname;
        int entry_len, sname_len;
        int dsize;
        Eet_Node *node;

        HashTable *hash_edd, *hash_data;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsasab", &zef, &sname, &sname_len, &zedd, &entry, &entry_len, &zdata, &compress) == FAILURE)
        {
                RETURN_NULL();
        }

        ZEND_FETCH_RESOURCE(ef, Eet_File *, &zef, -1, PHP_EET_EET_FILE, le_eet_file);

        hash_data = Z_ARRVAL_P(zdata);
        hash_edd = Z_ARRVAL_P(zedd);

        node = _php_group_to_eet_node(hash_edd, hash_data, EET_G_HASH, sname); //default create a struct

        dsize = eet_data_node_write_cipher(ef, entry, NULL, node, compress);

        RETURN_TRUE;
}
