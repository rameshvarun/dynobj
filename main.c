#include "stdio.h"
#include "cutest.h"
#include "dynobj.h"

void test_simple_object(void) {
  dynobj *simple_object = dynobj_new();
  dynobj_set_int_slot(simple_object, "simple-slot", 100);

  TEST_CHECK(dynobj_has_own_slot(simple_object, "simple-slot"));
  TEST_CHECK(dynobj_get_own_int_slot(simple_object, "simple-slot") == 100);

  dynobj_destroy(simple_object);
}

void test_simple_inheritance(void) {
  dynobj *simple_parent = dynobj_new();
  dynobj_set_int_slot(simple_parent, "simple-slot", 100);

  dynobj *simple_child = dynobj_extend(simple_parent);

  TEST_CHECK(!dynobj_has_own_slot(simple_child, "simple-slot"));
  TEST_CHECK(dynobj_has_slot(simple_child, "simple-slot"));
  TEST_CHECK(dynobj_get_int_slot(simple_child, "simple-slot") == 100);

  dynobj_destroy(simple_child);
  dynobj_destroy(simple_parent);
}

DYNOBJ_DEFINE_SIMPLE_MESSAGE(counter_increment) {
		dynobj_set_int_slot(self, "count", dynobj_get_int_slot(self, "count") + 1);
}

void test_simple_message(void) {
  dynobj *counter_proto = dynobj_new();
  dynobj_set_int_slot(counter_proto, "count", 0);
	dynobj_set_ptr_slot(counter_proto, "increment", counter_increment);

  dynobj *a = dynobj_extend(counter_proto);
  dynobj *b = dynobj_extend(counter_proto);

  TEST_CHECK(dynobj_get_int_slot(a, "count") == 0);
  TEST_CHECK(dynobj_get_int_slot(b, "count") == 0);

	DYNOBJ_SEND_SIMPLE_MESSAGE(a, "increment");
	DYNOBJ_SEND_SIMPLE_MESSAGE(a, "increment");
	DYNOBJ_SEND_SIMPLE_MESSAGE(a, "increment");

	DYNOBJ_SEND_SIMPLE_MESSAGE(b, "increment");
	DYNOBJ_SEND_SIMPLE_MESSAGE(b, "increment");

	TEST_CHECK(dynobj_get_int_slot(a, "count") == 3);
  TEST_CHECK(dynobj_get_int_slot(b, "count") == 2);

  dynobj_destroy(a);
  dynobj_destroy(b);
  dynobj_destroy(counter_proto);
}

TEST_LIST = {{"simple-object", test_simple_object},
             {"simple-inheritance", test_simple_inheritance},
             {"test-simple-message", test_simple_message},
             {0}};
