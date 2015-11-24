#!/bin/bash

#	Name: make.sh
#	Author: Johan Hidding
#	Licence: do whatever you like with this, there's no waranty.
# -----------------
#	Description: this simple script should be able to compile and
#	link a simple but well structured c/c++ project.
# -----------------
#	Details: compiles every .cc file found in all subdirectories.
#	the script checks the modification times of the source files
#	and their header dependencies against the target files.
#	finally all .o files are linked to $target.
#	if you want to exclude files from compiling, put them in a
#	hidden directory (or somewhere outside the directory
#	structure, which is even better)

objdir="obj"

LDFLAGS="-lm -lrt -lgc"
CFLAGS="-g -std=c++0x"
#LIBS="cairomm-1.0 librsvg-2.0 sdl2 SDL2_image"

CC="g++"
ext=".cc"
ECHO="echo -e"

#/>==--------                                  --------==>\#
#>=----- you shouldn't need to edit below this line -----=<#
#\>==--------                                  --------==>/#

include_libs() {
	for lib in $*; do
		LDFLAGS="${LDFLAGS} $(pkg-config --libs $lib)"
		CFLAGS="${CFLAGS} $(pkg-config --cflags $lib)"
	done
}

eval "include_libs $libs"

case "$TERM" in
	dumb)
		prettyprint() {
			$ECHO " * $3 "
			if $1; then
				# $ECHO "\t[done]"
				return 1
			else
				$ECHO "\t[failed]"
				return 0
			fi
		} ;;
	*)
		prettyprint() {
			$ECHO "\033[$2m*\033[m $3"
			if $1; then
				$ECHO "\r\033[A\033[63C[\033[32mdone\033[m]"
				return 1
			else
				$ECHO "\033[62G[\033[31mfailed\033[m]"
				return 0
			fi
		} ;;
esac

checknewer() {
	local k
	if [ ! -e $1 ]; then
		return 0
	fi

	for k in $2; do
		if [ $k -nt $1 ]; then
			return 0
		fi
	done
	return 1
}

compile() {
	dirn=$objdir/$(dirname $1)
	if [ ! -e $dirn ]; then
		mkdir -p $dirn
	fi

	objf=$dirn/$(basename $1 $ext).o
	deps=`$CC -MM $1 $CFLAGS | sed -e '{ s/^.*: //; s/\\\//; s/^ *// }'`
	if checknewer $objf "$deps"; then
		if prettyprint "$CC -c $CFLAGS $1 -o $objf" 34 "Compiling $1 ... "; then
			exit 1
		fi
	fi
}

make_target() {
	additions=()
	source $1/makerc
	DIRS=""
	CCFILES=()
	for p in $modules; do
		module_dirs="$1 $(find $1/$p -maxdepth 2 -type d -name '[^\.]*' -printf '%p ')"
		DIRS="${module_dirs} ${DIRS}"
	done

	adds=()
	for i in ${additions}; do
		adds+="$1/$i "
	done

	for d in $DIRS; do
		dir_ccfiles="${adds} $(ls $d/*.cc 2> /dev/null)"
		for f in $dir_ccfiles; do
			CCFILES+="$(realpath $f --relative-to=.) "
		done
	done

	for f in $CCFILES; do
		compile $f
	done

	objfiles=$(find $objdir -name '*.o')
	if checknewer $target "$objfiles"; then
		if prettyprint "$CC $objfiles -o $target $LDFLAGS" 36 "Linking ..."; then
			exit 1
		fi
	else
		echo "$target allready is up to date."
	fi
}

targets=$(find . -name 'makerc' -printf '%h ')

case "$1" in
	all)
		for t in ${targets}; do
			make_target $t
		done ;;

	clean)
		rm -rf $objdir
		find . -name '*~' -exec rm {} \; ;;

	help)
		echo "This is a make script, written in bash. It is only good"
		echo "for compiling well-structured C++ programs. Read the"
		echo "source for help on configuring the script."
		echo
		echo "run> ./make [command]"
		echo "where [command] ∈ {all clean [target]}"
		echo "where [target]  ∈ {${targets}}"  ;;

	*)
		make_target $1 ;;
esac
