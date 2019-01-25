tars_demo: tc_epoller.h tc_epoller.cpp tc_epoll_server.h tc_epoll_server.cpp main.cpp
	g++ -g3 -otars_demo tc_epoll_server.cpp tc_epoller.cpp main.cpp
client: client.cpp
	g++ -o client client.cpp

clean:
	rm client tars_demo
