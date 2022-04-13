/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: a1f644fcc4fd4df50dca4dd5e5b7d5c6d436bb25 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_AeronSubscriber___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, handler, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, channel, IS_STRING, 0, "\"aeron:udp?control-mode=manual\"")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, stream_id, IS_LONG, 0, "1001")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, fragment_limit, IS_LONG, 0, "10")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_AeronSubscriber___destruct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AeronSubscriber_addDestination, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, channel, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_AeronSubscriber_removeDestination arginfo_class_AeronSubscriber_addDestination

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AeronSubscriber_poll, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_AeronPublisher___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, channel, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, stream_id, IS_LONG, 0, "1001")
ZEND_END_ARG_INFO()

#define arginfo_class_AeronPublisher___destruct arginfo_class_AeronSubscriber___destruct

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AeronPublisher_offer, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(AeronSubscriber, __construct);
ZEND_METHOD(AeronSubscriber, __destruct);
ZEND_METHOD(AeronSubscriber, addDestination);
ZEND_METHOD(AeronSubscriber, removeDestination);
ZEND_METHOD(AeronSubscriber, poll);
ZEND_METHOD(AeronPublisher, __construct);
ZEND_METHOD(AeronPublisher, __destruct);
ZEND_METHOD(AeronPublisher, offer);


static const zend_function_entry class_AeronSubscriber_methods[] = {
	ZEND_ME(AeronSubscriber, __construct, arginfo_class_AeronSubscriber___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(AeronSubscriber, __destruct, arginfo_class_AeronSubscriber___destruct, ZEND_ACC_PUBLIC)
	ZEND_ME(AeronSubscriber, addDestination, arginfo_class_AeronSubscriber_addDestination, ZEND_ACC_PUBLIC)
	ZEND_ME(AeronSubscriber, removeDestination, arginfo_class_AeronSubscriber_removeDestination, ZEND_ACC_PUBLIC)
	ZEND_ME(AeronSubscriber, poll, arginfo_class_AeronSubscriber_poll, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_AeronPublisher_methods[] = {
	ZEND_ME(AeronPublisher, __construct, arginfo_class_AeronPublisher___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(AeronPublisher, __destruct, arginfo_class_AeronPublisher___destruct, ZEND_ACC_PUBLIC)
	ZEND_ME(AeronPublisher, offer, arginfo_class_AeronPublisher_offer, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_AeronSubscriber(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "AeronSubscriber", class_AeronSubscriber_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);

	return class_entry;
}

static zend_class_entry *register_class_AeronPublisher(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "AeronPublisher", class_AeronPublisher_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);

	return class_entry;
}
