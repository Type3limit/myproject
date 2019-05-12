SP=server_thread
C=client_thread

all:$(SP) $(C) 

$(SP):$(SP).c
	gcc $^  -o  $@ -lpthread
$(C):$(C).c
	gcc $^  -o  $@

clean:
	rm -f $(SP)  $(C)
