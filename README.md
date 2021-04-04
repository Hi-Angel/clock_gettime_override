This library creates a "bullet-time" effect in any game it is preloaded to.

Underneath it overloads a function `clock_gettime` to slow down its reports by 4 times. This was tested wth Alien Shooter 2, Dungeon Nightmares, and GTAⅣ running in WINE.

# Building

Assuming you're on x86_64 platform, use

```
cargo build --release
cargo build --release --target i686-unknown-linux-gnu
```

to build 64 and 32 bit versions. You may need to add the `i686-unknown-linux-gnu` target by `rustup target install i686-unknown-linux-gnu`.

# Using

Just place paths into `LD_PRELOAD`. Unless you're sure whether you need 32 or 64 bit version, I recommend using them both. It won't hurt, the wrong bittness version just won't get loaded. Example from my system:

	LD_PRELOAD="/home/constantine/Projects/clock_gettime_override/target/i686-unknown-linux-gnu/release/libclock_gettime_override.so /home/constantine/Projects/clock_gettime_override/target/release/libclock_gettime_override.so" wine AlienShooter.exe

# FAQ

1. How do I know if a game is using this function?

    Run the game, find its pid with `ps aux | grep game`, and attach to it with `strace -p game-pid`. You'll see a bunch of system calls that the game does, something about time in particular should come up a lot. If it's a function `clock_gettime`, then it most likely gonna work.
