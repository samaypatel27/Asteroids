# Samay Patel

CFLAGS= -std=c99 -pedantic -Wformat -Wreturn-type
LDFLAGS= -L.
LIBS= -lrx -lncursesw -lm

# add -llinkedlist back if you need to

all:  tags headers lab4 lab4u


headers: *.c tags
	./headers.sh

tags: *.c
	ctags -R .

lab2: lab2.o bits.o sim.o input.o output.o text.o graphics.o physics.o n2.o
	gcc -g -o $@ $^  $(LDFLAGS) $(LIBS)

pro1: pro1_CLArguments.o
	gcc -g -o $@ $^  $(LDFLAGS) $(LIBS)
pro2: pro2_LL.o unreliable.o linkedlist.o
	gcc -g -o $@ $^  $(LDFLAGS) $(LIBS)
pro3: pro3_printShotData.o
	gcc -g -o $@ $^  $(LDFLAGS) $(LIBS)
pro4: pro4_alternateMemory.o libll.h liblinkedlist.a unreliable.o
	gcc -g -o $@ $^  $(LDFLAGS) $(LIBS)
pro5: pro5_rockShot.o physics.o
	gcc -g -o $@ $^  $(LDFLAGS) $(LIBS)

# additional file includes commonGround.c
lab4: lab4.o bits.o sim.o input.o output.o text.o graphics.o physics.o n2.o librx.a rocks.o ships.o commonGround.o linkedlist.o unreliable.o
	gcc -g -o $@ $^  $(LDFLAGS) $(LIBS)

# additional file includes commonGround.c
lab4u: lab4.o bits.o sim.o input.o output.o text.o graphics.o physics.o n2.o librx.a rocks.o ships.o commonGround.o linkedlist.o reliable.o
	gcc -g -o $@ $^  $(LDFLAGS) $(LIBS)

lab4.zip: makefile README_LAB4 n2.o unreliable.o reliable.o *.c *.h *.a
	# in the rule below, $^ can save you – why?	
	zip $@ $^
	# self-test is required: first remove old install folder
	rm -rf install
	# create the install folder
	mkdir install
	# unzip to the install folder
	unzip lab4.zip -d install
	 # make ONLY the lab4 target, not lab4.zip
	# make all other things that get graded too (never the zip!)
	# the above means things like stuff and all prototypes
	# LOOK FOR ERRORS BELOW HERE
	make -C install -r lab4 lab4u
	# LOOK FOR BUILD ERRORS ABOVE HERE
	# coelinux has a race condition so we call sleep to fix that
	sleep 1

	# Finally: remove i
	rm -rf install


# this is our master compiler rule to generate .o files.
# It needs all 4 warnings

%.o:%.c *.h
	gcc $(CFLAGS) -g -c $< -o $@

