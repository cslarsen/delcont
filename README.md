Delimited continuations in C
============================

This is just an unfinished experiment with delimited continuations in C, done
by copyin the stack frames. Its interface is similar to shift-reset, but
doesn't use a stack for the start frames yet.

It's also pretty easy to mess things up at the moment.

There are many caveats. Currently, it does not work correctly if

  * you add three `printf`s after `start` in `calltest()`,
  * compile with anything other than `-O0`,
  * `-fomit-frame-pointer`s.

It's probaly got more problems. But I'm quite sure I'll be able to create a
more robust version, even if it will always have inherent limitations.
