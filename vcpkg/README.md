# VCPKG based ports

# base

The subfolder `base` is a submodule from microsoft/vcpkg. It contains the base ports.

# overlay

The subfolder `overlay` contains specific patches required for certain ports
which are not compatible enough.

# overlay\_system\_qt

The subfolder `overlay_system_qt` contains empty qt ports to allow installing
qt via aqtinstall. This avoids having to compile the complete qt dependency
which is a costly operation and takes a long time.
It also contains patches for qt based ports to make sure they pick up
qt installed this way.
