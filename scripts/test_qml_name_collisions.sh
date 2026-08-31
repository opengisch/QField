#!/usr/bin/env bash

# This test checks that no QML file shares its name with a type registered in QML
# from C++. When they collide, the name resolves to one or the other depending on
# import order, silently swapping the type instead of failing to compile.

RES=
DIR=$(git rev-parse --show-toplevel)

pushd "${DIR}" >/dev/null || exit

REGISTERED=$(
	{
		git grep -hoP 'qmlRegister\w*\s*(?:<[^>]*>)?\s*\([^;]*?\b\d+\s*,\s*\d+\s*,\s*"\K[^"]+' -- 'src/*.cpp'
		git grep -hoP 'REGISTER_SINGLETON\s*\([^;]*,\s*"\K[^"]+' -- 'src/*.cpp'
	} | sort -u
)

for QML_FILE in $(git ls-files 'src/**/*.qml'); do
	NAME=$(basename "${QML_FILE}" .qml)

	if grep -qxF "${NAME}" <<<"${REGISTERED}"; then
		echo "${QML_FILE} collides with the C++ type registered as '${NAME}'"
		RES=1
	fi
done

popd >/dev/null || exit

if [ $RES ]; then
	echo
	echo " *** Found QML name collisions"
	echo "     Rename the QML file or the registered type so they no longer share a name"
	exit 1
fi
