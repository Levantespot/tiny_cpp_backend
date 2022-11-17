#ifndef TCB_BASE_COPYABLE_H
#define TCB_BASE_COPYABLE_H

namespace TCB {
// A tag class emphasises the objects are copyable.
// The empty base class optimization applies.
// Any derived class of copyable should be a value type.
class copyable {
 protected:
  copyable() = default;
  ~copyable() = default;
  copyable(copyable&) = default;
  copyable& operator=(copyable&) = default;
};

}  // namespace TCB

#endif  // TCB_BASE_COPYABLE_H