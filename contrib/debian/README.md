
Debian
====================
This directory contains files used to package ojacoind/ojacoin-qt
for Debian-based Linux systems. If you compile ojacoind/ojacoin-qt yourself, there are some useful files here.

## ojacoin: URI support ##


ojacoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install ojacoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your ojacoinqt binary to `/usr/bin`
and the `../../share/pixmaps/bitcoin128.png` to `/usr/share/pixmaps`

ojacoin-qt.protocol (KDE)

