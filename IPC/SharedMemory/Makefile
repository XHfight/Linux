ser=server
cli=client
cc=gcc

.PHONY:all
all:$(ser) $(cli)

$(ser):server.c comm.c
	$(cc) -o $@ $^
$(cli):client.c comm.c
	$(cc) -o $@ $^

.PHONY:clean
clean:
	rm -f $(ser) $(cli)


