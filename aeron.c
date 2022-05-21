/* aeron extension for PHP (c) 2022 nomnoms12 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
#include "sched.h"
#include "aeron/aeronc.h"
#include "php_aeron.h"
#include "aeron_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
    ZEND_PARSE_PARAMETERS_START(0, 0) \
    ZEND_PARSE_PARAMETERS_END()
#endif

#define DEFAULT_CHANNEL "aeron:udp?endpoint=localhost:20121"
#define DEFAULT_STREAM_ID (1001)
#define DEFAULT_FRAGMENT_COUNT_LIMIT (10)

zend_class_entry *aeron_ce_exception;
zend_class_entry *aeron_ce_publication_exception;
zend_class_entry *aeron_ce_publication_not_connected_exception;
zend_class_entry *aeron_ce_publication_back_pressured_exception;
zend_class_entry *aeron_ce_publication_admin_action_exception;
zend_class_entry *aeron_ce_publication_closed_exception;
zend_class_entry *aeron_ce_publication_max_position_exceeded_exception;

static zend_class_entry *publisher_class_entry = NULL;
static zend_object_handlers publisher_object_handlers;

typedef struct publisher_t {
    aeron_context_t *context;
    aeron_t *aeron;
    aeron_publication_t *publication;
    zend_object std;
} publisher_t;

#define Z_PUBLISHER_P(zv) \
    ((publisher_t*)((char*)(Z_OBJ_P(zv)) - XtOffsetOf(publisher_t, std)))

zend_object *publisher_new(zend_class_entry *ce)
{
    publisher_t *publisher = zend_object_alloc(sizeof(publisher_t), ce);

    zend_object_std_init(&publisher->std, ce);
    publisher->std.handlers = &publisher_object_handlers;
    return &publisher->std;
}

PHP_METHOD(Aeron_Publisher, __construct)
{
    publisher_t *publisher = Z_PUBLISHER_P(ZEND_THIS);
    aeron_async_add_publication_t *async = NULL;
    char *channel = DEFAULT_CHANNEL;
    size_t channel_len;
    long stream_id = DEFAULT_STREAM_ID;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(channel, channel_len)
        Z_PARAM_LONG(stream_id)
    ZEND_PARSE_PARAMETERS_END();

    if (aeron_context_init(&publisher->context) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    if (aeron_init(&publisher->aeron, publisher->context) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    if (aeron_start(publisher->aeron) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    if (aeron_async_add_publication(&async, publisher->aeron, channel, (int) stream_id) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    while (NULL == publisher->publication) {
        if (aeron_async_add_publication_poll(&publisher->publication, async) < 0) {
            zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
            RETURN_THROWS();
        }

        sched_yield();
    }
}

PHP_METHOD(Aeron_Publisher, offer)
{
    publisher_t* publisher = Z_PUBLISHER_P(ZEND_THIS);

    char* message;
    size_t message_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(message, message_len)
    ZEND_PARSE_PARAMETERS_END();

    int64_t result = aeron_publication_offer(
        publisher->publication,
        (const uint8_t *) message,
        message_len,
        NULL,
        NULL
    );

    if (AERON_PUBLICATION_NOT_CONNECTED == result) {
        zend_throw_exception(
            aeron_ce_publication_not_connected_exception,
            "Offer failed because publisher is not connected to a subscriber",
            0
        );
        RETURN_THROWS();
    } else if (AERON_PUBLICATION_BACK_PRESSURED == result) {
        zend_throw_exception(
            aeron_ce_publication_back_pressured_exception,
            "Offer failed due to back pressure",
            0
        );
        RETURN_THROWS();
    } else if (AERON_PUBLICATION_ADMIN_ACTION == result) {
        zend_throw_exception(
            aeron_ce_publication_admin_action_exception,
            "Offer failed because of an administration action in the system",
            0
        );
        RETURN_THROWS();
    } else if (AERON_PUBLICATION_CLOSED == result) {
        zend_throw_exception(
            aeron_ce_publication_closed_exception,
            "Offer failed because publication is closed",
            0
        );
        RETURN_THROWS();
    } else if (AERON_PUBLICATION_MAX_POSITION_EXCEEDED == result) {
        zend_throw_exception(
            aeron_ce_publication_max_position_exceeded_exception,
            "Offer failed due to reaching the maximum position",
            0
        );
        RETURN_THROWS();
    } else if (AERON_PUBLICATION_ERROR == result) {
        char exception_message[256];
        snprintf(exception_message, 256, "Offer failed due to unknown reason %ld", result);
        zend_throw_exception(
            aeron_ce_publication_exception,
            exception_message,
            0
        );
        RETURN_THROWS();
    }

    RETURN_LONG(result);
}

PHP_METHOD(Aeron_Publisher, close)
{
    publisher_t *publisher = Z_PUBLISHER_P(ZEND_THIS);

    if (aeron_publication_close(publisher->publication, NULL, NULL) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    if (aeron_close(publisher->aeron) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    if (aeron_context_close(publisher->context) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }
}

static zend_class_entry *subscriber_class_entry = NULL;
static zend_object_handlers subscriber_object_handlers;

typedef struct subscriber_t {
    zend_fcall_info handler;
    zend_fcall_info_cache handler_cache;
    aeron_context_t *context;
    aeron_t *aeron;
    aeron_async_add_publication_t *async;
    aeron_subscription_t *subscription;
    aeron_fragment_assembler_t *fragment_assembler;
    zend_object std;
} subscriber_t;

#define Z_SUBSCRIBER_P(zv) \
    ((subscriber_t*)((char*)(Z_OBJ_P(zv)) - XtOffsetOf(subscriber_t, std)))

zend_object *subscriber_new(zend_class_entry *ce)
{
    subscriber_t *subscriber = zend_object_alloc(sizeof(subscriber_t), ce);

    zend_object_std_init(&subscriber->std, ce);
    subscriber->std.handlers = &subscriber_object_handlers;
    return &subscriber->std;
}

void poll_handler(void *clientd, const uint8_t *buffer, size_t length, __attribute__((unused)) aeron_header_t *header)
{
    subscriber_t* subscriber = (subscriber_t*) clientd;

    zval retval;
    zval args[1];
    ZVAL_STRINGL(&args[0], (const char *) buffer, length);

    subscriber->handler.retval = &retval;
    subscriber->handler.param_count = 1;
    subscriber->handler.params = args;
    zend_call_function(&subscriber->handler, &subscriber->handler_cache);
}

PHP_METHOD(Aeron_Subscriber, __construct)
{
    subscriber_t *subscriber = Z_SUBSCRIBER_P(ZEND_THIS);
    char *channel = DEFAULT_CHANNEL;
    size_t channel_len;
    long stream_id = DEFAULT_STREAM_ID;

    ZEND_PARSE_PARAMETERS_START(1, 3)
        Z_PARAM_FUNC(subscriber->handler, subscriber->handler_cache)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(channel, channel_len)
        Z_PARAM_LONG(stream_id)
    ZEND_PARSE_PARAMETERS_END();

    if (aeron_context_init(&subscriber->context) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    if (aeron_init(&subscriber->aeron, subscriber->context) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    if (aeron_start(subscriber->aeron) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    int result = aeron_async_add_subscription(
        &subscriber->async,
        subscriber->aeron,
        channel,
        (int) stream_id,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (result < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    while (NULL == subscriber->subscription) {
        if (aeron_async_add_subscription_poll(&subscriber->subscription, subscriber->async) < 0) {
            zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
            RETURN_THROWS();
        }

        sched_yield();
    }

    if (aeron_fragment_assembler_create(&subscriber->fragment_assembler, poll_handler, subscriber) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }
}

PHP_METHOD(Aeron_Subscriber, addDestination)
{
    subscriber_t *subscriber = Z_SUBSCRIBER_P(ZEND_THIS);
    char *channel = DEFAULT_CHANNEL;
    size_t channel_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(channel, channel_len)
    ZEND_PARSE_PARAMETERS_END();

    int result = aeron_subscription_async_add_destination(
        &subscriber->async,
        subscriber->aeron,
        subscriber->subscription,
        channel
    );

    if (result < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }
}

PHP_METHOD(Aeron_Subscriber, removeDestination)
{
    subscriber_t *subscriber = Z_SUBSCRIBER_P(ZEND_THIS);
    char *channel = DEFAULT_CHANNEL;
    size_t channel_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(channel, channel_len)
    ZEND_PARSE_PARAMETERS_END();

    int result = aeron_subscription_async_remove_destination(
        &subscriber->async,
        subscriber->aeron,
        subscriber->subscription,
        channel
    );

    if (result < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }
}

PHP_METHOD(Aeron_Subscriber, poll)
{
    subscriber_t *subscriber = Z_SUBSCRIBER_P(ZEND_THIS);

    int fragments_read = aeron_subscription_poll(
        subscriber->subscription,
        aeron_fragment_assembler_handler,
        subscriber->fragment_assembler,
        DEFAULT_FRAGMENT_COUNT_LIMIT
    );

    if (fragments_read < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    RETURN_LONG(fragments_read);
}

PHP_METHOD(Aeron_Subscriber, close)
{
    subscriber_t *subscriber = Z_SUBSCRIBER_P(ZEND_THIS);

    if (aeron_subscription_close(subscriber->subscription, NULL, NULL) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    if (aeron_close(subscriber->aeron) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    if (aeron_context_close(subscriber->context) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }

    if (aeron_fragment_assembler_delete(subscriber->fragment_assembler) < 0) {
        zend_throw_exception(aeron_ce_exception, aeron_errmsg(), 0);
        RETURN_THROWS();
    }
}

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(aeron)
{
#if defined(ZTS) && defined(COMPILE_DL_AERON)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    aeron_ce_exception = register_class_Aeron_AeronException(
        zend_ce_exception
    );
    aeron_ce_publication_exception = register_class_Aeron_AeronPublicationException(
        aeron_ce_exception
    );
    aeron_ce_publication_not_connected_exception = register_class_Aeron_AeronPublicationNotConnectedException(
        aeron_ce_publication_exception
    );
    aeron_ce_publication_back_pressured_exception = register_class_Aeron_AeronPublicationBackPressuredException(
        aeron_ce_publication_exception
    );
    aeron_ce_publication_admin_action_exception = register_class_Aeron_AeronPublicationAdminActionException(
        aeron_ce_publication_exception
    );
    aeron_ce_publication_closed_exception = register_class_Aeron_AeronPublicationClosedException(
        aeron_ce_publication_exception
    );
    aeron_ce_publication_max_position_exceeded_exception = register_class_Aeron_AeronPublicationMaxPositionExceededException(
        aeron_ce_publication_exception
    );

    publisher_class_entry = register_class_Aeron_Publisher();
    publisher_class_entry->create_object = publisher_new;

    memcpy(&publisher_object_handlers, &std_object_handlers,
           sizeof(zend_object_handlers));
    publisher_object_handlers.offset = XtOffsetOf(publisher_t, std);

    subscriber_class_entry = register_class_Aeron_Subscriber();
    subscriber_class_entry->create_object = subscriber_new;

    memcpy(&subscriber_object_handlers, &std_object_handlers,
           sizeof(zend_object_handlers));
    subscriber_object_handlers.offset = XtOffsetOf(subscriber_t, std);

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(aeron)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "aeron support", "enabled");
    php_info_print_table_end();
}
/* }}} */

/* {{{ aeron_module_entry */
zend_module_entry aeron_module_entry = {
    STANDARD_MODULE_HEADER,
    "aeron",                    /* Extension name */
    NULL,                       /* zend_function_entry */
    PHP_MINIT(aeron),           /* PHP_MINIT - Module initialization */
    NULL,                       /* PHP_MSHUTDOWN - Module shutdown */
    NULL,                       /* PHP_RINIT - Request initialization */
    NULL,                       /* PHP_RSHUTDOWN - Request shutdown */
    PHP_MINFO(aeron),           /* PHP_MINFO - Module info */
    PHP_AERON_VERSION,          /* Version */
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_AERON
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(aeron)
#endif
