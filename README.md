# Class 8

In today's class, we discuss common forms of multiple inheritance
supported by OOP languages. Specifically, we will look at

1. Interface inheritance in Java

1. Multiple class inheritance in C++, and

1. Mix-In Composition with traits in Scala (see Class 9).

## Interface Inheritance

James Gosling, the inventor of Java, was once asked "If you could do
Java over again, what would you change?", he replied, "I'd leave out
classes." He went on to explain that the real problem was not classes
per se, but rather *class inheritance* (the `extends`
relationship). Everything we have talked w.r.t. inheritance in this
course thus far relates to class inheritance.

A few weeks ago we noted a few problems with this type of inheritance...

* Encapsulation is broken; subclass is exposed to
  implementation details of superclass.

* Tight coupling between super and subclasses.

* Only good for true 'is a' relationships.

* False hierarchies are common.

All of those disadvantages can be understood as problems with
inheritance of behavior and state. To alleviate these problems, many
OOP languages support a notion of *interface inheritance* as an
alternative to class inheritance.

Objects define their interaction with the outside world through the
methods that they expose. That is, methods form the object's interface
with the outside world. An interface is a formalization of this
concept as a language primitive. Interface inheritance means that we
only inherit class contracts (i.e., method signatures), but no state
or behavior.  We discuss Java's interface mechanism as an example of
this OOP language feature.

### Interfaces in Java

A Java interface is a group of related methods with empty bodies and
static variables (this is no longer true for Java 8. We talk about
Java 8 specifics later.). Implementing an interface allows a class to
be explicit about the behavior it promises to provide -- its contract.
This contract is enforced by the compiler. If your class claims to
implement an interface, all methods defined by that interface must
appear in the class' source code before it will compile.

* Interface constraints:

  * Interfaces can only define method signatures.

  * Interfaces can only define public static fields.

  * Interfaces cannot have constructors.

  * A class that implements an interface must implement all the
    methods defined in the interface.

But wait... is this not just a *pure* abstract class? In fact,
abstract classes seem more flexible:

* Abstract classes can have method implementations.

* Abstract classes can have instance fields.

* Abstract classes can have constructors.

However, interfaces provide a significant feature that abstract
classes do not:

* A class can implement many interfaces but can extend only one
  superclass.

Java does not support multiple inheritance for classes! Any class that
defines all of the required methods is permitted to implement an
interface, regardless of where the class resides in the class
hierarchy. Because Java permits only single inheritance with classes,
abstract classes are severely constrained in their usefulness.
Existing classes can be easily retrofitted to implement a new
interface. Simply add the required methods and an implements clause to
the class declaration. 

In order for two classes to extend the same abstract class, you must
place the abstract class high up in the type hierarchy where it
subclasses an ancestor of both classes. Suppose we have this class
representing a duck.

```java
public abstract class Duck {
  public abstract void quack();
}
  
class Mallard extends Duck {
  public void quack() {
    System.out.println("Quack!");
  }
}

class RubberDuck extends Duck {
  public void quack() {
    System.out.println("Squeak!");
  }
}
```
  
All ducks quack so no problem to have it as part of the abstract class
`Duck`.  But what if we want to have ducks fly? Not all ducks fly so
adding a `fly` method to `Duck` does not work. This is a perfect use
case for an interface, which is simple to retrofit:

```java
public interface Flyable {
  public void fly();
}
  
abstract class Duck {
  public void quack();
}
  
class Mallard extends Duck implements Flyable {
  public void quack() {
    System.out.println("Quack!");
  }
    
  public void fly() {
    System.out.println("Heading south!");
  }
}

class RubberDuck extends Duck {
  public void quack() {
    System.out.println("Squeak!");
  }
}
```

Interfaces are ideal for defining *mixins*. A *mixin* is a type that a
class can implement in addition to its *"primary type"* to declare that
it provides some optional behavior (as in the example we just saw).
For example, `Comparable` is a mixin interface that allows a class to
declare that its instances are ordered with respect to other
instances.

Abstract classes can't be used to define mixins for the same reason
that they can't be retrofitted onto existing classes.  Interfaces thus
allow the construction of non-treelike type hierarchies. Treelike subclass
hierarchies are great for organizing some things, but other things
don't fall neatly into a rigid hierarchy. You don't always need this
level of flexibility, but when you do, interfaces are a lifesaver.
The alternative is a bloated class hierarchy containing a separate
class for every supported combination of class members.

#### Extensions in Java 8

The restriction that interfaces are not allowed to implement methods
has been relaxed in Java 8. Java 8 introduced many new features
including *default* and static methods for interfaces. A default
method is an instance method defined in an interface whose method
header begins with the `default` keyword. The default method provides
an implementation. Every class that implements the interface inherits
the interface's default methods and can override them.

Here is a variant of our `Duck` example where we have added
a `push` method with a default implementation to the interface
`Duck`. The default implementation is overridden by `Mallard` and
inherited by `RubberDuck`.

```java
public interface Flyable {
  public void fly();
}
  
public interface Duck {
  public void quack();
  
  default public void push() {
    quack();
  }
}
  
class Mallard implements Duck, Flyable {
  public void quack() {
    System.out.println("Quack!");
  }
    
  public void fly() {
    System.out.println("Heading south!");
  }
  
  public void push() {
    quack();
    fly();
  }
}

class RubberDuck implements Duck {
  public void quack() {
    System.out.println("Squeak!");
  }
}
```
  
Default methods are useful for evolving existing interfaces:

* one problem with interfaces has historically been evolving them and
  breaking existing implementations.

* Default methods enable you to add new functionality to the
  interfaces of your libraries and ensure binary compatibility with
  code written for older versions of those interfaces.

* With default methods, a major drawback of interfaces is now
  eliminated, as they behave like abstract classes in this case.


One limitation of Java 8's default methods is that you can't mix
multiple interfaces that provide default implementations for the same
method into a single class:

```java
interface Interface1 {
  default void m() {
    System.out.println("Interface1");
  }
}
  
interface Interface2 {
  default void m() {
    System.out.println("Interface2");
  }
}
  
public class InterfaceTest implements Interface1, Interface2 {
  public static void main(String[] args) {
    new InterfaceTest().m();
  }
}
```
  
This yields a compiler error!


### Interfaces vs. Abstract Classes

An interface is generally the best way to define a type that permits
multiple implementations in Java. An exception to this rule is when
you have a very constrained problem in which Coad's Rule is satisfied,
most importantly:

1. A subclass would express a "is a special kind of" and not "is a
   role played by a" relationship.

1. An instance of a subclass never needs to behave as an object of
   another class.

On the other hand, if sharing state is important, then you must use
abstract classes.  If a crucial part of your class is its data and
that data needs to be shared with subclasses, then interfaces will not
work. Interfaces cannot have any instance fields. 

Abstract classes are also easier to evolve than interfaces. Though, the
addition of default methods in Java 8 has made this less of an issue.

Finally, dynamic dispatch is more efficient for calls to virtual
methods of abstract classes than calls to interface methods. Calls to
interface methods cannot be easily implemented via a simple vtable
lookup. Instead, they involve a more complex lookup procedure.
Though, in practice JVM implementations are effective at optimizing
these lookups for monomorphic (i.e. non-generic) interfaces. If you
are interested in
how
[virtual method calls](https://wiki.openjdk.java.net/display/HotSpot/VirtualCalls) and
[interface calls](https://wiki.openjdk.java.net/display/HotSpot/InterfaceCalls) are
commonly implemented in a JVM, you can read about this in
the
[documentation of the HotSpot JVM](https://wiki.openjdk.java.net/display/HotSpot) of
OpenJDK.


## Multiple Class Inheritance

Some OOP languages support unrestricted class inheritance where a
subclass can inherit both state and behavior from multiple parent
classes. We discuss C++ as a prominent example of such a language.

### Multiple Class Inheritance in C++

Despite the generality of multiple inheritance, perhaps the most
common use case of this feature in C++ is to express interface
inheritance a la Java by using *pure* abstract classes. Here is how
our duck example looks like in C++:

```c++
#include <iostream>

class Flyable {
public:
  // abstract method fly
  virtual void fly() = 0;
};

class Duck {
public:
  // abstract method quack
  virtual void quack() = 0;

  // virtual method push with default implementation
  virtual void push() {
    quack();
  }
};

class Mallard : public Duck, public Flyable {
public:
  void quack() {
    std::cout << "Quack!" << std::endl;
  }

  void fly() {
    std::cout << "Heading south!" << std::endl;
  }

  // Overriding Flyable::push
  void push() {
    quack();
    fly();
  }
};

class RubberDuck : public Duck {
public:
  void quack() {
    std::cout << "Squeak!" << std::endl;
  }
};
```

Unlike Java, C++ supports multiple class inheritance. That is, a
subclass can inherit from multiple base classes each of which has its
own instance fields, virtual and non-virtual methods, etc. In
practice, the use of multiple inheritance that goes beyond pure
abstract classes is discouraged as it may lead to the so-called
*diamond problem*.

### The Diamond Problem

The *diamond problem* is an ambiguity that arises in situations like
the following:

* Two classes `B` and `C` inherit from `A`

* Class `D` inherits from both `B` and `C`

**Variant 1:**

* There is a method `m` in `A` that `B` and `C` has overridden, but
  `D` does not override it.

* Which version of the method does `D` inherit: that of `B`, or that of `C`?

**Variant 2:**

* There is an instance field `a` in A that D inherits through both B
  and C.
    
* How many `a` fields should `D` have and how are these fields
  accessed when a `D` instance is viewed as an `A`, `B`, or `C` instance?
    
The answer to these questions are complex and depend on whether we use
*virtual* or *non-virtual* inheritance.


#### Non-virtual Inheritance

Let us first consider the case of non-virtual inheritance. The
following code snippet defines four classes `A`, `B`, `C`, and `D`
that capture the two variants of the diamond problem above for the
case where `B` and `C` extend `A` non-virtually:

```c++
class A {
public:
  int a;

  A() : a(1) {}
  
  virtual void m() = 0;
};

class B : public A {
public:
  int b;

  B() : b(1) {
    this->a = 2;
  }
  
  void m() { cout << "B" << endl; }
};

class C : public A {
public:
  int c;

  C() : c(1) {
    this->a = 3;
  }
  
  void m() { cout << "C" << endl; }
};

class D : public B, public C {
public:
  int d;

  D() : d(1) {}
};
```

To understand how the diamond problem is resolved in this particular
case, it is best to study how the compiler arranges the data layout
and vtables of class `D`. The actual details of how the data layouts
and vtables of C++ classes are arranged in memory are compiler
specific. However, they tend to be similar across compilers. Most
compilers allow you to inspect the memory representations of object
instances and vtables using appropriate command line options.
  
* For the GNU C++ compiler, you can do the following:
  
  ```g++ -fdump-class-hierarchy multiple-inheritance.cpp``` 

  This will generate a file called
  `multiple-inheritance.cpp.002t.class` or similar that will contain
  the data layouts and vtable layouts of all classes declared in the
  file `multiple-inheritance.cpp`.
    
* For LLVM's C++ compiler, you can use

  ```
  clang -E multiple-inheritance.cpp > preprocessed.cpp
  clang -cc1 -emit-obj -fdump-vtable-layouts -fdump-record-layouts preprocessed.cpp
  ``` 
    
  which will dump the layouts in your terminal during
  compilation. The layout dumps generated by LLVM are a bit easier
  to read than those generated by `g++`.

From the compiler's output, we can extract the following diagram
illustrating the data layout of `D` instances in our example:

```
      D Instance:
    0┌──────────────────┐─┐          ────┐
     │ ptr. to A vtable │ │              │
    8├──────────────────┤  > A instance  │
     │ value of a       │ │               > B instance
   12├──────────────────┤─┘              │
     │ value of b       │                │
   16├══════════════════┤─┐          ────┤
     │ ptr. to A vtable │ │              │
   24├──────────────────┤  > A instance  │
     │ value of a       │ |               > C instance
   28├──────────────────┤─┘              │
     │ value of c       │                │
   32├══════════════════┤            ────┘
     │ value of d       │
     └──────────────────┘
```

As you can see, the data layout of a `D` instance consists of a copy
of the data layout of a `B` instance, followed by
a copy of the data layout of a `C` instance, followed by the entries
for the new instance fields of `D`. Since both `B` and `C` extend `A`,
a `D` instance ends up with two copies of `A`'s data layout.

When we use a `D` instance in a context that expects an `A` instance,
then the observed behavior depends on whether we view `D` through `B`
or `C`. For example, consider the following code snippet:

```c++
D* x = new D();
B* y = x;
A* z = y;

z->a  // evaluates to `2`
```

The last line will access the `a` field stored in the `B` portion of
`D`s data layout because we have cast `x` first to a `B*` and then to
an `A*`. In our example, the last line will therefore evaluate to `2`.

On the other hand, the following code will access the `a` field stored
in the `C` portion of `D`'s data layout and will therefore evaluate to
`3`.

```c++
D* x = new D();
C* y = x;
A* z = y;

z->a  // evaluates to `3`
```

How is this behavior implemented by the compiler? Remember that
according to the substitution principle, compiled code that works on
an `A` instance should work the same way no matter whether it operates
on

* an `A` instance,
* the `A` portion of a `B` instance,
* the `A` portion of a `C` instance,
* the `A` portion of the `B` portion of a `D` instance,
* the `A` portion of the `C` portion of a `D` instance,
* etc.

For instance, consider a function

```c++
void foo(A* z) {
  cout << z->a << endl;
}
```

The compiled code for `foo` should be able to perform the exact same
steps when it is called with `foo((A*) ((B*) x))` and `foo((A*) ((C*)
x))` for some `x` of type `D*` or for any other `foo(z)` with `z` of
type `A*`.

To understand how this is realized, it is instructive to inspect what
happens when we cast a `D*` to a `B*`, respectively, a `C*`:

```c++
D* x = new D();
B* y = x;
C* z = x;

cout << "x: " << (void*) x << endl;
cout << "y: " << (void*) y << endl;
cout << "z: " << (void*) z << endl;
```

Running this code will print the memory addresses stored in `x`, `y`,
and `z`, respectively:

```c++
x: 0x555af88bfe70
y: 0x555af88bfe70
z: 0x555af88bfe80
```

Observe, that the value of `z` differs from the values of `x` and `y`
by 16 bytes, which corresponds exactly to the size of a `B`
instance. An upcast

```c++
C* z = x;
```

implicitly executes a so-called *thunk*, which is code generated by
the compiler to adjust the cast pointer to the base of the source
type's data layout (here `D`) to point to the subsection of that data
layout for the target type of the cast (here `C`). In our example, the
thunk adds 16 bytes to the address stored in `x`. Thus, `z` points to
the start of the `C` instance within the `D` instance pointed to by
`x`:

```
        D Instance:
x,y ->  0┌──────────────────┐─┐          ────┐
         │ ptr. to A vtable │ │              │
        8├──────────────────┤ ├> A instance  │
         │ value of a       │ │              ├> B instance
       12├──────────────────┤─┘              │
         │ value of b       │                │
z ---> 16├══════════════════┤─┐          ────┤
         │ ptr. to A vtable │ │              │
       24├──────────────────┤ ├> A instance  │
         │ value of a       │ │              ├> C instance
       28├──────────────────┤─┘              │
         │ value of c       │                │
       32├══════════════════┤            ────┘
         │ value of d       │
         └──────────────────┘
```

If we access field `a` through either of `y` or `z`, the access
pattern is now exactly the same in both cases: take the base pointer
(`y`, respectively, `z`) and add the relative offset of `a` to the
base pointer within the `A` data layout, which is always 8
bytes. Thus, we will access the correct copy of `a` within `D` while
executing the exact same machine code instructions in each case.

Note that both the `B` and `C` layouts within `D` contain a vpointer
to a vtable that stores the pointers to the implementations of methods
inherited from `A` through `B` respectively `C`. Thus, when we call
`m` on a `D` instance, then whether the version inherited from `B` or
the version inherited from `C` is executed depends on whether we view
the `D` instance as a `B` or a `C` instance at the call site. If we
view it as a `B` instance, the method call is dispatched via the
vtable pointed to by the vpointer stored in the `B` portion of the
data layout, and similarly for `C`.

Since a `D` instance has two copies of field `a`, we can't access `a`
directly from a `D` instance unless we make explicit which version we
mean. For example, the code

```c++
D* x = new D();
x->a
```

will result in a compiler error:

```c++
error: request for member `a` is ambiguous
   x->a
      ^
```

We can disambiguate the access to field `a` by telling the compiler
explicitly whether we mean `B::a` or `C::a`:

```c++
D* x = new D();
x->B::a;  // evaluates to `2`
```

For similar reasons, we can't cast a `D*` directly to an `A*` but must
instead make explicit whether we want to cast it through `B*` or
through `C*`.

#### Virtual Inheritance

What if we want to avoid the duplication of the `A` data layout by
inheriting `A` through both `B` and `C`? In C++, this can be achieved
by using *virtual* inheritance:

```c++
class A {
public:
  int a;

  A() : a(1) {}
};

class B : virtual public A {
public:
  int b;

  B() : b(1) {
    this->a = 2;
  }
};

class C : virtual public A {
public:
  int c;

  C() : c(1) {
    this->a = 3;
  }
};

class D : public B, public C {
public:
  int d;

  D() : d(1) {}
};
```

By inspecting the compiler output for this new code, we obtain the
following diagram illustrating the data layout of D instances for this
code:

```
      D Instance:
    0┌──────────────────┐─┐
     │ ptr. to B vtable │ │
    8├──────────────────┤ ├> B specific info
     │ value of b       │ │
   12├──────────────────┤─┘
     │ padding (wasted) │
   16├══════════════════┤─┐
     │ ptr. to C vtable │ │
   24├──────────────────┤ ├> C specific info
     │ value of c       │ │
   28├══════════════════┤─┘
     │ value of d       │>   D specific info
   32├══════════════════┤
     │ value of a       │>   A specific info
   44└──────────────────┘
```

First, note that there is only one `a` field in the new data layout,
as desired. However, the organization of the data layout has changed
dramatically. With virtual inheritance from multiple superclasses, the
fields of the individual classes can no longer be laid out in memory
such that the different portions of the layout correspond to the
superclass layouts. Instead the data layout of the superclass
instances will in general not be congruent with the layout of the
subclass instances. For instance, here is how the layout of class `B`
looks like for the above code example:

```
      B Instance:
    0┌──────────────────┐─┐
     │ ptr. to B vtable │ │
    8├──────────────────┤ ├> B specific data
     │ value of b       │ │
   12├──────────────────┤─┘
     │ padding (wasted) │
   16├══════════════════┤
     │ value of a       │>   A specific data
   44└──────────────────┘
```

The relative offset 16 of `a` in the `B` data layout differs from the
relative offset 32 of `a` in the `D` data layout. How does the
compiler realize subtype polymorphism if the layouts of sub and
superclasses are no longer congruent? The secret lies in the
vtables. 

Observe that the datalayouts contain vpointers to vtables even
though none of the classes declares any virtual methods. The vtables
are needed to store auxiliary information about the relative positions
of the different data layout portions for `A`, `B`, and `C` within
`D`. Using this information the compiler can then generate code that
works uniformly across sub and superclass instances, even if the data
layouts of their instances are no longer congruent.

Here is how `D`'s vtable looks like:

```
      D Instance:                       D vtable:
    0┌──────────────────┐             0┌─────────────────────┐─┐
     │ ptr. to B vtable │─────────────>│ vbase offset (32)   │ │
    8├──────────────────┤             8├─────────────────────┤ │
     │ value of b       │              │ offset to top (0)   │ ├> B vtable section 
   12├──────────────────┤            16├─────────────────────┤ │
     │ padding (wasted) │              │ type info           │ │
   16├══════════════════┤            24├═════════════════════┤─┤
     │ ptr. to C vtable │─────────────>│ vbase offset (16)   │ │
   24├──────────────────┤            32├─────────────────────┤ │
     │ value of c       │              │ offset to top (-16) │ ├> C vtable section
   28├══════════════════┤            40├─────────────────────┤ │
     │ value of d       │              │ type info           │ │
   32├══════════════════┤            48└─────────────────────┘─┘
     │ value of a       │
   40└──────────────────┘
```

The vtable consists of two portions, one for each of the two
superclasses `B` and `C`. The vpointers in the sections for `B` and
`C` in `D`'s data layout point to the beginning of the corresponding
vtable sections in `D`'s vtable.

Each portion consists of three entries:

* the vbase offset
  
* the offset to top, and

* type information related to the superclass.
  
For our discussion, only the vbase offsets are relevant. The vbase
offset stores the relative offset of the beginning of the virtual base
class `A` to the beginning of the superclass segment in the data
layout for the superclass to which the vbase offset belongs. For
example, the vbase offset in the `B` vtable section stores the value
32 because the fields of `A` in the data layout are found at a
relative offset of 32 bytes from the base 0 of the `B` portion of
`D`'s data layout. Similarly, the vbase offset in the `C` vtable
section stores the value 16 because the `A` fields are found at a
relative offset of 16 bytes from the base 16 of the `C` portion of
`D`'s data layout.

Now consider again our code that casts a `D*` to a `B*` and a `C*`
respectively:

```c++
D* x = new D();
B* y = x;
C* z = z;
```

Similar to the case of non-virtual inheritance, `x`, `y`, and `z`
point to the base of the portion in `D`'s data layout corresponding to
their static types.

```
         D Instance:                       D vtable:
x,y -> 0┌──────────────────┐             0┌─────────────────────┐─┐
        │ ptr. to B vtable │─────────────>│ vbase offset (32)   │ │
       8├──────────────────┤             8├─────────────────────┤ │
        │ value of b       │              │ offset to top (0)   │ ├> B vtable section 
      12├──────────────────┤            16├─────────────────────┤ │
        │ padding (wasted) │              │ type info           │ │
z ──> 16├══════════════════┤            24├═════════════════════┤─┤
        │ ptr. to C vtable │─────────────>│ vbase offset (16)   │ │
      24├──────────────────┤            32├─────────────────────┤ │
        │ value of c       │              │ offset to top (-16) │ ├> C vtable section
      28├══════════════════┤            40├─────────────────────┤ │
        │ value of d       │              │ type info           │ │
      32├══════════════════┤            48└─────────────────────┘─┘
        │ value of a       │
      40└──────────────────┘
```

Now, for a field read such as `y->a`, the compiler generates
code that performs the following steps:

1. read the vpointer at address `y` and follow it to the corresponding
   portion in the vtable.
   
1. read the vbase offset at the obtained address in the vtable and add
   it to `y`.

1. read the value stored at the calculated address (i.e. `a`).

Note that the same code works also for the field access `z->a` (as
well as `x->a`) if we replace the base address `y` by `z`.


As we have seen, the support for multiple class inheritance adds a lot of
flexibility to an OOP language, but it also adds a lot of complexity both
to the language implementation as well as the language semantics. As a
result, programmers need a good understanding of how the different
variants of multiple inheritance are implemented in order to
use these features efficiently and correctly.

Given the complexity of multiple class inheritance and its
questionable usefulness in OOP programming, language developers have
striven for a simpler mechanism that combines the benefits of multiple
class inheritance with the simplicity and clean semantics of interface
inheritance. *Traits* are Scala's answer to this problem, which we
will talk about in the next class.

