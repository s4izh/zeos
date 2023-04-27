# ZeOS

ZeOS is the skeleton of a simple Operating System based on a Linux 2.4
kernel and developed for the intel 80386 architecture. The goal of
this project is to extend its functionalities.

Inside each directory there is more information about the
milestones completed at each stage of the project.

- [e2](e2/README.md)
- [project](project/README.md)

## How to use the flake for building the development environment

Install [nix](https://nixos.org/download.html)
and enable [nix flakes](https://nixos.wiki/wiki/Flakes).

Then on this project's directory run:

```bash
nix develop
```

You will now be inside a shell with `bochs`, `gdb`, `gcc`, etc. available.
