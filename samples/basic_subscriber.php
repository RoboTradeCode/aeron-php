<?php

use Aeron\Subscriber;

function handler(string $message): void
{
    echo "<<$message>>", PHP_EOL;
}

$subscriber = new Subscriber(
    handler: 'handler',                             // callable
    channel: 'aeron:udp?endpoint=localhost:20121',  // string
    stream_id: 1001,                                // int
);

for ($i = 0; $i < 10000000; $i++) {
    $fragments_read = $subscriber->poll();
    sleep(1);
}

$subscriber->close();
