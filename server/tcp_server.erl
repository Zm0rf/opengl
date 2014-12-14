-module(tcp_server).
-author('ludde90@gmail.com').
-export([start_server/0, do_recv/2, client_handler/3, loop/2, echo_server/1, send_message/2, send_message_recursive/2]).


send_message(Client, Message) ->
	gen_tcp:send(Client, Message).

send_message_recursive([], _Message) ->
	done;

send_message_recursive([Head|Tail], Message) ->
	send_message(Tail, Message),
	send_message_recursive(Head, Message).

% Echo server 
echo_server(Clients) ->
	receive 
		{newclient, Client} ->
			echo_server([Clients|Client]);
		{newmessage, Message} ->
			spawn(?MODULE, send_message_recursive, [Clients, Message])
	end,
	echo_server(Clients).

loop(ServerSock, EchoSock) ->
	{ok, ClientSock} = gen_tcp:accept(ServerSock),
	io:format("Got connection: "),
	io:format(ClientSock),
	io:format("\n"),
	spawn_link(?MODULE, client_handler, [ServerSock, ClientSock, EchoSock]),
	EchoSock ! {newclient, ClientSock},
	loop(ServerSock, EchoSock).

start_server() ->
	EchoSock = spawn_link(?MODULE, echo_server, [[]]),
	{ok, ServerSock} = gen_tcp:listen(8080, [binary, {packet, 0}, {active, false}]),
	loop(ServerSock, EchoSock).

do_recv(ClientSock, EchoSock) ->
	case gen_tcp:recv(ClientSock, 0) of
		{ok, B} ->
			EchoSock ! {newmessage, B},
			{ok, B};
		{error, closed} ->
			{ok, closed}
	end.

client_handler(ServerSock, ClientSock, EchoServer) ->
	case do_recv(ClientSock, EchoServer) of
		{ok, closed} ->
			{ok};
		{ok, Data} ->
			client_handler(ServerSock, ClientSock, EchoServer)
	end.
