#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct dynobj dynobj;

typedef enum DYNOBJ_SLOT_TYPE {
  DYNOBJ_BOOL_SLOT,

  DYNOBJ_INT_SLOT,
  DYNOBJ_UNSIGNED_INT_SLOT,

  DYNOBJ_CHAR_SLOT,
  DYNOBJ_UNSIGNED_CHAR_SLOT,

  DYNOBJ_FLOAT_SLOT,
  DYNOBJ_DOUBLE_SLOT,

  DYNOBJ_OBJ_SLOT,
  DYNOBJ_POINTER_SLOT
} DYNOBJ_SLOT_TYPE;

typedef struct dynobj_slot_value {
  DYNOBJ_SLOT_TYPE slot_type;
  union {
    bool bool_value;
    int int_value;
    float float_value;
    double double_value;

    dynobj *obj_value;
    void *ptr_value;
  };
} dynobj_slot_value;

// Object creation.
dynobj *dynobj_new();
dynobj *dynobj_extend();

// Object destruction.
void dynobj_destroy(dynobj *obj);

// Manipulate the owned slots of an object.
dynobj_slot_value *dynobj_get_own_slot(dynobj *obj, const char *name);
bool dynobj_has_own_slot(dynobj *obj, const char *name);
void dynobj_set_slot(dynobj *obj, const char *name, dynobj_slot_value *value);

// Manipulate slots taking into account inheritance.
bool dynobj_has_slot(dynobj *obj, const char *name);
dynobj_slot_value *dynobj_get_slot(dynobj *obj, const char *name);

// Typed helpers for getting and setting slot values.
#define CREATE_TYPED_SLOT_HELPERS(SLOT_TYPE, TYPE_NAME, CTYPE, VALUE_NAME)                         \
  static inline void dynobj_set_##TYPE_NAME##_slot(dynobj *obj, const char *name, CTYPE value) {   \
    dynobj_slot_value slot_value;                                                                  \
    slot_value.slot_type = SLOT_TYPE;                                                              \
    slot_value.VALUE_NAME = value;                                                                 \
    dynobj_set_slot(obj, name, &slot_value);                                                       \
  }                                                                                                \
  static inline CTYPE dynobj_get_own_##TYPE_NAME##_slot(dynobj *obj, const char *name) {           \
    dynobj_slot_value *slot_value = dynobj_get_own_slot(obj, name);                                \
    assert(slot_value != NULL);                                                                    \
    return slot_value->VALUE_NAME;                                                                 \
  }                                                                                                \
  static inline CTYPE dynobj_get_##TYPE_NAME##_slot(dynobj *obj, const char *name) {               \
    dynobj_slot_value *slot_value = dynobj_get_slot(obj, name);                                    \
    assert(slot_value != NULL);                                                                    \
    return slot_value->VALUE_NAME;                                                                 \
  }

CREATE_TYPED_SLOT_HELPERS(DYNOBJ_BOOL_SLOT, bool, bool, bool_value)
CREATE_TYPED_SLOT_HELPERS(DYNOBJ_INT_SLOT, int, int, int_value)
CREATE_TYPED_SLOT_HELPERS(DYNOBJ_OBJ_SLOT, obj, dynobj *, obj_value)
CREATE_TYPED_SLOT_HELPERS(DYNOBJ_POINTER_SLOT, ptr, void *, ptr_value)

#define DYNOBJ_DEFINE_SIMPLE_MESSAGE(MESSAGE_NAME) void MESSAGE_NAME(dynobj *self)

#define DYNOBJ_SEND_SIMPLE_MESSAGE(OBJ, MESSAGE_NAME)                                              \
  ({                                                                                               \
    void (*func_ptr)(dynobj *) = dynobj_get_ptr_slot(OBJ, MESSAGE_NAME);                           \
    func_ptr(OBJ);                                                                                 \
  })
