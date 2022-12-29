OUTPUT=samu

if [ "$#" -gt 0 ]
then
    OUTPUT=$1
fi

-- mac hack
CFLAGS=-DNO_GETLOADAVG

cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter $CFLAGS -c -o build.o build.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -c -o deps.o deps.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -c -o env.o env.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -c -o graph.o graph.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -c -o htab.o htab.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -c -o log.o log.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -c -o parse.o parse.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -DSAMU_MAIN -c -o samu.o samu.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -c -o scan.o scan.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -c -o tool.o tool.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -c -o tree.o tree.c
cc -O -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -c -o util.o util.c
cc -o $OUTPUT build.o deps.o env.o graph.o htab.o log.o parse.o samu.o scan.o tool.o tree.o util.o
