#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_aeron.h"
#include "aeron_arginfo.h"
#include "aeronc.h"

#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
    ZEND_PARSE_PARAMETERS_START(0, 0) \
    ZEND_PARSE_PARAMETERS_END()
#endif

static zend_object_handlers subscriber_object_handlers;

typedef struct subscriber_t {
    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;
    aeron_context_t* context;
    aeron_t* aeron;
    aeron_async_add_publication_t* async;
    aeron_subscription_t* subscription;
    aeron_fragment_assembler_t* fragment_assembler;
    int fragment_limit;
    zend_object std;
} subscriber_t;

#define Z_SUBSCRIBER_P(zv) ((subscriber_t*)((char*)(Z_OBJ_P(zv)) - XtOffsetOf(subscriber_t, std)))

zend_object *subscriber_new(zend_class_entry *ce)
{
    subscriber_t *subscriber = zend_object_alloc(sizeof(subscriber_t), ce);

    zend_object_std_init(&subscriber->std, ce);
    subscriber->std.handlers = &subscriber_object_handlers;
    return &subscriber->std;
}

void poll_handler(void *clientd, const uint8_t *buffer, size_t length, aeron_header_t *header)
{
    subscriber_t* subscriber = (subscriber_t*) clientd;
    aeron_subscription_t* subscription = subscriber->subscription;

    zval retval, args[1];
    ZVAL_STRING(&args[0], buffer);

    subscriber->fci.retval = &retval;
    subscriber->fci.param_count = 1;
    subscriber->fci.params = args;

    zend_call_function(&subscriber->fci, &subscriber->fci_cache);
}

PHP_METHOD(AeronSubscriber, __construct)
{
    subscriber_t* subscriber = Z_SUBSCRIBER_P(ZEND_THIS);

    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;
    char* channel = "aeron:udp?control-mode=manual";
    size_t channel_len;
    zend_long stream_id = 1001;
    zend_long fragment_limit = 10;

    ZEND_PARSE_PARAMETERS_START(1, 4)
        Z_PARAM_FUNC(fci, fci_cache)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(channel, channel_len)
        Z_PARAM_LONG(stream_id)
        Z_PARAM_LONG(fragment_limit)
    ZEND_PARSE_PARAMETERS_END();

    subscriber->fragment_limit = (int)fragment_limit;

    if (aeron_context_init(&subscriber->context) < 0)
        php_printf("aeron_context_init: %s\n", aeron_errmsg());
    if (aeron_init(&subscriber->aeron, subscriber->context) < 0)
        php_printf("aeron_init: %s\n", aeron_errmsg());
    if (aeron_start(subscriber->aeron) < 0)
        php_printf("aeron_start: %s\n", aeron_errmsg());
    if (aeron_async_add_subscription(&subscriber->async, subscriber->aeron, channel, (int)stream_id, NULL, NULL, NULL, NULL) < 0)
        php_printf("aeron_async_add_subscription: %s\n", aeron_errmsg());
    while (NULL == subscriber->subscription)
        if (aeron_async_add_subscription_poll(&subscriber->subscription, subscriber->async) < 0)
            php_printf("aeron_async_add_subscription_poll: %s\n", aeron_errmsg());

    subscriber->fci = fci;
    subscriber->fci_cache = fci_cache;

    if (aeron_fragment_assembler_create(&subscriber->fragment_assembler, poll_handler, subscriber) < 0)
        php_printf("aeron_fragment_assembler_create: %s\n", aeron_errmsg());
}

PHP_METHOD(AeronSubscriber, addDestination)
{
    subscriber_t * subscriber = Z_SUBSCRIBER_P(ZEND_THIS);

    char* channel;
    size_t channel_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(channel, channel_len)
    ZEND_PARSE_PARAMETERS_END();

    aeron_subscription_async_add_destination(&subscriber->async, subscriber->aeron, subscriber->subscription, channel);
}

PHP_METHOD(AeronSubscriber, removeDestination)
{
    subscriber_t * subscriber = Z_SUBSCRIBER_P(ZEND_THIS);

    char* channel;
    size_t channel_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_STRING(channel, channel_len)
    ZEND_PARSE_PARAMETERS_END();

    aeron_subscription_async_remove_destination(&subscriber->async, subscriber->aeron, subscriber->subscription,
                                                channel);
}

PHP_METHOD(AeronSubscriber, poll)
{
    subscriber_t * subscriber = Z_SUBSCRIBER_P(ZEND_THIS);

    int fragments_read = aeron_subscription_poll(subscriber->subscription, aeron_fragment_assembler_handler,
        subscriber->fragment_assembler, subscriber->fragment_limit);

    if (fragments_read < 0)
        php_printf("aeron_subscription_poll: %s\n", aeron_errmsg());

    RETURN_LONG(fragments_read);
}

PHP_METHOD(AeronSubscriber, __destruct)
{
    subscriber_t * subscriber = Z_SUBSCRIBER_P(ZEND_THIS);

    aeron_subscription_close(subscriber->subscription, NULL, NULL);
    aeron_close(subscriber->aeron);
    aeron_context_close(subscriber->context);
    aeron_fragment_assembler_delete(subscriber->fragment_assembler);
}

static zend_object_handlers publisher_object_handlers;

typedef struct publisher_t {
    aeron_context_t* context;
    aeron_t* aeron;
    aeron_publication_t* publication;
    zend_object std;
} publisher_t;

#define Z_PUBLISHER_P(zv) ((publisher_t*)((char*)(Z_OBJ_P(zv)) - XtOffsetOf(publisher_t, std)))

zend_object *publisher_new(zend_class_entry *ce)
{
    publisher_t *publisher = zend_object_alloc(sizeof(publisher_t), ce);

    zend_object_std_init(&publisher->std, ce);
    publisher->std.handlers = &publisher_object_handlers;
    return &publisher->std;
}

PHP_METHOD(AeronPublisher, __construct)
{
    publisher_t* publisher = Z_PUBLISHER_P(ZEND_THIS);

    char* channel;
    size_t channel_len;
    zend_long stream_id = 1001;

    ZEND_PARSE_PARAMETERS_START(1, 3)
        Z_PARAM_STRING(channel, channel_len)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(stream_id)
    ZEND_PARSE_PARAMETERS_END();

    aeron_async_add_publication_t* async;

    if (aeron_context_init(&publisher->context) < 0)
        php_printf("aeron_context_init: %s\n", aeron_errmsg());
    if (aeron_init(&publisher->aeron, publisher->context) < 0)
        php_printf("aeron_init: %s\n", aeron_errmsg());
    if (aeron_start(publisher->aeron) < 0)
        php_printf("aeron_start: %s\n", aeron_errmsg());
    if (aeron_async_add_publication(&async, publisher->aeron, channel, (int)stream_id) < 0)
        php_printf("aeron_async_add_publication: %s\n", aeron_errmsg());
    while (NULL == publisher->publication)
        if (aeron_async_add_publication_poll(&publisher->publication, async) < 0)
            php_printf("aeron_async_add_publication_poll: %s\n", aeron_errmsg());
}

PHP_METHOD(AeronPublisher, offer)
{
    publisher_t* publisher = Z_PUBLISHER_P(ZEND_THIS);

    char* message;
    size_t message_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(message, message_len)
    ZEND_PARSE_PARAMETERS_END();

    long result = aeron_publication_offer(publisher->publication, (const uint8_t*)message, message_len, NULL, NULL);
    RETURN_LONG(result);
}

PHP_METHOD(AeronPublisher, __destruct)
{
    publisher_t* publisher = Z_PUBLISHER_P(ZEND_THIS);

    aeron_publication_close(publisher->publication, NULL, NULL);
    aeron_close(publisher->aeron);
    aeron_context_close(publisher->context);
}

PHP_MINIT_FUNCTION(aeron)
{
#if defined(COMPILE_DL_BCMATH) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    zend_class_entry* subscriber_ce;
    subscriber_ce = register_class_AeronSubscriber();
    subscriber_ce->create_object = subscriber_new;

    memcpy(&subscriber_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    subscriber_object_handlers.offset = XtOffsetOf(subscriber_t, std);

    zend_class_entry* publisher_ce;
    publisher_ce = register_class_AeronPublisher();
    publisher_ce->create_object = publisher_new;

    memcpy(&publisher_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    publisher_object_handlers.offset = XtOffsetOf(publisher_t, std);

    return SUCCESS;
}

PHP_MINFO_FUNCTION(aeron)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "aeron support", "enabled");
    php_info_print_table_end();
}

zend_module_entry aeron_module_entry = {
    STANDARD_MODULE_HEADER,
    "aeron",                       /* Extension name */
    NULL,                          /* zend_function_entry */
    PHP_MINIT(aeron),              /* PHP_MINIT - Module initialization */
    NULL,                          /* PHP_MSHUTDOWN - Module shutdown */
    NULL,                          /* PHP_RINIT - Request initialization */
    NULL,                          /* PHP_RSHUTDOWN - Request shutdown */
    PHP_MINFO(aeron),              /* PHP_MINFO - Module info */
    PHP_AERON_VERSION,             /* Version */
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_AERON
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(aeron)
#endif
