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

        struct
        {
            int version;
            char *name;
            int id;
            double value;
        }
    */
/*
    $edd = array (
        "version" => EET_T_INT,
        "name" => EET_T_STRING,
        "id" => EET_T_INT,
        "value" => EET_T_DOUBLE,
    );

    $data = array (
        "version" => 1,
        "name" => "name of data",
        "id" => 123,
        "value" => 3.14,
    );

    eet_data_write($f, $edd, "my_entry", $data, true);

    eet_sync($f);

    $my_data = eet_data_read($f, $edd, "my_entry");

    var_dump($my_data);

*/
    eet_close($f);


?>
