# A minimal, self-contained port of SGI's GLU libtess

Polygon tessellation is a major pain in the neck. Have you ever tried
writing fast and robust code for it? libtess is, to my knowledge, the
only GPL-compatible, liberally-licensed, high-quality polygon
triangulator out there.

This repository includes a self-contained function (tessellate, in
tessellate.c) that you can call to triangulate a polygon that is
potentially self-intersecting, with holes, or with duplicate
vertices. Simple examples of calling the tessellate function directly
are located in main.c.

