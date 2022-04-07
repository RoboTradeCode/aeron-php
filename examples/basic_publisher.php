<?php

$publisher = new AeronPublisher('aeron:ipc');

for ($i = 0; $i < 10; $i++) {
    $publisher->offer("Hello, World! ($i)");
    sleep(1);
}
