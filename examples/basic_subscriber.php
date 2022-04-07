<?php

function handler(string $message)
{
    echo "<<$message>>", PHP_EOL;
}

$subscriber = new AeronSubscriber('handler', 'aeron:ipc');

while (true) {
    $subscriber->poll();
    sleep(1);
}
