# DOSWord

Wordle clone for DOS, written in C using Allegro 4.2. Build system and "engine" is based on another game of mine (Endless Attack REMIX), so chalk any weird remnants up to that.

## System Requirements

486 CPU (with or without FPU; FPU-less users should run `DOSWORDE.EXE`) or better, VGA mode 13h-compatible display adapter, MS-DOS 5? or later (or compatible)

Can also run under Windows 3.x and 9x! 

## Building

You'll need either DJGPP on DOS, or a cross-compiler. I recommend the latter, specifically GCC 9.3.0 from [here](https://github.com/andrewwutw/build-djgpp/releases/tag/v3.0).

You'll also need Allegro 4.2.3, which you can get complete with DJGPP [here](https://www.mrdictionary.net/allegro/).

Your cross-compiler should reside in TOOLS/djgpp, as well as Allegro's include and lib folders in TOOLS/allegro. See the Makefile for specifics.

```bash
make TARGET=DOS
```

Use `DOS-DEBUG` instead to produce an executable with support for debugging via `gdb`, or `DOS-NOFPU` to produce an executable that will run without an FPU.

## Acknowledgements

- [Josh Wardle](https://twitter.com/powerlanguish) for the original Wordle

- vitawrap's [WOR-DOS](https://github.com/vitawrap/WORDOS), another DOS port (which works on 16-bit systems!)
