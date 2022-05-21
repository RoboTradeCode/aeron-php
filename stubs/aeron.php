<?php

namespace Aeron;

use Exception;

class AeronException extends Exception
{
}

/**
 * An error has occurred. Such as a bad argument.
 */
class AeronPublicationException extends AeronException
{
}

/**
 * The publication is not connected to a subscriber, this can be an intermittent state as subscribers come and go.
 */
class AeronPublicationNotConnectedException extends AeronPublicationException
{
}

/**
 * The offer failed due to back pressure from the subscribers preventing further transmission.
 */
class AeronPublicationBackPressuredException extends AeronPublicationException
{
}

/**
 * The offer failed due to an administration action and should be retried.
 * The action is an operation such as log rotation which is likely to have succeeded by the next retry attempt.
 */
class AeronPublicationAdminActionException extends AeronPublicationException
{
}

/**
 * The publication has been closed and should no longer be used.
 */
class AeronPublicationClosedException extends AeronPublicationException
{
}

/**
 * The offer failed due to reaching the maximum position of the stream given term buffer length times the total possible
 * number of terms.
 *
 * If this happens then the publication should be closed and a new one added. To make it less likely to happen then
 * increase the term buffer length.
 */
class AeronPublicationMaxPositionExceededException extends AeronPublicationException
{
}

/**
 * A simple publisher that sends messages
 */
class Publisher
{
    /**
     * @param string $channel uri for the channel of the publication.
     * @param int $stream_id for the publication.
     *
     * @throws AeronException
     */
    public function __construct(
        string $channel = "aeron:udp?endpoint=localhost:20121",
        int    $stream_id = 1001,
    )
    {
    }

    /**
     * Publish the message.
     *
     * @param string $message to publish.
     *
     * @return int the new stream position.
     *
     * @throws AeronPublicationAdminActionException
     * @throws AeronPublicationClosedException
     * @throws AeronPublicationMaxPositionExceededException
     * @throws AeronPublicationNotConnectedException
     * @throws AeronPublicationBackPressuredException
     */
    public function offer(string $message): int
    {
    }

    /**
     * Close the publication.
     *
     * @return void
     *
     * @throws AeronException
     */
    public function close(): void
    {
    }
}

/**
 * A simple subscriber that receives messages
 */
class Subscriber
{
    /**
     * @param callable $handler for handling each message.
     * @param string $channel for the channel of the subscription.
     * @param int $stream_id for the subscription.
     *
     * @throws AeronException
     */
    public function __construct(
        callable $handler,
        string   $channel = "aeron:udp?endpoint=localhost:20121",
        int      $stream_id = 1001,
    )
    {
    }

    /**
     * Add a destination manually to a multi-destination-subscription.
     *
     * @param string $channel uri for the destination to add.
     *
     * @return void
     *
     * @throws AeronException
     */
    public function addDestination(string $channel): void
    {
    }

    /**
     * Remove a destination manually from a multi-destination-subscription.
     *
     * @param string $channel uri for the destination to remove.
     *
     * @return void
     *
     * @throws AeronException
     */
    public function removeDestination(string $channel): void
    {
    }

    /**
     * Poll the images under the subscription for available message fragments.
     *
     * Each fragment read will be a whole message if it is under MTU length.
     * If larger than MTU then it will come as a series of fragments ordered within a session.
     *
     * @return int the number of fragments received
     *
     * @throws AeronException
     */
    public function poll(): int
    {
    }

    /**
     * Close the subscription.
     *
     * @return void
     *
     * @throws AeronException
     */
    public function close(): void
    {
    }
}
