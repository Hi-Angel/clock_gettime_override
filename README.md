It's an overloaded function `clock_gettime` to slow down its reports by 4 times. It's useful to slow down games, in particular is tested wth Alien Shooter 2 and Dungeon Nightmares running in WINE.

# Building

Assuming you're on x86_64 platform, use

```
cargo build --release
cargo build --release --target i686-unknown-linux-gnu 
```

to build 64 and 32 bit versions. You may need to add the `i686-unknown-linux-gnu` target by `rustup target install i686-unknown-linux-gnu`.

# Using

Just place it into `LD_PRELOAD`. Example from my system:

	LD_PRELOAD="/home/constantine/Projects/clock_gettime_override/target/i686-unknown-linux-gnu/release/libclock_gettime_override.so /home/constantine/Projects/clock_gettime_override/target/release/libclock_gettime_override.so" wine AlienShooter.exe

# FAQ

1. How do I know if a game is using this function?

    Run the game, find its pid with `ps aux | grep game`, and attach to it with `strace -p game-pid`. You'll see a lot of system calls that the game is making, in particular something about time should come up to get called a lot. If it's a function `clock_gettime`, then you know you can try to use the library. 
