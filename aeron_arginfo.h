/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b508d8d6b9cb1bee1510da1ad3c494aa7f4e20a6 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Aeron_Publisher___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, channel, IS_STRING, 0, "\"aeron:udp?endpoint=localhost:20121\"")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, stream_id, IS_LONG, 0, "1001")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Aeron_Publisher_offer, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Aeron_Publisher_close, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Aeron_Subscriber___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, handler, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, channel, IS_STRING, 0, "\"aeron:udp?endpoint=localhost:20121\"")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, stream_id, IS_LONG, 0, "1001")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Aeron_Subscriber_addDestination, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, channel, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Aeron_Subscriber_removeDestination arginfo_class_Aeron_Subscriber_addDestination

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Aeron_Subscriber_poll, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Aeron_Subscriber_close arginfo_class_Aeron_Publisher_close


ZEND_METHOD(Aeron_Publisher, __construct);
ZEND_METHOD(Aeron_Publisher, offer);
ZEND_METHOD(Aeron_Publisher, close);
ZEND_METHOD(Aeron_Subscriber, __construct);
ZEND_METHOD(Aeron_Subscriber, addDestination);
ZEND_METHOD(Aeron_Subscriber, removeDestination);
ZEND_METHOD(Aeron_Subscriber, poll);
ZEND_METHOD(Aeron_Subscriber, close);


static const zend_function_entry class_Aeron_AeronException_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_Aeron_AeronPublicationException_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_Aeron_AeronPublicationNotConnectedException_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_Aeron_AeronPublicationBackPressuredException_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_Aeron_AeronPublicationAdminActionException_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_Aeron_AeronPublicationClosedException_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_Aeron_AeronPublicationMaxPositionExceededException_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_Aeron_Publisher_methods[] = {
	ZEND_ME(Aeron_Publisher, __construct, arginfo_class_Aeron_Publisher___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Aeron_Publisher, offer, arginfo_class_Aeron_Publisher_offer, ZEND_ACC_PUBLIC)
	ZEND_ME(Aeron_Publisher, close, arginfo_class_Aeron_Publisher_close, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_Aeron_Subscriber_methods[] = {
	ZEND_ME(Aeron_Subscriber, __construct, arginfo_class_Aeron_Subscriber___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Aeron_Subscriber, addDestination, arginfo_class_Aeron_Subscriber_addDestination, ZEND_ACC_PUBLIC)
	ZEND_ME(Aeron_Subscriber, removeDestination, arginfo_class_Aeron_Subscriber_removeDestination, ZEND_ACC_PUBLIC)
	ZEND_ME(Aeron_Subscriber, poll, arginfo_class_Aeron_Subscriber_poll, ZEND_ACC_PUBLIC)
	ZEND_ME(Aeron_Subscriber, close, arginfo_class_Aeron_Subscriber_close, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_Aeron_AeronException(zend_class_entry *class_entry_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Aeron", "AeronException", class_Aeron_AeronException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_Exception);

	return class_entry;
}

static zend_class_entry *register_class_Aeron_AeronPublicationException(zend_class_entry *class_entry_Aeron_AeronException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Aeron", "AeronPublicationException", class_Aeron_AeronPublicationException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_Aeron_AeronException);

	return class_entry;
}

static zend_class_entry *register_class_Aeron_AeronPublicationNotConnectedException(zend_class_entry *class_entry_Aeron_AeronPublicationException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Aeron", "AeronPublicationNotConnectedException", class_Aeron_AeronPublicationNotConnectedException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_Aeron_AeronPublicationException);

	return class_entry;
}

static zend_class_entry *register_class_Aeron_AeronPublicationBackPressuredException(zend_class_entry *class_entry_Aeron_AeronPublicationException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Aeron", "AeronPublicationBackPressuredException", class_Aeron_AeronPublicationBackPressuredException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_Aeron_AeronPublicationException);

	return class_entry;
}

static zend_class_entry *register_class_Aeron_AeronPublicationAdminActionException(zend_class_entry *class_entry_Aeron_AeronPublicationException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Aeron", "AeronPublicationAdminActionException", class_Aeron_AeronPublicationAdminActionException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_Aeron_AeronPublicationException);

	return class_entry;
}

static zend_class_entry *register_class_Aeron_AeronPublicationClosedException(zend_class_entry *class_entry_Aeron_AeronPublicationException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Aeron", "AeronPublicationClosedException", class_Aeron_AeronPublicationClosedException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_Aeron_AeronPublicationException);

	return class_entry;
}

static zend_class_entry *register_class_Aeron_AeronPublicationMaxPositionExceededException(zend_class_entry *class_entry_Aeron_AeronPublicationException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Aeron", "AeronPublicationMaxPositionExceededException", class_Aeron_AeronPublicationMaxPositionExceededException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_Aeron_AeronPublicationException);

	return class_entry;
}

static zend_class_entry *register_class_Aeron_Publisher(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Aeron", "Publisher", class_Aeron_Publisher_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);

	return class_entry;
}

static zend_class_entry *register_class_Aeron_Subscriber(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Aeron", "Subscriber", class_Aeron_Subscriber_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);

	return class_entry;
}
