<?php

    $f = eet_open("file.eet");

    var_dump($f);

    var_dump(eet_file_get($f));

    eet_close($f);

?>
