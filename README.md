# CujusQue Library
"Cujusque" is the genitive form of a Latin pronoun "quisque", which means "every one", "each one", or "whoever". Living up to that name, this library is a repository where the author will just send anything he feels like at the moment. The overaching namespace for the library is `CQue`, a shortened version of "**C**ujus**Que**". This library is implemented using C++20 standard. As it stands, it has two major parts: type handling and containers.

## 1. Type Handling
Handles problems related to data types and their information. Currently consists of two major classes:
### 1.1. Custom Implementation of Type Information (`CQue::TypeTag`)
Uniquely identifies each data type. The const reference to the corresponding `CQue::TypeTag` object of a type can be obtained through the method `CQue::GetType<T>()` The declared constructor is made private and the copy constructor (and implicitly also the move constructor) is explicitly deleted to ensure the uniqueness of each type's corresponding `CQue::TypeTag` object. `constexpr`-friendly for the most part except the `GetID()` method.

P.S., Do keep in mind that one may simply use the provided equality operator to compare two `CQue::TypeTag`s instead of comparing the ID. 
### 1.2. Type Erasure (`CQue::Any`)
Capable of storing a data regardless of its underlying type under "one single roof". Since the use of standard C++ type information will disallow any remaining attempt to make it usable within `constexpr` evaluation, this class (indirectly) uses `CQue::TypeTag` to identify each element instead of `std::type_info`. Whilst `CQue::Any` cannot be itself used for declaring a `constexpr` variable, it can be used within a `constexpr` function or any evaluation under `constexpr` context. `constexpr`-friendly.

## 2. Containers
Handles problems related to collection of data. Many of the names used are unashamedly given due to .NET generic collection library. Currently consists of two major classes:
### 2.1. Naive Reference Wrapper of Iterable Objects (`CQue::IterWrapper<T>`)
Binds to the iterators of the given iterable object as specified by `Iterable<T>` concept. Termed "naive" because it can only wrap around an iterable object whose iterators are convertible to raw pointers. `constexpr`-friendly.
### 2.2. Contiguous Container (`CQue::List<T>`)
A contiguous, array-based collection equipped with indexer and some helper methods, e.g. sorting, searching, etc. Due to its nature, `List<T>` can only accept objects that are default initializable, copyable, and movable. `constexpr`-friendly.
