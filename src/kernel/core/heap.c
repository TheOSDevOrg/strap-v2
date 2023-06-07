#include <heap.h>
#include <stdio.h>

heap_t __STRAPV2_heap_create(uintptr_t base, uintptr_t alloc_stack_base)
{
  return (heap_t) {
    .base = base,
    .alloc_entries_count = 0,
    .alloc_stack_base = (alloc_entry_t *)alloc_stack_base
  };
}

alloc_entry_t __STRAPV2_INTERNAL_heap_push_entry(
  heap_t *heap,
  alloc_entry_t entry
)
{
  if (heap->alloc_entries_count != 0)
  {
    alloc_entry_t prev_entry = *(heap->alloc_stack_base);
    entry.offset_start = prev_entry.offset_end;
    entry.offset_end += entry.offset_start;
  }
  *(--heap->alloc_stack_base) = entry;
  heap->alloc_entries_count++;
  return entry;
}
alloc_entry_t __STRAPV2_INTERNAL_heap_insert_entry(
  heap_t *heap,
  alloc_entry_t entry,
  uintptr_t index
)
{
  heap->alloc_stack_base--;
  for (uintptr_t i = 0; i < index; i++)
    heap->alloc_stack_base[i] = heap->alloc_stack_base[i + 1];
  
  if (index != 0)
  {
    entry.offset_start = heap->alloc_stack_base[index + 1].offset_end;
    entry.offset_end += entry.offset_start;
  }
  heap->alloc_stack_base[index] = entry;
  heap->alloc_entries_count++;
  return entry;
}
bool __STRAPV2_INTERNAL_heap_delete_entry(
  heap_t *heap,
  size_t index
)
{
  if (index >= heap->alloc_entries_count) return false;
  for (size_t i = index; i > 0; i--)
    heap->alloc_stack_base[i] = heap->alloc_stack_base[i - 1];
  heap->alloc_stack_base++;
  heap->alloc_entries_count--;
  return true;
}
size_t __STRAPV2_INTERNAL_heap_find_entry_index(
  heap_t *heap,
  alloc_entry_t entry
)
{
  for (uintptr_t i = 0; i < heap->alloc_entries_count; i++)
    if (
      heap->alloc_stack_base[i].offset_start == entry.offset_start &&
      heap->alloc_stack_base[i].offset_end == entry.offset_end
    ) return i;
  return -1;
}
alloc_entry_t __STRAPV2_INTERNAL_heap_find_entry_from_address(
  heap_t *heap,
  uintptr_t address
)
{
  for (uintptr_t i = 0; i < heap->alloc_entries_count; i++)
    if (
      heap->alloc_stack_base[i].offset_start <= address &&
      heap->alloc_stack_base[i].offset_end > address
    ) return heap->alloc_stack_base[i];
  return (alloc_entry_t) {
    .offset_end = 0xFFFFFFFF
  };
}

alloc_entry_t __STRAPV2_heap_alloc(
  heap_t *heap,
  size_t sz,
  alloc_type_t type,
  alloc_data_type_t data_type
)
{

  // let's create the requested entry, with
  // offset_start set to 0, as it will be 
  /// modified later on if the allocation is offset-ed
  alloc_entry_t entry = (alloc_entry_t) {
    .offset_end = sz,
    .data_type = data_type,
    .type = type
  };

  // if the allocation stack is empty or
  // has just one entry, create a new slot
  // and push the new entry then return
  // the created entry
  if (heap->alloc_entries_count == 0 || heap->alloc_entries_count == 1)
    return __STRAPV2_INTERNAL_heap_push_entry(heap, entry);

  // if the allocation stack is not empty,
  // check for free space between the allocated
  // entries
  uint32_t count;
  for (count = 0; count < heap->alloc_entries_count; count++)
  {
    alloc_entry_t entry = heap->alloc_stack_base[count];
    alloc_entry_t next_entry = heap->alloc_stack_base[count + 1];

    if (entry.offset_start - next_entry.offset_end >= sz) break;
  }

  // if count is equal to the heap's alloc_entries_count
  // then there is no free space between the allocated
  // entries
  if (count == heap->alloc_entries_count)
    return __STRAPV2_INTERNAL_heap_push_entry(heap, entry);
  else // if count is not equal to the heap's alloc_entries_count
    return __STRAPV2_INTERNAL_heap_insert_entry(heap, entry, count + 1);
}
void __STRAPV2_heap_free(heap_t *heap, alloc_entry_t entry)
{
  __STRAPV2_INTERNAL_heap_delete_entry(
    heap,
    __STRAPV2_INTERNAL_heap_find_entry_index(heap, entry)
  );
}
alloc_entry_t __STRAPV2_heap_get_alloc_info(heap_t *heap, uintptr_t addr)
{
  alloc_entry_t entry = __STRAPV2_INTERNAL_heap_find_entry_from_address(
    heap,
    addr
  );
  return entry;
}

char *__STRAPV2_INTERNAL_convert_type(alloc_type_t type)
{
  switch (type)
  {
    case HEAP_ALLOC_TYPE_DIRECT:
      return "direct";
    case HEAP_ALLOC_TYPE_NEW:
      return "new operator";
    case HEAP_ALLOC_TYPE_NEW_ARRAY:
      return "new[] operator";
    default:
      return "unknown";
  }
}
char *__STRAPV2_INTERNAL_convert_data_type(alloc_data_type_t data_type)
{
  switch (data_type)
  {
    case HEAP_ALLOC_DATA_TYPE_OTHER:
      return "other";
    case HEAP_ALLOC_DATA_TYPE_INTEGER:
      return "integer";
    case HEAP_ALLOC_DATA_TYPE_STRING:
      return "string";
    case HEAP_ALLOC_DATA_TYPE_ARRAY:
      return "array";
    default:
      return "unknown";
  }
}

void __STRAPV2_heap_print_alloc_stack(heap_t *heap)
{
  for (int32_t i = heap->alloc_entries_count-1; i >= 0; i--) printf(
    "Entry n. %ud:\n"
    "   start: 0x%ux\n"
    "   end:   0x%ux\n"
    "   type:  %s\n"
    "   data:  %s\n"
    "   size:  %d bytes\n",
    i,
    heap->alloc_stack_base[i].offset_start,
    heap->alloc_stack_base[i].offset_end,
    __STRAPV2_INTERNAL_convert_type(heap->alloc_stack_base[i].type),
    __STRAPV2_INTERNAL_convert_data_type(heap->alloc_stack_base[i].data_type),
    heap->alloc_stack_base[i].offset_end - heap->alloc_stack_base[i].offset_start
  );
}