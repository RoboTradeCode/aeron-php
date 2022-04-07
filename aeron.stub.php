<?php

/** @generate-class-entries */

class AeronSubscriber
{
    public function __construct(callable $handler, string $channel = "aeron:udp?control-mode=manual",
                                int $stream_id = 1001, int $fragment_limit = 10) {}

    public function __destruct() {}

    public function addDestination(string $channel): void {}

    public function removeDestination(string $channel): void {}

    public function poll(): int {}
}

class AeronPublisher
{
    public function __construct(string $channel, int $stream_id = 1001) {}

    public function __destruct() {}

    public function offer(string $message): int {}
}
