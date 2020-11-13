Concepts
========

QField was designed with a few key concepts in mind.

Keep it simple
..............

The requirements on the field are not the same as on a desktop. The screen is
smaller, the input devices are different and the tasks are different.

QField aims to help the user to perform the tasks he needs to do without
cluttering the user interface. This means, that only tasks which need to be
done on the field are availble from the interface. Everything else is not.

This means that everything like layer styling, form definitions and other
project setup steps should be done on a computer with QGIS installed first.

Be compatible with QGIS
.......................

QField is based on QGIS. It is not a rebuild of QGIS it really *does* use QGIS
libraries. The rendering engine is exactly the same as in QGIS for desktop and
your project will therefore look exactly the same on your mobile device as it
does on your computer.

If something is already available as a configuration option in a QGIS project,
it should not be re-invented. QField therefore uses the same edit widgets as
QGIS desktop does. If a project is already configured for the desktop, it
should just run on mobile as well.

Remember, this is just the *concept*. This is what we have in mind when we
develop QField. It does not mean that it is already completely there yet.

Mode based
..........

QField is built around *modes*. Modes are similar to a *map tool* in QGIS
desktop. A mode defines the task which a user is currently doing. Either a user
is *browsing* through the data or he is *digitizing* something new.
