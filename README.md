This is a start of a wrapper extension to allow you to use EET from PHP.
See Eet here: http://docs.enlightenment.org/auto/eet

Requires:
 * PHP 5.2 or greater
 * Eet 1.7.x or greater

To build:
 * cd php_eet
 * phpize
 * ./configure
 * make

To test:
 * php -dextension=modules/eet.so example.php
