# C++ Commodity Functionality

## Some functions that we will wrap up into libraries.

C++ is definitely a language where 'batteries are not included'.  So we will want a place to try some things out from time to time.  And if everything goes as expected, we can maybe build them into libraries.

This is not for OpenGL specific libraries.  There is another project for that.

So this project contains a library area for making reusable class libraries.
New executable targets can then be added to explore new features, using the
libraries.

This project is setup to use the GNU autoconf utilities, so in order to configure it for your platform, you need autoconf, automake, libtools, etc.


## Quick start for the impatient.

First, do you have the required GNU tools for this?  Run the following command to find out:

```
$ apt list --installed |egrep -i "autoconf|libtool"

autoconf/stable,now 2.69-11 all [installed]
libtool/stable,now 2.4.6-9 all [installed]
```

If you don't see autoconf & libtool installed, you need to install them.  Here's how:

```
$ sudo apt install autoconf libtool
```

Next is the part where we build our configuration to tailor it to your own environment.  Run the command:

```
$ libtoolize && aclocal && autoconf && automake --add-missing
...
$ ./configure
...
```

Next, we compile our project.  Run the command:

```
$ make all
```

At this point, if nothing has gone wrong, you should be able to run the demo programs.
