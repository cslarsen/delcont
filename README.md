Delimited continuations in C
============================

This is just an unfinished experiment with delimited continuations in C, done
by copying stack frames. Its interface is similar to shift-reset, but doesn't
use a runtime-dynamic stack of start frames yet.

It's also pretty easy to mess things up at the moment. It messes up the stack
and environment if

  * you add three `printf`s after `start` in `calltest()`,
  * compile with anything other than `-O0`,
  * `-fomit-frame-pointer`s.

It's probaly other caveats as well. But I'm quite sure I'll be able to create a
more robust version, even if it will always have inherent limitations.
