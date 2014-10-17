<?php


        // Get the list of functions in the module...
        $module    = "eet";
        $functions = get_extension_funcs($module);

        print("Functions available in the $module extension:\n");

        foreach ($functions as $func)
        {
                print("$func\n");
        }

        print("\n");



        /*second param could be:
          r => read
          w => write
          rw => read and write
        */
        $f = eet_open("database.eet", EET_FILE_MODE_READ);

        var_dump($f);

        var_dump(eet_file_get($f));

        $list = eet_list($f, "*");

        var_dump($list);

        unset($list);

        //setup an Eet_Data_Descriptor here
        /*
                similar to C struct:

                struct data
                {
                        int version;
                        char *name;
                        int id;
                        double value;
                        Eina_List *list;  //list of char *
                        Eina_Hash *subhash;  //new hashtable

                        Eina_List *sublist; //list of sub_data
                };

                struct sub_data
                {
                        int subversion;
                        char *subname;
                        int subid;
                }
        */

        $edd = array (
                EET_G_HASH //this is a hashtable
                "version" => EET_T_INT,
                "name" => EET_T_STRING,
                "id" => EET_T_INT,
                "value" => EET_T_DOUBLE,
                "list" => array(
                        EET_G_LIST, //list of element
                        EET_T_STRING, //list of string
                ),
                "subhash" => array (
                        EET_G_HASH, //this is a sub hashtable
                        "subversion" => EET_T_INT,
                        "subname" => EET_T_STRING,
                        "subid" => EET_T_INT
                )
        );

        $data = array (
                "version" => 1,
                "name" => "name of data",
                "id" => 123,
                "value" => 3.14,
                "list" => array (
                        "data1",
                        "data2",
                        "data3",
                ),
        );

        print("Write some data to test.eet\n");
        $f = eet_open("test.eet", EET_FILE_MODE_WRITE);
        eet_data_write($f, "struct_name", $edd, "my_entry", $data, true);
        eet_close($f);

        //eet_sync($f);

        //$my_data = eet_data_read($f, $edd, "my_entry");

        //var_dump($my_data);

        eet_close($f);

?>
