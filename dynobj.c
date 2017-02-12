#include "stdlib.h"
#include "string.h"

#include "dynobj.h"

typedef struct dynobj_slot {
  struct dynobj_slot *next_slot;
  char *name;
  dynobj_slot_value value;
} dynobj_slot;

struct dynobj {
  dynobj_slot *slots;
};

dynobj *dynobj_new() {
  dynobj *obj = malloc(sizeof(dynobj));
  obj->slots = NULL;
  return obj;
}

dynobj *dynobj_extend(dynobj *parent) {
  dynobj *obj = dynobj_new();
  dynobj_set_obj_slot(obj, "parent", parent);
  return obj;
}

void dynobj_destroy(dynobj *obj) {
  dynobj_slot *slot = obj->slots;
  while (slot != NULL) {
    dynobj_slot *next = slot->next_slot;
    free(slot->name);
    free(slot);
    slot = next;
  }
  free(obj);
}

dynobj_slot_value *dynobj_get_own_slot(dynobj *obj, const char *name) {
  for (dynobj_slot *slot = obj->slots; slot != NULL; slot = slot->next_slot)
    if (strcmp(slot->name, name) == 0)
      return &slot->value;
  return NULL;
}

bool dynobj_has_own_slot(dynobj *obj, const char *name) {
  for (dynobj_slot *slot = obj->slots; slot != NULL; slot = slot->next_slot)
    if (strcmp(slot->name, name) == 0)
      return true;
  return false;
}

bool dynobj_has_slot(dynobj *obj, const char *name) {
  if (dynobj_has_own_slot(obj, name))
    return true;
  if (dynobj_has_own_slot(obj, "parent"))
    return dynobj_has_slot(dynobj_get_own_obj_slot(obj, "parent"), name);
  else
    return false;
}

dynobj_slot_value *dynobj_get_slot(dynobj *obj, const char *name) {
  dynobj_slot_value *value = dynobj_get_own_slot(obj, name);
  if (value != NULL)
    return value;

  if (dynobj_has_own_slot(obj, "parent"))
    return dynobj_get_slot(dynobj_get_own_obj_slot(obj, "parent"), name);
  else
    return NULL;
}

void dynobj_set_slot(dynobj *obj, const char *name, dynobj_slot_value *value) {
  for (dynobj_slot *slot = obj->slots; slot != NULL; slot = slot->next_slot) {
    if (strcmp(slot->name, name) == 0) {
      memcpy(&slot->value, value, sizeof(dynobj_slot_value));
      return;
    }
  }

  dynobj_slot *slot = malloc(sizeof(dynobj_slot));
  slot->name = strdup(name);
  memcpy(&slot->value, value, sizeof(dynobj_slot_value));

  // Add the new slot to the object's list of slots.
  slot->next_slot = obj->slots;
  obj->slots = slot;
}
