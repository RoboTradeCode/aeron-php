<?php

use Aeron\Publisher;

$publisher = new Publisher(
    channel: 'aeron:udp?endpoint=localhost:20121',  // string
    stream_id: 1001,                                // int
);

for ($i = 0; $i < 10000000; $i++) {
    try {
        $result = $publisher->offer(message: 'Hello, World!');
        echo "yay!", PHP_EOL;
    } catch (Exception $e) {
        echo $e->getMessage(), PHP_EOL;
    }

    sleep(1);
}

$publisher->close();
