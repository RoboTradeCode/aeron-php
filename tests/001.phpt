--TEST--
Check if aeron is loaded
--EXTENSIONS--
aeron
--FILE--
<?php
echo 'The extension "aeron" is available';
?>
--EXPECT--
The extension "aeron" is available
