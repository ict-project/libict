# `ict::register` module

Provides a tool for a simple object register.

The register is kept for object of the same kind across whole application.

## Usage

In order to access an object register `ict::reg::get<T>()` function must be used.

Following functions are available in the register interface:
* `add(ptr,desc="")` - Adds new object to the register (optionally with description);
* `del(ptr)`- Removes the object from the register;
* `desc(ptr,desc)` - Modifies the description of the object in the register;
* `test(ptr)` - Tests if the object is present in the register;
* `get(ptr)` - Gets pointer of the object if it is present in the register (nullptr is returned otherwise);
* `rand()` - Gets pointer of the randomly chosen object if any in the register (nullptr is returned otherwise);
* `size()` - Gets number of objects kept in the register;
* `init()` - Calls function `initThis()` on all objects kept in the register;
* `destroy()` - Calls function `destroyThis()` on all objects kept in the register;
* `begin()` - Returns *begin* iterator;
* `end()` - Returns *end* iterator.

The object in the register must be derived from `ict::reg::Base` class.
