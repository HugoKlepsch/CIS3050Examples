
# Shell script examples

The examples in this directory are intended to give you some
sense of what you can accomplish with Bourne and bash shell
scripting.  They are described here in order of increasing
complexity.

# Simple - introducing control flow and quoting

* `counting-for` : this example shows the use of a `for` loop,
        which simply iterates over the list of arguments that
        it is provided.  Frequently used with the list of
        arguments from the command line, `"$@"`

* `counting-while` : this example introduces the other common
        looping strategy, as well as if statements and the
        use of `test(1)`

* `quoting` : different effects of different types of quoting.
        Any variables referenced inside double quotes are expanded,
        while anything inside single quotes is treated simply
        as the literal characters typed.  Quotes can start and
        stop within a given argument string.

* `exports-subshell` : an example showing how subshells influence
        what happens with local versus "exported" or "environment"
        variables.

* `commandLineParsing` : shows a common pattern for handling the
        command line within scripts.

# Full programs, using piping, back-ticks, and looping

* `listarguments` : print out the command line arguments, and
        demonstrate some other special shell variables

* `listextensions` : examine directories named in the arguments,
        printing the extensions of all files found within them

* `listextensions-commandlineoptions` : a version of the `listextensions`
        script that has more extensive command line handling of the
        type typically found in utility programs

* `listextensions-recursive` : as above, but now bringing find(1)
        into the mix to do all this recursively into subdirectories


# Sed - a world of its own

* `sedexamples` : gives sed a bit of a workout in order to show
        what kind of things it can do


# FIFOs - named pipes and what we can do with them

* `fiforeaderSingleStream` and `fifowriterSingleStream`: a pair of
        FIFO tools that open the pipe once and send a long message

* `fiforeaderPiecewise` and `fifowriterPiecewise`: a pair of FIFO
        tools that open and close the pipe for each line of the message

    * try mixing these four together, and see if you can understand
        why they behave as they do

