<?php

/** @generate-class-entries */

namespace Aeron;

class AeronException extends \Exception {}

class AeronPublicationException extends AeronException {}

class AeronPublicationNotConnectedException extends AeronPublicationException {}

class AeronPublicationBackPressuredException extends AeronPublicationException {}

class AeronPublicationAdminActionException extends AeronPublicationException {}

class AeronPublicationClosedException extends AeronPublicationException {}

class AeronPublicationMaxPositionExceededException extends AeronPublicationException {}

class Publisher
{
    public function __construct(
        string $channel = "aeron:udp?endpoint=localhost:20121",
        int $stream_id = 1001
    ) {}

    public function offer(string $message): int {}

    public function close(): void {}
}

class Subscriber
{
    public function __construct(
        callable $handler,
        string $channel = "aeron:udp?endpoint=localhost:20121",
        int $stream_id = 1001,
    ) {}

    public function addDestination(string $channel): void {}

    public function removeDestination(string $channel): void {}

    public function poll(): int {}

    public function close(): void {}
}
